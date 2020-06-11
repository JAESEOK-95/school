#ifndef GLOBAL_H
#define GLOBAL_H

#pragma comment(lib, "ws2_32")

#include <winsock2.h>
#include <stdlib.h>
#include<string>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include "LinkedList.h"
#define SERVERPORT 9000
#define STR_MAX 255
#define BUFSIZE 4096
#define USER_NUMBERRING 1000
#define ROOMADDR_1 "235.7.8.9"
#define ROOMADDR_2 "235.7.8.10"
#define ROOMADDR_3 "235.7.8.11"
#define LOGIN_DATA "LOGIN"
#define JOIN_DATA "JOIN"
#define ID_ERROR_MSG	"���̵� �߸��Է��ϼ̽��ϴ�.\n����Ͻ÷��� �ƹ�Ű�� ��������\n"
#define PW_ERROR_MSG "�н����带 �߸��Է��ϼ̽��ϴ�\n����Ͻ÷��� �ƹ�Ű�� ��������\n"
#define JOIN_ID_ERROR_MSG  "�̹� �ִ� ���̵��Դϴ�. �ٽ� �õ��ϼ���.\n����Ͻ÷��� �ƹ�Ű�� ��������\n"
#define JOIN_COMMIT_MSG "ȸ������ �Ϸ�! ����Ͻ÷��� �ƹ�Ű�� ��������!\n"
#define USE_LOGIN_MSG	"�̹� �α��εǾ� �ִ� �����Դϴ�. �ٽýõ� �ϼ���.\n"
#define LOGIN_MSG  "�α���!\n"

void err_quit(const char *msg);
void err_display(const char *msg);

enum PROTOCOL
{
	NON_PROTOCOL,		//�������� ����
	JOIN_INFO,				//ȸ������ ����
	LOGIN_INFO,			//�α��� ����
	CASH_IN_INFO,			//�Ա� ����
	AUCTION_MENU_SELECT,	//��� �޴� ��û����
	AUCTION_MENU_INFO,		//�޴� ���� �� ����
	AUCTION_INFO,		//��� ������ = �����ݾ�
	LOG_OUT,		//�α׾ƿ�
	JOIN_RESULT,	//ȸ������ ����
	LOGIN_RESULT,  //�α��� ����
	CASH_IN_RESULT,  //�Ա� ����
	AUCTION_RESULT,  //��� ����
	AUCTION_MENU_SELECT_RESULT,		//���� ��Ź�ǰ ����
	MULTIJOIN_RESULT,		//��Ƽ ĳ���ÿ� �������� (�� ������)
	QUIT
};

enum STATE
{
	MENU_STATE = 1, //ó������
	JOIN_STATE, //ȸ������ ����
	LOGIN_STATE, // �α��� ����
	CASH_IN_STATE, //�Ա� ����
	AUCTION_STATE, //��� ���� ����
	LOGIN_MENU_STATE  //�α��� �� �޴� ����
};
enum ERR_CODE
{
	NON_ERROR = -2,
	ID_ERROR = 1, //���̵� ����
	PW_ERROR,  // �н����� ����
	USE_LOGIN,		// �̹� �α��ε� ����
	JOIN_ERROR_ID,  //�ߺ� ID
	JOIN_SUCCESS,		//ȸ������ ����
	LOGIN_SUCCESS,		// �α��� ����
	CASH_UPDATE_SUCCESS,	// �� ������Ʈ ����
	CASH_LACK,  // ���� �����ϴ�(������� ���Ұ��)
	CLIENT_OVER, //Ŭ���̾�Ʈ ����(�ش� ���ǰ ����)
	ITEM_SOLD,		//�����ߴ� ��� ��ǰ�� �ȷ���!
	ITEM_GET    //�����ߴ� ��� �������� ����!
};





#endif // !GLOBAL_H

