#pragma once
#include <windows.h>
#include<stdlib.h>
#include "stdio.h"
#include "winsock.h"
#include "mysql.h"
#include<mysql.h>
#include<iostream>
#include<sstream>
#include<vector>
#include<string>

using namespace std;

namespace DBHelper
{
	struct DBHelper_Exception {
		DBHelper_Exception(const string& sBuffer)
			:errorInfo(sBuffer)
		{};
		~DBHelper_Exception() {};
		string errorInfo;
	};

	struct DBConf {//database configuration
		string _host;//host address
		string _user;
		string _password;
		string _database;
		string _charset;
		int _port;
		int _flag;

		DBConf(const string& host = "", const string& user = "", const string& password = "",
			const string& database = "", const string& charset = "", unsigned port = 3306)
			:_host(host), _user(user), _password(password), _database(database), _charset(charset), _port(port), _flag(0)
		{}
	};

	struct Date {
		unsigned _year, _month, _date;
	public:
		Date(unsigned year = 2020, unsigned month = 1, unsigned date = 1)
			:_year(year), _month(month), _date(date) {}

		Date(const string& date) {
			vector<string> s = split(date, "-");
			_year = stoi(s[0]); _month = stoi(s[1]);
			_date = stoi(s[2]);
		}

		Date(const char* date) {
			string sDate = date;
			vector<string> s = split(sDate, "-");
			_year = stoi(s[0]); _month = stoi(s[1]);
			_date = stoi(s[2]);
		}

		vector<string> split(const string& str, const string& pattern)
		{
			vector<string> res;
			if ("" == str)
				return res;

			string strs = str + pattern;

			size_t pos = strs.find(pattern);
			size_t size = strs.size();

			while (pos != string::npos)
			{
				string x = strs.substr(0, pos);
				res.push_back(x);//stoi(x)ת����
				strs = strs.substr(pos + 1, size);
				pos = strs.find(pattern);
			}
			return res;
		}

		string getYear() const{
			return to_string(_year);
		}

		string getMonth() const{
			return to_string(_month);
		}

		string getDate() const{
			return to_string(_date);
		}

		string toString() const{
			string sDate = to_string(_year) + "-" + to_string(_month) + "-" + to_string(_date);
			//cout << sDate << endl;
			return sDate;
		}
	};

	struct OrderInfo {
		friend class p_DataBaseOp;
	private:
		string _name;//ԤԼ������
		string _id;//ԤԼ�����֤��
		string _place;//ԤԼ��ԤԼ�����
		string _phone;//ԤԼ�˵绰����
		bool _rec;//�Ƿ���ҪԤԼƾ֤
		Date _date;//ԤԼ����ʱ��
	public:
		OrderInfo(const string& name = "", const string& id = "",
			const string& place = "", const string& phone = "", bool rec = false, const Date& date = { 2020, 1, 1 })
			:_name(name), _id(id), _place(place), _phone(phone), _rec(rec), _date(date)
		{
		}

		OrderInfo(const char* name = "", const char* id = "",
			const char* place = "", const char* phone = "", bool rec = false, const Date& date = { 2020, 1, 1 })
			:_name(name), _id(id), _place(place), _phone(phone), _rec(rec), _date(date)
		{
		}
	};

	struct MaskInfo {
		friend class p_DataBaseOp;
	private:
		string _place;
		string _maskType;
		unsigned _surplus;
	public:
		MaskInfo(const string& place = "", const string& mt = "", unsigned sp = 0)
			:_place(place), _maskType(mt), _surplus(sp)
		{
		}

		MaskInfo(const char* place = "", const char* mt = "", unsigned sp = 0)
			:_place(place), _maskType(mt), _surplus(sp)
		{
		}
	};

	class p_DataBaseOp {
	private:
		DBConf _dbc;
		MYSQL* _pstMql;//���ݿ�ָ��
		bool bConnected;//�Ƿ��Ѿ�����
		string _sLastSql;//���ִ�е�sql
	public:
		const vector<string> OrderRecord = { "_name","_id","_place","_phone","_rec","_date" };//colum names of table OrderRecord
		const vector<string> MaskInformation = { "_place","_maskType","_surplus" };//colum names of table MaskInformation

		p_DataBaseOp(const char* host = "", const char* user = "", const char* password = "",
			const char* dbname = "", const char* charset = "", unsigned port = 3306)
			:_dbc(host, user, password, dbname, charset, port),_pstMql(NULL),bConnected(false)
		{
			disconnect();
			connect();
		}

		p_DataBaseOp(const DBConf& dbconf) 
			:_dbc(dbconf), _pstMql(NULL), bConnected(false)
		{
			disconnect();
			connect();
		}

		~p_DataBaseOp() {
			disconnect();
		}

		void connect();

		void disconnect();

		/**
		* @brief update or insert data
		* @param sSql sql sentence
		**/
		void execute(const string& sSql);

		//only for first time
		//demo
		int buildDatabase() {//initially build a database
			string sORCreatSQL = "CREATE TABLE OrderRecord("
				"_name VARCHAR(20),"
				"_id VARCHAR(20),"
				"_place VARCHAR(20),"
				"_phone VARCHAR(20),"
				"_rec BOOL,"
				"_date DATE,"
				"PRIMARY KEY(_id, _phone)"
				");";

			string sMICreatSQL = "CREATE TABLE MaskInformation ("
				"_place VARCHAR(20),"
				"_maskType VARCHAR(20),"
				"_surplus VARCHAR(20)"
				"); ";

			execute(sORCreatSQL);
			execute(sMICreatSQL);
			return 1;
		}

		/**
		* @brief �����ݿ����ԤԼ��Ϣ
		* @param ԤԼ��Ϣ
		**/
		int addOrder(const OrderInfo& order) {
			if (isOrderExist(order._id, true)) { return 1; }
			string sInsertSql = buildInsertSQL(order);
			execute(sInsertSql);
			return 0;
		}

		/**
		* @brief �����ݿ�ɾ��ԤԼ��Ϣ
		* @param ֻ�ṩ���֤�Ż�绰���뼴��
		* @param true�ṩid��false�ṩphone
		**/
		int deleteOrder(const string& ip, bool isId) {
			if (!isOrderExist(ip, isId)) { return 1; }
			string sDeleteSql = buildDeleteSQL(ip, isId);
			execute(sDeleteSql);
			return 0;
		}

		/**
		* @brief ����Ƿ�����ԤԼ��Ϣ
		*/
		bool isOrderExist(const string& ip, bool isId) {
			string sSelectSql = "select count(*) as num from OrderRecord where " +
				OrderRecord[isId ? 1 : 3] + "=" + ip;
			execute(sSelectSql);

			MYSQL_RES* result = mysql_store_result(_pstMql);//get result

			//deal with result getted
			if (NULL == result) {
				printf("error(%d):%s\n", mysql_errno(_pstMql), mysql_error(_pstMql));
			}
			MYSQL_ROW row = mysql_fetch_row(result);
			int num = stoi(row[0]);
			return (num == 0 ? false : true);
		}

		bool isMaskExist(const string& place, const string& type) {
			string sSelectSql = "select count(*) as num from MaskInformation where " +
				MaskInformation[0] + "='" + place + "' and " + MaskInformation[1] + "='" + type + "'";
			//cout << sSelectSql << endl;
			execute(sSelectSql);

			MYSQL_RES* result = mysql_store_result(_pstMql);//get result

			//deal with result getted
			if (NULL == result) {
				printf("error(%d):%s\n", mysql_errno(_pstMql), mysql_error(_pstMql));
			}
			MYSQL_ROW row = mysql_fetch_row(result);
			int num = stoi(row[0]);
			return (num == 0 ? false : true);
		}

		/**
		* @brief �����ݿ����ԤԼ��Ϣ
		* @param �ṩ���֤�Ż�绰����
		* @param true��ʾ�ṩ���֤�ţ�false��ʾ�ṩ�绰����
		**/
		OrderInfo selectOrder(const string& ip, bool isId);

		/**
		* @brief �����ݿ���ҿ���������Ϣ
		* @param �ṩ����
		* @param �ṩ��������
		**/
		MaskInfo selectMaskInfo(const string& place, const string& type);

		/**
		* @brief ��MaskInformation������ӿ�������
		* @param �ṩ����ӵĿ�������
		**/
		int addMaskInfo(const MaskInfo& maskinfo) {
			if (isMaskExist(maskinfo._place, maskinfo._maskType)) { return 1; }
			string sInsertSQL = buildInsertSQL(maskinfo);
			execute(sInsertSQL);
			return 0;
		}

		/**
		* @brief ��MaskInformation����ɾ����������
		* @param �ṩ���������ڵ�
		* @param �ṩ�����������
		**/
		int deleteMaskInfo(const string& place, const string& type) {
			if (!isMaskExist(place, type)) { return 1; }
			string sDeleteSQL = buildDeleteSQL(place, type);
			execute(sDeleteSQL);
			return 0;
		}

	private:
		string sColumNames(const vector<string>& table)const {
			string columName = "";
			for (string x : table) {
				columName += x + ",";
			}
			columName.pop_back();
			return "(" + columName + ")";
		}

		string sColumNames(const OrderInfo& order)const {
			return "('" + order._name + "','" + order._id + "','" + order._place + "','" + order._phone +
				"'," + (order._rec ? "true" : "false") + ",'" + order._date.toString() + "')";
		}

		string sColumNames(const MaskInfo& maskInfo)const {
			return "('" + maskInfo._place + "','" + maskInfo._maskType + "'," + to_string(maskInfo._surplus) + ")";
		}

		/**
		* @brief ����SQL�������
		**/
		string buildInsertSQL(const OrderInfo& order) {
			string sInsertSQL;
			sInsertSQL = "insert into OrderRecord" + sColumNames(OrderRecord) + " values" + sColumNames(order);
			cout << sInsertSQL;
			return sInsertSQL;
		}

		/**
		* @brief ����SQL�������
		**/
		string buildInsertSQL(const MaskInfo& maskinfo) {
			ostringstream os;
			os << "insert into " << "MaskInformation" << sColumNames(MaskInformation) << " values" << sColumNames(maskinfo);
			//cout << os.str();
			return os.str();
		}

		/**
		* @brief ����SQLɾ�����
		**/
		string buildDeleteSQL(const string& ip, bool isId) {
			string sDelete = "";
			if (isId) { sDelete = "delete from OrderRecord where " + OrderRecord[1] + "='" + ip + "'"; }
			else { sDelete = "delete from OrderRecord where " + OrderRecord[3] + "='" + ip + "'"; }
			return sDelete;
		}

		string buildDeleteSQL(const string& place, const string& type) {
			string sDelete = "delete from MaskInformation where " +
				MaskInformation[0] + "='" + place + "' and " + MaskInformation[1] + "='" + type + "'";
			return sDelete;
		}

		/**
		* @brief ����SQL��ѯ���
		**/
		string buildSelectSQL(const string& id) {
			string sSelect = "select * from OrderRecord where " + OrderRecord[1] + "='" + id+"'";
			return sSelect;
		}

		vector<string> getSelectInfo(const string& sSql);
	};
}