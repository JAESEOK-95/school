#ifndef ITEM_H
#define ITEM_H


#include "Global.h"



class Item
{
public:

	Item( const int _num,const char* _name , const int _price,const int client_max);  // set ��ũ�� ��ü
	~Item();
	int Get_ItemNumber();
	int Get_temPrice();		// �ּ� �ݾ� ��������
	int Get_ClientMax();		// Ŭ���̾�Ʈ �� ��������
	char* Get_temName();  //�̸� ��������
	bool Get_Auctionflag();
	Item* Get_ItemData();
	int Get_ClientCount();
	int Get_MaxUserPrice();		//�ְ� ������ ���ϱ�
	char* Get_BidUser();				// �ְ� ���� �������ϱ�
	HANDLE GetHandle();		//��� �̺�Ʈ �ڵ�
	void SetTemPrice(const int _price);
	void SetTemNumber(const int _Number);
	void SetMaxClient(const int _Number);
	void SetAuctionflag(const bool flag);
	void UpdateClientCount(int num);
	void Set_MaxUserPrice(int price);		//�ְ� ������ ����
	void Set_BidUser(char* id);				// �ְ� ���� ��������
private:

	int tem_number;			//���ǰ�� ������ȣ
	char tem_name[STR_MAX];		//���ǰ�� 
	int tem_price;					//��� ǰ���� �ּұݾ�
	int tem_maxClient;			//�ִ� Ŭ���̾�Ʈ �� üũ
	int ClientCount;
	int Max_UserPrice;			//����ū ������ ���ϱ�
	char bidUser[STR_MAX];		// �ְ���������
	bool Auction_flag;				//�ǸſϷ� ����
	HANDLE WaitEvent;
};



#endif // !ITEM_H

