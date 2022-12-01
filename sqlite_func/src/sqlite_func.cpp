#include <iostream>
#include "../sqlite_func.h"








void functest() {

	

	//做成单例之后，使用宏定义将column、value进行简化
	//my_sql.insertData("USER", my_sql.Columns("ID", "NAME"), my_sql.Values("4", "'WENJY4'"));
	//my_sql.insertData("USER2", my_sql.Values("1", "'NAME_1'", "10" , "asd", "ada"));

	//my_sql.updateData("USER2", my_sql.Columns("NAME"), my_sql.Values("'LILI'"));
	//my_sql.updateData("USER", my_sql.Columns("ID"), my_sql.Values("10"), my_sql.Where("NAME=='WENJY'"));
	//my_sql.updateData("USER2", my_sql.Columns("NAME"), my_sql.Values("'Sam'"), my_sql.Where("NAME=='LILI'"), my_sql.And("ID>=2", "AGE>=10"));

	//my_sql.deleteData("USER");
	//my_sql.deleteData("USER2", my_sql.Where("AGE==10"), my_sql.And("ID>5"), my_sql.And("NAME=='NAME_6'"));


	//my_sql.selectData("USER2", my_data, my_sql.Where("NAME=='Sam'"));
	//my_sql.selectData("USER2", my_data, my_sql.Where("NAME=='Sam'"), my_sql.And("ID>0"));
	 
	//select指定表头
	//my_sql.selectData("USER2", my_sql.Columns("NAME"), my_data);
	//my_sql.selectData("USER2", my_sql.Columns("NAME"), my_data, my_sql.Where("ID>1"));
	//my_sql.selectData("USER2", my_sql.Columns("NAME", "AGE"), my_data, my_sql.Where("ID>1"), my_sql.And("ID<4"));




	


	std::vector<std::unordered_map<std::string, std::string>> my_data;
	sqlfunc my_sql("../src/test.db");
	
	//测试从外部的值写入
	//my_sql.insertData("USER2", my_sql.Columns("ID", "AGE", "NAME"), my_sql.Values("11", "11", "Wen11"));
	
	std::string test_tablename = "USER2";
	//std::string test_value1 = "16";
	char* testchar = "Wen18";
	//my_sql.insertData(test_tablename, my_sql.Values(18, 18, testchar));




	//因为struct无法拿到内容，可以用map
	std::vector<std::unordered_map<std::string, std::string>> vec_user2;
	std::unordered_map<std::string, std::string> map_user2;
	map_user2["ID"] = "";
	map_user2["AGE"] = "10";
	map_user2["NAME"] = "Sam";
	vec_user2.push_back(map_user2);
	//注意map的column必须完整。insert会限制，并且无法插入相同的主键
	//my_sql.insertData_v2("USER2", map_user2);

	//my_sql.updateData("USER2", my_sql.Columns("AGE","NAME"), my_sql.Values(23,"Sam"), NULL, NULL);
	//注意map的0项必须为该数据的主键，它将作为update的索引
	//my_sql.updateData_v2("USER2", map_user2);


	//my_sql.selectData("USER2", my_data, my_sql.Where("ID>1"), my_sql.And("ID<7"), my_sql.Or(""));
	//调用多个条件直接叠加，不需要and、or直接null即可
	//my_sql.selectData("USER2", my_data, NULL, NULL);
	//for (int i = 0; i < my_data.size(); i++) {
	//	std::cout << "****the " << i << " data****" << std::endl;
	//	for (auto iter = my_data[i].begin(); iter != my_data[i].end(); iter++) {
	//		std::cout << "the column, value is: " << (*iter).first << ", " << (*iter).second << std::endl;
	//	}
	//}


	//my_sql.insertData("USER2", my_sql.Columns("NAME", "ID", "AGE"), my_sql.Values("TEST",32, 32));
	
    //my_sql.deleteData("USER2", my_sql.Where("AGE>0"), my_sql.And("ID>=30"));
	//my_sql.deleteData("USER2", my_sql.Where("AGE>0"), NULL);
	//my_sql.deleteData("USER", NULL, NULL);

}


int main()
{
	std::cout << "start main" << std::endl;

	functest();


	std::cout << "end main" << std::endl;



}


