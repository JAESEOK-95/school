#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

#define BUFSIZE 4096
#define IDSIZE 255
#define PWSIZE 255
#define NICKNAMESIZE 255


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


enum PROTOCOL
{
	NON_PROTOCOL,
	JOIN_INFO,
	LOGIN_INFO,
	CASH_IN_INFO,
	AUCTION_MENU_SELECT,
	AUCTION_MENU_INFO,
	AUCTION_INFO,
	LOG_OUT,
	JOIN_RESULT,	//회원가입 리턴
	LOGIN_RESULT,  //로그인 리턴
	CASH_IN_RESULT,  //입금 리턴
	AUCTION_RESULT,  //경매 리턴
	AUCTION_MENU_SELECT_RESULT,		//현재 경매물품 리턴
	MULTIJOIN_RESULT,
	QUIT
};

enum MENU 
{ 
	WAIT = -1,
	JOIN_MENU = 1, 
	LOGIN_MENU, 
	EXIT_MENU = 3, 
	AUCTION_MENU = 4, 
	CASH_ADD_MENU = 5, 
	LOGOUT_MENU = 6
};

enum STATE
{
	WAIT_STATE,
	MENU_STATE = 1, //처음시작
	JOIN_STATE, //회원가입 상태
	LOGIN_STATE, // 로그인 상태
	CASH_IN_STATE, //입금 상태
	AUCTION_STATE, //경매 참여 상태
	LOGIN_MENU_STATE  //로그인 후 메뉴 상태
};

struct Login
{
	int u_number;  // 유저고유번호
	char u_id[IDSIZE];   //유저 ID
	char u_pw[PWSIZE];  // 유저PW
	int U_Cash;			//해당 클라의 잔액
	bool Auction_flag = false;		//경매 시작했는지 구분(선택지 부터 경매시작)

};

// 소켓 함수 오류 출력 후 종료
void err_quit(const char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(-1);
}

// 소켓 함수 오류 출력
void err_display(const char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (LPCTSTR)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

// 사용자 정의 데이터 수신 함수
int recvn(SOCKET s, char *buf, int len, int flags)
{
	int received;
	char *ptr = buf;
	int left = len;

	while (left > 0) {
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;
		left -= received;
		ptr += received;
	}

	return (len - left);
}

bool PacketRecv(SOCKET _sock, char* _buf)
{
	int size;

	int retval = recvn(_sock, (char*)&size, sizeof(size), 0);
	if (retval == SOCKET_ERROR)
	{
		err_display("recv error()");
		return false;
	}
	else if (retval == 0)
	{
		return false;
	}

	retval = recvn(_sock, _buf, size, 0);
	if (retval == SOCKET_ERROR)
	{
		err_display("recv error()");
		return false;

	}
	else if (retval == 0)
	{
		return false;
	}

	return true;
}

void GetProtocol(char* _ptr, PROTOCOL& _protocol)
{
	memcpy(&_protocol, _ptr, sizeof(PROTOCOL));

}

void GetResult(char* _ptr, ERR_CODE& _err)//프로토콜 뒤에 에러코드가 나옴
{
	char* temp = _ptr + sizeof(PROTOCOL);
	memcpy(&_err, temp, sizeof(ERR_CODE));

}

void PackPacket(char* _buf, PROTOCOL _protocol,int ItemNumber, int money, int&_size)
{
	char* ptr = _buf;

	_size = 0;
	int;
	ptr = ptr + sizeof(_size);

	memcpy(ptr, &_protocol, sizeof(PROTOCOL));
	ptr = ptr + sizeof(PROTOCOL);
	_size = _size + sizeof(PROTOCOL);


	memcpy(ptr, &ItemNumber, sizeof(int));
	ptr = ptr + sizeof(int);
	_size = _size + sizeof(int);

	memcpy(ptr, &money, sizeof(int));
	ptr = ptr + sizeof(int);
	_size = _size + sizeof(int);



	ptr = _buf;
	memcpy(ptr, &_size, sizeof(_size));

	_size = _size + sizeof(_size);
}
void PackPacket(char* _buf, PROTOCOL _protocol, int money, int&_size)
{
	char* ptr = _buf;

	_size = 0;
	int ;
	ptr = ptr + sizeof(_size);

	memcpy(ptr, &_protocol, sizeof(PROTOCOL));
	ptr = ptr + sizeof(PROTOCOL);
	_size = _size + sizeof(PROTOCOL);

	memcpy(ptr, &money, sizeof(int));
	ptr = ptr + sizeof(int);
	_size = _size + sizeof(int);

	

	ptr = _buf;
	memcpy(ptr, &_size, sizeof(_size));

	_size = _size + sizeof(_size);
}
void PackPacket(char* _buf, PROTOCOL _protocol, int& _size)
{
	char* ptr = _buf;

	_size = 0;

	ptr = ptr + sizeof(_size);

	memcpy(ptr, &_protocol, sizeof(PROTOCOL));
	ptr = ptr + sizeof(PROTOCOL);
	_size = _size + sizeof(PROTOCOL);

	ptr = _buf;
	memcpy(ptr, &_size, sizeof(_size));

	_size = _size + sizeof(_size);
}

void PackPacket(char* _buf, PROTOCOL _protocol, char* _str1, char* _str2, char* _str3, int& _size)
{
	char* ptr = _buf;
	int strsize1 = strlen(_str1);
	int strsize2 = strlen(_str2);
	int strsize3 = strlen(_str3);

	_size = 0;

	ptr = ptr + sizeof(_size);

	memcpy(ptr, &_protocol, sizeof(_protocol));
	ptr = ptr + sizeof(_protocol);
	_size = _size + sizeof(_protocol);

	memcpy(ptr, &strsize1, sizeof(strsize1));
	ptr = ptr + sizeof(strsize1);
	_size = _size + sizeof(strsize1);

	memcpy(ptr, _str1, strsize1);
	ptr = ptr + strsize1;
	_size = _size + strsize1;

	memcpy(ptr, &strsize2, sizeof(strsize2));
	ptr = ptr + sizeof(strsize2);
	_size = _size + sizeof(strsize2);

	memcpy(ptr, _str2, strsize2);
	ptr = ptr + strsize2;
	_size = _size + strsize2;

	memcpy(ptr, &strsize3, sizeof(strsize3));
	ptr = ptr + sizeof(strsize3);
	_size = _size + sizeof(strsize3);

	memcpy(ptr, _str3, strsize3);
	ptr = ptr + strsize3;
	_size = _size + strsize3;

	ptr = _buf;
	memcpy(ptr, &_size, sizeof(_size));

	_size = _size + sizeof(_size);
}

void PackPacket(char* _buf, PROTOCOL _protocol, char* _str1, char* _str2, int& _size)
{
	char* ptr = _buf;
	int strsize1 = strlen(_str1);
	int strsize2 = strlen(_str2);

	_size = 0;

	ptr = ptr + sizeof(_size);

	memcpy(ptr, &_protocol, sizeof(PROTOCOL));
	ptr = ptr + sizeof(PROTOCOL);
	_size = _size + sizeof(PROTOCOL);

	memcpy(ptr, &strsize1, sizeof(strsize1));
	ptr = ptr + sizeof(strsize1);
	_size = _size + sizeof(strsize1);

	memcpy(ptr, _str1, strsize1);
	ptr = ptr + strsize1;
	_size = _size + strsize1;

	memcpy(ptr, &strsize2, sizeof(strsize2));
	ptr = ptr + sizeof(strsize2);
	_size = _size + sizeof(strsize2);

	memcpy(ptr, _str2, strsize2);
	ptr = ptr + strsize2;
	_size = _size + strsize2;

	ptr = _buf;
	memcpy(ptr, &_size, sizeof(_size));

	_size = _size + sizeof(_size);
}


void PackPacket(char* _buf, PROTOCOL _protocol, char* _str1, char* _str2, int money, int& _size)
{
	char* ptr = _buf;
	int strsize1 = strlen(_str1);
	int strsize2 = strlen(_str2);

	_size = 0;

	ptr = ptr + sizeof(_size);

	memcpy(ptr, &_protocol, sizeof(PROTOCOL));
	ptr = ptr + sizeof(PROTOCOL);
	_size = _size + sizeof(PROTOCOL);

	memcpy(ptr, &strsize1, sizeof(strsize1));
	ptr = ptr + sizeof(strsize1);
	_size = _size + sizeof(strsize1);

	memcpy(ptr, _str1, strsize1);
	ptr = ptr + strsize1;
	_size = _size + strsize1;

	memcpy(ptr, &strsize2, sizeof(strsize2));
	ptr = ptr + sizeof(strsize2);
	_size = _size + sizeof(strsize2);

	memcpy(ptr, _str2, strsize2);
	ptr = ptr + strsize2;
	_size = _size + strsize2;


	memcpy(ptr, &money, sizeof(int));
	ptr = ptr + sizeof(int);
	_size = _size + sizeof(int);

	ptr = _buf;
	memcpy(ptr, &_size, sizeof(_size));

	_size = _size + sizeof(_size);
}

void UnPackPacket(char* _buf, char* _str1)
{
	int strsize1;

	char* ptr = _buf + sizeof(PROTOCOL)+sizeof(ERR_CODE);

	memcpy(&strsize1, ptr, sizeof(strsize1));
	ptr = ptr + sizeof(strsize1);

	memcpy(_str1, ptr, strsize1);
	ptr = ptr + strsize1;
}
void UnPackPacket(char* _buf, int& _number,char* _str1,char* _str2,int&money)
{
	int strsize1;
	int strsize2;

	char* ptr = _buf + sizeof(PROTOCOL) + sizeof(ERR_CODE);
	
	memcpy(&_number, ptr, sizeof(int));
	ptr = ptr + sizeof(int);


	memcpy(&strsize1, ptr, sizeof(strsize1));
	ptr = ptr + sizeof(strsize1);

	memcpy(_str1, ptr, strsize1);
	ptr = ptr + strsize1;

	memcpy(&strsize2, ptr, sizeof(strsize2));
	ptr = ptr + sizeof(strsize2);

	memcpy(_str2, ptr, strsize2);
	ptr = ptr + strsize2;

	memcpy(&money, ptr, sizeof(int));
	ptr = ptr + sizeof(int);

}

void UnPackPacket(char* _buf, int&money, char* _str1)
{
	int strsize1;

	char* ptr = _buf + sizeof(PROTOCOL) + sizeof(ERR_CODE);


	memcpy(&money, ptr, sizeof(int));
	ptr = ptr + sizeof(int);

	memcpy(&strsize1, ptr, sizeof(strsize1));
	ptr = ptr + sizeof(strsize1);

	memcpy(_str1, ptr, strsize1);
	ptr = ptr + strsize1;

}
void UnPackPacket(char* _buf, SOCKADDR_IN& _addr)
{

	char* ptr = _buf + sizeof(PROTOCOL) + sizeof(ERR_CODE);

	memcpy(&_addr, ptr, sizeof(SOCKADDR_IN));
	ptr = ptr + sizeof(SOCKADDR_IN);

}
int MenuSelect(STATE flag, char* msg);
int main(int argc, char* argv[])
{
	int retval;
	Login Temp;
	ZeroMemory(&Temp, sizeof(Login));
	struct ip_mreq mreq;
	STATE state = MENU_STATE;
	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return -1;

	// socket()
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	SOCKET udp_sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (udp_sock == INVALID_SOCKET) err_quit("socket()");


	int optval = true;
	retval = setsockopt(udp_sock, SOL_SOCKET, SO_REUSEADDR,
		(char*)&optval, sizeof(optval));
	if (retval == SOCKET_ERROR) err_quit("setsockopt()");

	// connect()
	SOCKADDR_IN serveraddr,udp_localaddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	ZeroMemory(&udp_localaddr, sizeof(udp_localaddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(9000);
	serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");


	char buf[BUFSIZE];
	char msg[BUFSIZE];
	char login_text[BUFSIZE];
	char item_text[BUFSIZE];
	char Auction_text[BUFSIZE];
	int size;
	int money;
	bool endflg = false;
	ERR_CODE result = NON_ERROR;
	int select = WAIT, item_num = 0;
	while (1)
	{
		
		switch (state)
		{
		case MENU_STATE:
			select = MenuSelect(state, nullptr);
			break;
		case CASH_IN_STATE:
		case LOGIN_MENU_STATE:
			select = MenuSelect(state, login_text);
			break;
		case AUCTION_STATE:
			select = MenuSelect(state, item_text);
			break;
		}
		//잘못된 부분. 입력을 잘못하면 다시해야함
		// 옥션모드인지 체크안함
		if (select >= 1000 && state == AUCTION_STATE) //품목번호 기준
		{
			printf("\n 입찰할 금액 :");
			scanf("%d", &money);
			PackPacket(buf, AUCTION_MENU_INFO, select, money, size);
			int retval = send(sock, buf, size, 0);
			if (retval == SOCKET_ERROR)
			{
				err_display("send()");
				break;
			}
			state = WAIT_STATE;
			select = WAIT;		//바로 리시브로.
			printf("\n 인원이 다 차면 체크를 시작합니다....\n");
			continue;

		}
		else if (select >= 1 && state == AUCTION_STATE)
		{
			printf("다시 입력하세요. 올바른 품목번호가 아닙니다.\n");
			system("pause");
			continue;
		}
		switch (select)
		{
		case JOIN_MENU:
		{
			char id[IDSIZE];
			printf("ID:");
			scanf("%s", id);

			char pw[PWSIZE];
			printf("PW:");
			scanf("%s", pw);

			printf("최초 입금금액:");
			scanf("%d", &money);

			int size;
			PackPacket(buf, JOIN_INFO, id, pw, money, size);

			int retval = send(sock, buf, size, 0);
			if (retval == SOCKET_ERROR)
			{
				err_display("send()");
				break;
			}

		}
		break;
		case LOGIN_MENU:
		{
			char id[IDSIZE];
			printf("ID:");
			scanf("%s", id);

			char pw[PWSIZE];
			printf("PW:");
			scanf("%s", pw);

			int size;
			PackPacket(buf, LOGIN_INFO, id, pw, size);

			int retval = send(sock, buf, size, 0);
			if (retval == SOCKET_ERROR)
			{
				err_display("send()");
				break;
			}

		}
		break;
		case EXIT_MENU:
			endflg = true;
			break;

		case WAIT:
			break;

		case AUCTION_MENU:
			PackPacket(buf, AUCTION_MENU_SELECT, size);
			retval = send(sock, buf, size, 0);
			if (retval == SOCKET_ERROR)
			{
				err_display("send()");
				break;
			}
			break;
		case CASH_ADD_MENU:
			system("cls");
			printf("%s\n", login_text);
			printf("\n\n");
			printf(" 추가할금액 :");
			scanf("%d", &money);
			PackPacket(buf, CASH_IN_INFO, money, size);
			retval = send(sock, buf, size, 0);
			if (retval == SOCKET_ERROR)
			{
				err_display("send()");
				break;
			}
			break;
		case LOGOUT_MENU:
			PackPacket(buf, LOG_OUT, size);
			retval = send(sock, buf, size, 0);
			if (retval == SOCKET_ERROR)
			{
				err_display("send()");
				break;
			}
			ZeroMemory(&Temp, sizeof(Login));
			system("cls");	//로그아웃 선택시 아웃
			state = MENU_STATE;
			continue;		//다시 메뉴 선택으로
			break;
		default:
			state = MENU_STATE;
			select = 0;
			printf("잘못입력했습니다.\n");
			continue;
			break;

		}

		if (endflg)
		{
			break;
		}

		if (!PacketRecv(sock, buf))
		{
			break;
		}

		PROTOCOL protocol;
		int key = 0;
		GetProtocol(buf, protocol);
		GetResult(buf, result);
		switch (protocol)
		{
		case JOIN_RESULT:

			switch (result)
			{
			case JOIN_SUCCESS:
			case JOIN_ERROR_ID:  //아이디 실패 했으므로 돌아감

				memset(msg, 0, sizeof(msg));
				UnPackPacket(buf, msg);
				printf("%s\n\n", msg);
				system("pause");
				state = MENU_STATE;
				continue;
				break;
			}

			break;
		case CASH_IN_RESULT:
		case LOGIN_RESULT:
			switch (result)
			{
			case ID_ERROR:
			case PW_ERROR:
			case USE_LOGIN:
				memset(msg, 0, sizeof(msg));
				UnPackPacket(buf, msg);
				printf("%s\n\n", msg);
				system("pause");
				state = MENU_STATE;
				continue;
				break;
			case CASH_UPDATE_SUCCESS:
			case LOGIN_SUCCESS:
				memset(login_text, 0, sizeof(login_text));
				UnPackPacket(buf,Temp.u_number,Temp.u_id,Temp.u_pw,Temp.U_Cash);
				system("cls");
					
				if (result == CASH_UPDATE_SUCCESS)
				{
					sprintf(login_text, "로그인된 아이디 : %s\n 현재 저장되어 있는 돈 : %d 원\n업데이트 완료! 아래 메뉴를 다시고르세요!\n\n\n",
						Temp.u_id, Temp.U_Cash);
				}
				else
				{
					sprintf(login_text, "로그인된 아이디 : %s\n 현재 저장되어 있는 돈 : %d 원\n아래 메뉴를 다시고르세요!\n\n\n",
						Temp.u_id, Temp.U_Cash);
				}
				state = LOGIN_MENU_STATE;
				continue;
					
				break; //RESULT BREAK
			}
		
			break;  //PROTOCOL BREAK
		case AUCTION_MENU_SELECT_RESULT:

			system("cls");
			switch (result)
			{
			case ITEM_SOLD:		// 경매 상품이 다 팔렸으니
				memset(msg, 0, sizeof(msg));
				UnPackPacket(buf, msg);
				printf("%s\n\n", msg);
				system("pause");
				state = LOGIN_MENU_STATE;
				break;
			default:
				memset(item_text, 0, sizeof(item_text));
				UnPackPacket(buf, item_text);
				state = AUCTION_STATE;
				continue;
				break;
			}

			break;
		case AUCTION_RESULT:		//경매 리턴
			system("cls");
			switch (result)
			{
				case ITEM_GET:
					
					UnPackPacket(buf, Temp.U_Cash, msg);
					memset(login_text, 0, sizeof(login_text));
					sprintf(login_text, "로그인된 아이디 : %s\n 현재 저장되어 있는 돈 : %d 원\n업데이트 완료! 아래 메뉴를 다시고르세요!\n\n\n",
						Temp.u_id, Temp.U_Cash);
					printf("%s\n\n", msg);
					system("pause");
					state = LOGIN_MENU_STATE;
					select = WAIT;
					continue;
					break;
				case CASH_LACK:
				case CLIENT_OVER:
				case ITEM_SOLD:
					memset(msg, 0, sizeof(msg));
					UnPackPacket(buf, msg);
					printf("%s\n\n", msg);
					system("pause");
					state = LOGIN_MENU_STATE;
					select = WAIT;
					continue;
					break;
			}

			break;
		}
		break;		//while BREAK
	}
		
		// closesocket()
		closesocket(sock);

		// 윈속 종료
		WSACleanup();
		return 0;
	}



int MenuSelect(STATE flag,char* msg)
{
	int select = 0;
	switch (flag)
	{
	case MENU_STATE:			//메인메뉴
		system("cls");
		printf("<<메뉴>>\n");
		printf("1. 회원가입\n");
		printf("2. 로그인\n");
		printf("3. 종료\n");
		printf("선택:");


		scanf("%d", &select);
		break;
	case CASH_IN_STATE:		//캐쉬
	case LOGIN_MENU_STATE:		//로그인 메뉴
		system("cls");
		printf("%s\n\n", msg);
		printf("<<메뉴>>\n");
		printf("1.경매참가\n");
		printf("2.온라인 입금\n");
		printf("3.로그아웃\n");

		printf("선택:");
		scanf("%d", &select);
		select += 3;		//로그인메뉴 선택 위한 계산
		break;
	case AUCTION_STATE:

		system("cls");
		printf("%s\n", msg);
		printf("선택할 경매품목번호:");
		scanf("%d", &select);
		break;
	}


	return select;
}