#ifndef CLIENT_H
#define CLIENT_H


#include "Global.h"
#include "Item.h"

using namespace std;

struct Login
{
	int u_number;  // ����������ȣ
	char u_id[STR_MAX];   //���� ID
	char u_pw[STR_MAX];  // ����PW
	int U_Cash;			//�ش� Ŭ���� �ܾ�
	bool UseLogin;
};

class ClientInfo
{
public:
	//���۳� üũ�ϴ� �͵��� �ٷιٷ� ������ �� �ְ� ����
	FD_SET c_Rset;
	char buf[BUFSIZE];
	int recvbytes;
	int recvbytes_Check;
	bool r_sizeflag;


	/////
	ClientInfo(SOCKET _sock,SOCKADDR_IN _addr);		//Ŭ���̾�Ʈ ���� �� ���� AddClientInfo ��ü
	~ClientInfo();
	void InItData();
	void InItLogin();
	void InItItem();
	void UpdateMoney(Login* _login,int money);
	void UpdateState(STATE _state);
	void SetAuctionItem(Item* _tem);			//������ �������� �ִ´�.
	void SetClient_Login(Login* _login);		//�α��� ������ ������ Ŭ���ʿ� �����Ѵ�.
	void SetErrCode(ERR_CODE _code);
	void SetSock(SOCKET _sock);
	void SetAddr(SOCKADDR_IN _addr);
	void SetLogOut(Login* _login);

	SOCKET GetSock();
	SOCKADDR_IN GetAddr();
	SOCKADDR_IN GetMuitiAddr();
	STATE GetState();
	int GetMoney();
	Login* GetLoginData();

private:

	SOCKET sock;
	SOCKADDR_IN addr, m_addr;		//��������
	STATE state;			//Ŭ�� ���°�
	Login* login;		//Ŭ�� �α�������
	Item* Check_Item;			//������ �ִ� ����
	ERR_CODE err_code;
	HANDLE AuctionEvent;			//���� �̺�Ʈ ����

};


#endif // !CLIENT_H
