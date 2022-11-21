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



private:
	//sql
	sqlite3* db;

	//ƴ���ַ�
	//ƴ���ַ���Ҫ������������ʽ�������ֶ������ݣ���������ʽ���Ƶ�ʱ�򣬿��Կ��������ģ�������Ķ����ַ�����
	std::vector<char*> columns_vec;
	std::vector<char*> values_vec;
	std::vector<char*> where_vec;	//where_vecֻ����һ������
	std::vector<char*> and_vec;
	std::vector<char*> or_vec;
	//vector����˳��ģ�set��map������ģ�unorderset��map�������
    //��ʼ��ƴ���ַ����ڹ����ÿ��ָ�����ʱ���ã��������������
	void initVec();

public:

	//�򿪹ر�
	int openSQL(const char* sqlpath);
	int closeSQL();


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

	void* Where(char* condition);

	template<typename T>
	void* And(T condition);
	template<typename T, typename ...Args>
	void* And(T condition, Args ...conditions);

	template<typename T>
	void* Or(T condition);
	template<typename T, typename ...Args>
	void* Or(T condition, Args ...conditions);


	//���룺ָ����ͷ
	int insertData(const char* tablename, void* columns, void* values);
	//���룺Ĭ������
	int insertData(const char* tablename, void* values);
	
	//update tablename set column1=value1,column2=value2,...,columnN=valueN where[codition]
	//�����condition�Ƕ����ģ����Խ��and��or�����б�ͷɸѡ
	//�磺where ID=2
	//	  where NAME='Sam' and AGE>=32;
	//ϣ���ĺ����ṹ��sql.updateData("USER", sql.setcolumns("SCORE", "POWER"), sql.setvalues(23, 3), 
	//									sql.where("NAME", "='Sam'"), sql.and("AGE", ">=30"));
	//���£�ȫ���޸�
	int updateData(const char* tablename, void* columns, void* values);
	//���£�where�����޸�
	int updateData(const char* tablename, void* columns, void* values, void* where);
	//���£�where�����������������޸�
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






//ָ����ͷ
int sqlfunc::insertData(const char* tablename, void* columns, void* values){

	//���columns_vec��values_vec�Ĵ�С�Ƿ����
	if (this->columns_vec.size() != this->values_vec.size()) {
		this->initVec();
		return SQLITE_ERROR;
	}

	//INSERT INTO USER(ID,NAME,SCORE) VALUES(1,'WENJY',20.98);
	//����vector���ݣ�������ƥ��
	char sql_tablename[CHAR_MAX] = "";
	char sql_columns[CHAR_MAX] = "(";
	char sql_values[CHAR_MAX]="VALUES(";

	//�����sql_word����ʹ��static��ԭ���ǣ�char��ʵ��һ�����飬����������鳤��̫Сʱ���������ݹ����������������
	//��Ҫ���������⣬��õľ���ʹ��char*ָ��������������Ͳ������ַ�������
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
	char sql_tablename[CHAR_MAX] = "";
	char sql_values[CHAR_MAX] = "VALUES(";

	//�����sql_word����ʹ��static��ԭ���ǣ�char��ʵ��һ�����飬����������鳤��̫Сʱ���������ݹ����������������
	//��Ҫ���������⣬��õľ���ʹ��char*ָ��������������Ͳ������ַ�������
	char sql_word[CHAR_MAX] = "INSERT INTO ";

	//Ӧ�ò���Ҫ���column��value�������Ƿ��Ӧ��sqlite3�ײ����ʶ���
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

//������update��ȫ���޸�
int sqlfunc::updateData(const char* tablename, void* columns, void* values) {
	//���columns_vec��values_vec�Ĵ�С�Ƿ����
	if (this->columns_vec.size() != this->values_vec.size()) {
		this->initVec();
		return SQLITE_ERROR;
	}

	//UPDATE table_name SET column1=value1...., columnN=valueN WHERE ID=6;;
	//����char����ʱ��ֵ��ֵҲ��Ϊ��������ʼ��
	char sql_tablename[CHAR_MAX] = "";
	char sql_set[CHAR_MAX] = "SET ";

	//�����sql_word����ʹ��static��ԭ���ǣ�char��ʵ��һ�����飬����������鳤��̫Сʱ���������ݹ����������������
	//��Ҫ���������⣬��õľ���ʹ��char*ָ��������������Ͳ������ַ�������
	char sql_word[CHAR_MAX] = "UPDATE ";

	//Ӧ�ò���Ҫ���column��value�������Ƿ��Ӧ��sqlite3�ײ����ʶ���
	strcpy(sql_tablename, tablename);
	strcat(sql_word, sql_tablename);
	strcat(sql_word, " ");

	//��column��value������ϣ�ƴ�ӵ�set��
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

	//char[]��char*�Ľ��޺�ģ����char*���ǵ�ַ����char���׵�ַ��char�պþ��ǡ����Բ���Ҫ��ת��
	//char* p_chr = &(chr[0]);	��һ����ʵ�ǲ���Ҫ��
	int rec = sqlite3_exec(this->db, sql_word, NULL, NULL, NULL);

	if (rec != SQLITE_OK) {
		std::cout << "update data fail, errorcode is: " << rec << std::endl;
		this->initVec();
		return rec;
	}

	this->initVec();

	return SQLITE_OK;

}

//һ��where����update
int sqlfunc::updateData(const char* tablename, void* columns, void* values, void* where) {

	//���columns_vec��values_vec�Ĵ�С�Ƿ����
	if (this->columns_vec.size() != this->values_vec.size()) {
		this->initVec();
		return SQLITE_ERROR;
	}
	//whereֻ��С�ڵ���1
	if (this->where_vec.size() != 1) {
		this->initVec();
		return SQLITE_ERROR;
	}

	//UPDATE table_name SET column1=value1...., columnN=valueN WHERE ID=6;;
	//����char����ʱ��ֵ��ֵҲ��Ϊ��������ʼ��
	//���������������ַ��ĳ��ȡ���Ϊ��������ĳ����ǲ������ġ����Ƕ���һ��������������Ȼ���˷ѵ�
	//������������Ǹ��������vec����ȡchar*���ܳ��ȣ�Ȼ���ڴ˻����ϴ�����̬��char
	char sql_tablename[CHAR_MAX] = "";
	char sql_set[CHAR_MAX] = "SET ";
	char sql_where[CHAR_MAX] = "";

	//�����sql_word����ʹ��static��ԭ���ǣ�char��ʵ��һ�����飬����������鳤��̫Сʱ���������ݹ����������������
	//��Ҫ���������⣬��õľ���ʹ��char*ָ��������������Ͳ������ַ�������
	char sql_word[CHAR_MAX] = "UPDATE ";

	//Ӧ�ò���Ҫ���column��value�������Ƿ��Ӧ��sqlite3�ײ����ʶ���
	strcpy(sql_tablename, tablename);
	strcat(sql_word, sql_tablename);
	strcat(sql_word, " ");

	//��column��value������ϣ�ƴ�ӵ�set��
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

	//where����
	if (this->where_vec.size() == 1) {
		strcat(sql_where, "WHERE ");
		strcat(sql_where, this->where_vec[0]);
	}

	strcat(sql_word, sql_set);
	strcat(sql_word, sql_where);
	strcat(sql_word, ";");

	std::cout << "update(1 where) words is: " << sql_word << std::endl;

	//char[]��char*�Ľ��޺�ģ����char*���ǵ�ַ����char���׵�ַ��char�պþ��ǡ����Բ���Ҫ��ת��
	//char* p_chr = &(chr[0]);	��һ����ʵ�ǲ���Ҫ��
	int rec = sqlite3_exec(this->db, sql_word, NULL, NULL, NULL);

	if (rec != SQLITE_OK) {
		std::cout << "update data fail, errorcode is: " << rec << std::endl;
		this->initVec();
		return rec;
	}
	
	this->initVec();

	return SQLITE_OK;

}

//���where���ۺ�and��or��update
int sqlfunc::updateData(const char* tablename, void* columns, void* values, void* where, void* condition) {
	int rec = this->updateData(tablename, columns, values, where, condition, NULL);
	return rec;
}

//���where���ۺ�and��or��update
int sqlfunc::updateData(const char* tablename, void* columns, void* values, void* where, void* condition1, void* condition2){

	//���columns_vec��values_vec�Ĵ�С�Ƿ����
	if (this->columns_vec.size() != this->values_vec.size()) {
		this->initVec();
		return SQLITE_ERROR;
	}
	//whereֻ��Ϊ1
	if (this->where_vec.size() != 1) {
		this->initVec();
		return SQLITE_ERROR;
	}

	//UPDATE table_name SET column1=value1...., columnN=valueN WHERE ID=6;;
	//����char����ʱ��ֵ��ֵҲ��Ϊ��������ʼ��
	//���������������ַ��ĳ��ȡ���Ϊ��������ĳ����ǲ������ġ����Ƕ���һ��������������Ȼ���˷ѵ�
	//������������Ǹ��������vec����ȡchar*���ܳ��ȣ�Ȼ���ڴ˻����ϴ�����̬��char
	char sql_tablename[CHAR_MAX] = "";
	char sql_set[CHAR_MAX] = "SET ";
	char sql_where[CHAR_MAX] = "";
	char sql_and[CHAR_MAX] = "";
	char sql_or[CHAR_MAX] = "";

	//�����sql_word����ʹ��static��ԭ���ǣ�char��ʵ��һ�����飬����������鳤��̫Сʱ���������ݹ����������������
	//��Ҫ���������⣬��õľ���ʹ��char*ָ��������������Ͳ������ַ�������
	char sql_word[CHAR_MAX] = "UPDATE ";

	//Ӧ�ò���Ҫ���column��value�������Ƿ��Ӧ��sqlite3�ײ����ʶ���
	strcpy(sql_tablename, tablename);
	strcat(sql_word, sql_tablename);
	strcat(sql_word, " ");

	//��column��value������ϣ�ƴ�ӵ�set��
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

	//where����
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

	//char[]��char*�Ľ��޺�ģ����char*���ǵ�ַ����char���׵�ַ��char�պþ��ǡ����Բ���Ҫ��ת��
	//char* p_chr = &(chr[0]);	��һ����ʵ�ǲ���Ҫ��
	int rec = sqlite3_exec(this->db, sql_word, NULL, NULL, NULL);

	if (rec != SQLITE_OK) {
		std::cout << "update data fail, errorcode is: " << rec << std::endl;
		this->initVec();
		return rec;
	}

	this->initVec();

	return SQLITE_OK;

}
