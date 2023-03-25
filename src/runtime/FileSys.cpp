#include"FileSys.hpp"
namespace myEngine
{
	void getFiles(fileTreeNode *rootnode)
	{
		vector<fileTreeNode*> files;
		string path = rootnode->myPath.string().c_str();
		intptr_t hFile = 0;

		struct _finddata_t fileinfo;

		string p;
		
		if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1) {
			do {
				fileTreeNode* ppp = new fileTreeNode;
				if ((string)fileinfo.name != ".." && (string)fileinfo.name != ".")
				{
					
                   ppp->myPath = fs::path(p.assign(path).append("\\").append(fileinfo.name));
				
					files.push_back(ppp);
				}
					
					

			} while (_findnext(hFile, &fileinfo) == 0);

			_findclose(hFile);
		}
		rootnode->son = files;
		for (fileTreeNode* f:rootnode->son)
		{
			fs::directory_entry de(f->myPath);
			if (de.is_directory())
			{
				getFiles(f);
			}
		}
	}




	filePathTree::filePathTree(string& rootPath)
	{
		rootNode.myPath = fs::path(rootPath);
		getFiles(&rootNode);
		
	}


	const fs::path pathProcess::getRelativePath(const fs::path& dir_path, const fs::path& file_path)
	{
		return file_path.lexically_relative(dir_path);
	}
	const vector<string> pathProcess::getPathSegments(const fs::path& file_path)
	{
		vector<string> segments;
		for (auto iter = file_path.begin(); iter != file_path.end(); ++iter)
		{
			segments.emplace_back(iter->generic_string());
		}
		return segments;
	}
	const tuple<string, string, string> pathProcess::getFileExtensions(const fs::path& file_path)
	{
		return tuple<string, string, string>(file_path.extension().generic_string(),
			file_path.stem().extension().generic_string(),
			file_path.stem().stem().extension().generic_string());

	}
	const string pathProcess::getFilePureName(string file_full_name)
	{
		return file_full_name.substr(0, file_full_name.find_first_of('.'));




	}



}
