#include "ClientManager.h"




ClientManager::ClientManager()
{
	ClientList = nullptr;
	ClientList = new CLinkedList<ClientInfo*>();
	ClientCount = 0;
	Item_master = new ItemManager();
	memset(&serveraddr, 0, sizeof(serveraddr));
	ZeroMemory(&User_List, sizeof(Login));
	User_Count = 0;
	FileDataLoad(User_List, User_Count);

	ConnetSelect();


}

//�Ŵ��� ���� ����� ȣ��
ClientManager::~ClientManager()
{
	CNode<ClientInfo*>* node = ClientList->GetmHead()->GetmNext();

	while (node)
	{
		if (node->GetmData() != nullptr)
		{
			delete node->GetmData();
		}

		node = node->GetmNext();
	}

	delete ClientList;
}



void ClientManager::ClientDisconnect(ClientInfo* ptr)
{
	CNode<ClientInfo*>* node = ClientList->GetmHead()->GetmNext();



	printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
	inet_ntoa(ptr->GetAddr().sin_addr), ntohs(ptr->GetAddr().sin_port));

	while (node)
	{
		if (node->GetmData() == ptr)
		{
			switch (ptr->GetState())
			{
			case CASH_IN_STATE:			//�ش� ���°��� �������̿����� �α׾ƿ� ������Ʈ �� ����.
			case LOGIN_STATE:
			case AUCTION_STATE:
				ptr->SetLogOut(ptr->GetLoginData());
				FileDataUpdate(ptr);
				break;
			}
			ClientList->Delete(ptr);
			ClientCount--;
		}
	}

}

//Ŭ���̾�Ʈ ���μ���
DWORD WINAPI ClientManager::ProcessClient(LPVOID arg)
{
	bool run_flag=true;
	int retval; 
	ClientInfo* ptr = (ClientInfo*)arg;
	PROTOCOL protocol;
	Login	TempLogin;
	Item** Temp_temList;
	int TempMoney = 0, TempData1=0,TempData2=0,size = 0;
	FD_SET P_Rset;		//Ŭ���̾�Ʈ ���μ��� ���� ��

	char temp[BUFSIZE],temp2[CHAR_MAX];
	ZeroMemory(temp, sizeof(BUFSIZE));
	ZeroMemory(temp2, sizeof(CHAR_MAX));
	ZeroMemory(&TempLogin, sizeof(Login));
	ZeroMemory(&protocol, sizeof(PROTOCOL));
	Temp_temList = nullptr;
	while (run_flag)
	{
		FD_ZERO(&P_Rset);
		FD_SET(ptr->GetSock(), &P_Rset);
		if (select(0, &P_Rset, 0, 0, NULL) == SOCKET_ERROR)
		{
			ClientDisconnect(ptr);
			err_quit("select() error");
			break;
		}

		if (FD_ISSET(ptr->GetSock(), &P_Rset))
		{
			ZeroMemory(ptr->buf, sizeof(BUFSIZE));
			if (!PacketRecv(ptr->GetSock(), ptr->buf))
			{

				ClientDisconnect(ptr);
				break;
			}

			GetProtocol(ptr->buf,protocol);

			switch (protocol)
			{
			case JOIN_INFO:
				UnPackPacketTCP(ptr->buf, TempLogin.u_id, TempLogin.u_pw,TempLogin.U_Cash);

				if (CheckDataProcess(ptr, &TempLogin, JOIN_DATA))
				{
					ptr->UpdateState(JOIN_STATE);
				}
				else
				{
					ZeroMemory(&TempLogin, sizeof(Login));
					ptr->UpdateState(MENU_STATE);
				}
				break;
			case LOGIN_INFO:
				UnPackPacketTCP(ptr->buf, TempLogin.u_id, TempLogin.u_pw);

				if (CheckDataProcess(ptr, &TempLogin, LOGIN_DATA))
				{
					ptr->UpdateState(LOGIN_STATE);
				}
				else
				{
					ZeroMemory(&TempLogin, sizeof(Login));
					ptr->UpdateState(MENU_STATE);
				}

				break;

			case CASH_IN_INFO:
				UnPackPacketTCP(ptr->buf, TempMoney);
				ptr->UpdateMoney(ptr->GetLoginData(), TempMoney);
				if (FileDataUpdate(ptr))
				{
					ptr->UpdateState(CASH_IN_STATE);

					size = PackPacketTCP(ptr->buf, CASH_IN_RESULT, CASH_UPDATE_SUCCESS, ptr->GetLoginData()->u_number, 
						ptr->GetLoginData()->u_id, ptr->GetLoginData()->u_pw, ptr->GetLoginData()->U_Cash);

					retval = send(ptr->GetSock(), ptr->buf, size, 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("DATA_RETURN_ERROR");
						run_flag = false;
						return FALSE;
					}
				}
				break;
			case AUCTION_MENU_INFO:
				UnPackPacketTCP(ptr->buf, TempData1,TempData2);
				for (int i = 0; i < 3; i++)
				{
					if ((*(Item*)Temp_temList[i]).Get_ItemNumber() == TempData1)
					{
						if (AuctionProcess(ptr, Temp_temList[i],TempData2))
						{
							ptr->UpdateState(AUCTION_STATE);
						}
					}
					else
					{
						continue;
					}
				}

				break;
			case LOG_OUT:
				ptr->SetLogOut(ptr->GetLoginData());
				break;
			case AUCTION_MENU_SELECT:
				Temp_temList = Item_master->Get_ItemList();
				sprintf(temp, "<<<���� ���ǰ�񸮽�Ʈ>>>\n");
				for (int i = 0; i < 3; i++)
				{
					//��Ű� ������ ��쿡�� �����ֱ�
					if ((*(Item*)Temp_temList[i]).Get_Auctionflag())
					{
						sprintf(temp2, "ǰ���ȣ: %d  ǰ���:  %s  ��Ž��۰�:%d  ���� �����ο� : %d\n",
							(*(Item*)Temp_temList[i]).Get_ItemNumber(), (*(Item*)Temp_temList[i]).Get_temName(),
							(*(Item*)Temp_temList[i]).Get_temPrice(),(*(Item*)Temp_temList[i]).Get_ClientMax()- (*(Item*)Temp_temList[i]).Get_ClientCount());

						strcat(temp, temp2);
					}
				}
				if (strlen(temp2) == 0)
				{
					sprintf(temp, "������ ��Ź�ǰ�� ��� �����մϴ�. ��� �ǸŵǾ����ϴ�.\n\n");
					temp[strlen(temp)] = '\0';
					ptr->UpdateState(AUCTION_STATE);
					size = PackPacketTCP(ptr->buf, AUCTION_MENU_SELECT_RESULT, ITEM_SOLD, temp);
				}
				else
				{
					temp[strlen(temp)] = '\0';
					ptr->UpdateState(AUCTION_STATE);
					size = PackPacketTCP(ptr->buf, AUCTION_MENU_SELECT_RESULT, NON_ERROR, temp);
				}

				retval = send(ptr->GetSock(), ptr->buf, size, 0);
				if (retval == SOCKET_ERROR)
				{
					err_display("DATA_RETURN_ERROR");
					run_flag = false;
					return FALSE;
				}

				break;

			}
		}
	
	}


	if (!run_flag)
	{
		ClientDisconnect(ptr);
	}
	return 0;
}


//Ŭ�� ���� ���� Ȯ��
void ClientManager::ConnetSelect()
{

	FD_SET Rset;			//SELECT ���� ���� FD_SET
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		err_quit("WSAStartup error@@@");
	}

	listen_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET)
	{
		err_quit("listen_sock_err@@@@");
	}

	udpSock = socket(PF_INET, SOCK_DGRAM, 0);
	if (udpSock == INVALID_SOCKET)
	{
		err_quit("udpSock_err@@@@");
	}
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(9000);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	u_long on = 1;
	retval = ioctlsocket(listen_sock, FIONBIO, &on);
	if (retval == SOCKET_ERROR)
	{
		err_display("ioctlsocket()");
	}

	retval = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	
	if (retval == SOCKET_ERROR)
	{
		err_quit("bind() err@@@@@");
	}


	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR)
	{
		err_quit("listen() err@@@@@@");
	}


	while (1)
	{
		FD_ZERO(&Rset);
		FD_SET(listen_sock, &Rset);
		
		if (select(0, &Rset, 0, 0, NULL) == SOCKET_ERROR)
		{
			err_quit("select() error");
			break;
		}

		if (FD_ISSET(listen_sock, &Rset))
		{
			// accept()
			addrlen = sizeof(clientaddr);
			client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
			if (client_sock == INVALID_SOCKET)
			{
				err_display("accept()");
				continue;
			}
			printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
				inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

			ClientInfo* _ptr = new ClientInfo(client_sock, clientaddr);
			ClientList->Insert(_ptr);
			ClientCount++;

			// ������ ����
			hThread = CreateThread(NULL, 0, ProcessClient,
				(LPVOID)_ptr, 0, &ThreadId);
			if (hThread == NULL)
			{
				printf("[����] ������ ���� ����!\n");
			}
			else
			{
				CloseHandle(hThread);
			}
			continue;
		}



	}
	closesocket(listen_sock);
	WSACleanup();
}



//PackPacket,UnPackPacket//////////////////////////////////////////////
void ClientManager::UnPackPacketTCP(char* buf, int&money)
{
	char* ptr = buf + sizeof(PROTOCOL);

	memcpy(&money, ptr, sizeof(int));

}
void ClientManager::UnPackPacketTCP(char* buf, int& ItemNumber, int&money)
{
	char* ptr = buf + sizeof(PROTOCOL);

	memcpy(&ItemNumber, ptr, sizeof(int));
	ptr = ptr + sizeof(int);

	memcpy(&money, ptr, sizeof(int));

}
void ClientManager::UnPackPacketTCP(char* buf, char* _str1, char* _str2, int& money)
{
	int strlen1=0, strlen2=0;
	char* ptr = buf + sizeof(PROTOCOL);

	memcpy(&strlen1, ptr, sizeof(int));
	ptr = ptr + sizeof(strlen1);

	memcpy(_str1, ptr, strlen1);
	ptr = ptr + strlen1;

	memcpy(&strlen2, ptr, sizeof(int));
	ptr = ptr + sizeof(int);

	memcpy(_str2, ptr, strlen2);
	ptr = ptr + strlen2;

	memcpy(&money, ptr, sizeof(int));

}

void ClientManager::UnPackPacketTCP(char* buf, char* _str1, char* _str2)
{
	int strlen1 = 0, strlen2 = 0;
	char* ptr = buf + sizeof(PROTOCOL);

	memcpy(&strlen1, ptr, sizeof(int));
	ptr = ptr + sizeof(strlen1);

	memcpy(_str1, ptr, strlen1);
	ptr = ptr + strlen1;

	memcpy(&strlen2, ptr, sizeof(int));
	ptr = ptr + sizeof(int);

	memcpy(_str2, ptr, strlen2);

}

int ClientManager::PackPacketTCP(char* _buf, PROTOCOL _protocol, ERR_CODE _err, char* _msg)
{
	int str1 = strlen(_msg);

	int size = 0;
	char* ptr = _buf + sizeof(int);

	memcpy(ptr, &_protocol, sizeof(PROTOCOL));
	ptr = ptr + sizeof(PROTOCOL);
	size = size + sizeof(PROTOCOL);

	memcpy(ptr, &_err, sizeof(ERR_CODE));
	ptr = ptr + sizeof(ERR_CODE);
	size = size + sizeof(ERR_CODE);

	memcpy(ptr, &str1, sizeof(int));
	ptr = ptr + sizeof(int);
	size = size + sizeof(int);

	memcpy(ptr, _msg, str1);
	ptr = ptr + str1;
	size = size + str1;

	ptr = _buf;

	memcpy(ptr, &size, sizeof(int));
	ptr = ptr + sizeof(int);


	return size + sizeof(int);

}

int ClientManager::PackPacketTCP(char* _buf, PROTOCOL _protocol, ERR_CODE _err,int _money, char* _msg)
{
	int str1 = strlen(_msg);

	int size = 0;
	char* ptr = _buf + sizeof(int);

	memcpy(ptr, &_protocol, sizeof(PROTOCOL));
	ptr = ptr + sizeof(PROTOCOL);
	size = size + sizeof(PROTOCOL);

	memcpy(ptr, &_err, sizeof(ERR_CODE));
	ptr = ptr + sizeof(ERR_CODE);
	size = size + sizeof(ERR_CODE);

	memcpy(ptr, &_money, sizeof(int));
	ptr = ptr + sizeof(int);
	size = size + sizeof(int);

	memcpy(ptr, &str1, sizeof(int));
	ptr = ptr + sizeof(int);
	size = size + sizeof(int);

	memcpy(ptr, _msg, str1);
	ptr = ptr + str1;
	size = size + str1;

	ptr = _buf;

	memcpy(ptr, &size, sizeof(int));
	ptr = ptr + sizeof(int);


	return size + sizeof(int);

}

int ClientManager::PackPacketTCP(char* _buf, PROTOCOL _protocol, ERR_CODE _err, int _number, char* _str1, char*_str2, int _money)
	{
	int str1 = strlen(_str1);
	int str2 = strlen(_str2);

	int size = 0;
	char* ptr = _buf + sizeof(int);

	memcpy(ptr, &_protocol, sizeof(PROTOCOL));
	ptr = ptr + sizeof(PROTOCOL);
	size = size + sizeof(PROTOCOL);

	memcpy(ptr, &_err, sizeof(ERR_CODE));
	ptr = ptr + sizeof(ERR_CODE);
	size = size + sizeof(ERR_CODE);

	memcpy(ptr, &_number, sizeof(int));
	ptr = ptr + sizeof(int);
	size = size + sizeof(int);

	memcpy(ptr, &str1, sizeof(int));
	ptr = ptr + sizeof(int);
	size = size + sizeof(int);

	memcpy(ptr, _str1, str1);
	ptr = ptr + str1;
	size = size + str1;

	memcpy(ptr, &str2, sizeof(int));
	ptr = ptr + sizeof(int);
	size = size + sizeof(int);

	memcpy(ptr, _str2, str2);
	ptr = ptr + str2;
	size = size + str2;

	memcpy(ptr, &_money, sizeof(int));
	ptr = ptr + sizeof(int);
	size = size + sizeof(int);

	ptr = _buf;

	memcpy(ptr, &size, sizeof(int));
	ptr = ptr + sizeof(int);


	return size + sizeof(int);

	}

int ClientManager::PackPacketTCP(char* _buf, PROTOCOL _protocol, ERR_CODE _err, SOCKADDR_IN _addr)
{
	int size = 0;
	char* ptr = _buf + sizeof(int);

	memcpy(ptr, &_protocol, sizeof(PROTOCOL));
	ptr = ptr + sizeof(PROTOCOL);
	size = size + sizeof(PROTOCOL);

	memcpy(ptr, &_err, sizeof(ERR_CODE));
	ptr = ptr + sizeof(ERR_CODE);
	size = size + sizeof(ERR_CODE);

	memcpy(ptr, &_addr, sizeof(SOCKADDR_IN));
	ptr = ptr + sizeof(SOCKADDR_IN);
	size = size + sizeof(SOCKADDR_IN);

	ptr = _buf;

	memcpy(ptr, &size, sizeof(int));
	ptr = ptr + sizeof(int);


	return size + sizeof(int);


}
/////////////////////////////////////////////////////////////////////////////////////////

//�Ŵ���CPP����/////////////////////////////////////////////////////////////////////////////

void GetProtocol(const char* _ptr, PROTOCOL& _protocol)
{
	memcpy(&_protocol, _ptr, sizeof(PROTOCOL));

}
BOOL SearchFile(const char *filename)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFindFile = FindFirstFile(filename, &FindFileData);
	if (hFindFile == INVALID_HANDLE_VALUE)
		return FALSE;
	else {
		FindClose(hFindFile);
		return TRUE;
	}
}
int recvn(SOCKET s, char *buf, int len, int flags)
{
	int received;
	char *ptr = buf;
	int left = len;

	while (left > 0)
	{
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
	int size, retval;
	retval = recvn(_sock, (char*)&size, sizeof(int), 0);
	if (retval == SOCKET_ERROR)
	{
		err_display("recvn()");
		return false;
	}
	else if (retval == 0)
	{
		return false;
	}

	retval = recvn(_sock, _buf, size, 0);
	if (retval == SOCKET_ERROR)
	{
		err_display("recvn()");
		return false;
	}
	else if (retval == 0)
	{
		return false;
	}

	return true;
}
//Login* _data,int&UserCount
bool FileDataLoad(Login* _data, int&UserCount)
{
	if (!SearchFile("UserInfo.info"))
	{
		FILE* fp = fopen("UserInfo.info", "wb");
		fclose(fp);
		return true;
	}

	FILE* fp = fopen("UserInfo.info", "rb");
	if (fp == NULL)
	{
		return false;
	}

	Login info;
	memset(&info, 0, sizeof(Login));

	while (1)
	{
		fread(&info, sizeof(Login), 1, fp);
		if (feof(fp))
		{
			break;
		}
		Login* ptr = new Login;
		memcpy(ptr, &info, sizeof(Login));
		memcpy(&_data[UserCount++], ptr, sizeof(Login));
	}

	fclose(fp);

	for (int i = 0; i < UserCount; i++)
	{
		printf("���� : %s :  �ݾ� %d\n",_data[i].u_id, _data[i].U_Cash);
	}
	return true;
}

bool FileDataAdd(Login* _info)
{
	FILE* fp = fopen("UserInfo.info", "ab");
	if (fp == NULL)
	{
		return false;
	}

	int retval = fwrite(_info,1, sizeof(Login), fp);

	if (retval != sizeof(Login))
	{
		fclose(fp);
		return false;
	}

	fclose(fp);
	return true;
}
//������Ʈ �� ������ �����ʿ� �ٽ� ���ش�.
bool FileDataUpdate(ClientInfo* _ptr)
{
	FILE* fp = fopen("UserInfo.info", "rt+");

	Login temp;
	Login* _info = nullptr;
	_info = _ptr->GetLoginData();
	ZeroMemory(&temp, sizeof(Login));
	int count = 0;

	while (1)
	{
		fread(&temp, sizeof(Login), 1, fp);
		if (feof(fp))
		{
			break;
		}
		else
		{
			//ã������ Ŭ�� �����Ͱ� ������ 
			if (temp.u_number == _info->u_number)
			{
				fseek(fp, sizeof(Login)*count, SEEK_SET);//�ش� ����ġ �̵�
				int retval = fwrite(_info, 1, sizeof(Login), fp);	// �� ���� �ٽ� ���

				if (retval != sizeof(Login))
				{
					fclose(fp);
					return false;
				}
				fseek(fp, 0L, SEEK_END);	//�ٽ� ������
				break;
			}
			else
			{
				count++;
			}

		}
	}

	return true;
}


// ȸ������ �α���(���а� �ο�) üũ �� ������ ���� ���� �ϴ� ���μ���
BOOL ClientManager::CheckDataProcess(ClientInfo* _info, Login* _login,const char* flag)
{
	//ũ��Ƽ�� ���� �κ� �߰� �ٶ�.
	ERR_CODE err = NON_ERROR;
	PROTOCOL protocol = NON_PROTOCOL;

	char temp[CHAR_MAX];
	memset(temp, 0, sizeof(CHAR_MAX));
	int size, retval;
	
	if (User_Count == 0)
	{
		if (!strcmp(flag, LOGIN_DATA))
		{
			err = ID_ERROR;
		}
	}

	//���̵� �ߺ��˻�
	for (int i = 0; i < User_Count; i++)
	{
		if (!strcmp(flag, JOIN_DATA))
		{
			if (!strcmp(User_List[i].u_id, _login->u_id))
			{
				err = JOIN_ERROR_ID;
				break;
			}
			else
			{
				err = JOIN_SUCCESS;
			}
		}
		else if (!strcmp(flag, LOGIN_DATA))
		{
			if (!strcmp(User_List[i].u_id, _login->u_id))
			{
				if (User_List[i].UseLogin)
				{
					err = USE_LOGIN;
					break;
				}
				if (!strcmp(User_List[i].u_pw, _login->u_pw))
				{
					_login = &User_List[i];
					err = LOGIN_SUCCESS;
					break;
				}
				else
				{
					err = PW_ERROR;
					break;
				}
			}
			else
			{
				err = ID_ERROR;
			}
		}
	}

	if (err != NON_ERROR && (err != JOIN_SUCCESS && err != LOGIN_SUCCESS))
	{
		switch (err)
		{
		case JOIN_ERROR_ID:
			protocol = JOIN_RESULT;
			strcpy(temp, JOIN_ID_ERROR_MSG);
			break;
		case ID_ERROR:
			protocol = LOGIN_RESULT;
			strcpy(temp, ID_ERROR_MSG);
			break;
		case PW_ERROR:
			protocol = LOGIN_RESULT;
			strcpy(temp, PW_ERROR_MSG);
			break;
		case USE_LOGIN:
			protocol = LOGIN_RESULT;
			strcpy(temp, USE_LOGIN_MSG);
			break;
		}
	
		size = PackPacketTCP(_info->buf, protocol, err, temp);

		retval = send(_info->GetSock(), _info->buf, size, 0);
		if (retval == SOCKET_ERROR)
		{
			err_display("DATA_RETURN_ERROR");
			return FALSE;
		}
		return FALSE;
	}
	else
	{
		if (!strcmp(flag, JOIN_DATA))
		{
			//ȸ�������� �����ϴ� ȸ�����Խ�Ű��
			strcpy(temp, JOIN_COMMIT_MSG);

			err = JOIN_SUCCESS;
			protocol = JOIN_RESULT;
			size = PackPacketTCP(_info->buf, protocol, err, temp);

			retval = send(_info->GetSock(), _info->buf, size, 0);
			if (retval == SOCKET_ERROR)
			{
				err_display("DATA_RETURN_ERROR");
				return FALSE;
			}
			_login->u_number = USER_NUMBERRING + User_Count;
			_login->UseLogin = false;
			User_List[User_Count++] = *(Login*)_login;
			FileDataAdd(_login);
			printf("ȸ������!  ȸ����ȣ= %d  ���̵� : %s    ���� �Ա� : %d �� \n", _login->u_number, _login->u_id, _login->U_Cash);
		}
		else if(!strcmp(flag, LOGIN_DATA))
		{

			_login->UseLogin = true;
			protocol = LOGIN_RESULT;
			err = LOGIN_SUCCESS;
			size = PackPacketTCP(_info->buf, protocol, err,_login->u_number,_login->u_id,_login->u_pw,_login->U_Cash);
			retval = send(_info->GetSock(), _info->buf, size, 0);
			if (retval == SOCKET_ERROR)
			{
				err_display("DATA_RETURN_ERROR");
				return FALSE;
			}
			_info->SetClient_Login(_login);
		}
	}
	return TRUE;


}


/*
���ǵ����͸� �˻��ϰ� Ŭ���� �̺�Ʈ�� �̿��Ͽ� 
��� Ŭ�� ������ ������ �����ϰ� 
�̺�Ʈ�� ���ÿ� Ű������ ��� ����� ������ �����Ѵ�.
*/
BOOL ClientManager::AuctionProcess(ClientInfo* _info, Item* _item,int _money)
{
	ERR_CODE err = NON_ERROR;
	char temp[BUFSIZE];
	int size,retval;
	ZeroMemory(temp, sizeof(BUFSIZE));
	if (_item->Get_ClientMax() == _item->Get_ClientCount())
	{
		//�ش� Ŭ���� Ŭ���̾�Ʈ ���� ��á��@@@@@@
		err = CLIENT_OVER;
		
	}
	else if(_item->Get_temPrice() > _info->GetLoginData()->U_Cash || _item->Get_temPrice() > _money)
	{
		//������ ��� ��ǰ�� �����ʹ� ������ �Ұ����մϴ�.
		err = CASH_LACK;
	}
	else
	{
		while (1)
		{
			if (_item->Get_ClientMax() > _item->Get_ClientCount()+1)
			{

				if (_item->Get_MaxUserPrice() <= _money)
				{
					_item->Set_MaxUserPrice(_money);
					_item->Set_BidUser(_info->GetLoginData()->u_id);

				}
				_item->UpdateClientCount(1);

				WaitForSingleObject(_item->GetHandle(), INFINITE);
				continue;
			}
			else if (_item->Get_ClientMax() == _item->Get_ClientCount() + 1)
			{
				_item->UpdateClientCount(1);
				if (_item->Get_MaxUserPrice() <= _money)
				{
					_item->Set_MaxUserPrice(_money);
					_item->Set_BidUser(_info->GetLoginData()->u_id);

				}
				SetEvent(_item->GetHandle());
				continue;
			}
			else  // ��� �����尡 �̰��� �鸰��.
			{
				if (!strcmp(_item->Get_BidUser(), _info->GetLoginData()->u_id))
				{
					// �� ��ǰ�� ������������ ���� �����޼����� �����ϰ� �ݾ��� �����Ѵ�.
					_info->UpdateMoney(_info->GetLoginData(), _money*-1);
					FileDataUpdate(_info);	//�ش� ���� �ٽ� ������Ʈ
					err = ITEM_GET;
					_item->SetAuctionflag(false);		//�ǸſϷ� ����
				}
				else
				{
					err = ITEM_SOLD;
				}
				break;
			}
		}

		//���� ������ ���� �ѱ�ٸ� Ŭ���̾�Ʈ ���� ���� 
		//��� ��á�� ��� ���� ū �������� �����س��Ƽ� �̺�Ʈ�� ��ٸ���
		//��� á�� ��� �̺�Ʈ�� Ǯ�� ����ū ���������� ������ 
		//���� ���ϰ� ������������� ������ Ȯ�����ش�.

	}

	if (err != NON_ERROR && err != ITEM_GET)
	{
		switch (err)
		{
		case CLIENT_OVER:
			sprintf(temp, "Ŭ���̾�Ʈ ���� ��á���ϴ�\n �ٸ� ���ǰ�� �������ּ���.\n");
			break;
		case CASH_LACK:
			sprintf(temp, "�Էµ� ���� �ʹ� �۽��ϴ�\n �ٸ� ���ǰ�� �������ּ���.\n");
			break;
		case ITEM_SOLD:
			sprintf(temp, "�ش� ���ǰ�� ���̵� : %s�Կ��� %d ���� �����Ǿ����ϴ�.\n �ٸ� ���ǰ�� �������ּ���.\n", _item->Get_BidUser(), _item->Get_MaxUserPrice());
			break;
		}
		size = PackPacketTCP(_info->buf, AUCTION_RESULT, err,temp);
		retval = send(_info->GetSock(), _info->buf, size, 0);
		if (retval == SOCKET_ERROR)
		{
			err_display("DATA_RETURN_ERROR");
			return FALSE;
		}
		return FALSE;
	}
	else
	{
		sprintf(temp, "�ش� ���ǰ�� �����Ͽ����ϴ�!! �����մϴ�!!@@@@@@@@@@@@@\n\n ���� ���� %d ���� �ݾ׿��� �����˴ϴ�.\n", _money);
		size = PackPacketTCP(_info->buf, AUCTION_RESULT, err, _info->GetMoney(),temp);
		retval = send(_info->GetSock(), _info->buf, size, 0);
		if (retval == SOCKET_ERROR)
		{
			err_display("DATA_RETURN_ERROR");
			return FALSE;
		}

		return TRUE;
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////