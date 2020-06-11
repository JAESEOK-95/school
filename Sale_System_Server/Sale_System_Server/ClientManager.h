#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H
/*
메인 연결 클래스

*/


#include "Global.h"
#include "ItemManager.h"
#include "Client.h"


//분명히 public에 있었어야할 놈들인데 망할
static CLinkedList<ClientInfo*>* ClientList;
static int ClientCount;
static Login User_List[100];
static int User_Count;
static ItemManager* Item_master;
class ClientManager
{
public:  //모든 클라에 대한 사용 때문에라도 전역선언
	WSADATA wsa;
	SOCKET client_sock, listen_sock, udpSock;
	SOCKADDR_IN serveraddr,clientaddr;
	HANDLE hThread;
	DWORD ThreadId;

	int  retval, addrlen;
	ClientManager();	//LINKED LIST  전문 초기화 및 wsa 초기화
	~ClientManager();

	void ConnetSelect();

	static void ClientDisconnect(ClientInfo* _ptr);
	static BOOL CheckDataProcess(ClientInfo* _info, Login* _login,const char* flag);
	static BOOL AuctionProcess(ClientInfo*, Item* _item,int _money);
	static void UnPackPacketTCP(char* buf, char* _str1,char* _str2,int& money);
	static void UnPackPacketTCP(char* buf, char* _str1, char* _str2);
	static void UnPackPacketTCP(char* buf,int&money);
	static void UnPackPacketTCP(char* buf,int& ItemNumber, int&money);
	static int PackPacketTCP(char* _buf,PROTOCOL _protocol,ERR_CODE _err, char* _msg);
	static int PackPacketTCP(char* _buf, PROTOCOL _protocol, ERR_CODE _err,int _money, char* _msg);
	static int PackPacketTCP(char* _buf, PROTOCOL _protocol, ERR_CODE _err, int _number,char* _str1,char*_str2,int _money);
	static int PackPacketTCP(char* _buf, PROTOCOL _protocol, ERR_CODE _err, SOCKADDR_IN _addr);
	static DWORD WINAPI ProcessClient(LPVOID arg);

};

bool PacketRecv(SOCKET _sock, char* _buf);
bool FileDataLoad(Login* _data, int&UserCount);
bool FileDataAdd(Login* _info);
bool FileDataUpdate(ClientInfo* _ptr);
int recvn(SOCKET s, char *buf, int len, int flags);
void GetProtocol(const char* _ptr, PROTOCOL& _protocol);
BOOL SearchFile(const char *filename);

//필요시 추가생성
int PackPacketTCP();
int PackPacketUDP();

#endif // !CLIENTMANAGER_H
