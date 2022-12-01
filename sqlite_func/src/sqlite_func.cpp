#include <iostream>
#include "../sqlite_func.h"








void functest() {

	sqlfunc my_sql("../src/test.db");
	
	//select函数的回调数据
	std::vector<std::unordered_map<std::string, std::string>> vec_user2;
	std::unordered_map<std::string, std::string> map_user2;
	map_user2["ID"] = "10";
	map_user2["AGE"] = "20";
	map_user2["NAME"] = "Sam";
	vec_user2.push_back(map_user2);

	//数据类型问题
	int a = 11;
	float b = 23.33f;
	char* c = "Sam";
	std::string t_err = "";
	my_sql.insertData("USER2", my_sql.Columns("NAME", "ID", "AGE"), my_sql.Values(c, a, b));
	std::cout << t_err << std::endl;

}


int main()
{
	std::cout << "start main" << std::endl;

	functest();


	std::cout << "end main" << std::endl;



}


