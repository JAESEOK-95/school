#include "ItemManager.h"


ItemManager::ItemManager()
{

	this->ItemNumber = 1000;  // ������ ���ع�ȣ ����
	this->MultiPortNumber = 9001;
	ZeroMemory(ItemList, sizeof(ItemList));

	this->CreateDeafult_Item();  // ������ �⺻  �� ����

}

ItemManager::~ItemManager()
{


}

void ItemManager::CreateDeafult_Item()
{	
	ItemList[0] = new Item(this->ItemNumber++, "1�����ǰ", 30000, 2);

	ItemList[1] = new Item(this->ItemNumber++, "2�����ǰ", 40000, 2);

	ItemList[2] = new Item(this->ItemNumber++, "3�����ǰ", 100000, 2);

}

Item** ItemManager::Get_ItemList()
{
	return this->ItemList;
}