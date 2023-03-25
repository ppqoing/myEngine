#include<iostream>
#include<io.h>
#include"runtime/FileSys.hpp"
#include<logSys.hpp>


using namespace std;
using namespace myEngine;
namespace fs = std::filesystem;
int main()
{
	string s("E:\\pilot\\Piccolo-main\\build");
	cout << s << endl;
	auto m_logger_system = std::make_shared<LogSystem>();
	m_logger_system->log(LogSystem::LogLevel::error, "[" + std::string(__FUNCTION__) + "] "+ s);

}