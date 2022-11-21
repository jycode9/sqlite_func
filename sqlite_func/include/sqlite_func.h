#pragma once
#include <iostream>
#include <vector>
#include "../thirdparty/sqlite3.h"

//sqlite3�ķ�װ����
//��Ϊʹ����ģ�庯�������Ժ�����������ʵ�ֶ���ͷ�ļ���


//����Ҫ��һ������
class sqlfunc {

public:
	sqlfunc(const char* sqlpath);
	~sqlfunc();

	//sql
	sqlite3* db;

	//�򿪹ر�
	int openSQL(const char* sqlpath);
	int closeSQL();

public:
	//ƴ���ַ�
	//ƴ���ַ���Ҫ������������ʽ�������ֶ������ݣ���������ʽ���Ƶ�ʱ�򣬿��Կ��������ģ�������Ķ����ַ�����
	std::vector<char*> columns_vec;
	std::vector<char*> values_vec;
	std::vector<char*> where_vec;
	std::vector<char*> and_vec;
	std::vector<char*> or_vec;
	//vector����˳��ģ�set��map������ģ�unorderset��map�������
    //��ʼ��ƴ���ַ����ڹ����ÿ��ָ�����ʱ���ã��������������
	void initVec();

public:
	//����һ��Ԫ��ʱ�������Ԫ��ʱ�����һ��Ԫ�ص���
	template<typename T>
	void* Values(T value);

	//ֻ��ģ�庯���ܹ�ʵ�ֶ����
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

	//���룺ָ����ͷ
	int insertData(const char* tablename, void* columns, void* values);
	//���룺Ĭ������
	int insertData(const char* tablename, void* values);
	
	//update tablename set column1=value1,column2=value2,...,columnN=valueN where[codition]
	//�����condition�Ƕ����ģ����Խ��and��or�����б�ͷɸѡ
	//�磺where ID=2
	//	  where NAME='Sam' and AGE>=32;
	//ϣ���ĺ����ṹ��sql.updateData("USER", sql.setcolumns("SCORE", "POWER"), sql.setvalues(23, 3), 
	//									sql.where("NAME", "='Sam'"), sql.andwhere("AGE", ">=30"));
	//���£�����������
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

	//����
	//int findData(const char* tablename, const char* where_column);
};


void sqlfunc::initVec() {
	//��ʼ��vec
	this->columns_vec.clear();
	this->values_vec.clear();
	this->where_vec.clear();
	this->and_vec.clear();
	this->or_vec.clear();
}



sqlfunc::sqlfunc(const char* sqlpath) {
	db = NULL;
	//�ڹ����ʱ������ݿ�
	this->openSQL(sqlpath);
	this->initVec();
}

sqlfunc::~sqlfunc() {
	//��������ʱ��ر����ݿ�
	this->closeSQL();
	//����ʱ��Ҫɾ��ָ��
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

//ֻ��ģ�庯���ܹ�ʵ�ֶ����
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






//ָ����ͷ
int sqlfunc::insertData(const char* tablename, void* columns, void* values){
	//INSERT INTO USER(ID,NAME,SCORE) VALUES(1,'WENJY',20.98);
	//����vector���ݣ�������ƥ��
	char sql_tablename[20] = "";
	char sql_columns[20] = "(";
	char sql_values[20]="VALUES(";

	//�����sql_word����ʹ��static��ԭ���ǣ�char��ʵ��һ�����飬����������鳤��̫Сʱ���������ݹ����������������
	//��Ҫ���������⣬��õľ���ʹ��char*ָ��������������Ͳ������ַ�������
	 char sql_word[60]="INSERT INTO ";

	//Ӧ�ò���Ҫ���column��value�������Ƿ��Ӧ��sqlite3�ײ����ʶ���
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
	
	//char[]��char*�Ľ��޺�ģ����char*���ǵ�ַ����char���׵�ַ��char�պþ��ǡ����Բ���Ҫ��ת��
	//char* p_chr = &(chr[0]);	��һ����ʵ�ǲ���Ҫ��
	int rec = sqlite3_exec(this->db, sql_word, NULL, NULL, NULL);

	if (rec != SQLITE_OK) {
		std::cout << "insert data fail, errorcode is: " << rec << std::endl;
		this->initVec();
		return rec;
	}
	this->initVec();
	
	return SQLITE_OK;
}

//Ĭ�ϱ�ͷ
int sqlfunc::insertData(const char* tablename, void* values) {
	//����char����ʱ��ֵ��ֵҲ��Ϊ��������ʼ��
	char sql_tablename[20] = "";
	char sql_values[20] = "VALUES(";

	//�����sql_word����ʹ��static��ԭ���ǣ�char��ʵ��һ�����飬����������鳤��̫Сʱ���������ݹ����������������
	//��Ҫ���������⣬��õľ���ʹ��char*ָ��������������Ͳ������ַ�������
	char sql_word[60] = "INSERT INTO ";

	//Ӧ�ò���Ҫ���column��value�������Ƿ��Ӧ��sqlite3�ײ����ʶ���
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

	//char[]��char*�Ľ��޺�ģ����char*���ǵ�ַ����char���׵�ַ��char�պþ��ǡ����Բ���Ҫ��ת��
	//char* p_chr = &(chr[0]);	��һ����ʵ�ǲ���Ҫ��
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

