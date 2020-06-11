#ifndef ITEMMANAGER_H
#define ITEMMANAGER_H


#include "Global.h"
#include "Item.h"



class ItemManager
{
public:
	ItemManager();
	~ItemManager();

	void CreateDeafult_Item();
	Item** Get_ItemList();
private:
	int ItemNumber;
	int MultiPortNumber;		//멀티캐스트 포트넘버
	Item* ItemList[3];  //기본 3개 만들기
	//PORT


};

#endif // !ITEMMANAGER_H
