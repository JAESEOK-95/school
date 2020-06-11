#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H
/*
���� ���� Ŭ����

*/


#include "Global.h"
#include "ItemManager.h"
#include "Client.h"


//�и��� public�� �־������ ����ε� ����
static CLinkedList<ClientInfo*>* ClientList;
static int ClientCount;
static Login User_List[100];
static int User_Count;
static ItemManager* Item_master;
class ClientManager
{
public:  //��� Ŭ�� ���� ��� �������� ��������
	WSADATA wsa;
	SOCKET client_sock, listen_sock, udpSock;
	SOCKADDR_IN serveraddr,clientaddr;
	HANDLE hThread;
	DWORD ThreadId;

	int  retval, addrlen;
	ClientManager();	//LINKED LIST  ���� �ʱ�ȭ �� wsa �ʱ�ȭ
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

//�ʿ�� �߰�����
int PackPacketTCP();
int PackPacketUDP();

#endif // !CLIENTMANAGER_H
