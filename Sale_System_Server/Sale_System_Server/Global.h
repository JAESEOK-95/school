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
#define ID_ERROR_MSG	"아이디를 잘못입력하셨습니다.\n계속하시려면 아무키나 누르세요\n"
#define PW_ERROR_MSG "패스워드를 잘못입력하셨습니다\n계속하시려면 아무키나 누르세요\n"
#define JOIN_ID_ERROR_MSG  "이미 있는 아이디입니다. 다시 시도하세요.\n계속하시려면 아무키나 누르세요\n"
#define JOIN_COMMIT_MSG "회원가입 완료! 계속하시려면 아무키나 누르세요!\n"
#define USE_LOGIN_MSG	"이미 로그인되어 있는 계정입니다. 다시시도 하세요.\n"
#define LOGIN_MSG  "로그인!\n"

void err_quit(const char *msg);
void err_display(const char *msg);

enum PROTOCOL
{
	NON_PROTOCOL,		//프로토콜 없음
	JOIN_INFO,				//회원가입 정보
	LOGIN_INFO,			//로그인 정보
	CASH_IN_INFO,			//입금 정보
	AUCTION_MENU_SELECT,	//경매 메뉴 요청정보
	AUCTION_MENU_INFO,		//메뉴 선택 후 정보
	AUCTION_INFO,		//경매 데이터 = 입찰금액
	LOG_OUT,		//로그아웃
	JOIN_RESULT,	//회원가입 리턴
	LOGIN_RESULT,  //로그인 리턴
	CASH_IN_RESULT,  //입금 리턴
	AUCTION_RESULT,  //경매 리턴
	AUCTION_MENU_SELECT_RESULT,		//현재 경매물품 리턴
	MULTIJOIN_RESULT,		//멀티 캐스팅용 프로토콜 (현 사용안함)
	QUIT
};

enum STATE
{
	MENU_STATE = 1, //처음시작
	JOIN_STATE, //회원가입 상태
	LOGIN_STATE, // 로그인 상태
	CASH_IN_STATE, //입금 상태
	AUCTION_STATE, //경매 참여 상태
	LOGIN_MENU_STATE  //로그인 후 메뉴 상태
};
enum ERR_CODE
{
	NON_ERROR = -2,
	ID_ERROR = 1, //아이디 에러
	PW_ERROR,  // 패스워드 에러
	USE_LOGIN,		// 이미 로그인된 계정
	JOIN_ERROR_ID,  //중복 ID
	JOIN_SUCCESS,		//회원가입 성공
	LOGIN_SUCCESS,		// 로그인 성공
	CASH_UPDATE_SUCCESS,	// 돈 업데이트 성공
	CASH_LACK,  // 돈이 부족하다(경매참여 못할경우)
	CLIENT_OVER, //클라이언트 오버(해당 경매품 종료)
	ITEM_SOLD,		//참여했던 경매 물품이 팔렸음!
	ITEM_GET    //참여했던 경매 아이템을 겟함!
};





#endif // !GLOBAL_H

