/*
�������� �߰����
18032062 ���缮 
 ��� �ý��� ����


 �α��� ���� ����ü
 ������ ���� ����ü(�̸� ���) => �ڵ�����
 ȸ������ ����� ������ �ҽ��� �������� ������.


 ù�޴� ����(Ŭ���̾�Ʈ�� ��ư���� ����) ���� �����ϰԲ� ����
 
 1ȸ������ 2�α��� 3 ����
 ȸ������ �� �α��� �ϰ��� �޴�
 1.���������û 2.�Ա� 3.�α׾ƿ�


 1���� : ������ ��� ���� �� ������� �� ��������,�����̴����� ��� ���� 

 enum STATE
 {
 : INIT_STATE  =1, //ó������
 JOIN_STATE , //ȸ������ ����
 LOGIN_STATE, // �α��� ����
 CASH_IN_STATE, //�Ա� ����
 AUCTION_STATE //��� ���� ����
 }
 enum ERRCODE
 {
	ID_ERROR=1, //���̵� ����
	PW_ERROR,  // �н����� ����
	JOIN_ERROR_ID,  //�ߺ� ID
	CASH_LACK,  // ���� �����ϴ�(������� ���Ұ��)
	CLIENT_OVER, //Ŭ���̾�Ʈ ����(�ش� ���ǰ ����)
	ITEM_SOLD		//�����ߴ� ��� ��ǰ�� �ȷ���!
 }


 enum 

 {
	JOIN_RESULT,		//ȸ������ ����
	LOGIN_RESULT,  //�α��� ����
	CASH_IN_RESULT,  //�Ա� ����
	AUCTION_RESULT  //��� ����

 }

 -------------------------------------------- GLOBAL
 struct Login
 {
	int u_number;  // ����������ȣ
	char u_id[CHAR_MAX];   //���� ID
	char u_pw[CHAR_MAX];  // ����PW

 }


 struct Item
 {
	int tem_number;			//���ǰ�� ������ȣ
	char tem_name[CHAR_MAX];		//���ǰ�� 
	char tem_price[CHAR_MAX];		//��� ǰ���� �ִ밹��
	int tem_maxClient;			//�ִ� Ŭ���̾�Ʈ �� üũ
 }

 
 sturct ClientInfo
 {
	SOCKET sock;		//����
	SOCKADDR_IN addr;		//��������
	STATE state;			//Ŭ�� ���°�
	Login login;		//Ŭ�� �α�������
	Item* Check_Item			//������ �ִ� ����
	ERR_CODE err_code;
	char recvbuf[BUFSIZE];
	char sendbuf[BUFSIZE];
	int recvbytes;
	int recvbytes_Check;
	int sendbytes;
	int sendbytes_Check;
	bool r_sizeflag;
	int U_Cash;			//�ش� Ŭ���� �ܾ�
 }
*/


#include "Global.h"
#include "Client.h"
#include "ClientManager.h"
int main()
{

	ClientManager* i_manager = new ClientManager();


	delete i_manager;
	return 0;

}
