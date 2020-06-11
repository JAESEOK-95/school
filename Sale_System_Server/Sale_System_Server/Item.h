#ifndef ITEM_H
#define ITEM_H


#include "Global.h"



class Item
{
public:

	Item( const int _num,const char* _name , const int _price,const int client_max);  // set 메크로 대체
	~Item();
	int Get_ItemNumber();
	int Get_temPrice();		// 최소 금액 가져오기
	int Get_ClientMax();		// 클라이언트 수 가져오기
	char* Get_temName();  //이름 가져오기
	bool Get_Auctionflag();
	Item* Get_ItemData();
	int Get_ClientCount();
	int Get_MaxUserPrice();		//최고 입찰가 구하기
	char* Get_BidUser();				// 최고 입찰 유저구하기
	HANDLE GetHandle();		//대기 이벤트 핸들
	void SetTemPrice(const int _price);
	void SetTemNumber(const int _Number);
	void SetMaxClient(const int _Number);
	void SetAuctionflag(const bool flag);
	void UpdateClientCount(int num);
	void Set_MaxUserPrice(int price);		//최고 입찰가 세팅
	void Set_BidUser(char* id);				// 최고 입찰 유저세팅
private:

	int tem_number;			//경매품목 고유번호
	char tem_name[STR_MAX];		//경매품목 
	int tem_price;					//경매 품목의 최소금액
	int tem_maxClient;			//최대 클라이언트 수 체크
	int ClientCount;
	int Max_UserPrice;			//가장큰 입찰금 구하기
	char bidUser[STR_MAX];		// 최고입찰유저
	bool Auction_flag;				//판매완료 여부
	HANDLE WaitEvent;
};



#endif // !ITEM_H

