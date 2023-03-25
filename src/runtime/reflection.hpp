#include<iostream>
#include<map>
#include<vector>
#include<functional>
#include<json/json11.hpp>
using Json = json11::Json;
using namespace std;
namespace myengine
{
#define META(...)
#define CLASS(class_name, ...) class class_name
#define STRUCT(struct_name, ...) struct struct_name

#define REFLECTION_BODY(class_name) \
    friend class reflection::TypeFieldReflectionOparator::Type##class_name##Operator; \
    friend class Serializer;
	// public: virtual std::string getTypeName() override {return #class_name;}

#define REFLECTION_TYPE(class_name) \
    namespace reflection \
    { \
        namespace TypeFieldReflectionOparator \
        { \
            class Type##class_name##Operator; \
        } \
    };

#define REGISTER_FIELD_TO_MAP(name, value) TypeMetaRegisterinterface::registerToFieldMap(name, value);
	
#define REGISTER_BASE_CLASS_TO_MAP(name, value) TypeMetaRegisterinterface::registerToClassMap(name, value);
#define REGISTER_ARRAY_TO_MAP(name, value) TypeMetaRegisterinterface::registerToArrayMap(name, value);
#define UNREGISTER_ALL TypeMetaRegisterinterface::unregisterAll();

#define REFLECTION_NEW(name, ...) reflection::reflectionPtr(#name, new name(__VA_ARGS__));
#define REFLECTION_DELETE(value) \
    if (value) \
    { \
        delete value.operator->(); \
        value.getPtrReference() = nullptr; \
    }
#define REFLECTION_DEEP_COPY(type, dst_ptr, src_ptr) \
    *static_cast<type*>(dst_ptr) = *static_cast<type*>(src_ptr.getPtr());

#define TypeMetaDef(class_name, ptr) \
    myengine::reflection::reflectionInstance(myengine::reflection::TypeMeta::newMetaFromName(#class_name), (class_name*)ptr)

#define TypeMetaDefPtr(class_name, ptr) \
    new myengine::reflection::reflectionInstance(myengine::reflection::TypeMeta::newMetaFromName(#class_name), \
                                              (class_name*)ptr)


	namespace reflection
	{
		class TypeMeta;
		class FieldAccessor;
		class ArrayAccessor;
		class reflectionInstance;
	}
	typedef std::function<void(void*, void*)>      SetFuncion;
	typedef std::function<void* (void*)>            GetFuncion;
	typedef std::function<const char* ()>           GetNameFuncion;
	typedef std::function<void(int, void*, void*)> SetArrayFunc;
	typedef std::function<void* (int, void*)>       GetArrayFunc;
	typedef std::function<int(void*)>              GetSizeFunc;
	typedef std::function<bool()>                  GetBoolFunc;

	typedef std::function<void* (const Json&)>                          ConstructorWithJson;
	typedef std::function<Json(void*)>                                 WriteJsonByName;
	typedef std::function<int(reflection::reflectionInstance*&, void*)>
		GetBaseClassReflectionInstanceListFunc;

	typedef std::tuple<SetFuncion, GetFuncion, GetNameFuncion, GetNameFuncion, GetNameFuncion, GetBoolFunc>
		FieldFunctionTuple;
	typedef std::tuple<GetBaseClassReflectionInstanceListFunc, ConstructorWithJson, WriteJsonByName>
		ClassFunctionTuple;
	typedef std::tuple<SetArrayFunc, GetArrayFunc, GetSizeFunc, GetNameFuncion, GetNameFuncion> ArrayFunctionTuple;

	namespace reflection
	{


		class TypeMetaRegisterinterface
		{
		public:
			static void registerToClassMap(const char* name, ClassFunctionTuple* value);
			static void registerToFieldMap(const char* name, FieldFunctionTuple* value);
			static void registerToArrayMap(const char* name, ArrayFunctionTuple* value);

			static void unregisterAll();
		};
		class TypeMeta
		{
			friend class FieldAccessor;
			friend class ArrayAccessor;
			friend class reflectionInstance;
		public:

			TypeMeta();
			static TypeMeta newMetaFromName(std::string typeName);

			static bool               newArrayAccessorFromName(std::string array_type_name, ArrayAccessor& accessor);
			static reflectionInstance newFromNameAndJson(std::string typeName, const Json& json_context);
			static Json              writeByName(std::string typeName, void* instance);

			std::string getTypeName();

			int getFieldsList(FieldAccessor*& out_list);

			int getBaseClassReflectionInstanceList(reflectionInstance*& out_list, void* instance);

			FieldAccessor getFieldByName(const char* name);

			bool isValid() { return is_valid; }

			TypeMeta& operator=(const TypeMeta& dest);

		private:
			TypeMeta(string type_name);
			bool is_valid;
			string type_name;
			vector<FieldAccessor, allocator<FieldAccessor>> child_fields;
			
		};
		class FieldAccessor
		{
		
			friend class TypeMeta;
		public:
			FieldAccessor();
			void* get(void* instance);
			void  set(void* instance, void* value);

			TypeMeta getOwnerTypeMeta();
			bool        getTypeMeta(TypeMeta& field_type);
			const char* getFieldName() const;
			const char* getFieldTypeName();
			bool        isArrayType();

			FieldAccessor& operator=(const FieldAccessor& dest);

		private:
			FieldAccessor(FieldFunctionTuple* functions);

		private:
			const char* field_type_name;
			const char* field_name;
			FieldFunctionTuple* field_functions;
		};
		class ArrayAccessor
		{
		public:
			friend class TypeMeta;
			ArrayAccessor();
			const char* getArrayTypeName();
			const char* getElementTypeName();
			void* getValue(int index, void* instance);
			int getSize(void* instance);
			void        set(int index, void* instance, void* element_value);
			ArrayAccessor& operator=(ArrayAccessor& dest);
		private:
			ArrayAccessor(ArrayFunctionTuple* dest);
			ArrayFunctionTuple* array_functions;
			const char* array_type_name;
			const char* element_type_name;
		};
		class reflectionInstance
		{
		public:
			reflectionInstance():meta(),instance(nullptr){};
			reflectionInstance(TypeMeta m, void* i) :meta(m), instance(i) {};
			reflectionInstance& operator=(reflectionInstance& dest);


		private:
			TypeMeta meta;
			void* instance;
		};
		template<typename T>
		class reflectionPtr
		{
			template<typename U>
			friend class reflectionPtr;

		public:
			reflectionPtr(std::string type_name, T* instance) : m_type_name(type_name), m_instance(instance) {}
			reflectionPtr() : m_type_name(), m_instance(nullptr) {}

			reflectionPtr(const reflectionPtr& dest) : m_type_name(dest.m_type_name), m_instance(dest.m_instance) {}

			template<typename U /*, typename = typename std::enable_if<std::is_safely_castable<T*, U*>::value>::type */>
			reflectionPtr<T>& operator=(const reflectionPtr<U>& dest)
			{
				if (this == static_cast<void*>(&dest))
				{
					return *this;
				}
				m_type_name = dest.m_type_name;
				m_instance = static_cast<T*>(dest.m_instance);
				return *this;
			}

			template<typename U /*, typename = typename std::enable_if<std::is_safely_castable<T*, U*>::value>::type*/>
			reflectionPtr<T>& operator=(reflectionPtr<U>&& dest)
			{
				if (this == static_cast<void*>(&dest))
				{
					return *this;
				}
				m_type_name = dest.m_type_name;
				m_instance = static_cast<T*>(dest.m_instance);
				return *this;
			}

			reflectionPtr<T>& operator=(const reflectionPtr<T>& dest)
			{
				if (this == &dest)
				{
					return *this;
				}
				m_type_name = dest.m_type_name;
				m_instance = dest.m_instance;
				return *this;
			}

			reflectionPtr<T>& operator=(reflectionPtr<T>&& dest)
			{
				if (this == &dest)
				{
					return *this;
				}
				m_type_name = dest.m_type_name;
				m_instance = dest.m_instance;
				return *this;
			}

			std::string getTypeName() const { return m_type_name; }

			void setTypeName(std::string name) { m_type_name = name; }

			bool operator==(const T* ptr) const { return (m_instance == ptr); }

			bool operator!=(const T* ptr) const { return (m_instance != ptr); }

			bool operator==(const reflectionPtr<T>& rhs_ptr) const { return (m_instance == rhs_ptr.m_instance); }

			bool operator!=(const reflectionPtr<T>& rhs_ptr) const { return (m_instance != rhs_ptr.m_instance); }

			template<
				typename T1 /*, typename = typename std::enable_if<std::is_safely_castable<T*, T1*>::value>::type*/>
			explicit operator T1* ()
			{
				return static_cast<T1*>(m_instance);
			}

			template<
				typename T1 /*, typename = typename std::enable_if<std::is_safely_castable<T*, T1*>::value>::type*/>
			operator reflectionPtr<T1>()
			{
				return reflectionPtr<T1>(m_type_name, (T1*)(m_instance));
			}

			template<
				typename T1 /*, typename = typename std::enable_if<std::is_safely_castable<T*, T1*>::value>::type*/>
			explicit operator const T1* () const
			{
				return static_cast<T1*>(m_instance);
			}

			template<
				typename T1 /*, typename = typename std::enable_if<std::is_safely_castable<T*, T1*>::value>::type*/>
			operator const reflectionPtr<T1>() const
			{
				return reflectionPtr<T1>(m_type_name, (T1*)(m_instance));
			}

			T* operator->() { return m_instance; }

			T* operator->() const { return m_instance; }

			T& operator*() { return *(m_instance); }

			T* getPtr() { return m_instance; }

			T* getPtr() const { return m_instance; }

			const T& operator*() const { return *(static_cast<const T*>(m_instance)); }

			T*& getPtrReference() { return m_instance; }

			operator bool() const { return (m_instance != nullptr); }

		private:
			std::string m_type_name{ "" };
			typedef T   m_type;
			T* m_instance{ nullptr };
		};
	}
}