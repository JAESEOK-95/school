#include "Item.h"


Item::Item(const int _num,const char* _name, const int _price, const int client_max)
{

	memset(this->tem_name, 0, sizeof(this->tem_name));
	memset(this->bidUser, 0, sizeof(this->bidUser));
	strcpy(this->tem_name, _name);
	this->tem_price = _price;
	this->tem_maxClient = client_max;
	this->tem_number = _num;
	this->ClientCount = 0;
	this->Max_UserPrice = 0;
	this->WaitEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

}
Item::~Item()
{

}
HANDLE Item::GetHandle()
{
	return this->WaitEvent;
}

int Item::Get_MaxUserPrice()		
{
	return this->Max_UserPrice;
}

char* Item::Get_BidUser()
{
	return this->bidUser;
}

int Item::Get_ItemNumber()
{
	return this->tem_number;
}
int Item::Get_temPrice()
{
	return this->tem_price;
}

int Item::Get_ClientMax()
{
	return this->tem_maxClient;
}

char* Item::Get_temName()
{
	return this->tem_name;
}

bool Item::Get_Auctionflag()
{
	return this->Auction_flag;
}

Item* Item::Get_ItemData()
{
	return this;
}

int Item::Get_ClientCount()
{
	return this->ClientCount;
}

void Item::SetTemPrice(const int _price)
{

	this->tem_price = _price;
}

void Item::SetTemNumber(const int _Number)
{
	this->tem_number = _Number;
}

void Item::SetMaxClient(const int _Number)
{
	this->tem_maxClient = _Number;
}

void Item::SetAuctionflag(const bool flag)
{
	this->Auction_flag = flag;
}

void Item::UpdateClientCount(int num)
{
	this->ClientCount += num;
}

void Item::Set_MaxUserPrice(int price)		//최고 입찰가 세팅
{
	this->Max_UserPrice = price;
}
void Item::Set_BidUser(char* id)				// 최고 입찰 유저세팅
{
	strcpy(this->bidUser, id);
}