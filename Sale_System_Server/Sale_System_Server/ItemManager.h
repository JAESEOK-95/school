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
	int MultiPortNumber;		//��Ƽĳ��Ʈ ��Ʈ�ѹ�
	Item* ItemList[3];  //�⺻ 3�� �����


};

#endif // !ITEMMANAGER_H
