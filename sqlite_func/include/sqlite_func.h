#pragma once
#include <iostream>
#include <vector>
#include "../thirdparty/sqlite3.h"

//sqlite3的封装函数
//因为使用了模板函数，所以函数的声明和实现都在头文件中


//首先要做一个单例
class sqlfunc {

public:
	sqlfunc(const char* sqlpath);
	~sqlfunc();

	//sql
	sqlite3* db;

	//打开关闭
	int openSQL(const char* sqlpath);
	int closeSQL();

public:
	//拼接字符
	//拼接字符不要局限于数据形式，当出现多条数据，且数据形式类似的时候，可以考虑容器的（可输入的都是字符串）
	std::vector<char*> columns_vec;
	std::vector<char*> values_vec;
	std::vector<char*> where_vec;
	std::vector<char*> and_vec;
	std::vector<char*> or_vec;
	//vector是有顺序的，set、map是无序的，unorderset、map是有序的
    //初始化拼接字符，在构造和每个指令结束时调用（正常结束与错误）
	void initVec();

public:
	//插入一个元素时、插入多元素时的最后一个元素调用
	template<typename T>
	void* Values(T value);

	//只有模板函数能够实现多参数
	template<typename T, typename ...Args>
	void* Values(T value, Args ...values);

	template<typename T>
	void* Columns(T column);

	template<typename T, typename ...Args>
	void* Columns(T column, Args ...columns);



	
	template<typename T>
	void* where_column(T input_word);
	template<typename T>
	void* where_value(T input_word);

	void* where(void* where_column, void* where_value);

	//插入：指定表头
	int insertData(const char* tablename, void* columns, void* values);
	//插入：默认排序
	int insertData(const char* tablename, void* values);
	
	//update tablename set column1=value1,column2=value2,...,columnN=valueN where[codition]
	//这里的condition是多样的，可以结合and、or语句进行表头筛选
	//如：where ID=2
	//	  where NAME='Sam' and AGE>=32;
	//希望的函数结构：sql.updateData("USER", sql.setcolumns("SCORE", "POWER"), sql.setvalues(23, 3), 
	//									sql.where("NAME", "='Sam'"), sql.andwhere("AGE", ">=30"));
	//更新：可增加条件
	int updateData(const char* tablename, const char* where_column, const char* where_value,
		const char* set_word, const char* set_value);

	//int deleteData(const char* tablename, const char* where_column, const char* where_value);

	//callback
	//typedef int (*sqlite3_callback)(
	//	void*,    /* Data provided in the 4th argument of sqlite3_exec() */
	//	int,      /* The number of columns in row */
	//	char**,   /* An array of strings representing fields in the row */
	//	char**    /* An array of strings representing column names */
	//	);
	//int callback(void* func, int values_num, char** values, char** columns_name);

	//查找
	//int findData(const char* tablename, const char* where_column);
};


void sqlfunc::initVec() {
	//初始化vec
	this->columns_vec.clear();
	this->values_vec.clear();
	this->where_vec.clear();
	this->and_vec.clear();
	this->or_vec.clear();
}



sqlfunc::sqlfunc(const char* sqlpath) {
	db = NULL;
	//在构造的时候打开数据库
	this->openSQL(sqlpath);
	this->initVec();
}

sqlfunc::~sqlfunc() {
	//在析构的时候关闭数据库
	this->closeSQL();
	//析构时候要删除指针
	this->columns_vec.clear();
	this->values_vec.clear();
}



int sqlfunc::openSQL(const char* sqlpath) {

	int rec = sqlite3_open_v2(sqlpath, &this->db, SQLITE_OPEN_READWRITE, NULL);

	if (rec != SQLITE_OK) {
		std::cout << "open sql fail, errorcode is: " << rec << std::endl;
		return rec;
	}

	return SQLITE_OK;
}


int sqlfunc::closeSQL() {
	int rec = sqlite3_close_v2(this->db);

	if (rec != SQLITE_OK) {
		std::cout << "close sql fail, errorcode is: " << rec << std::endl;
		return rec;
	}

	return SQLITE_OK;
}


template<typename T>
void* sqlfunc::Values(T value) {

	this->values_vec.push_back((char*)value);

	for (unsigned int i = 0; i < this->values_vec.size(); i++) {
		std::cout << "the vec value is: " << this->values_vec[i] << std::endl;
	}
	return SQLITE_OK;
}

//只有模板函数能够实现多参数
template<typename T, typename ...Args>
void* sqlfunc::Values(T value, Args ...values) {

	this->values_vec.push_back((char*)value);
	this->Values(values...);

	return SQLITE_OK;

}

template<typename T>
void* sqlfunc::Columns(T column) {

	this->columns_vec.push_back((char*)column);

	for (unsigned int i = 0; i < this->columns_vec.size(); i++) {
		std::cout << "the vec column is: " << this->columns_vec[i] << std::endl;
	}
	return SQLITE_OK;
}

template<typename T, typename ...Args>
void* sqlfunc::Columns(T column, Args ...columns) {

	this->columns_vec.push_back((char*)column);
	this->Columns(columns...);

	return SQLITE_OK;

}






//指定表头
int sqlfunc::insertData(const char* tablename, void* columns, void* values){
	//INSERT INTO USER(ID,NAME,SCORE) VALUES(1,'WENJY',20.98);
	//便利vector内容，将数据匹配
	char sql_tablename[20] = "";
	char sql_columns[20] = "(";
	char sql_values[20]="VALUES(";

	//这里的sql_word必须使用static的原因是？char其实是一种数组，当定义的数组长度太小时，插入内容过多就造成数据溢出了
	//想要解决这个问题，最好的就是使用char*指针来解决，这样就不限于字符长度了
	 char sql_word[60]="INSERT INTO ";

	//应该不需要检测column和value的数量是否对应，sqlite3底层可以识别的
	strcpy(sql_tablename, tablename);
	strcat(sql_word, sql_tablename);

	for (unsigned int i = 0; i < this->columns_vec.size(); i++) {
		
		char t_column[20];
		strcpy(t_column, this->columns_vec[i]);
		strcat(sql_columns, t_column);
		if (i!=this->columns_vec.size()-1) {
			strcat(sql_columns, ",");
		}
	}
	strcat(sql_columns, ") ");


	for (unsigned int j = 0; j < this->values_vec.size(); j++) {

		char t_value[20];
		strcpy(t_value, this->values_vec[j]);
		strcat(sql_values, t_value);
		if (j != this->values_vec.size() - 1) {
			strcat(sql_values, ",");
		}
	}
	strcat(sql_values, ");");


	strcat(sql_word, sql_columns);
	strcat(sql_word, sql_values);
	std::cout << "insert(column) words is: " << sql_word << std::endl;
	
	//char[]和char*的界限很模糊。char*就是地址，是char的首地址。char刚好就是。所以不需要再转的
	//char* p_chr = &(chr[0]);	这一步其实是不需要的
	int rec = sqlite3_exec(this->db, sql_word, NULL, NULL, NULL);

	if (rec != SQLITE_OK) {
		std::cout << "insert data fail, errorcode is: " << rec << std::endl;
		this->initVec();
		return rec;
	}
	this->initVec();
	
	return SQLITE_OK;
}

//默认表头
int sqlfunc::insertData(const char* tablename, void* values) {
	//创建char变量时候赋值空值也是为了做到初始化
	char sql_tablename[20] = "";
	char sql_values[20] = "VALUES(";

	//这里的sql_word必须使用static的原因是？char其实是一种数组，当定义的数组长度太小时，插入内容过多就造成数据溢出了
	//想要解决这个问题，最好的就是使用char*指针来解决，这样就不限于字符长度了
	char sql_word[60] = "INSERT INTO ";

	//应该不需要检测column和value的数量是否对应，sqlite3底层可以识别的
	strcpy(sql_tablename, tablename);
	strcat(sql_word, sql_tablename);
	strcat(sql_word, " ");

	for (unsigned int j = 0; j < this->values_vec.size(); j++) {

		char t_value[20];
		strcpy(t_value, this->values_vec[j]);
		strcat(sql_values, t_value);
		if (j != this->values_vec.size() - 1) {
			strcat(sql_values, ",");
		}
	}
	strcat(sql_values, ");");

	strcat(sql_word, sql_values);
	std::cout << "insert(no columns) words is: " << sql_word << std::endl;

	//char[]和char*的界限很模糊。char*就是地址，是char的首地址。char刚好就是。所以不需要再转的
	//char* p_chr = &(chr[0]);	这一步其实是不需要的
	int rec = sqlite3_exec(this->db, sql_word, NULL, NULL, NULL);

	if (rec != SQLITE_OK) {
		std::cout << "insert data fail, errorcode is: " << rec << std::endl;
		this->initVec();
		return rec;
	}
	//this->initVec();

	return SQLITE_OK;
}


/*
int sqlfunc::updateData(const char* tablename, const char* where_column, const char* where_value,
	const char* set_word, const char* set_value) {

	UPDATE COMPANY SET ADDRESS = 'Texas' WHERE ID = 6;
	char sql_tablename[20];
	char sql_where_column[20];
	char sql_where_value[20];
	char sql_set_word[20];
	char sql_set_value[20];

	static char sql_word[30] = "UPDATE ";

	strcpy(sql_tablename, tablename);
	strcat(sql_word, sql_tablename);
	strcat(sql_word, " SET ");

	strcpy(sql_set_word, set_word);
	strcat(sql_word, sql_set_word);
	strcat(sql_word, " = ");

	strcpy(sql_set_value, set_value);
	strcat(sql_word, sql_set_value);
	
	strcat(sql_word, " WHERE ");

	strcpy(sql_where_column, where_column);
	strcat(sql_word, sql_where_column);
	strcat(sql_word, " = ");

	strcpy(sql_where_value, where_value);
	strcat(sql_word, sql_where_value);
	strcat(sql_word, ";");

	char* sql = &sql_word[0];
	std::cout << "the sql words is: " << sql << std::endl;

	char* errmsg = NULL;
	int rec = sqlite3_exec(this->db, sql, NULL, NULL, NULL);

	if (rec != SQLITE_OK) {
		std::cout << "update data fail, errorcode is: " << rec << std::endl;
		return rec;
	}

	return SQLITE_OK;

}



int sqlfunc::deleteData(const char* tablename, const char* where_column, const char* where_value) {

	DELETE FROM COMPANY WHERE ID = 7;
	char sql_tablename[20];
	char sql_where_column[20];
	char sql_where_value[20];

	static char sql_word[30] = "DELETE FROM ";

	strcpy(sql_tablename, tablename);
	strcat(sql_word, sql_tablename);
	strcat(sql_word, " WHERE ");

	strcpy(sql_where_column, where_column);
	strcat(sql_word, sql_where_column);
	strcat(sql_word, " = ");

	strcpy(sql_where_value, where_value);
	strcat(sql_word, sql_where_value);
	strcat(sql_word, ";");

	char* sql = &sql_word[0];
	std::cout << "the sql words is: " << sql << std::endl;

	char* errmsg = NULL;
	int rec = sqlite3_exec(this->db, sql, NULL, NULL, NULL);

	if (rec != SQLITE_OK) {
		std::cout << "delete data fail, errorcode is: " << rec << std::endl;
		return rec;
	}

	return SQLITE_OK;

}



int sqlfunc::callback(void* func, int values_num, char** values, char** columns_name) {
	std::cout << "callback is running" << std::endl;
	for (int i = 0; i < values_num; i++) {
		std::cout << "***the " << columns_name[i] << " 's value is: " << values[i] << std::endl;
	}

	return SQLITE_OK;
}
*/

//int sqlfunc::findData(const char* tablename, const char* where_column) {
//
//	//SELECT ID FROM COMPANY
//	char sql_tablename[20];
//	char sql_where_column[20];
//	char sql_where_value[20];
//
//	static char sql_word[30] = "SELECT ";
//
//	strcpy(sql_where_column, where_column);
//	strcat(sql_word, sql_where_column);
//	strcat(sql_word, " FROM ");
//
//	strcpy(sql_tablename, tablename);
//	strcat(sql_word, sql_tablename);
//
//	char* sql = &sql_word[0];
//	std::cout << "the sql words is: " << sql << std::endl;
//
//	int rec = sqlite3_exec(this->db, sql, NULL, NULL, NULL);
//
//	if (rec != SQLITE_OK) {
//		std::cout << "find data fail, errorcode is: " << rec << std::endl;
//		return rec;
//	}
//
//	return SQLITE_OK;
//}

