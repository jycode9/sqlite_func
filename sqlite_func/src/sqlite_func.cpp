#include <iostream>
#include "../sqlite_func.h"

void functest() {

	sqlfunc my_sql("../src/test.db");

	//做成单例之后，使用宏定义将column、value进行简化
	//my_sql.insertData("USER", my_sql.Columns("ID", "NAME"), my_sql.Values("4", "'WENJY4'"));
	//my_sql.insertData("USER2", my_sql.Values("1", "'NAME_1'", "10"));

	//my_sql.updateData("USER2", my_sql.Columns("NAME"), my_sql.Values("'LILI'"));
	//my_sql.updateData("USER", my_sql.Columns("ID"), my_sql.Values("10"), my_sql.Where("NAME=='WENJY'"));
	//my_sql.updateData("USER2", my_sql.Columns("NAME"), my_sql.Values("'Sam'"), my_sql.Where("NAME=='LILI'"), my_sql.And("ID>=2", "AGE>=10"));

	//my_sql.deleteData("USER");
	//my_sql.deleteData("USER2", my_sql.Where("AGE==10"), my_sql.And("ID>5"), my_sql.And("NAME=='NAME_6'"));

	std::vector<std::map<char*, char*>> my_data;
	//my_sql.selectData("USER2", my_data);
	//my_sql.selectData("USER2", my_data, my_sql.Where("NAME=='Sam'"), my_sql.And("ID>0"));
	//my_sql.selectData("USER2", my_sql.Columns("NAME"), my_data);

	//for (int i = 0; i < my_data.size(); i++) {
	//	for (auto iter = my_data[i].begin(); iter != my_data[i].end(); iter++) {
	//		std::cout << "the column, value is: " << iter->first << ", " << iter->second << std::endl;
	//	}
	//	std::cout << "***************" << std::endl;

	//}

	//my_data.clear();

	my_sql.selectData("USER2", my_data);
	my_sql.selectData("USER2", my_sql.Columns("NAME"), my_data);


}


int main()
{
	std::cout << "start main" << std::endl;

	functest();


	std::cout << "end main" << std::endl;



}


