#ifndef CLIENT_H
#define CLIENT_H


#include "Global.h"
#include "Item.h"

using namespace std;

struct Login
{
	int u_number;  // 유저고유번호
	char u_id[STR_MAX];   //유저 ID
	char u_pw[STR_MAX];  // 유저PW
	int U_Cash;			//해당 클라의 잔액
	bool UseLogin;
};

class ClientInfo
{
public:
	//버퍼나 체크하는 것들은 바로바로 접근할 수 있게 조정
	FD_SET c_Rset;
	char buf[BUFSIZE];
	int recvbytes;
	int recvbytes_Check;
	bool r_sizeflag;


	/////
	ClientInfo(SOCKET _sock,SOCKADDR_IN _addr);		//클라이언트 정의 및 생성 AddClientInfo 대체
	~ClientInfo();
	void InItData();
	void InItLogin();
	void InItItem();
	void UpdateMoney(Login* _login,int money);
	void UpdateState(STATE _state);
	void SetAuctionItem(Item* _tem);			//선택한 아이템을 넣는다.
	void SetClient_Login(Login* _login);		//로그인 성공된 정보를 클라쪽에 저장한다.
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
	SOCKADDR_IN addr, m_addr;		//소켓정보
	STATE state;			//클라 상태값
	Login* login;		//클라 로그인정보
	Item* Check_Item;			//가지고 있는 정보
	ERR_CODE err_code;
	HANDLE AuctionEvent;			//옥션 이벤트 선언

};


#endif // !CLIENT_H
