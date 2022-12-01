#include <iostream>
#include "../sqlite_func.h"








int functest() {

	//以类的形式实现
	sqlfunc my_sql;

	//sql_func沿用sqlite3的错误码，0为OK，1为通用ERR，其他值有具体错误含义
	int rec = SQLITE_OK;

	//数据库需要手动打开。在类析构时自动关闭
	//当数据库路径不存在时，不会自动创建，返回错误
	//sql_func函数内使用std::cout进行提醒和错误语句打印
	rec = my_sql.openSQL("../src/test.db");
	if (rec != SQLITE_OK) {
		return SQLITE_ERROR;
	}
	std::cout << "******1" << std::endl;





	//insert插入（自定义column顺序）
	//insertData( 表名， column内容， value内容）
	//当有主键的时候，插入的主键重复会报错
	//当输入的column和value数量不一致会报错
	//当自定义column顺序与value顺序不一致（sqlite3识别报错）时，会报错（一般数据格式兼容性很高）
	rec = my_sql.insertData("USER2", my_sql.Columns("NAME", "ID", "AGE"), my_sql.Values("Sam", 1, 23.33));
	std::cout << "******2" << std::endl;

	//insert插入（默认column顺序）
	//将column设空，则自动按照数据库中column顺序匹配插入
	//当value和column数量不一致，会报错
	rec = my_sql.insertData("USER2", NULL, my_sql.Values(2, "Jim", 30.23));
	std::cout << "******3" << std::endl;



	//delete删除（指定where、and、or条件）
	//条件where、and、or单个语句只有一个值，但是可以多个累加
	//使用and、or的前提是有where
	//注意的是条件中有字符串需写单引号：NAME='Sam'
	rec = my_sql.deleteData("USER2", my_sql.Where("ID>2"), NULL);
	std::cout << "******4" << std::endl;

	rec = my_sql.deleteData("USER2", my_sql.Where("ID>2"), my_sql.And("ID<10"));
	std::cout << "******5" << std::endl;

	rec = my_sql.deleteData("USER2", my_sql.Where("ID>2"), my_sql.And("ID<10"), my_sql.Or("NAME='LILI'"));
	std::cout << "******6" << std::endl;

	rec = my_sql.deleteData("USER2", my_sql.Where("ID>2"), my_sql.And("ID<10"), my_sql.And("NAME='Sam'"));
	std::cout << "******7" << std::endl;

	rec = my_sql.deleteData("USER2", NULL, my_sql.And("ID<10"), my_sql.And("NAME='Sam'"));	//会报错
	std::cout << "******8" << std::endl;

	//delete删除（全部删除）
	rec = my_sql.deleteData("USER2", NULL, NULL);
	std::cout << "******9" << std::endl;





	//select函数的回调数据
	//使用vec+unorder map的形式。vec为返回的数据条数，map主键first为column名，second为数据
	//暂时使用string格式
	std::vector<std::unordered_map<std::string, std::string>> select_call;
	//select指令的where、and、or使用方法与delete一致（update等也是一样的用法）
	rec = my_sql.selectData("USER2", select_call, my_sql.Where("ID=2"), NULL);
	std::cout << "******10" << std::endl;
	select_call.clear();

	rec = my_sql.selectData("USER2", select_call, my_sql.Where("ID=2"), my_sql.And("AGE>=30"));
	std::cout << "******11" << std::endl;
	select_call.clear();

	//获取全部
	rec = my_sql.selectData("USER2", select_call, NULL, NULL);
	std::cout << "******12" << std::endl;
	//以获取全部为例，遍历拿到全部数据
	if (rec == SQLITE_OK) {
		for (int i = 0; i < select_call.size(); i++) {
			std::cout << "///////" << "  get the number " << i << " data:" << std::endl;
			for (auto itr = select_call[i].begin(); itr != select_call[i].end(); itr++) {
				std::cout << "column, value is: " << (*itr).first << ", " << (*itr).second << std::endl;
			}
		}
	}
	std::cout << "******13" << std::endl;
	select_call.clear();


	//update更新
	//具体的条件用法和前面的一样
	//需要更改的column和value的顺序需要对应起来
	rec = my_sql.updateData("USER2", my_sql.Columns("NAME", "AGE"), my_sql.Values("Peter", 23.3), my_sql.Where("ID=6"), NULL);
	std::cout << "******14" << std::endl;

	//update全部更新
	rec = my_sql.updateData("USER2", my_sql.Columns("NAME"), my_sql.Values("Peter"), NULL, NULL);
	std::cout << "******15" << std::endl;



	//insertv2函数，使用map来指定插入的column和value
	std::unordered_map<std::string, std::string > insert_map;
	insert_map["ID"] = "9";
	insert_map["NAME"] = "Tim";
	insert_map["AGE"] = "12.9";
	rec = my_sql.insertData_v2("USER2", insert_map);
	std::cout << "******16" << std::endl;

	//updatev2函数，使用map指定修改的内容。这里map的第0个内容会被作为where的条件（只支持一个条件，且为等于）
	//如下面的程序中，条件是： WHERE ID=9;
	std::unordered_map<std::string, std::string > update_map;
	update_map["ID"] = "9";
	update_map["NAME"] = "Sim";
	update_map["AGE"] = "30.9";
	rec = my_sql.updateData_v2("USER2", update_map);
	std::cout << "******17" << std::endl;

	return SQLITE_OK;
}


int main()
{
	std::cout << "start main" << std::endl;

	functest();


	std::cout << "end main" << std::endl;



}


