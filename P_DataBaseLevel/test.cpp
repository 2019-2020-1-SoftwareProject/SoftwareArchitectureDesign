#include"p_DataBaseOp.h"

using namespace DBHelper;

int main(void)
{
	const string host = "localhost";  //��Ϊ����Ϊ�������ԣ�������д���Ǳ���IP
	const string user = "root";       //����MySQL���û���
	const string passwd = "abcdefg"; //����û�����
	const string db = "bookstore";      //��Ҫ���ӵ����ݿ������
	unsigned int port = 3306;           //����MySQL�ķ������Ķ˿ڣ����û���޸Ĺ��Ļ�����3306��
	const char* unix_socket = NULL;    


	DBConf dbc(host, user, passwd, db, "GBK", port);
	p_DataBaseOp dbo(dbc);
	/*OrderInfo ori1 = dbo.selectOrder("350802200003052010",true);
	OrderInfo ori2("ty", "3508", "A", "188", false, string("2020-03-25"));
	dbo.deleteOrder("350802200003052010", true);
	dbo.addOrder(ori2);*/
	//MaskInfo mi1("A", "N95", 200);
	//dbo.addMaskInfo(mi1);
	////dbo.deleteMaskInfo("A", "N95");
	//MaskInfo mi2 = dbo.selectMaskInfo("A", "N95");
	
	return 0;
}