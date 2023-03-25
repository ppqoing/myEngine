#include<iostream>
#include <fstream> //读写文件 c++标准库
#include"json/json11.hpp"
using namespace std;
using namespace json11;
template<typename T>
void ppppp(const T vvv)
{
    cout << hash<T>{}(vvv);
}
int main()
{
	/*ofstream fi;
	fi.open("sss.json",ios_base::end);

    const string simple_test =
        R"({"k1":"v1", 
        "k2":42, 
        "k3":["a",123,true,false,null]})";

    string err;
    const auto json = Json::parse(simple_test, err);
    fi << json.dump() << endl;
    fi.close();
    cout << json["k2"].int_value() << endl;*/
    const string a{ "csscvonsii955" };  //1860665665677512953
    ppppp<string>(a);                //13546835454607312515
    return 0;
}