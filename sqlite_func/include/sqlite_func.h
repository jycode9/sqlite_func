#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <sstream>
#include "../thirdparty/sqlite3.h"


//select������䣨�ص�����ʱ��̬��
namespace SelectData {

	int select_data_num;
	std::vector<std::vector<std::string>> select_columns;
	std::vector<std::vector<std::string>> select_values;

	//��ʼ������
	void initSelectVec() {
		select_columns.clear();
		select_values.clear();
		select_data_num = 0;
	}

}

//sqlfunc��
class sqlfunc {


	//���������
public:
	sqlfunc(const std::string& sqlpath);
	~sqlfunc();



	//�����
private:
	sqlite3* db;

	std::vector<std::string> columns_vec;
	std::vector<std::string> values_vec;
	std::vector<std::string> where_vec;	//where_vecֻ����һ������
	std::vector<std::string> and_vec;
	std::vector<std::string> or_vec;

	


	//��亯��
public:

	//����һ��Ԫ��ʱ�������Ԫ��ʱ�����һ��Ԫ�ص���
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




	//�����ĺ���
public:
	//����������תΪstring
	template <typename T>
	std::string convertAnyToString(T data);

	//��ʼ��
	void initWordsVec();



	//ֱ�Ӳ���ֵ�ĺ���
public:

	//�򿪹ر�
	int openSQL(const std::string& sqlpath);
	int closeSQL();

	//���룬ָ����ͷ
	int insertData(const std::string& tablename, void* columns, void* values);
	//���룬Ĭ������v2��û��ʹ�ã�
	int insertData(const std::string& tablename, void* values);

	//���£�ָ��where
	int updateData(const std::string& tablename, void* columns, void* values, void* where);

	//���ң�ָ��where��and��or



	//ʹ�ýṹ��ĺ���
public:
	//���룬���սṹ��
	int insertData_v2(const std::string& tablename, std::unordered_map<std::string, std::string>& values);

	//���£����սṹ��
	int updateData_v2(const std::string& tablename, std::unordered_map<std::string, std::string>& values);

};


//��ʼ�����
void sqlfunc::initWordsVec() {
	//��ʼ��vec�͸������
	this->columns_vec.clear();
	this->values_vec.clear();
	this->where_vec.clear();
	this->and_vec.clear();
	this->or_vec.clear();
	SelectData::initSelectVec();
}

//����
sqlfunc::sqlfunc(const std::string& sqlpath) {
	this->db = NULL;
	//�ڹ����ʱ������ݿ�
	this->openSQL(sqlpath);
	this->initWordsVec();
}

//����
sqlfunc::~sqlfunc() {
	//��������ʱ��ر����ݿ�
	this->closeSQL();
	//����������ʼ��
	this->initWordsVec();
}

//�����ݿ�
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

//�ر����ݿ�
int sqlfunc::closeSQL() {

	int rec = sqlite3_close_v2(this->db);

	if (rec != SQLITE_OK) {
		std::cout << "close sql fail, errorcode is: " << rec << std::endl;
		return rec;
	}

	return SQLITE_OK;
}


//����������תΪstring
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

	this->and_vec.push_back(this->convertAnyToString(condition));

	return SQLITE_OK;
}

template<typename T, typename ...Args>
void* sqlfunc::And(T condition, Args ...conditions) {

	this->and_vec.push_back(this->convertAnyToString(condition));
	this->And(conditions...);

	return SQLITE_OK;
}


template<typename T>
void* sqlfunc::Or(T condition) {
	this->or_vec.push_back(this->convertAnyToString(condition));

	return SQLITE_OK;
}

template<typename T, typename ...Args>
void* sqlfunc::Or(T condition, Args ...conditions) {
	this->or_vec.push_back(this->convertAnyToString(condition));
	this->Or(conditions...);

	return SQLITE_OK;
}


//���룬ָ����ͷ
int sqlfunc::insertData(const std::string& tablename, void* columns, void* values) {
	//INSERT INTO USER(ID,NAME,SCORE) VALUES(1,'WENJY',20.98);

	//���columns_vec��values_vec�Ĵ�С�Ƿ����
	if (this->columns_vec.size() != this->values_vec.size()) {
		this->initWordsVec();
		std::cout << "columns'num and values'num not equal" << std::endl;
		return SQLITE_ERROR;

	}

	std::string sql_tablename = tablename;
	std::string sql_columns = "(";
	std::string sql_values = "VALUES(";
	std::string sql_word = "INSERT INTO " + sql_tablename;

	for (unsigned int i = 0; i < this->columns_vec.size(); i++) {

		std::string t_column = this->columns_vec[i];
		sql_columns = sql_columns + t_column;
		if (i != this->columns_vec.size() - 1) {
			sql_columns = sql_columns + ",";
		}
	}
	sql_columns = sql_columns + ") ";

	for (unsigned int i = 0; i < this->values_vec.size(); i++) {

		std::string t_value = this->values_vec[i];
		sql_values = sql_values + "'" + t_value + "'";
		if (i != this->values_vec.size() - 1) {
			sql_values = sql_values + ",";
		}
	}
	sql_values = sql_values + ");";

	sql_word = sql_word + sql_columns + sql_values;
	std::cout << "sql words is: " << sql_word << std::endl;

	char sql_word_c[CHAR_MAX] = "";
	strcpy(sql_word_c, (char*)sql_word.c_str());
	int rec = sqlite3_exec(this->db, sql_word_c, NULL, NULL, NULL);
	if (rec != SQLITE_OK) {
		std::cout << "insert data fail, errorcode is: " << rec << std::endl;
		this->initWordsVec();
		return rec;
	}

	//ָ����ɺ���г�ʼ�����ں���ͷ���ܽ��У���Ϊ����ͷ�Ѿ���������亯��ִ���꣬�����
	this->initWordsVec();
	return SQLITE_OK;
}

//���룬Ĭ������
int sqlfunc::insertData(const std::string& tablename, void* values) {
	//INSERT INTO USER VALUES(1,'WENJY',20.98);

	std::string sql_tablename = tablename + " ";
	std::string sql_values = "VALUES(";
	std::string sql_word = "INSERT INTO " + sql_tablename;

	for (unsigned int i = 0; i < this->values_vec.size(); i++) {

		std::string t_value = this->values_vec[i];
		sql_values = sql_values + "'" + t_value + "'";
		if (i != this->values_vec.size() - 1) {
			sql_values = sql_values + ",";
		}
	}
	sql_values = sql_values + ");";

	sql_word = sql_word + sql_values;
	std::cout << "sql words is: " << sql_word << std::endl;

	char sql_word_c[CHAR_MAX] = "";
	strcpy(sql_word_c, (char*)sql_word.c_str());
	int rec = sqlite3_exec(this->db, sql_word_c, NULL, NULL, NULL);
	if (rec != SQLITE_OK) {
		std::cout << "insert data fail, errorcode is: " << rec << std::endl;
		this->initWordsVec();
		return rec;
	}

	//ָ����ɺ���г�ʼ�����ں���ͷ���ܽ��У���Ϊ����ͷ�Ѿ���������亯��ִ���꣬�����
	this->initWordsVec();
	return SQLITE_OK;
}



//���£�ָ��where
int sqlfunc::updateData(const std::string& tablename, void* columns, void* values, void* where) {
	//UPDATE USER2 SET NAME='Sam',AGE=23 WHERE ID=6;

	//���columns_vec��values_vec�Ĵ�С�Ƿ����
	if (this->columns_vec.size() != this->values_vec.size()) {
		this->initWordsVec();
		std::cout << "columns'num != values'num" << std::endl;
		return SQLITE_ERROR;
	}

	//whereֻ�ܵ���1
	if (this->where_vec.size() != 1) {
		this->initWordsVec();
		std::cout << "where's num != 1" << std::endl;
		return SQLITE_ERROR;
	}

	std::string sql_tablename = tablename;
	std::string sql_set = "SET ";
	std::string sql_where = "WHERE ";
	std::string sql_word = "UPDATE " + sql_tablename + " ";

	//ƴ��set
	for (unsigned int i = 0; i < this->columns_vec.size(); i++) {

		std::string t_set = this->columns_vec[i] + "=" + "'" + this->values_vec[i] + "'";
		if (i != this->columns_vec.size() - 1) {
			t_set = t_set + ",";
		}
		sql_set = sql_set + t_set;
	}
	sql_set = sql_set + " ";

	//where����
	sql_where = sql_where + this->where_vec[0];

	sql_word = sql_word + sql_set + sql_where + ";";
	std::cout << "sql words is: " << sql_word << std::endl;

	char sql_word_c[CHAR_MAX] = "";
	strcpy(sql_word_c, (char*)sql_word.c_str());
	int rec = sqlite3_exec(this->db, sql_word_c, NULL, NULL, NULL);
	if (rec != SQLITE_OK) {
		std::cout << "update data fail, errorcode is: " << rec << std::endl;
		this->initWordsVec();
		return rec;
	}

	//ָ����ɺ���г�ʼ�����ں���ͷ���ܽ��У���Ϊ����ͷ�Ѿ���������亯��ִ���꣬�����
	this->initWordsVec();
	return SQLITE_OK;
}




//���룬ʹ��map���ܲ���
int sqlfunc::insertData_v2(const std::string & tablename, std::unordered_map<std::string, std::string>& values) {
	
	std::vector<std::string> t_columns;
	std::vector<std::string> t_values;

	//ʹ�õ�����
	for (auto iter = values.begin(); iter != values.end(); iter++) {
		//std::cout << (*iter).first << ": " << (*iter).second << std::endl;
		t_columns.push_back((*iter).first);
		t_values.push_back((*iter).second);
	}

	//���ûص���������ƴ��
	for (int i = 0; i < t_columns.size(); i++) {
		this->Columns(t_columns[i]);
		this->Values(t_values[i]);
	}

	//����ԭʼ����
	int rec = this->insertData(tablename, this->Columns(""), this->Values(""));

	if (rec != SQLITE_OK) {
		return rec;
	}
	return SQLITE_OK;
}


//���£����սṹ��
int sqlfunc::updateData_v2(const std::string& tablename, std::unordered_map<std::string, std::string>& values) {

	std::vector<std::string> t_columns;
	std::vector<std::string> t_values;
	std::string t_where_column;
	std::string t_where_value;

	//ʹ�õ�����
	for (auto iter = values.begin(); iter != values.end(); iter++) {
		//������һ���Ϊwhere��
		if (iter == values.begin()) {
			t_where_column = (*iter).first;
			t_where_value = (*iter).second;
			iter++;
		}
		//std::cout << (*iter).first << ": " << (*iter).second << std::endl;
		t_columns.push_back((*iter).first);
		t_values.push_back((*iter).second);
	}

	//���ûص���������ƴ��
	for (int i = 0; i < t_columns.size(); i++) {
		this->Columns(t_columns[i]);
		this->Values(t_values[i]);
	}
	//where
	std::string t_where = t_where_column + "=" + "'" + t_where_value + "'";
	this->Where(t_where);

	//����ԭʼ����
	int rec = this->updateData(tablename, this->Columns(""), this->Values(""), this->Where(""));

	if (rec != SQLITE_OK) {
		return rec;
	}
	return SQLITE_OK;


}



