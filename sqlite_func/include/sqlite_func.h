#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <sstream>
#include "../thirdparty/sqlite3.h"



//select辅助语句（回调必须静态）
namespace SelectData {

	int select_data_num;
	std::vector<std::vector<std::string>> select_columns;
	std::vector<std::vector<std::string>> select_values;

	//初始化数据
	void initSelectVec() {
		select_columns.clear();
		select_values.clear();
		select_data_num = 0;
	}

}

//sqlfunc类
class sqlfunc {


	//构造和析构
public:
	sqlfunc(const std::string& sqlpath);
	~sqlfunc();



	//类变量
private:
	sqlite3* db;

	std::vector<std::string> columns_vec;
	std::vector<std::string> values_vec;
	std::vector<std::string> where_vec;	//where_vec只会有一个内容
	std::vector<std::string> and_vec;
	std::vector<std::string> or_vec;

	//回调函数
	static int callback(void* callfunc, int column_num, char** values, char** columns);


	//语句函数
public:

	//插入一个元素时、插入多元素时的最后一个元素调用
	template<typename T>
	void* Values(T value);
	template<typename T, typename ...Args>
	void* Values(T value, Args ...values);

	template<typename T>
	void* Columns(T column);
	template<typename T, typename ...Args>
	void* Columns(T column, Args ...columns);

	void* Where(const std::string& condition);

	template<typename T>
	void* And(T condition);
	template<typename T, typename ...Args>
	void* And(T condition, Args ...conditions);

	template<typename T>
	void* Or(T condition);
	template<typename T, typename ...Args>
	void* Or(T condition, Args ...conditions);




	//辅助的函数
private:
	//将任意类型转为string
	template <typename T>
	std::string convertAnyToString(T data);

	//初始化
	void initWordsVec();



	//直接操作值的函数
private:

	//打开关闭
	int openSQL(const std::string& sqlpath);
	int closeSQL();


public:
	//所有函数情况集成为一个，判断对应语句的vec大小。如果vec为空，则不添加语句关键字即可
	//插入，指定表头（当columns为NULL时，使用默认排序）
	int insertData(const std::string& tablename, void* columns, void* values);

	//更新，指定where，and，or
	template<typename T, typename ...Args>
	int updateData(const std::string& tablename, void* columns, void* values, void* where, T condition, Args ...conditions);

	//查找，指定where，and，or
	template<typename T, typename ...Args>
	int selectData(const std::string& tablename, std::vector<std::unordered_map<std::string, std::string>>& calldatas,
		void* where, T condition, Args ...conditions);

	//删除，指定where，and，or
	template<typename T, typename ...Args>
	int deleteData(const std::string& tablename, void* where, T condition, Args ...conditions);


	//使用map做指引的智能函数
public:
	//插入，按照map
	int insertData_v2(const std::string& tablename, std::unordered_map<std::string, std::string>& values);

	//更新，按照map
	//map第一个元素为主键
	int updateData_v2(const std::string& tablename, std::unordered_map<std::string, std::string>& values);

};


//初始化语句
void sqlfunc::initWordsVec() {
	//初始化vec和辅助语句
	this->columns_vec.clear();
	this->values_vec.clear();
	this->where_vec.clear();
	this->and_vec.clear();
	this->or_vec.clear();
	SelectData::initSelectVec();
}

//构造
sqlfunc::sqlfunc(const std::string& sqlpath) {
	this->db = NULL;
	//在构造的时候打开数据库
	this->openSQL(sqlpath);
	this->initWordsVec();
}

//析构
sqlfunc::~sqlfunc() {
	//在析构的时候关闭数据库
	this->closeSQL();
	//将辅助语句初始化
	this->initWordsVec();
}

//打开数据库
int sqlfunc::openSQL(const std::string& sqlpath) {

	char sql_path_c[CHAR_MAX] = "";
	strcpy(sql_path_c, (char*)sqlpath.c_str());

	int rec = sqlite3_open_v2(sql_path_c, &this->db, SQLITE_OPEN_READWRITE, NULL);

	if (rec != SQLITE_OK) {
		std::cout << "open sql fail, errorcode is: " << rec << std::endl;
		return rec;
	}

	return SQLITE_OK;
}

//关闭数据库
int sqlfunc::closeSQL() {

	int rec = sqlite3_close_v2(this->db);

	if (rec != SQLITE_OK) {
		std::cout << "close sql fail, errorcode is: " << rec << std::endl;
		return rec;
	}

	return SQLITE_OK;
}


//将任意类型转为string
template <typename T>
std::string sqlfunc::convertAnyToString(T data) {
	std::stringstream strstream;
	strstream.str("");
	strstream << data;
	return strstream.str();
}

template<typename T>
void* sqlfunc::Values(T value) {

	if (this->convertAnyToString(value) == "") {
		std::cout << "value null" << std::endl;
	}
	else {
		this->values_vec.push_back(this->convertAnyToString(value));
	}


	return SQLITE_OK;
}

template<typename T, typename ...Args>
void* sqlfunc::Values(T value, Args ...values) {

	if (this->convertAnyToString(value) == "") {
		std::cout << "value null" << std::endl;
	}
	else {
		this->values_vec.push_back(this->convertAnyToString(value));
	}
	this->Values(values...);

	return SQLITE_OK;

}


template<typename T>
void* sqlfunc::Columns(T column) {

	if (this->convertAnyToString(column) == "") {
		std::cout << "column null" << std::endl;
	}
	else {
		this->columns_vec.push_back(this->convertAnyToString(column));
	}


	return SQLITE_OK;
}

template<typename T, typename ...Args>
void* sqlfunc::Columns(T column, Args ...columns) {

	if (this->convertAnyToString(column) == "") {
		std::cout << "column null" << std::endl;
	}
	else {
		this->columns_vec.push_back(this->convertAnyToString(column));
	}

	this->Columns(columns...);

	return SQLITE_OK;
}


void* sqlfunc::Where(const std::string& condition) {

	if (this->convertAnyToString(condition) == "") {
		std::cout << "where null" << std::endl;
	}
	else {
		this->where_vec.push_back(this->convertAnyToString(condition));
	}
	

	return SQLITE_OK;
}


template<typename T>
void* sqlfunc::And(T condition) {

	if (this->convertAnyToString(condition) == "") {
		std::cout << "and null" << std::endl;
	}
	else {
		this->and_vec.push_back(this->convertAnyToString(condition));
	}
	

	return SQLITE_OK;
}

template<typename T, typename ...Args>
void* sqlfunc::And(T condition, Args ...conditions) {

	if (this->convertAnyToString(condition) == "") {
		std::cout << "and null" << std::endl;
	}
	else {
		this->and_vec.push_back(this->convertAnyToString(condition));
	}
	this->And(conditions...);

	return SQLITE_OK;
}


template<typename T>
void* sqlfunc::Or(T condition) {

	if (this->convertAnyToString(condition) == "") {
		std::cout << "or null" << std::endl;
	}
	else {
		this->or_vec.push_back(this->convertAnyToString(condition));
	}
	

	return SQLITE_OK;
}

template<typename T, typename ...Args>
void* sqlfunc::Or(T condition, Args ...conditions) {
	if (this->convertAnyToString(condition) == "") {
		std::cout << "or null" << std::endl;
	}
	else {
		this->or_vec.push_back(this->convertAnyToString(condition));
	}
	this->Or(conditions...);

	return SQLITE_OK;
}


//插入，指定表头
int sqlfunc::insertData(const std::string& tablename, void* columns, void* values) {
	//INSERT INTO USER(ID,NAME,SCORE) VALUES(1,'WENJY',20.98);

	std::string sql_tablename = tablename;
	std::string sql_columns = "";
	std::string sql_values = "VALUES(";
	std::string sql_word = "INSERT INTO " + sql_tablename;

	if (this->columns_vec.size() != 0) {
		sql_columns = sql_columns + "(";
	}

	for (unsigned int i = 0; i < this->columns_vec.size(); i++) {

		std::string t_column = this->columns_vec[i];
		sql_columns = sql_columns + t_column;
		if (i != this->columns_vec.size() - 1) {
			sql_columns = sql_columns + ",";
		}
	}

	if (this->columns_vec.size() != 0) {
		sql_columns = sql_columns + ")";
	}
	

	for (unsigned int i = 0; i < this->values_vec.size(); i++) {

		std::string t_value = this->values_vec[i];
		sql_values = sql_values + "'" + t_value + "'";
		if (i != this->values_vec.size() - 1) {
			sql_values = sql_values + ",";
		}
	}
	sql_values = sql_values + ");";

	sql_word = sql_word + sql_columns + " " + sql_values;
	std::cout << "sql words is: " << sql_word << std::endl;

	char sql_word_c[CHAR_MAX] = "";
	strcpy(sql_word_c, (char*)sql_word.c_str());
	char* err_msg_c="";
	int rec = sqlite3_exec(this->db, sql_word_c, NULL, NULL, &err_msg_c);
	if (rec != SQLITE_OK) {
		std::cout << "insert data fail, errorcode is: " << rec << std::endl;
		std::cout << "err msg is: " << err_msg_c << std::endl;
		this->initWordsVec();
		return rec;
	}

	//指令完成后进行初始化。在函数头不能进行，因为函数头已经将辅助语句函数执行完，会清空
	this->initWordsVec();
	return SQLITE_OK;
}


//更新，指定where
template<typename T, typename ...Args>
int sqlfunc::updateData(const std::string& tablename, void* columns, void* values, void* where, T condition, Args ...conditions) {
	//UPDATE USER2 SET NAME='Sam',AGE=23 WHERE ID=6 AND AGE>=20 AND AGE<=90;

	//检测columns_vec和values_vec的大小是否相等
	if (this->columns_vec.size() != this->values_vec.size()) {
		this->initWordsVec();
		std::cout << "columns'num != values'num" << std::endl;
		return SQLITE_ERROR;
	}

	std::string sql_tablename = tablename;
	std::string sql_set = "SET ";
	std::string sql_where = "";
	std::string sql_and = "";
	std::string sql_or = "";
	std::string sql_word = "UPDATE " + sql_tablename + " ";

	//拼接set
	for (unsigned int i = 0; i < this->columns_vec.size(); i++) {

		std::string t_set = this->columns_vec[i] + "=" + "'" + this->values_vec[i] + "'";
		if (i != this->columns_vec.size() - 1) {
			t_set = t_set + ",";
		}
		sql_set = sql_set + t_set;
	}
	sql_set = sql_set;

	//where条件
	if (this->where_vec.size() == 1) {
		sql_where = sql_where + " WHERE " + this->where_vec[0];
	}

	//and
	for (unsigned int i = 0; i < this->and_vec.size(); i++) {
		std::string t_and = "";
		t_and = t_and + " AND " + this->and_vec[i];
		sql_and = sql_and + t_and;
	}

	//or
	for (unsigned int i = 0; i < this->or_vec.size(); i++) {
		std::string t_or = "";
		t_or = t_or + " OR " + this->or_vec[i];
		sql_or = sql_or + t_or;
	}

	sql_word = sql_word + sql_set + sql_where + sql_and + sql_or + ";";
	std::cout << "sql words is: " << sql_word << std::endl;

	char sql_word_c[CHAR_MAX] = "";
	strcpy(sql_word_c, (char*)sql_word.c_str());
	char* err_msg = "";
	int rec = sqlite3_exec(this->db, sql_word_c, NULL, NULL, &err_msg);
	if (rec != SQLITE_OK) {
		std::cout << "update data fail, errorcode is: " << rec << std::endl;
		std::cout << "err msg is: " << err_msg << std::endl;
		this->initWordsVec();
		return rec;
	}

	//指令完成后进行初始化。在函数头不能进行，因为函数头已经将辅助语句函数执行完，会清空
	this->initWordsVec();
	return SQLITE_OK;
}


int sqlfunc::callback(void* callfunc, int column_num, char** values, char** columns) {

	std::vector<std::string> t_column_vec;
	std::vector<std::string> t_value_vec;

	for (int i = 0; i < column_num; i++) {

		std::string new_value = values[i];
		std::string new_column = columns[i];

		t_column_vec.push_back(new_column);
		t_value_vec.push_back(new_value);

	}

	SelectData::select_columns.push_back(t_column_vec);
	SelectData::select_values.push_back(t_value_vec);

	SelectData::select_data_num++;

	return SQLITE_OK;
}



//查找，指定where，and，or
template<typename T, typename ...Args>
int sqlfunc::selectData(const std::string& tablename, std::vector<std::unordered_map<std::string, std::string>>& calldatas,
	void* where, T condition, Args ...conditions) {
	//SELECT * FROM COMPANY WHERE NAME='Sam' AND ID>1 OR ID<5;

	std::string sql_tablename = tablename;
	std::string sql_where = "";
	std::string sql_and = "";
	std::string sql_or = "";

	std::string sql_word = "SELECT * FROM ";


	//where
	if (this->where_vec.size() == 1) {
		sql_where = sql_where + "WHERE " + this->where_vec[0];
	}

	//and
	for (unsigned int i = 0; i < this->and_vec.size(); i++) {
		std::string t_and = "";
		t_and = t_and + " AND " + this->and_vec[i];
		sql_and = sql_and + t_and;
	}

	//or
	for (unsigned int i = 0; i < this->or_vec.size(); i++) {
		std::string t_or = "";
		t_or = t_or + " OR " + this->or_vec[i];
		sql_or = sql_or + t_or;
	}

	sql_word = sql_word + sql_tablename + " " + sql_where + sql_and + sql_or + ";";

	std::cout << "sql words is: " << sql_word << std::endl;
	char sql_word_c[CHAR_MAX] = "";
	strcpy(sql_word_c, (char*)sql_word.c_str());
	char* err_msg = "";
	int rec = sqlite3_exec(this->db, sql_word_c, &this->callback, NULL, &err_msg);

	if (rec != SQLITE_OK) {
		std::cout << "select data fail, errorcode is: " << rec << std::endl;
		std::cout << "err msg is: " << err_msg << std::endl;
		this->initWordsVec();
		return rec;
	}

	for (int i = 0; i < SelectData::select_data_num; i++) {
		std::unordered_map<std::string, std::string> t_calldata;
		//std::cout << "**********the " << i << " data * *********" << std::endl;
		for (int j = 0; j < SelectData::select_columns[i].size(); j++) {
			//std::cout << "the column, value is: " << SelectData::select_columns[i][j] << ", " << SelectData::select_values[i][j] << std::endl;
			t_calldata[SelectData::select_columns[i][j]] = SelectData::select_values[i][j];
		}

		calldatas.push_back(t_calldata);
	}

	this->initWordsVec();
	return SQLITE_OK;
}



template<typename T, typename ...Args>
int sqlfunc::deleteData(const std::string& tablename, void* where, T condition, Args ...conditions) {
	//DELETE FROM COMPANY WHERE NAME='Sam' AND ID>1 OR ID<5;

	std::string sql_tablename = tablename;
	std::string sql_where = "";
	std::string sql_and = "";
	std::string sql_or = "";

	std::string sql_word = "DELETE FROM ";


	//where
	if (this->where_vec.size() == 1) {
		sql_where = sql_where + " WHERE " + this->where_vec[0];
	}

	//and
	for (unsigned int i = 0; i < this->and_vec.size(); i++) {
		std::string t_and = "";
		t_and = t_and + " AND " + this->and_vec[i];
		sql_and = sql_and + t_and;
	}

	//or
	for (unsigned int i = 0; i < this->or_vec.size(); i++) {
		std::string t_or = "";
		t_or = t_or + " OR " + this->or_vec[i];
		sql_or = sql_or + t_or;
	}

	sql_word = sql_word + sql_tablename + sql_where + sql_and + sql_or + ";";

	std::cout << "sql words is: " << sql_word << std::endl;
	char sql_word_c[CHAR_MAX] = "";
	strcpy(sql_word_c, (char*)sql_word.c_str());
	char* err_msg = "";
	int rec = sqlite3_exec(this->db, sql_word_c, &this->callback, NULL, &err_msg);
	if (rec != SQLITE_OK) {
		std::cout << "delete data fail, errorcode is: " << rec << std::endl;
		std::cout << "err msg is: " << err_msg << std::endl;
		this->initWordsVec();
		return rec;
	}

	this->initWordsVec();

	return SQLITE_OK;

}







//插入，按照map
int sqlfunc::insertData_v2(const std::string & tablename, std::unordered_map<std::string, std::string>& values) {
	
	std::vector<std::string> t_columns;
	std::vector<std::string> t_values;

	//使用迭代器
	for (auto iter = values.begin(); iter != values.end(); iter++) {
		//std::cout << (*iter).first << ": " << (*iter).second << std::endl;
		t_columns.push_back((*iter).first);
		t_values.push_back((*iter).second);
	}

	//调用回调函数进行拼接
	for (int i = 0; i < t_columns.size(); i++) {
		this->Columns(t_columns[i]);
		this->Values(t_values[i]);
	}

	//调用原始函数
	int rec = this->insertData(tablename, NULL, NULL);

	if (rec != SQLITE_OK) {
		return rec;
	}
	return SQLITE_OK;
}


//更新，按照map
int sqlfunc::updateData_v2(const std::string& tablename, std::unordered_map<std::string, std::string>& values) {

	std::vector<std::string> t_columns;
	std::vector<std::string> t_values;
	std::string t_where_column;
	std::string t_where_value;

	//使用迭代器
	for (auto iter = values.begin(); iter != values.end(); iter++) {
		//跳过第一项（作为where）
		if (iter == values.begin()) {
			t_where_column = (*iter).first;
			t_where_value = (*iter).second;
			iter++;
		}
		//std::cout << (*iter).first << ": " << (*iter).second << std::endl;
		t_columns.push_back((*iter).first);
		t_values.push_back((*iter).second);
	}

	//调用回调函数进行拼接
	for (int i = 0; i < t_columns.size(); i++) {
		this->Columns(t_columns[i]);
		this->Values(t_values[i]);
	}
	//where
	std::string t_where = t_where_column + "=" + "'" + t_where_value + "'";
	this->Where(t_where);

	//调用原始函数
	int rec = this->updateData(tablename, NULL, NULL, NULL, NULL);

	if (rec != SQLITE_OK) {
		return rec;
	}
	return SQLITE_OK;


}

