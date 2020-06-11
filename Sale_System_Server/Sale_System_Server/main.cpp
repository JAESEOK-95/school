/*
게임제작 중간고사
18032062 이재석 
 경매 시스템 서버


 로그인 계정 구조체
 아이템 계정 구조체(미리 등록) => 자동생성
 회원가입 방식은 교수님 소스에 저장방식을 따른다.


 첫메뉴 구성(클라이언트는 버튼으로 구축) 이후 선택하게끔 설정
 
 1회원가입 2로그인 3 종료
 회원가입 후 로그인 하고나면 메뉴
 1.경매참여신청 2.입금 3.로그아웃


 1일차 : 서버쪽 통신 구축 및 헤더파일 및 프로토콜,스테이더스값 등등 구축 

 enum STATE
 {
 : INIT_STATE  =1, //처음시작
 JOIN_STATE , //회원가입 상태
 LOGIN_STATE, // 로그인 상태
 CASH_IN_STATE, //입금 상태
 AUCTION_STATE //경매 참여 상태
 }
 enum ERRCODE
 {
	ID_ERROR=1, //아이디 에러
	PW_ERROR,  // 패스워드 에러
	JOIN_ERROR_ID,  //중복 ID
	CASH_LACK,  // 돈이 부족하다(경매참여 못할경우)
	CLIENT_OVER, //클라이언트 오버(해당 경매품 종료)
	ITEM_SOLD		//참여했던 경매 물품이 팔렸음!
 }


 enum 

 {
	JOIN_RESULT,		//회원가입 리턴
	LOGIN_RESULT,  //로그인 리턴
	CASH_IN_RESULT,  //입금 리턴
	AUCTION_RESULT  //경매 리턴

 }

 -------------------------------------------- GLOBAL
 struct Login
 {
	int u_number;  // 유저고유번호
	char u_id[CHAR_MAX];   //유저 ID
	char u_pw[CHAR_MAX];  // 유저PW

 }


 struct Item
 {
	int tem_number;			//경매품목 고유번호
	char tem_name[CHAR_MAX];		//경매품목 
	char tem_price[CHAR_MAX];		//경매 품목의 최대갯수
	int tem_maxClient;			//최대 클라이언트 수 체크
 }

 
 sturct ClientInfo
 {
	SOCKET sock;		//소켓
	SOCKADDR_IN addr;		//소켓정보
	STATE state;			//클라 상태값
	Login login;		//클라 로그인정보
	Item* Check_Item			//가지고 있는 정보
	ERR_CODE err_code;
	char recvbuf[BUFSIZE];
	char sendbuf[BUFSIZE];
	int recvbytes;
	int recvbytes_Check;
	int sendbytes;
	int sendbytes_Check;
	bool r_sizeflag;
	int U_Cash;			//해당 클라의 잔액
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
