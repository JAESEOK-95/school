#include "Client.h"

ClientInfo::ClientInfo(const SOCKET _sock,  const SOCKADDR_IN _addr)
{
	SetSock(_sock);
	UpdateState(MENU_STATE);
	InItLogin();
	InItData();
}

ClientInfo::~ClientInfo()
{


}

//Get  계열 =====
SOCKET ClientInfo::GetSock()
{
	return this->sock;
}

SOCKADDR_IN ClientInfo::GetAddr()
{
	return this->addr;
}
SOCKADDR_IN ClientInfo::GetMuitiAddr()
{
	return this->m_addr;
}

STATE ClientInfo::GetState()
{
	return this->state;
}

int ClientInfo::GetMoney()
{
	Login* Temp;
	Temp = this->GetLoginData();
	return Temp->U_Cash;
}

Login* ClientInfo::GetLoginData()
{
	return login;
}
//===============


//Set 계열=====
void ClientInfo::InItItem()
{
	this->Check_Item = nullptr;
}
void ClientInfo::InItLogin()
{
	this->login = nullptr;
}
void ClientInfo::InItData()
{
	// 데이터 송수신

	ZeroMemory(this->buf, sizeof(BUFSIZE));
	this->recvbytes_Check = 0;
	this->recvbytes = 0;
	this->err_code = NON_ERROR;
	this->r_sizeflag = false;

	// 데이터 송수신
}
void ClientInfo::UpdateState(STATE _state)
{
	this->state = _state;
}


void ClientInfo::SetAuctionItem(Item* _tem)
{
	this->Check_Item = _tem;
}


void ClientInfo::SetClient_Login(Login* _login)
{
	this->login = _login;
}


void ClientInfo::SetErrCode(ERR_CODE _code)
{
	this->err_code = _code;
}

void ClientInfo::SetSock(SOCKET _sock)
{
	this->sock = _sock;
}

void ClientInfo::SetAddr(SOCKADDR_IN _addr)
{
	memcpy(&this->addr, &_addr, sizeof(SOCKADDR_IN));
}

void ClientInfo::SetLogOut(Login* _login)
{
	_login->UseLogin = false;
}
void ClientInfo::UpdateMoney(Login* _login, int money)
{
	_login->U_Cash += money;
}



//=================