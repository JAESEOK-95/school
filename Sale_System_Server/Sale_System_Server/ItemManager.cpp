#include "ItemManager.h"


ItemManager::ItemManager()
{

	this->ItemNumber = 1000;  // 아이템 기준번호 세팅
	this->MultiPortNumber = 9001;
	ZeroMemory(ItemList, sizeof(ItemList));

	this->CreateDeafult_Item();  // 아이템 기본  값 세팅

}

ItemManager::~ItemManager()
{


}

void ItemManager::CreateDeafult_Item()
{	
	ItemList[0] = new Item(this->ItemNumber++, "1번경매품", 30000, 2);

	ItemList[1] = new Item(this->ItemNumber++, "2번경매품", 40000, 2);

	ItemList[2] = new Item(this->ItemNumber++, "3번경매품", 100000, 2);

}

Item** ItemManager::Get_ItemList()
{
	return this->ItemList;
}