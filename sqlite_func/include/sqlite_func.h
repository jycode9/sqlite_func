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



private:
	//sql
	sqlite3* db;

	//拼接字符
	//拼接字符不要局限于数据形式，当出现多条数据，且数据形式类似的时候，可以考虑容器的（可输入的都是字符串）
	std::vector<char*> columns_vec;
	std::vector<char*> values_vec;
	std::vector<char*> where_vec;	//where_vec只会有一个内容
	std::vector<char*> and_vec;
	std::vector<char*> or_vec;
	//vector是有顺序的，set、map是无序的，unorderset、map是有序的
    //初始化拼接字符，在构造和每个指令结束时调用（正常结束与错误）
	void initVec();

public:

	//打开关闭
	int openSQL(const char* sqlpath);
	int closeSQL();


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

	void* Where(char* condition);

	template<typename T>
	void* And(T condition);
	template<typename T, typename ...Args>
	void* And(T condition, Args ...conditions);

	template<typename T>
	void* Or(T condition);
	template<typename T, typename ...Args>
	void* Or(T condition, Args ...conditions);


	//插入：指定表头
	int insertData(const char* tablename, void* columns, void* values);
	//插入：默认排序
	int insertData(const char* tablename, void* values);
	
	//update tablename set column1=value1,column2=value2,...,columnN=valueN where[codition]
	//这里的condition是多样的，可以结合and、or语句进行表头筛选
	//如：where ID=2
	//	  where NAME='Sam' and AGE>=32;
	//希望的函数结构：sql.updateData("USER", sql.setcolumns("SCORE", "POWER"), sql.setvalues(23, 3), 
	//									sql.where("NAME", "='Sam'"), sql.and("AGE", ">=30"));
	//更新：全部修改
	int updateData(const char* tablename, void* columns, void* values);
	//更新：where索引修改
	int updateData(const char* tablename, void* columns, void* values, void* where);
	//更新：where可增加条件索引的修改
	//template<typename T, typename ...Args> 
	//int updateData(const char* tablename, void* columns, void* values, T where, Args ...wheres);
	int updateData(const char* tablename, void* columns, void* values, void* where, void* condition);
	int updateData(const char* tablename, void* columns, void* values, void* where, void* condition1, void* condition2);

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

void* sqlfunc::Where(char* condition) {

	this->where_vec.push_back(condition);
	std::cout << "the vec where is: " << this->where_vec[0] << std::endl;

	return SQLITE_OK;
}

template<typename T>
void* sqlfunc::And(T condition) {
	this->and_vec.push_back((char*)condition);

	for (unsigned int i = 0; i < this->and_vec.size(); i++) {
		std::cout << "the vec and is: " << this->and_vec[i] << std::endl;
	}
	return SQLITE_OK;
}

template<typename T, typename ...Args>
void* sqlfunc::And(T condition, Args ...conditions) {
	this->and_vec.push_back((char*)condition);
	this->And(conditions...);

	return SQLITE_OK;
}

template<typename T>
void* sqlfunc::Or(T condition) {
	this->or_vec.push_back((char*)condition);

	for (unsigned int i = 0; i < this->or_vec.size(); i++) {
		std::cout << "the vec or is: " << this->or_vec[i] << std::endl;
	}
	return SQLITE_OK;
}

template<typename T, typename ...Args>
void* sqlfunc::Or(T condition, Args ...conditions) {
	this->or_vec.push_back((char*)condition);
	this->Or(conditions...);

	return SQLITE_OK;
}






//指定表头
int sqlfunc::insertData(const char* tablename, void* columns, void* values){

	//检测columns_vec和values_vec的大小是否相等
	if (this->columns_vec.size() != this->values_vec.size()) {
		this->initVec();
		return SQLITE_ERROR;
	}

	//INSERT INTO USER(ID,NAME,SCORE) VALUES(1,'WENJY',20.98);
	//便利vector内容，将数据匹配
	char sql_tablename[CHAR_MAX] = "";
	char sql_columns[CHAR_MAX] = "(";
	char sql_values[CHAR_MAX]="VALUES(";

	//这里的sql_word必须使用static的原因是？char其实是一种数组，当定义的数组长度太小时，插入内容过多就造成数据溢出了
	//想要解决这个问题，最好的就是使用char*指针来解决，这样就不限于字符长度了
	 char sql_word[CHAR_MAX]="INSERT INTO ";

	strcpy(sql_tablename, tablename);
	strcat(sql_word, sql_tablename);

	for (unsigned int i = 0; i < this->columns_vec.size(); i++) {
		
		char t_column[CHAR_MAX] = "";
		strcpy(t_column, this->columns_vec[i]);
		strcat(sql_columns, t_column);
		if (i!=this->columns_vec.size()-1) {
			strcat(sql_columns, ",");
		}
	}
	strcat(sql_columns, ") ");


	for (unsigned int i = 0; i < this->values_vec.size(); i++) {

		char t_value[CHAR_MAX] = "";
		strcpy(t_value, this->values_vec[i]);
		strcat(sql_values, t_value);
		if (i != this->values_vec.size() - 1) {
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
	char sql_tablename[CHAR_MAX] = "";
	char sql_values[CHAR_MAX] = "VALUES(";

	//这里的sql_word必须使用static的原因是？char其实是一种数组，当定义的数组长度太小时，插入内容过多就造成数据溢出了
	//想要解决这个问题，最好的就是使用char*指针来解决，这样就不限于字符长度了
	char sql_word[CHAR_MAX] = "INSERT INTO ";

	//应该不需要检测column和value的数量是否对应，sqlite3底层可以识别的
	strcpy(sql_tablename, tablename);
	strcat(sql_word, sql_tablename);
	strcat(sql_word, " ");

	for (unsigned int i = 0; i < this->values_vec.size(); i++) {

		char t_value[CHAR_MAX] = "";
		strcpy(t_value, this->values_vec[i]);
		strcat(sql_values, t_value);
		if (i != this->values_vec.size() - 1) {
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
	this->initVec();

	return SQLITE_OK;
}

//无条件update，全部修改
int sqlfunc::updateData(const char* tablename, void* columns, void* values) {
	//检测columns_vec和values_vec的大小是否相等
	if (this->columns_vec.size() != this->values_vec.size()) {
		this->initVec();
		return SQLITE_ERROR;
	}

	//UPDATE table_name SET column1=value1...., columnN=valueN WHERE ID=6;;
	//创建char变量时候赋值空值也是为了做到初始化
	char sql_tablename[CHAR_MAX] = "";
	char sql_set[CHAR_MAX] = "SET ";

	//这里的sql_word必须使用static的原因是？char其实是一种数组，当定义的数组长度太小时，插入内容过多就造成数据溢出了
	//想要解决这个问题，最好的就是使用char*指针来解决，这样就不限于字符长度了
	char sql_word[CHAR_MAX] = "UPDATE ";

	//应该不需要检测column和value的数量是否对应，sqlite3底层可以识别的
	strcpy(sql_tablename, tablename);
	strcat(sql_word, sql_tablename);
	strcat(sql_word, " ");

	//将column和value进行组合，拼接到set中
	for (unsigned int i = 0; i < this->columns_vec.size(); i++) {
		char t_column[CHAR_MAX] = "";
		char t_value[CHAR_MAX] = "";
		char t_set[CHAR_MAX] = "";

		strcpy(t_column, this->columns_vec[i]);
		strcpy(t_value, this->values_vec[i]);
		strcat(t_set, t_column);
		strcat(t_set, "=");
		strcat(t_set, t_value);

		if (i != this->columns_vec.size() - 1) {
			strcat(t_set, ",");
		}
		strcat(sql_set, t_set);
	}

	strcat(sql_word, sql_set);
	strcat(sql_word, ";");

	std::cout << "update(no where) words is: " << sql_word << std::endl;

	//char[]和char*的界限很模糊。char*就是地址，是char的首地址。char刚好就是。所以不需要再转的
	//char* p_chr = &(chr[0]);	这一步其实是不需要的
	int rec = sqlite3_exec(this->db, sql_word, NULL, NULL, NULL);

	if (rec != SQLITE_OK) {
		std::cout << "update data fail, errorcode is: " << rec << std::endl;
		this->initVec();
		return rec;
	}

	this->initVec();

	return SQLITE_OK;

}

//一个where条件update
int sqlfunc::updateData(const char* tablename, void* columns, void* values, void* where) {

	//检测columns_vec和values_vec的大小是否相等
	if (this->columns_vec.size() != this->values_vec.size()) {
		this->initVec();
		return SQLITE_ERROR;
	}
	//where只能小于等于1
	if (this->where_vec.size() != 1) {
		this->initVec();
		return SQLITE_ERROR;
	}

	//UPDATE table_name SET column1=value1...., columnN=valueN WHERE ID=6;;
	//创建char变量时候赋值空值也是为了做到初始化
	//现在最大的问题是字符的长度。因为参数输入的长度是不定长的。但是定义一个过长的数据显然是浪费的
	//这里可以利用那个输入完的vec，获取char*的总长度，然后在此基础上创建动态的char
	char sql_tablename[CHAR_MAX] = "";
	char sql_set[CHAR_MAX] = "SET ";
	char sql_where[CHAR_MAX] = "";

	//这里的sql_word必须使用static的原因是？char其实是一种数组，当定义的数组长度太小时，插入内容过多就造成数据溢出了
	//想要解决这个问题，最好的就是使用char*指针来解决，这样就不限于字符长度了
	char sql_word[CHAR_MAX] = "UPDATE ";

	//应该不需要检测column和value的数量是否对应，sqlite3底层可以识别的
	strcpy(sql_tablename, tablename);
	strcat(sql_word, sql_tablename);
	strcat(sql_word, " ");

	//将column和value进行组合，拼接到set中
	for (unsigned int i = 0; i < this->columns_vec.size(); i++) {
		char t_column[CHAR_MAX] = "";
		char t_value[CHAR_MAX] = "";
		char t_set[CHAR_MAX] = "";

		strcpy(t_column, this->columns_vec[i]);
		strcpy(t_value, this->values_vec[i]);
		strcat(t_set, t_column);
		strcat(t_set, "=");
		strcat(t_set, t_value);

		if (i != this->columns_vec.size() - 1) {
			strcat(t_set, ",");
		}

		strcat(sql_set, t_set);
	}
	strcat(sql_set, " ");

	//where条件
	if (this->where_vec.size() == 1) {
		strcat(sql_where, "WHERE ");
		strcat(sql_where, this->where_vec[0]);
	}

	strcat(sql_word, sql_set);
	strcat(sql_word, sql_where);
	strcat(sql_word, ";");

	std::cout << "update(1 where) words is: " << sql_word << std::endl;

	//char[]和char*的界限很模糊。char*就是地址，是char的首地址。char刚好就是。所以不需要再转的
	//char* p_chr = &(chr[0]);	这一步其实是不需要的
	int rec = sqlite3_exec(this->db, sql_word, NULL, NULL, NULL);

	if (rec != SQLITE_OK) {
		std::cout << "update data fail, errorcode is: " << rec << std::endl;
		this->initVec();
		return rec;
	}
	
	this->initVec();

	return SQLITE_OK;

}

//多个where，综合and、or的update
int sqlfunc::updateData(const char* tablename, void* columns, void* values, void* where, void* condition) {
	int rec = this->updateData(tablename, columns, values, where, condition, NULL);
	return rec;
}

//多个where，综合and、or的update
int sqlfunc::updateData(const char* tablename, void* columns, void* values, void* where, void* condition1, void* condition2){

	//检测columns_vec和values_vec的大小是否相等
	if (this->columns_vec.size() != this->values_vec.size()) {
		this->initVec();
		return SQLITE_ERROR;
	}
	//where只能为1
	if (this->where_vec.size() != 1) {
		this->initVec();
		return SQLITE_ERROR;
	}

	//UPDATE table_name SET column1=value1...., columnN=valueN WHERE ID=6;;
	//创建char变量时候赋值空值也是为了做到初始化
	//现在最大的问题是字符的长度。因为参数输入的长度是不定长的。但是定义一个过长的数据显然是浪费的
	//这里可以利用那个输入完的vec，获取char*的总长度，然后在此基础上创建动态的char
	char sql_tablename[CHAR_MAX] = "";
	char sql_set[CHAR_MAX] = "SET ";
	char sql_where[CHAR_MAX] = "";
	char sql_and[CHAR_MAX] = "";
	char sql_or[CHAR_MAX] = "";

	//这里的sql_word必须使用static的原因是？char其实是一种数组，当定义的数组长度太小时，插入内容过多就造成数据溢出了
	//想要解决这个问题，最好的就是使用char*指针来解决，这样就不限于字符长度了
	char sql_word[CHAR_MAX] = "UPDATE ";

	//应该不需要检测column和value的数量是否对应，sqlite3底层可以识别的
	strcpy(sql_tablename, tablename);
	strcat(sql_word, sql_tablename);
	strcat(sql_word, " ");

	//将column和value进行组合，拼接到set中
	for (unsigned int i = 0; i < this->columns_vec.size(); i++) {
		char t_column[CHAR_MAX] = "";
		char t_value[CHAR_MAX] = "";
		char t_set[CHAR_MAX] = "";

		strcpy(t_column, this->columns_vec[i]);
		strcpy(t_value, this->values_vec[i]);
		strcat(t_set, t_column);
		strcat(t_set, "=");
		strcat(t_set, t_value);

		if (i != this->columns_vec.size() - 1) {
			strcat(t_set, ",");
		}

		strcat(sql_set, t_set);
	}
	strcat(sql_set, " ");

	//where条件
	if (this->where_vec.size() == 1) {
		strcat(sql_where, "WHERE ");
		strcat(sql_where, this->where_vec[0]);
	}

	//and
	for (unsigned int i = 0; i < this->and_vec.size(); i++) {
		char t_and[CHAR_MAX] = "";
		char t_where[CHAR_MAX] = "";

		strcat(t_where, " AND ");
		strcpy(t_and, this->and_vec[i]);
		strcat(t_where, t_and);
		strcat(sql_and, t_where);
	}


	//or
	for (unsigned int i = 0; i < this->or_vec.size(); i++) {
		char t_or[CHAR_MAX] = "";
		char t_where[CHAR_MAX] = "";

		strcat(t_where, " OR ");
		strcpy(t_or, this->or_vec[i]);
		strcat(t_where, t_or);
		strcat(sql_or, t_where);
	}

	strcat(sql_word, sql_set);
	strcat(sql_word, sql_where);
	strcat(sql_word, sql_and);
	strcat(sql_word, sql_or);
	strcat(sql_word, ";");

	std::cout << "uodate(wheres) words is: " << sql_word << std::endl;

	//char[]和char*的界限很模糊。char*就是地址，是char的首地址。char刚好就是。所以不需要再转的
	//char* p_chr = &(chr[0]);	这一步其实是不需要的
	int rec = sqlite3_exec(this->db, sql_word, NULL, NULL, NULL);

	if (rec != SQLITE_OK) {
		std::cout << "update data fail, errorcode is: " << rec << std::endl;
		this->initVec();
		return rec;
	}

	this->initVec();

	return SQLITE_OK;

}
