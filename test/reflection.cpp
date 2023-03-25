#include"reflection.hpp"
using namespace std;
using  Json=json11::Json;
namespace myengine
{
	map<string, ClassFunctionTuple*> class_reflection_map;
	multimap<string, FieldFunctionTuple*> field_reflection_map;
	map<string, ArrayFunctionTuple*> array_reflection_map;

	namespace reflection
	{ 
		void TypeMetaRegisterinterface::registerToClassMap(const char* name, ClassFunctionTuple* value)
		{
			class_reflection_map.insert(make_pair(name, value));
		}
		void TypeMetaRegisterinterface::registerToFieldMap(const char* name,FieldFunctionTuple* value)
		{
			field_reflection_map.insert(make_pair(name, value));
		}
		void TypeMetaRegisterinterface::registerToArrayMap(const char* name, ArrayFunctionTuple* value)
		{
			array_reflection_map.insert(make_pair(name, value));
		}
		void TypeMetaRegisterinterface::unregisterAll()
		{
			for (const auto& itr : field_reflection_map)
			{
				delete itr.second;
			}
			field_reflection_map.clear();
			for (const auto& itr : class_reflection_map)
			{
				delete itr.second;
			}
			class_reflection_map.clear();
			for (const auto& itr : array_reflection_map)
			{
				delete itr.second;
			}
			array_reflection_map.clear();
		}
		TypeMeta::TypeMeta() :is_valid(false), type_name("unKnownTypeName") { child_fields.clear(); };
		TypeMeta::TypeMeta(string typeName)
		{
			is_valid = false;
			child_fields.clear();
			auto iterator_s = field_reflection_map.equal_range(typeName);
			while (iterator_s.first != iterator_s.second)
			{
				FieldAccessor f_field(iterator_s.first->second);
				child_fields.emplace_back(f_field);
				is_valid = true;

				++iterator_s.first;
			}

		}
		TypeMeta TypeMeta::newMetaFromName(string typeName)
		{
			TypeMeta f_type(typeName);
			return f_type;
		}
		bool TypeMeta::newArrayAccessorFromName(string array_type_name, ArrayAccessor& accessor)
		{
			auto iter = array_reflection_map.find(array_type_name);

			if (iter != array_reflection_map.end())
			{
				ArrayAccessor new_accessor(iter->second);
				accessor = new_accessor;
				return true;
			}

			return false;
		}
		reflectionInstance TypeMeta::newFromNameAndJson(string typeName, const Json& json_context)
		{
			auto iter = class_reflection_map.find(typeName);

			if (iter != class_reflection_map.end())
			{
				return reflectionInstance(TypeMeta(typeName), (std::get<1>(*iter->second)(json_context)));
			}
			return reflectionInstance();
		}
		Json TypeMeta::writeByName(string typeName, void* instance)
		{
			auto iter = class_reflection_map.find(typeName);

			if (iter !=class_reflection_map.end())
			{
				return std::get<2>(*iter->second)(instance);
			}
			return Json();
		}
		string TypeMeta::getTypeName()
		{
			return type_name;
		}
		int TypeMeta::getFieldsList(FieldAccessor*& out_list)
		{
			int count = child_fields.size();
			out_list = new FieldAccessor[count];
			for (int i = 0; i < count; ++i)
			{
				out_list[i] = child_fields[i];
			}
			return count;
		}
		int TypeMeta::getBaseClassReflectionInstanceList(reflectionInstance*& out_list, void* instance)
		{
			auto iter = class_reflection_map.find(type_name);

			if (iter != class_reflection_map.end())
			{
				return (std::get<0>(*iter->second))(out_list, instance);
			}

			return 0;
		}
		FieldAccessor TypeMeta::getFieldByName(const char* name)
		{
			const auto it = std::find_if(child_fields.begin(), child_fields.end(), [&](const auto& i) {
				return std::strcmp(i.getFieldName(), name) == 0;
				});
			if (it != child_fields.end())
				return *it;
			return FieldAccessor(nullptr);
		
		}
		TypeMeta& TypeMeta::operator=(const TypeMeta& dest)
		{
			if (this == &dest)
			{
				return *this;
			}
			child_fields.clear();
			child_fields = dest.child_fields;

			type_name = dest.type_name;
			is_valid = dest.is_valid;

			return *this;
		}





		FieldAccessor::FieldAccessor() :field_type_name("unKnownTypeName"), field_name("unKnownName") {};
		FieldAccessor::FieldAccessor(FieldFunctionTuple* functions) :field_functions(functions)
		{
			field_type_name = "unKnownTypeName";
			field_name = "unKnownName";
			if (functions == nullptr)
			{
				return;
			}
			field_name = std::get<3>(*field_functions)();
			field_type_name = std::get<4>(*field_functions)();
		};
		void* FieldAccessor::get(void* instance)
		{
			return static_cast<void*>(std::get<1>(*field_functions)(instance));
		}
	 	void FieldAccessor::set(void* instance, void* value)
		{
			std::get<0>(*field_functions)(instance, value);
		}
		TypeMeta FieldAccessor::getOwnerTypeMeta()  
		{
			TypeMeta ownType(std::get<2>(*field_functions)());
			return ownType;
		}
		bool FieldAccessor::getTypeMeta(TypeMeta& field_type)
		{
			TypeMeta f_type(field_type_name);
			field_type = f_type;
			return f_type.is_valid;

		}
		const char* FieldAccessor::getFieldName() const
		{
			return field_name;
		}
		const char* FieldAccessor::getFieldTypeName()
		{
			return field_type_name;
		}
		bool FieldAccessor::isArrayType()
		{
			return std::get<5>(*field_functions)();
		}
		FieldAccessor& FieldAccessor::operator=(const FieldAccessor& dest)
		{
			if (this == &dest)
			{
				return *this;
			}
			this->field_functions = dest.field_functions;
			this->field_name = dest.field_name;
			this->field_type_name = dest.field_type_name;
			return *this;
		}




		ArrayAccessor::ArrayAccessor() :array_type_name("unKnownType"), element_type_name("unKnownElementType"), array_functions(nullptr) {};
		ArrayAccessor::ArrayAccessor(ArrayFunctionTuple* dest):array_functions(dest)
		{
			array_type_name = "unKnownTypeName";
			element_type_name = "unKnownElementTypeName";
			if (dest == nullptr)
			{
				return ;
			}
			array_type_name = get<3>(*array_functions)();
			element_type_name = get<4>(*array_functions)();

		}
		const char* ArrayAccessor::getArrayTypeName()
		{
			return array_type_name;
		}
		const char* ArrayAccessor::getElementTypeName()
		{
			return element_type_name;
		}
		void* ArrayAccessor::getValue(int index, void* instance)
		{
			size_t count = getSize(instance);
			return get<1>(*array_functions)(index, instance);
		}
		int ArrayAccessor::getSize(void* instance)
		{
			return get<2>(*array_functions)(instance);
		}
		ArrayAccessor& ArrayAccessor::operator=(ArrayAccessor& dest)
		{
			if (this == &dest)
			{
				return *this;
			}
			array_functions = dest.array_functions;
			array_type_name = dest.array_type_name;
			element_type_name = dest.element_type_name;
			return *this;
		}
		reflectionInstance& reflectionInstance::operator=(reflectionInstance& dest)
		{
			if (this == &dest)
			{
				return *this;
			}
			else
			{
				this->instance = dest.instance;
				this->meta = dest.meta;
				return *this;
			}
		}
		

	}
}