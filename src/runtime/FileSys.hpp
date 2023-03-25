#pragma once
#include<filesystem>
#include<iostream>
#include<vector>
#include<io.h>
using namespace std;
namespace fs = std::filesystem;
namespace myEngine
{
	struct fileTreeNode
	{
		fs::path myPath;
		vector<struct fileTreeNode*> son;

	};
	class filePathTree
	{
	public:
		filePathTree(string &rootPath);
		fileTreeNode rootNode;
	private:

	};
	class pathProcess
	{
	public:
		static const fs::path getRelativePath(const fs::path& dir_path,
			                                   const fs::path& file_path);
		static const std::vector<std::string> getPathSegments(const std::filesystem::path& file_path);

		static const std::tuple<std::string, std::string, std::string>
			getFileExtensions(const std::filesystem::path& file_path);

		static const std::string getFilePureName(const std::string);
	
	};
}
