#pragma once
#include <iostream>
#include <vector>
#include <map>
#include "../thirdparty/sqlite3.h"

//sqlite3�ķ�װ����
//��Ϊʹ����ģ�庯�������Ժ�����������ʵ�ֶ���ͷ�ļ���
namespace SelectData {
	int select_data_num;
	std::vector<std::vector<char*>> select_columns;
	std::vector<std::vector<char*>> select_values;
	
	//��ʼ������
	//������ɾ������ʹ�õ�������char*delete��
	//����Ƕ�ָ����ͷţ�ÿ�κ�������ǰ������ʱʹ��
	void initSelectData() {

		//�ⲿvec���м�������
		for (int i = 0; i < select_columns.size(); i++) {
			//�ڲ�vec���м���column
			//iter���ŵľ���char*������
			for (auto iter = select_columns[i].begin(); iter!= select_columns[i].end(); iter++) {
				char* pt = (char*)*iter;
				delete pt;
			}
		}

		for (int i = 0; i < select_values.size(); i++) {
			//�ڲ�vec���м���column
			//iter���ŵľ���char*������
			for (auto iter = select_values[i].begin(); iter != select_values[i].end(); iter++) {
				char* pt = (char*)*iter;
				delete pt;
			}
		}

		//������clear���ܹ�ɾ���ڲ�new��char*����Ҫ����ȥ�ͷ�
		select_columns.clear();
		select_values.clear();
		select_data_num = 0;
	}
}

using namespace SelectData;

//����Ҫ��һ������
class sqlfunc {

public:
	sqlfunc(const char* sqlpath);
	~sqlfunc();

private:
	sqlite3* db;

	std::vector<char*> columns_vec;
	std::vector<char*> values_vec;
	std::vector<char*> where_vec;	//where_vecֻ����һ������
	std::vector<char*> and_vec;
	std::vector<char*> or_vec;

	void initWordsVec();
	static int getSelectColumnNum();

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

	//���£�ȫ���޸�
	int updateData(const char* tablename, void* columns, void* values);
	//���£�where����
	int updateData(const char* tablename, void* columns, void* values, void* where);
	//���£�and��or����
	template<typename T, typename ...Args>
	int updateData(const char* tablename, void* columns, void* values, void* where, T condition, Args ...conditions);


	//ɾ����ȫ��ɾ��
	int deleteData(const char* tablename);
	//ɾ����where����
	int deleteData(const char* tablename, void* where);
	//ɾ����and��or����
	template<typename T, typename ...Args>
	int deleteData(const char* tablename, void* where, T condition, Args ...conditions);


	//�ص�����
	static int callback(void* callfunc, int column_num, char** values, char** columns);

	//���ң�ȫ������
	int selectData(const char* tablename, std::vector<std::map<char*, char*>>& calldatas);
	//���ң�where����ȫ������
	int selectData(const char* tablename, std::vector<std::map<char*, char*>>& calldatas, void* where);
	//���ң�and��or����ȫ������
	template<typename T, typename ...Args>
	int selectData(const char* tablename, std::vector<std::map<char*, char*>>& calldatas,
		void* where, T condition, Args ...conditions);

	///���ң���ȡָ����ͷ
	int selectData(const char* tablename, void* columns, std::vector<std::map<char*, char*>>& calldatas);
	//���ң�where������ȡָ����ͷ
	int selectData(const char* tablename, void* columns, std::vector<std::map<char*, char*>>& calldatas, void* where);
	//���ң�and��or����ȫ������
	template<typename T, typename ...Args>
	int selectData(const char* tablename, void* columns, std::vector<std::map<char*, char*>>& calldatas,
		void* where, T condition, Args ...conditions);


};


void sqlfunc::initWordsVec() {
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
	this->initWordsVec();
}

sqlfunc::~sqlfunc() {
	//��������ʱ��ر����ݿ�
	this->closeSQL();
	//����ʱ��Ҫɾ��ָ��
	this->columns_vec.clear();
	this->values_vec.clear();

	SelectData::initSelectData();	//��������ʱ����г�ʼ������ֹһ��ָ�ʼ��û�еڶ���ָ��
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

	//for (unsigned int i = 0; i < this->values_vec.size(); i++) {
	//	std::cout << "the vec value is: " << this->values_vec[i] << std::endl;
	//}
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

	//for (unsigned int i = 0; i < this->columns_vec.size(); i++) {
	//	std::cout << "the vec column is: " << this->columns_vec[i] << std::endl;
	//}
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
	//std::cout << "the vec where is: " << this->where_vec[0] << std::endl;

	return SQLITE_OK;
}


template<typename T>
void* sqlfunc::And(T condition) {
	this->and_vec.push_back((char*)condition);

	//for (unsigned int i = 0; i < this->and_vec.size(); i++) {
	//	std::cout << "the vec and is: " << this->and_vec[i] << std::endl;
	//}
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

	//for (unsigned int i = 0; i < this->or_vec.size(); i++) {
	//	std::cout << "the vec or is: " << this->or_vec[i] << std::endl;
	//}
	return SQLITE_OK;
}

template<typename T, typename ...Args>
void* sqlfunc::Or(T condition, Args ...conditions) {
	this->or_vec.push_back((char*)condition);
	this->Or(conditions...);

	return SQLITE_OK;
}


int sqlfunc::insertData(const char* tablename, void* columns, void* values) {

	//���columns_vec��values_vec�Ĵ�С�Ƿ����
	if (this->columns_vec.size() != this->values_vec.size()) {
		this->initWordsVec();
		return SQLITE_ERROR;
	}

	//INSERT INTO USER(ID,NAME,SCORE) VALUES(1,'WENJY',20.98);
	//����vector���ݣ�������ƥ��
	char sql_tablename[CHAR_MAX] = "";
	char sql_columns[CHAR_MAX] = "(";
	char sql_values[CHAR_MAX] = "VALUES(";

	//�����sql_word����ʹ��static��ԭ���ǣ�char��ʵ��һ�����飬����������鳤��̫Сʱ���������ݹ����������������
	//��Ҫ���������⣬��õľ���ʹ��char*ָ��������������Ͳ������ַ�������
	char sql_word[CHAR_MAX] = "INSERT INTO ";

	strcpy(sql_tablename, tablename);
	strcat(sql_word, sql_tablename);

	for (unsigned int i = 0; i < this->columns_vec.size(); i++) {

		char t_column[CHAR_MAX] = "";
		strcpy(t_column, this->columns_vec[i]);
		strcat(sql_columns, t_column);
		if (i != this->columns_vec.size() - 1) {
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
	std::cout << "sql words is: " << sql_word << std::endl;

	//char[]��char*�Ľ��޺�ģ����char*���ǵ�ַ����char���׵�ַ��char�պþ��ǡ����Բ���Ҫ��ת��
	//char* p_chr = &(chr[0]);	��һ����ʵ�ǲ���Ҫ��
	int rec = sqlite3_exec(this->db, sql_word, NULL, NULL, NULL);

	if (rec != SQLITE_OK) {
		std::cout << "insert data fail, errorcode is: " << rec << std::endl;
		this->initWordsVec();
		return rec;
	}
	this->initWordsVec();

	return SQLITE_OK;
}

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
	std::cout << "sql words is: " << sql_word << std::endl;

	//char[]��char*�Ľ��޺�ģ����char*���ǵ�ַ����char���׵�ַ��char�պþ��ǡ����Բ���Ҫ��ת��
	//char* p_chr = &(chr[0]);	��һ����ʵ�ǲ���Ҫ��
	int rec = sqlite3_exec(this->db, sql_word, NULL, NULL, NULL);

	if (rec != SQLITE_OK) {
		std::cout << "insert data fail, errorcode is: " << rec << std::endl;
		this->initWordsVec();
		return rec;
	}
	this->initWordsVec();

	return SQLITE_OK;
}


int sqlfunc::updateData(const char* tablename, void* columns, void* values) {
	//���columns_vec��values_vec�Ĵ�С�Ƿ����
	if (this->columns_vec.size() != this->values_vec.size()) {
		this->initWordsVec();
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

	std::cout << "sql words is: " << sql_word << std::endl;

	//char[]��char*�Ľ��޺�ģ����char*���ǵ�ַ����char���׵�ַ��char�պþ��ǡ����Բ���Ҫ��ת��
	//char* p_chr = &(chr[0]);	��һ����ʵ�ǲ���Ҫ��
	int rec = sqlite3_exec(this->db, sql_word, NULL, NULL, NULL);

	if (rec != SQLITE_OK) {
		std::cout << "update data fail, errorcode is: " << rec << std::endl;
		this->initWordsVec();
		return rec;
	}

	this->initWordsVec();

	return SQLITE_OK;

}

int sqlfunc::updateData(const char* tablename, void* columns, void* values, void* where) {

	//���columns_vec��values_vec�Ĵ�С�Ƿ����
	if (this->columns_vec.size() != this->values_vec.size()) {
		this->initWordsVec();
		return SQLITE_ERROR;
	}
	//whereֻ��С�ڵ���1
	if (this->where_vec.size() != 1) {
		this->initWordsVec();
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

	std::cout << "sql words is: " << sql_word << std::endl;

	//char[]��char*�Ľ��޺�ģ����char*���ǵ�ַ����char���׵�ַ��char�պþ��ǡ����Բ���Ҫ��ת��
	//char* p_chr = &(chr[0]);	��һ����ʵ�ǲ���Ҫ��
	int rec = sqlite3_exec(this->db, sql_word, NULL, NULL, NULL);

	if (rec != SQLITE_OK) {
		std::cout << "update data fail, errorcode is: " << rec << std::endl;
		this->initWordsVec();
		return rec;
	}

	this->initWordsVec();

	return SQLITE_OK;
}

template<typename T, typename ...Args>
int sqlfunc::updateData(const char* tablename, void* columns, void* values, void* where, T condition, Args ...conditions) {
	//���columns_vec��values_vec�Ĵ�С�Ƿ����
	if (this->columns_vec.size() != this->values_vec.size()) {
		this->initWordsVec();
		return SQLITE_ERROR;
	}
	//whereֻ��Ϊ1
	if (this->where_vec.size() != 1) {
		this->initWordsVec();
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

	std::cout << "sql words is: " << sql_word << std::endl;

	//char[]��char*�Ľ��޺�ģ����char*���ǵ�ַ����char���׵�ַ��char�պþ��ǡ����Բ���Ҫ��ת��
	//char* p_chr = &(chr[0]);	��һ����ʵ�ǲ���Ҫ��
	int rec = sqlite3_exec(this->db, sql_word, NULL, NULL, NULL);

	if (rec != SQLITE_OK) {
		std::cout << "update data fail, errorcode is: " << rec << std::endl;
		this->initWordsVec();
		return rec;
	}

	this->initWordsVec();

	return SQLITE_OK;

}


int sqlfunc::deleteData(const char* tablename) {
	//DELETE FROM COMPANY;
	char sql_tablename[CHAR_MAX] = "";
	char sql_word[CHAR_MAX] = "DELETE FROM ";

	strcpy(sql_tablename, tablename);
	strcat(sql_word, sql_tablename);
	strcat(sql_word, ";");

	std::cout << "sql words is: " << sql_word << std::endl;

	int rec = sqlite3_exec(this->db, sql_word, NULL, NULL, NULL);

	if (rec != SQLITE_OK) {
		std::cout << "delete data fail, errorcode is: " << rec << std::endl;
		this->initWordsVec();
		return rec;
	}

	this->initWordsVec();
	return SQLITE_OK;
}

int sqlfunc::deleteData(const char* tablename, void* where) {

	//whereֻ��С�ڵ���1
	if (this->where_vec.size() != 1) {
		this->initWordsVec();
		return SQLITE_ERROR;
	}

	char sql_tablename[CHAR_MAX] = "";
	char sql_where[CHAR_MAX] = "";

	char sql_word[CHAR_MAX] = "DELETE FROM ";

	strcpy(sql_tablename, tablename);
	strcat(sql_word, sql_tablename);
	strcat(sql_word, " ");


	//where����
	if (this->where_vec.size() == 1) {
		strcat(sql_where, "WHERE ");
		strcat(sql_where, this->where_vec[0]);
	}

	strcat(sql_word, sql_where);
	strcat(sql_word, ";");

	std::cout << "sql words is: " << sql_word << std::endl;

	int rec = sqlite3_exec(this->db, sql_word, NULL, NULL, NULL);

	if (rec != SQLITE_OK) {
		std::cout << "update data fail, errorcode is: " << rec << std::endl;
		this->initWordsVec();
		return rec;
	}

	this->initWordsVec();

	return SQLITE_OK;
}

template<typename T, typename ...Args>
int sqlfunc::deleteData(const char* tablename, void* where, T condition, Args ...conditions) {

	//whereֻ��Ϊ1
	if (this->where_vec.size() != 1) {
		this->initWordsVec();
		return SQLITE_ERROR;
	}

	char sql_tablename[CHAR_MAX] = "";
	char sql_where[CHAR_MAX] = "";
	char sql_and[CHAR_MAX] = "";
	char sql_or[CHAR_MAX] = "";

	char sql_word[CHAR_MAX] = "DELETE FROM ";

	strcpy(sql_tablename, tablename);
	strcat(sql_word, sql_tablename);
	strcat(sql_word, " ");


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

	strcat(sql_word, sql_where);
	strcat(sql_word, sql_and);
	strcat(sql_word, sql_or);
	strcat(sql_word, ";");

	std::cout << "sql words is: " << sql_word << std::endl;

	int rec = sqlite3_exec(this->db, sql_word, NULL, NULL, NULL);

	if (rec != SQLITE_OK) {
		std::cout << "update data fail, errorcode is: " << rec << std::endl;
		this->initWordsVec();
		return rec;
	}

	this->initWordsVec();

	return SQLITE_OK;
}


int sqlfunc::callback(void* callfunc, int column_num, char** values, char** columns) {

	std::vector<char*> t_column_vec;
	std::vector<char*> t_value_vec;

	for (int i = 0; i < column_num; i++) {

		//������ָ�������ת�棬callback�ص�ʱ����ͷ�����

		int value_size = sizeof(values[i]);
		int column_size = sizeof(columns[i]);

		char* new_value = (char*)malloc(sizeof(char) * value_size); 
		char* new_column = (char*)malloc(sizeof(char) * column_size);

		//��ֹ��ǰָ��Ϊ��
		if (new_value != NULL) {
			memset(new_value, 0, column_size);
			strcpy(new_value, values[i]);
		}
		if (new_column != NULL) {
			memset(new_column, 0, value_size);
			strcpy(new_column, columns[i]);
		}

		t_column_vec.push_back(new_column);
		t_value_vec.push_back(new_value);

	}

	SelectData::select_columns.push_back(t_column_vec);
	SelectData::select_values.push_back(t_value_vec);

	SelectData::select_data_num++;

	return SQLITE_OK;
}


int sqlfunc::selectData(const char* tablename, std::vector<std::map<char*, char*>>& calldatas) {
	
	SelectData::initSelectData();

	//SELECT * FROM COMPANY;
	char sql_tablename[CHAR_MAX] = "";
	char sql_word[CHAR_MAX] = "SELECT * FROM ";

	strcpy(sql_tablename, tablename);
	strcat(sql_word, sql_tablename);
	strcat(sql_word, ";");

	std::cout << "sql words is: " << sql_word << std::endl;

	int rec = sqlite3_exec(this->db, sql_word, &this->callback, NULL, NULL);

	if (rec != SQLITE_OK) {
		//std::cout << "select data fail, errorcode is: " << rec << std::endl;
		this->initWordsVec();
		return rec;
	}

	//std::cout << "the data vec num is: " << select_data_num << std::endl;
	//std::cout << "***************" << std::endl;

	//std::vector<std::map<char*, char*>> calldatas


	for (int i = 0; i < select_data_num; i++) {
		std::map<char*, char*> t_calldata;

		for (int j = 0; j < SelectData::select_columns[i].size(); j++) {
			//std::cout << "the column, value is: " << SelectData::select_columns[i][j] << ", " << SelectData::select_values[i][j] << std::endl;
			t_calldata[SelectData::select_columns[i][j]] = SelectData::select_values[i][j];
		}

		calldatas.push_back(t_calldata);

		//std::cout << "***************" << std::endl;

	}


	this->initWordsVec();
	

	return SQLITE_OK;

}

int sqlfunc::selectData(const char* tablename, std::vector<std::map<char*, char*>>& calldatas, void* where) {
	
	SelectData::initSelectData();

	//whereֻ��С�ڵ���1
	if (this->where_vec.size() != 1) {
		this->initWordsVec();
		return SQLITE_ERROR;
	}

	//SELECT * FROM COMPANY WHERE NAME='Sam';
	char sql_tablename[CHAR_MAX] = "";
	char sql_where[CHAR_MAX] = "";

	char sql_word[CHAR_MAX] = "SELECT * FROM ";

	strcpy(sql_tablename, tablename);
	strcat(sql_word, sql_tablename);
	strcat(sql_word, " ");

	//where����
	if (this->where_vec.size() == 1) {
		strcat(sql_where, "WHERE ");
		strcat(sql_where, this->where_vec[0]);
	}

	strcat(sql_word, sql_where);
	strcat(sql_word, ";");

	std::cout << "sql words is: " << sql_word << std::endl;

	int rec = sqlite3_exec(this->db, sql_word, &this->callback, NULL, NULL);

	if (rec != SQLITE_OK) {
		//std::cout << "select data fail, errorcode is: " << rec << std::endl;
		this->initWordsVec();
		return rec;
	}

	//std::cout << "the data vec num is: " << select_data_num << std::endl;
	//std::cout << "***************" << std::endl;

	//std::vector<std::map<char*, char*>> calldatas


	for (int i = 0; i < select_data_num; i++) {
		std::map<char*, char*> t_calldata;

		for (int j = 0; j < SelectData::select_columns[i].size(); j++) {
			//std::cout << "the column, value is: " << SelectData::select_columns[i][j] << ", " << SelectData::select_values[i][j] << std::endl;
			t_calldata[SelectData::select_columns[i][j]] = SelectData::select_values[i][j];
		}

		calldatas.push_back(t_calldata);

		//std::cout << "***************" << std::endl;

	}


	this->initWordsVec();
	
	return SQLITE_OK;

}

template<typename T, typename ...Args>
int sqlfunc::selectData(const char* tablename, std::vector<std::map<char*, char*>>& calldatas,
	void* where, T condition, Args ...conditions) {
	
	SelectData::initSelectData();

	//whereֻ��С�ڵ���1
	if (this->where_vec.size() != 1) {
		this->initWordsVec();
		return SQLITE_ERROR;
	}

	//SELECT * FROM COMPANY WHERE NAME='Sam';
	char sql_tablename[CHAR_MAX] = "";
	char sql_where[CHAR_MAX] = "";
	char sql_and[CHAR_MAX] = "";
	char sql_or[CHAR_MAX] = "";

	char sql_word[CHAR_MAX] = "SELECT * FROM ";

	strcpy(sql_tablename, tablename);
	strcat(sql_word, sql_tablename);
	strcat(sql_word, " ");

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

	strcat(sql_word, sql_where);
	strcat(sql_word, sql_and);
	strcat(sql_word, sql_or);
	strcat(sql_word, ";");

	std::cout << "sql words is: " << sql_word << std::endl;

	int rec = sqlite3_exec(this->db, sql_word, &this->callback, NULL, NULL);

	if (rec != SQLITE_OK) {
		//std::cout << "select data fail, errorcode is: " << rec << std::endl;
		this->initWordsVec();
		return rec;
	}

	//std::cout << "the data vec num is: " << select_data_num << std::endl;
	//std::cout << "***************" << std::endl;

	//std::vector<std::map<char*, char*>> calldatas


	for (int i = 0; i < select_data_num; i++) {
		std::map<char*, char*> t_calldata;

		for (int j = 0; j < SelectData::select_columns[i].size(); j++) {
			//std::cout << "the column, value is: " << SelectData::select_columns[i][j] << ", " << SelectData::select_values[i][j] << std::endl;
			t_calldata[SelectData::select_columns[i][j]] = SelectData::select_values[i][j];
		}

		calldatas.push_back(t_calldata);

		//std::cout << "***************" << std::endl;

	}

	this->initWordsVec();
	
	return SQLITE_OK;

}

int sqlfunc::selectData(const char* tablename, void* columns, std::vector<std::map<char*, char*>>& calldatas) {

	SelectData::initSelectData();	

	//SELECT NAME,ID FROM USER2;
	char sql_tablename[CHAR_MAX] = "";
	char sql_word[CHAR_MAX] = "SELECT ";

	//columns
	for (unsigned int i = 0; i < this->columns_vec.size(); i++) {
		char t_column[CHAR_MAX] = "";

		strcpy(t_column, this->columns_vec[i]);

		strcat(sql_word, t_column);

		if (i != this->columns_vec.size() - 1) {
			strcat(sql_word, ",");
		}

	}

	strcat(sql_word, " FROM ");
	strcpy(sql_tablename, tablename);
	strcat(sql_word, sql_tablename);
	strcat(sql_word, ";");

	std::cout << "sql words is: " << sql_word << std::endl;

	int rec = sqlite3_exec(this->db, sql_word, &this->callback, NULL, NULL);

	if (rec != SQLITE_OK) {
		//std::cout << "select data fail, errorcode is: " << rec << std::endl;
		this->initWordsVec();
		return rec;
	}

	for (int i = 0; i < select_data_num; i++) {
		std::map<char*, char*> t_calldata;
		for (int j = 0; j < SelectData::select_columns[i].size(); j++) {
			//std::cout << "the column, value is: " << SelectData::select_columns[i][j] << ", " << SelectData::select_values[i][j] << std::endl;
			t_calldata[SelectData::select_columns[i][j]] = SelectData::select_values[i][j];
		}
		calldatas.push_back(t_calldata);
	}

	this->initWordsVec();

	return SQLITE_OK;

}

int sqlfunc::selectData(const char* tablename, void* columns, std::vector<std::map<char*, char*>>& calldatas, void* where) {

	SelectData::initSelectData();

	//whereֻ��С�ڵ���1
	if (this->where_vec.size() != 1) {
		this->initWordsVec();
		return SQLITE_ERROR;
	}

	//SELECT NAME,ID FROM USER2 WHERE ID>2;
	char sql_tablename[CHAR_MAX] = "";
	char sql_word[CHAR_MAX] = "SELECT ";
	char sql_where[CHAR_MAX] = "";

	//columns
	for (unsigned int i = 0; i < this->columns_vec.size(); i++) {
		char t_column[CHAR_MAX] = "";

		strcpy(t_column, this->columns_vec[i]);

		strcat(sql_word, t_column);

		if (i != this->columns_vec.size() - 1) {
			strcat(sql_word, ",");
		}

	}

	strcat(sql_word, " FROM ");
	strcpy(sql_tablename, tablename);
	strcat(sql_word, sql_tablename);
	strcat(sql_word, " ");


	//where
	if (this->where_vec.size() == 1) {
		strcat(sql_where, "WHERE ");
		strcat(sql_where, this->where_vec[0]);
	}
	strcat(sql_word, sql_where);
	strcat(sql_word, ";");

	std::cout << "sql words is: " << sql_word << std::endl;

	int rec = sqlite3_exec(this->db, sql_word, &this->callback, NULL, NULL);

	if (rec != SQLITE_OK) {
		//std::cout << "select data fail, errorcode is: " << rec << std::endl;
		this->initWordsVec();
		return rec;
	}

	for (int i = 0; i < select_data_num; i++) {
		std::map<char*, char*> t_calldata;
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
int sqlfunc::selectData(const char* tablename, void* columns, std::vector<std::map<char*, char*>>& calldatas,
	void* where, T condition, Args ...conditions) {


	SelectData::initSelectData();

	//whereֻ��С�ڵ���1
	if (this->where_vec.size() != 1) {
		this->initWordsVec();
		return SQLITE_ERROR;
	}

	//SELECT NAME,ID FROM USER2 WHERE ID>2 AND ID<4;
	char sql_tablename[CHAR_MAX] = "";
	char sql_word[CHAR_MAX] = "SELECT ";
	char sql_where[CHAR_MAX] = "";
	char sql_and[CHAR_MAX] = "";
	char sql_or[CHAR_MAX] = "";

	//columns
	for (unsigned int i = 0; i < this->columns_vec.size(); i++) {
		char t_column[CHAR_MAX] = "";

		strcpy(t_column, this->columns_vec[i]);

		strcat(sql_word, t_column);

		if (i != this->columns_vec.size() - 1) {
			strcat(sql_word, ",");
		}

	}

	strcat(sql_word, " FROM ");
	strcpy(sql_tablename, tablename);
	strcat(sql_word, sql_tablename);
	strcat(sql_word, " ");


	//where
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

	strcat(sql_word, sql_where);
	strcat(sql_word, sql_and);
	strcat(sql_word, sql_or);
	strcat(sql_word, ";");

	std::cout << "sql words is: " << sql_word << std::endl;

	int rec = sqlite3_exec(this->db, sql_word, &this->callback, NULL, NULL);

	if (rec != SQLITE_OK) {
		//std::cout << "select data fail, errorcode is: " << rec << std::endl;
		this->initWordsVec();
		return rec;
	}

	for (int i = 0; i < select_data_num; i++) {
		std::map<char*, char*> t_calldata;
		for (int j = 0; j < SelectData::select_columns[i].size(); j++) {
			//std::cout << "the column, value is: " << SelectData::select_columns[i][j] << ", " << SelectData::select_values[i][j] << std::endl;
			t_calldata[SelectData::select_columns[i][j]] = SelectData::select_values[i][j];
		}
		calldatas.push_back(t_calldata);
	}

	this->initWordsVec();

	return SQLITE_OK;




}

