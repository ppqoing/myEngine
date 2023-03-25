#pragma once
#include"../src/runtime/serializer.hpp"
#include"../src/runtime/macro.hpp"

#include<filesystem>
#include<fstream>
namespace myengine
{
	class AssetManager
	{
	public:
		template<typename T>
		bool loadAsset(const string& url, T& out_asset)const   //传入完整路径
		{
			std::filesystem::path asset_path(url);
			std::ifstream asset_json_file(asset_path);
			if (!asset_json_file)
			{
				LOG_ERROR("open file:{} filed", url);
				return false;
			}
			std::stringstream buffer;
			buffer << asset_json_file.rdbuf();
			std::string asset_json_text(buffer.str());

			std::string error;
			auto&& asset_json = Json::parse(asset_json_text, error);
			if (!error.empty())
			{
				LOG_ERROR("parse json file {} failed!", asset_url);
				return false;
			}

			Serializer::read(asset_json, out_asset);
			return true;
		}
		template<typename T>
		bool saveAsset(T& out_asset,const string& url)const
		{
			std::ofstream asset_json_file(url);
			if (!asset_json_file)
			{
				LOG_ERROR("open file {} failed!",url);
				return false;
			}


			auto&& asset_json = Serializer::write(out_asset);
			std::string&& asset_json_text = asset_json.dump();


			asset_json_file << asset_json_text;
			asset_json_file.flush();

			return true;
		}
	};

}