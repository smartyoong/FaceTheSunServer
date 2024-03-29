﻿
// FaceTheSunServerGUIDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "FaceTheSunServerGUI.h"
#include "FaceTheSunServerGUIDlg.h"
#include "afxdialogex.h"
#include <iostream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define ListenSockCall 1

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CFaceTheSunServerGUIDlg 대화 상자



CFaceTheSunServerGUIDlg::CFaceTheSunServerGUIDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FACETHESUNSERVERGUI_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFaceTheSunServerGUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_SERVERSTATUS, EditServerStatus);
	DDX_Control(pDX, IDC_LIST_USER, ConnectUserList);
	DDX_Control(pDX, IDServerOnOff, ServerOnOffButton);
	DDX_Control(pDX, IDC_LISTUSERDATA, ListUserData);
	DDX_Control(pDX, IDC_EDITUSERDATA, EditUserData);
	DDX_Control(pDX, IDC_LISTROBBY, ListLobby);
}

BEGIN_MESSAGE_MAP(CFaceTheSunServerGUIDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDServerOnOff, &CFaceTheSunServerGUIDlg::OnClickedIdserveronoff)
	ON_BN_CLICKED(IDC_BUTTON_SHUTDOWN, &CFaceTheSunServerGUIDlg::OnBnClickedButtonShutdown)
	ON_BN_CLICKED(IDRfreshUser, &CFaceTheSunServerGUIDlg::OnBnClickedRfreshuser)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_MODIFY, &CFaceTheSunServerGUIDlg::OnBnClickedButtonModify)
	ON_BN_CLICKED(IDC_BUTTONUSERDATA, &CFaceTheSunServerGUIDlg::OnBnClickedButtonuserdata)
	ON_LBN_SELCHANGE(IDC_LISTUSERDATA, &CFaceTheSunServerGUIDlg::OnLbnSelchangeListuserdata)
	ON_BN_CLICKED(IDROBBYCANCEL, &CFaceTheSunServerGUIDlg::OnBnClickedRobbycancel)
END_MESSAGE_MAP()


// CFaceTheSunServerGUIDlg 메시지 처리기

BOOL CFaceTheSunServerGUIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	//ShowWindow(SW_MAXIMIZE);

	//ShowWindow(SW_MINIMIZE);

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		AfxMessageBox(L"서버 초기화 오류");
		return FALSE;
	}
	try
	{
		if (FaceTheSunDB.OpenEx(_T("DSN=Localhost")))
		{
			FaceTheSunRecordSet = new CRecordset(&FaceTheSunDB);
		}
	}
	catch (CException* e)
	{
		e->ReportError();
	}
	InsertDBField();
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CFaceTheSunServerGUIDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CFaceTheSunServerGUIDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CFaceTheSunServerGUIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CFaceTheSunServerGUIDlg::OnClickedIdserveronoff()
{
	// TODO: Add your control notification handler code here
	if (IsServerOn) // 서버가 켜져있다면 종료 시킨다.
	{
		ConnectUserList.ResetContent(); // 리스트박스 내용 제거
		CleanUpAllSocketAndTP(); // 클라이언트 소켓 제거
		CancelIo(acceptTPIO);
		WaitForThreadpoolIoCallbacks(acceptTPIO,TRUE);
		CloseThreadpoolIo(acceptTPIO);
		closesocket(ListenSock);
		DeleteCriticalSection(&SyncroData);
		WSACleanup();
		CleanUpAllClientVar();
		EditServerStatus.SetWindowTextW(_T("서버가 정상적으로 종료되었습니다"));
		KillTimer(0);
		IsServerOn = false;
		ServerOnOffButton.SetWindowTextW(_T("ServerOn"));
	}
	else
	{
		InitializeCriticalSection(&SyncroData);
		SetTimer(0, 10000, NULL); // 타이머는 1개만 있을거니까 ID는 아무거나 설정을하고, 콜백함수도 기본함수를 사용한다.
		ListenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // 소켓 생성
		sockaddr_in ServerAddr;
		ServerAddr.sin_family = AF_INET;
		ServerAddr.sin_port = htons(18891);
		ServerAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
		int retval = 0;
		retval = bind(ListenSock, (sockaddr*)&ServerAddr, sizeof(ServerAddr));
		if (retval == SOCKET_ERROR)
		{
			AfxMessageBox(_T("바인드 실패"));
			exit(EXIT_FAILURE);
		}
		retval =  listen(ListenSock, SOMAXCONN);
		if (retval == SOCKET_ERROR)
		{
			AfxMessageBox(_T("리슨 소켓 생성 실패"));
			exit(EXIT_FAILURE);
		}
		EditServerStatus.SetWindowTextW(_T("서버설정 완료"));
		UserDataStream* us = new UserDataStream; // OverLadpped확장 구조체로 필요한 정보를 송수신할 예정 자세한건 헤더파일 참고
		us->Initialize();
		// 위의 변수 생성자에 초기화 함수가 존재하므로 참고할때에 반드시 OVERLAPPED구조체를 초기화 시켜주지 않으면 에러가 남
		acceptTPIO = CreateThreadpoolIo((HANDLE)ListenSock, TPAcceptCallBackFunc, this, NULL); //Dlg 갱신용으로 this 포인터 전송
		if (acceptTPIO == NULL)
		{
			AfxMessageBox(_T("스레드풀 생성 실패"));
			std::cout << WSAGetLastError() << std::endl;
			exit(EXIT_FAILURE);
		}
		StartThreadpoolIo(acceptTPIO);
		EditServerStatus.SetWindowTextW(_T("스레드풀 생성 완료"));
		GUID guid = WSAID_ACCEPTEX;
		DWORD dwb = 0;
		if (WSAIoctl(ListenSock, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid), &pAcceptEX, sizeof(pAcceptEX), &dwb, nullptr, nullptr) == SOCKET_ERROR) // acceptex함수획득
		{
			AfxMessageBox(_T("accpet함수 획득 실패"));
			std::cout << WSAGetLastError() << std::endl;
			exit(EXIT_FAILURE);
		}
		guid = WSAID_GETACCEPTEXSOCKADDRS;
		dwb = 0;
		if (WSAIoctl(ListenSock, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid), &pAcceptAddrs, sizeof(pAcceptAddrs), &dwb, nullptr, nullptr) == SOCKET_ERROR) // getacceptexsockaddr함수획득
		{
			AfxMessageBox(_T("sockaddr함수 획득 실패"));
			std::cout << WSAGetLastError() << std::endl;
			exit(EXIT_FAILURE);
		}
		SOCKET ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // 이 소켓은 절대 바인드시키거나 연결시키지 말것
		if (ClientSocket == INVALID_SOCKET)
		{
			std::cout << WSAGetLastError() << std::endl;
		}
		us->sock = ClientSocket; // 무조건 처음에는 소켓을 생성하니 Disconnected 소켓을 볼필요가 없다
		if (!pAcceptEX(ListenSock, ClientSocket, us->ID, 0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, nullptr, (LPOVERLAPPED)us)) 
			// 현재는 수신바이트를 16으로해두고 DB랑 연결후에 수신바이트 크기를 정해서 지정할 예정
		{
			int err = WSAGetLastError();
			if (err != WSA_IO_PENDING)
			{
				std::cout << err << std::endl;
				CancelThreadpoolIo(acceptTPIO);
				CloseThreadpoolIo(acceptTPIO);
				closesocket(ClientSocket);
				closesocket(ListenSock);
			}
		}
		EditServerStatus.SetWindowTextW(_T("서버대기 준비 완료"));
		IsServerOn = true;
		ServerOnOffButton.SetWindowTextW(_T("ServerOff"));
	}
}

void CALLBACK CFaceTheSunServerGUIDlg::TPAcceptCallBackFunc(PTP_CALLBACK_INSTANCE instance, PVOID context, PVOID overlapped, ULONG result, ULONG_PTR NumOfBytesTrans, PTP_IO tio)
{
	/*주의 이 콜백함수에서 MFC클라이언트 화면 갱신 사용시 문제 없이 사용되나 추후 서버를 닫을때 데드락이 발생합니다.*/
	UserDataStream* us = (UserDataStream*)overlapped;
	CFaceTheSunServerGUIDlg* dlg = (CFaceTheSunServerGUIDlg*)context;
	EnterCriticalSection(&dlg->SyncroData);
	dlg->USArray.push_back(us); //추후 스레드풀 해제 및 메모리 해제를 쉽게하도록 배열에 추가
	LeaveCriticalSection(&dlg->SyncroData);
	PSOCKADDR lsm, rsm;
	int nsiloc, nsirem = 0;
	dlg->pAcceptAddrs(us->ID, 0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN)+16, &lsm, &nsiloc, &rsm, &nsirem); //주소 구해오기
	SOCKADDR_IN saLoc;
	SOCKADDR_IN saRem;
	ZeroMemory(&saLoc, sizeof(SOCKADDR_IN));
	ZeroMemory(&saRem, sizeof(SOCKADDR_IN));
	saLoc = *((PSOCKADDR_IN)lsm);
	saRem = *((PSOCKADDR_IN)rsm);
	if (us->sock == INVALID_SOCKET) // 클라이언트가 정상인지 한번 더 체크
		AfxMessageBox(_T("AcceptSocketFail"));
	EnterCriticalSection(&dlg->SyncroData);
	inet_ntop(AF_INET, &saRem.sin_addr.S_un.S_addr, us->addr, sizeof(us->addr));
	LeaveCriticalSection(&dlg->SyncroData);
	StartThreadpoolIo(tio);
	if (us->Reuse)
	{
		dlg->BeginRecvStart(us); //disconnect에서 startthreadpool 시켰기때문에 호출만
	}
	else
	{
		us->ptpRecvSend = CreateThreadpoolIo((HANDLE)us->sock, TPRecvSendCallBackFunc, dlg, NULL);
		if (us->ptpRecvSend == NULL)
		{
			std::cout << WSAGetLastError() << std::endl;
			AfxMessageBox(_T("데이터 송수신 스레드풀 생성 실패"));
			exit(EXIT_FAILURE);
		}
		StartThreadpoolIo(us->ptpRecvSend);
		dlg->BeginRecvStart(us);
	}
	dlg->BeginAcceptStart(); 
	// acceptEX를 호출할때는 반드시 함수형식으로 선언해야한다. 여기서는 thread가 호출하게되므로, 함수형식으로 묶어서 한번에 호출시키지않으면 204.204.204.204 Overflow를 보게된다.
	// 이거 진짜 왜 안되는지 자료도 없어서 혼자 알아내는데 너무 고생했다. ㅠㅠㅠ
}

void CFaceTheSunServerGUIDlg::TPRecvSendCallBackFunc(PTP_CALLBACK_INSTANCE instance, PVOID context, PVOID overlapped, ULONG result, ULONG_PTR NumOfBytesTrans, PTP_IO tio)
{
	UserDataStream* us = (UserDataStream*)overlapped;
	CFaceTheSunServerGUIDlg* dlg = (CFaceTheSunServerGUIDlg*)context;
	if (us->Error)
		CancelIo(tio); // 오류발생시 종료
	if (us->Stop) // 종료시도
	{
		CancelIo((HANDLE)us->sock);
		CancelThreadpoolIo(tio);
		WaitForThreadpoolIoCallbacks(tio, TRUE);
		CloseThreadpoolIo(tio);
	}
	dlg->EditServerStatus.SetWindowTextW(_T("데이터 송수신 중"));
	if (NumOfBytesTrans > 0 && result == NO_ERROR)
	{
		if (NumOfBytesTrans>0) //바로 이전에 데이터를 전송했으니 받아야한다.
		{
			//us->buffer[NumOfBytesTrans] = '\0';
			dlg->SendKindOfData(us); //상황에 맞게 데이터를 전송하도록 직렬화 함수 필수
			StartThreadpoolIo(tio);
			dlg->BeginRecvStart(us); // 데이터 받기 실행
		}
	}
	else if (result == ERROR_OPERATION_ABORTED) // 작업을 중단할때 발생하는 오류
		return;
	else if (result == ERROR_SUCCESS || result == ERROR_NETNAME_DELETED) // 상대방이 연결을 끊음 혹은 내가 종료시킴
	{
		std::cout << "Client Disconnected" << std::endl;
		GUID guid = WSAID_DISCONNECTEX;
		DWORD dwb = 0; // 소켓을 연결 종료만 시키고 재사용 대기 소켓 큐로 추가한다.
		if (WSAIoctl(us->sock, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid), &dlg->pDisconnect, sizeof(pDisconnect), &dwb, nullptr, nullptr) == SOCKET_ERROR)
		{
			AfxMessageBox(_T("DisconnectEX 함수 획득 실패"));
			std::cout << WSAGetLastError() << std::endl;
			exit(EXIT_FAILURE);
		}
		if (dlg->pDisconnect(us->sock, NULL, TF_REUSE_SOCKET, 0) == FALSE)
		{
			std::cout << "Disconncet 실패" << WSAGetLastError() << std::endl;
		}
		EnterCriticalSection(&dlg->SyncroData);
		dlg->DisconnectedSocket.push(us->sock);
		dlg->ConnectedSocketSet.erase(dlg->FindIDBySocket[us->sock]); // 해당 소켓이 연결이 끊겼으므로 관리해야되는 목록에서 제거
		dlg->ConnectUserList.DeleteString(dlg->ConnectUserList.FindString(0, dlg->FindIDBySocket[us->sock]));
		dlg->OnlineUsers.erase(dlg->FindIDBySocket[us->sock]);
		dlg->FindIDBySocket.erase(us->sock);
		LeaveCriticalSection(&dlg->SyncroData);
		StartThreadpoolIo(tio); // 미래에 재사용을 준비하여 스레드풀 스타트 시작
	}
	else if(result != WSA_IO_PENDING)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			std::cout << "ErrorSendRecv" << std::endl;
			std::cout << WSAGetLastError() << std::endl;
			closesocket(us->sock);
		}
		CloseThreadpoolIo(tio);
	}
}

void CFaceTheSunServerGUIDlg::BeginAcceptStart() 
{
	UserDataStream* uss = new UserDataStream;
	uss->Initialize();
	if (DisconnectedSocket.size() > 0) // 재사용 가능한 소켓이 있을경우 사용
	{
		EnterCriticalSection(&SyncroData);
		uss->sock = DisconnectedSocket.front();
		uss->Reuse = true;
		LeaveCriticalSection(&SyncroData);
		if (!pAcceptEX(ListenSock, uss->sock, uss->ID, 0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, nullptr, (LPOVERLAPPED)uss))
			// 현재는 수신바이트를 0으로해두고 DB랑 연결후에 수신바이트 크기를 정해서 지정할 예정
		{
			int err = WSAGetLastError();
			if (err != WSA_IO_PENDING)
			{
				std::cout << err << std::endl;
				CancelThreadpoolIo(acceptTPIO);
				return;
			}
		}
		EnterCriticalSection(&SyncroData);
		DisconnectedSocket.pop();
		LeaveCriticalSection(&SyncroData);
	}
	else // 없으면 생성
	{
		SOCKET ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // 이 소켓은 절대 바인드시키거나 연결시키지 말것 (추후 해당 소켓을 DisconnectEX 함수를 통해서 재사용할 예정)
		if (ClientSocket == INVALID_SOCKET)
		{
			std::cout << WSAGetLastError() << std::endl;
			AfxMessageBox(_T("BeginAccept Socket Err"));
		}
		uss->sock = ClientSocket;
		if (!pAcceptEX(ListenSock, uss->sock, uss->ID, 0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, nullptr, (LPOVERLAPPED)uss))
			// 현재는 수신바이트를 0으로해두고 DB랑 연결후에 수신바이트 크기를 정해서 지정할 예정
		{
			int err = WSAGetLastError();
			if (err != WSA_IO_PENDING)
			{
				std::cout << err << std::endl;
				CancelThreadpoolIo(acceptTPIO);
				return;
			}
		}
	}
}

void CFaceTheSunServerGUIDlg::BeginRecvStart(UserDataStream* us) // 데이터 수신을 비동기적으로 처리한다.
{
	WSABUF wb;
	DWORD dwFlags = 0;
	wb.buf = us->buffer;
	wb.len = sizeof(us->buffer);
	int err = WSARecv(us->sock, &wb, 1, nullptr, &dwFlags, (OVERLAPPED*)us, nullptr);
	if (err == SOCKET_ERROR)
	{
		err = WSAGetLastError();
		if (err != WSA_IO_PENDING)
		{
			std::cout << "WSARecv" << err << std::endl;
			us->Error = true;
		}
	}
}

void CFaceTheSunServerGUIDlg::SendKindOfData(UserDataStream* us)
// 항상 데이터를 선 수신후 반드시 발송시키는 형태이기 때문에, 강제 종료가 일어나지 않는 이상 클라측 구현에서도 1번 보내면 1번은 받을수 있도록 구현해둘것 최소한 확인 응답 형식으로라도
// 일반 send를 사용했는데 받는 것은 비동기적으로 처리한다 할지라도 보내는것은 데이터를 수신후 동기적으로 바로 보내기위함. 다만 스레드풀에서 send가 떨어지므로 논블록된다.
// 데이터를 받는 것은 클라측에서 결과 등에 따라 맘대로 보내두되기때문에 비동기적으로 호출해도되지만, 데이터 전송은 반드시 즉시 보내게하여서 클라가 원할한 통신을 유지하도록한다.
{
	PackToBuffer* pb = new PackToBuffer(4096);
	pb->SetBuffer(us->buffer, sizeof(us->buffer));
	int id = 0;
	*pb >> &id;
	switch (id)
	{
	case PacketID::TryLogIn :
		LogIn(pb,us);
		break;
	case PacketID::LogInResult :
		break;
	case PacketID::AskLobby:
		SendRoomList(pb, us);
		break;
	case PacketID::TrySignIn:
		SignInDB(pb, us);
		break;
	case PacketID::SignInResult:
		break;
	case PacketID::TryID:
		IDCheck(pb, us);
		break;
	case PacketID::IDResult :
		break;
	case PacketID::AskCreateRoom :
		CreateRoom(pb);
		break;
	case PacketID::AskToRoom :
		JoinRoom(pb, us);
		break;
	case PacketID::DeleteRoom:
		DeleteRoom(pb, us);
		break;
	case PacketID::DeleteRoomHost:
		DeleteHost(pb, us);
		break;
	case PacketID::DeleteRoomMember:
		DeleteMember(pb, us);
		break;
	case PacketID::SendChat:
		Chatting(pb, us);
		break;
	case PacketID::GameStart:
		GameStart(pb, us);
		break;
	case PacketID::SendGameResult:
		GameEnd(pb, us);
		break;
	default :
		std::cout << "ErrorOrder" << std::endl;
		break;

	}
	delete pb;
}

void CFaceTheSunServerGUIDlg::CleanUpAllSocketAndTP() // 모든 스레드풀, 소켓, Raw포인터 해제
{
	EditServerStatus.SetWindowTextW(_T("서버 종료중입니다. 잠시만 기다려주세요"));
	for (auto a : USArray)
	{
		if (a != nullptr)
		{
			a->Stop = true;
			closesocket(a->sock);
			delete a;
		}
	}
}

void CFaceTheSunServerGUIDlg::OnBnClickedButtonShutdown() //유저 연결 종료
{
	// ID를 키로 받은 소켓 맵을 통해서 강제로 연결 종료가 필요한 유저는 셧다운 걸어버린다.
	// TODO: Add your control notification handler code here
	if (AfxMessageBox(_T("정말로 해당유저의 연결을 종료시키시겠습니까?"), MB_YESNO | MB_ICONSTOP) == IDYES)
	{
		CString temp;
		ConnectUserList.GetText(ConnectUserList.GetCurSel(),temp);
		ConnectUserList.DeleteString(ConnectUserList.GetCurSel());
		GUID guid = WSAID_DISCONNECTEX;
		DWORD dwb = 0; // 소켓을 연결 종료만 시키고 재사용 대기 소켓 큐로 추가한다.
		if (ConnectedSocketSet.find(temp) == ConnectedSocketSet.end())
		{
			AfxMessageBox(_T("아이디 값이 잘못되었습니다."));
			std::cout << WSAGetLastError() << std::endl;
			exit(EXIT_FAILURE);
		}
		FindIDBySocket.erase(ConnectedSocketSet[temp]);
		closesocket(ConnectedSocketSet[temp]);
		ConnectedSocketSet.erase(temp);
		OnlineUsers.erase(temp);
		AfxMessageBox(_T("정상적으로 연결 종료되었습니다."));
	}
}


void CFaceTheSunServerGUIDlg::OnBnClickedRfreshuser() //유저목록 수동 동기화 
{
	// TODO: Add your control notification handler code here
	ConnectUserList.ResetContent();
	for (auto a : OnlineUsers)
	{
		ConnectUserList.AddString(a);
	}
}


void CFaceTheSunServerGUIDlg::OnTimer(UINT_PTR nIDEvent) // 어차피 타이머는 1개 이므로 ID는 무시
{
	// TODO: Add your message handler code here and/or call default
	ConnectUserList.ResetContent();
	for (auto a : OnlineUsers)
	{
		ConnectUserList.AddString(a);
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CFaceTheSunServerGUIDlg::OnDestroy() // db종료용
{
	CDialogEx::OnDestroy();
	if (FaceTheSunRecordSet->IsOpen())
		FaceTheSunRecordSet->Close();
	if(FaceTheSunDB.IsOpen())
		FaceTheSunDB.Close();
	delete FaceTheSunRecordSet;
	// TODO: Add your message handler code here
}

void CFaceTheSunServerGUIDlg::SignInDB(PackToBuffer* pb, UserDataStream* us)
{
	std::string* ID;
	std::string* Password;
	ID = new std::string;
	Password = new std::string;
	*pb >> ID >> Password;
	CString SQL = _T("INSERT INTO userdata (ID,IP,password) VALUES( '");
	SQL += CString(ID->c_str());
	SQL += _T("', '");
	SQL += us->addr;
	SQL += _T("', PWDENCRYPT('");
	SQL += CString(Password->c_str());
	SQL += _T("'))");
	try
	{
		FaceTheSunDB.BeginTrans();
		FaceTheSunDB.ExecuteSQL(SQL);
		FaceTheSunDB.CommitTrans();
	}
	catch (CException* e)
	{
		e->ReportError();
	}
	*pb << PacketID::SignInResult << 1;
	int err = send(us->sock, pb->GetBuffer(), sizeof(pb->GetBuffer()), 0);
	if (err == SOCKET_ERROR)
	{
		std::cout << WSAGetLastError() << std::endl;
		AfxMessageBox(_T("SignInResultSend Error"));
		CancelThreadpoolIo(us->ptpRecvSend);
	}
}

void CFaceTheSunServerGUIDlg::LogIn(PackToBuffer* pb, UserDataStream* us)
{
	std::string* password = new std::string;
	std::string* ID = new std::string;
	*pb >> ID;
	*pb >> password;
	CString temp = _T("select (case when count(*)=1 then 'ok' else 'no' end) from userdata where ID = '");
	CString temp2 = _T("' and PWDCOMPARE('");
	temp += ID->c_str();
	temp += temp2;
	CString sql = _T("',password) =1");
	temp += password->c_str();
	temp += sql;
	CString stemp;
	EnterCriticalSection(&SyncroData);
	UserIPField.insert(std::make_pair(CString(ID->c_str()), CString(us->addr)));
	LeaveCriticalSection(&SyncroData);
	if (FaceTheSunRecordSet->Open(CRecordset::dynamic, temp))
	{
		FaceTheSunRecordSet->GetFieldValue(short(0), stemp);
	}
	FaceTheSunRecordSet->Close();
	if (stemp == "ok")
	{
		CString a(ID->c_str());
		OnlineUsers.insert(a);
		EnterCriticalSection(&SyncroData);
		ConnectedSocketSet.insert(std::make_pair(a, us->sock)); // ID와 관련된 소켓을 맵에 집어넣는다. (해당 아이디의 소켓의 연결을 끊기위함)
		FindIDBySocket.insert(std::make_pair(us->sock, a));
		LeaveCriticalSection(&SyncroData);
		*pb << PacketID::LogInResult << int(1);
		int err = send(us->sock,pb->GetBuffer() , sizeof(pb->GetBuffer()), 0);
		if (err == SOCKET_ERROR)
		{
			std::cout << WSAGetLastError() << std::endl;
			CancelThreadpoolIo(us->ptpRecvSend);
		}
	}
	else
	{
		*pb << PacketID::LogInResult << int(0);
		int err = send(us->sock, pb->GetBuffer(), sizeof(pb->GetBuffer()), 0);
		if (err == SOCKET_ERROR)
		{
			std::cout << WSAGetLastError() << std::endl;
			CancelThreadpoolIo(us->ptpRecvSend);
		}
	}
}

void CFaceTheSunServerGUIDlg::IDCheck(PackToBuffer* pb, UserDataStream* us)
{
	std::string* ID = new std::string;
	*pb >> ID;
	CString SQL = _T("SELECT (CASE WHEN ID = '");
	SQL += ID->c_str();
	SQL += _T("' THEN 'ok' ELSE 'no' END) FROM userdata");
	if (FaceTheSunRecordSet->Open(CRecordset::dynamic, SQL))
	{
		CString temp;
		FaceTheSunRecordSet->GetFieldValue(short(0), temp);
		if (temp == "ok") //중복
		{
			*pb << PacketID::IDResult << int(0);
			int err = send(us->sock, pb->GetBuffer(), sizeof(pb->GetBuffer()), 0);
			if (err == SOCKET_ERROR)
			{
				std::cout << WSAGetLastError() << std::endl;
				AfxMessageBox(_T("전송 오류"));
				CancelThreadpoolIo(us->ptpRecvSend);
			}
		}
		else //중복 아님
		{
			*pb << PacketID::IDResult << int(1);
			int err = send(us->sock, pb->GetBuffer(), sizeof(pb->GetBuffer()), 0);
			if (err == SOCKET_ERROR)
			{
				std::cout << WSAGetLastError() << std::endl;
				AfxMessageBox(_T("전송 오류"));
				CancelThreadpoolIo(us->ptpRecvSend);
			}
		}
	}
	else
	{
		AfxMessageBox(_T("ID 체크 레코드셋 읽기 실패"));
	}
	FaceTheSunRecordSet->Close();
}

void CFaceTheSunServerGUIDlg::CreateRoom(PackToBuffer* pb)
{
	EnterCriticalSection(&SyncroData);
	RoomInfo info;
	pb->DeSerialize(&info);
	RoomList.push_back(info);
	RoomUsers.insert(std::make_pair(info.HostName, info.HostName));
	LeaveCriticalSection(&SyncroData);
	CString RoomName(info.RoomName.c_str());
	RoomName += "  호스트 명 : ";
	RoomName += CString(info.HostName.c_str());
	/*
	* RoomName += "  현재 인원수/ 최대 인원수 : ";
	RoomName += std::to_string(info.CurrentPlayer).c_str();
	RoomName += "/3";
	*/
	ListLobby.AddString(RoomName);
}

void CFaceTheSunServerGUIDlg::SendRoomList(PackToBuffer* pb, UserDataStream* us)
{
	//RoomInfo 구조체의 개별의 사이즈가 다르므로 애를 좀 먹네요;;
	PackToBuffer pbb(8192); //sizeof(PacketID::SendLobby) + sizeof((int)RoomList.size())+sizeof(RoomList.capacity())+sizeof(int)*RoomList.size()*2
	pbb << PacketID::SendLobby << (int)RoomList.size();
	for (int i = 0; i <RoomList.size(); ++i)
	{
		pbb << (int)sizeof(RoomList[i].RoomName) << (int)sizeof(RoomList[i].HostName);
		pbb.Serialize(RoomList[i]);
	}
	int err = send(us->sock, pbb.GetBuffer(), pbb.GetBufferSize(), 0);
	if (err == SOCKET_ERROR)
	{
		std::cout << WSAGetLastError() << std::endl;
	}
}

void CFaceTheSunServerGUIDlg::JoinRoom(PackToBuffer* pb, UserDataStream* us)
{
	std::string HostNameInfo;
	std::string JoinUserName;
	int RoomIndex;
	*pb >> &HostNameInfo >> &JoinUserName >> &RoomIndex;
	if (RoomList[RoomIndex].CanJoin)
	{
		EnterCriticalSection(&SyncroData);
		RoomUsers.insert(std::make_pair(HostNameInfo, JoinUserName));
		RoomList[RoomIndex].CurrentPlayer++;
		LeaveCriticalSection(&SyncroData);
		PackToBuffer pbb(1024);
		pbb << PacketID::SendRoomInfo << RoomList[RoomIndex].CanJoin << RoomList[RoomIndex].CurrentPlayer;
		pbb.Serialize(RoomList[RoomIndex]);
		auto RUIter = RoomUsers.equal_range(HostNameInfo);
		for (auto i = RUIter.first; i != RUIter.second; ++i)
		{
			pbb << i->second;
			if (i->second == JoinUserName)
				continue;
			PackToBuffer pbbb(sizeof(PacketID::SomeBodyJoin)+sizeof(JoinUserName));
			pbbb << PacketID::SomeBodyJoin << JoinUserName;
			int err = send(ConnectedSocketSet[CString(i->second.c_str())], pbbb.GetBuffer(), pbbb.GetBufferSize(), 0);
			if (err == SOCKET_ERROR)
			{
				std::cout << WSAGetLastError() << std::endl;
			}
		}
		int errnom = send(us->sock, pbb.GetBuffer(), pbb.GetBufferSize(), 0);
		if (errnom == SOCKET_ERROR)
			std::cout << WSAGetLastError() << std::endl;
	}
	else
	{
		PackToBuffer pbb(1024);
		pbb << PacketID::SendRoomInfo << RoomList[RoomIndex].CanJoin << RoomList[RoomIndex].CurrentPlayer;
		int err = send(us->sock, pbb.GetBuffer(), pbb.GetBufferSize(), 0);
		if (err == SOCKET_ERROR)
		{
			std::cout << WSAGetLastError() << std::endl;
		}
	}
	if (RoomList[RoomIndex].CurrentPlayer >= 3)
	{
		EnterCriticalSection(&SyncroData);
		RoomList[RoomIndex].CanJoin = false;
		LeaveCriticalSection(&SyncroData);
	}
}

void CFaceTheSunServerGUIDlg::DeleteRoom(PackToBuffer* pb, UserDataStream* us)
{
	PackToBuffer pbb(sizeof(PacketID::DeleteRoom));
	pbb << PacketID::DeleteRoom;
	int err = send(us->sock, pbb.GetBuffer(), pbb.GetBufferSize(), 0);
	if (err == SOCKET_ERROR)
		std::cout << WSAGetLastError() << std::endl;
}

void CFaceTheSunServerGUIDlg::Chatting(PackToBuffer* pb, UserDataStream* us)
{
	std::string HostName;
	std::string CharacterName;
	std::string Chat;
	*pb >> &HostName >> &CharacterName >> &Chat;
	CharacterName += " : ";
	CharacterName += Chat;
	auto iterPair = RoomUsers.equal_range(HostName);
	for (auto iter = iterPair.first; iter != iterPair.second; ++iter)
	{
		PackToBuffer pbb(sizeof(PacketID::RecvChat) + sizeof(CharacterName));
		pbb << PacketID::RecvChat << CharacterName;
		int err = send(ConnectedSocketSet[CString(iter->second.c_str())], pbb.GetBuffer(), pbb.GetBufferSize(), 0);
		if (err == SOCKET_ERROR)
			std::cout << WSAGetLastError() << std::endl;
	}
}

void CFaceTheSunServerGUIDlg::DeleteMember(PackToBuffer* pb, UserDataStream* us)
{
	std::string room;
	std::string MemberString;
	*pb >> &room >> &MemberString;
	auto IterPair = RoomUsers.equal_range(room);
	for (auto iter = IterPair.first; iter != IterPair.second; ++iter)
	{
		if (MemberString == iter->second)
		{
			RoomUsers.erase(iter);
			break;
		}
	}
	IterPair = RoomUsers.equal_range(room);
	for (auto iter = IterPair.first; iter != IterPair.second; ++iter)
	{
		PackToBuffer pbb(512);
		pbb << PacketID::DeleteRoomMember << MemberString;
		if (send(ConnectedSocketSet[CString(iter->second.c_str())], pbb.GetBuffer(), pbb.GetBufferSize(), 0) == SOCKET_ERROR)
			std::cout << WSAGetLastError() << std::endl;
	}
	PackToBuffer pbb(32);
	pbb << PacketID::DeleteRoomHost; // 방장이 나간것은 아니지만, 효과는 방장이 나간것하고 똑같기때문에 방장이 나간것처럼 한다.
	if(send(us->sock,pbb.GetBuffer(),pbb.GetBufferSize(),0)==SOCKET_ERROR)
		std::cout << WSAGetLastError() << std::endl;
	for (auto a : RoomList)
	{
		if (a.HostName == room)
		{
			a.CurrentPlayer--;
			if (!a.CanJoin)
				a.CanJoin = true;
			break;
		}
	}
}

void CFaceTheSunServerGUIDlg::DeleteHost(PackToBuffer* pb, UserDataStream* us)
{
	std::string HostNameString;
	*pb >> &HostNameString;
	auto IterPair =  RoomUsers.equal_range(HostNameString);
	PackToBuffer pbb(32);
	pbb << PacketID::DeleteRoomHost;
	for (auto iter = IterPair.first; iter != IterPair.second; ++iter)
	{
		if (send(ConnectedSocketSet[CString(iter->second.c_str())], pbb.GetBuffer(), pbb.GetBufferSize(), 0) == SOCKET_ERROR)
			std::cout << WSAGetLastError() << std::endl;
	}
	RoomUsers.erase(HostNameString);
	int index = 0;
	for (auto a : RoomList)
	{
		if (a.HostName == HostNameString)
			break;
		index++;
	}
	RoomList.erase(RoomList.begin() + index);
	ListLobby.DeleteString(index);
}

void CFaceTheSunServerGUIDlg::CleanUpAllClientVar()
{
	ConnectUserList.ResetContent();
	ListUserData.ResetContent();
	EditUserData.Clear();
	ListLobby.ResetContent();
	ConnectedSocketSet.clear();
	FindIDBySocket.clear();
	USArray.clear();
	DisconnectedSocket = std::queue<SOCKET>();
	OnlineUsers.clear();
	UserDataField.clear();
	UserIPField.clear();
	RoomList.clear();
	RoomUsers.clear();
}

void CFaceTheSunServerGUIDlg::GameStart(PackToBuffer* pb, UserDataStream* us)
{
	std::string HostName;
	*pb >> &HostName;
	CString HostIP = UserIPField[CString(HostName.c_str())];
	std::string HostStringIP = std::string(CT2CA(HostIP));
	PackToBuffer pbb(sizeof(PacketID::GameStart) + sizeof(HostStringIP));
	pbb << PacketID::GameStart << HostStringIP;
	if (send(us->sock, pbb.GetBuffer(), pbb.GetBufferSize(), 0) == SOCKET_ERROR)
		std::cout << WSAGetLastError() << std::endl;


}

void CFaceTheSunServerGUIDlg::GameEnd(PackToBuffer* pb, UserDataStream* us)
{
	std::string CharacterName;
	bool IsWin;
	*pb >> &CharacterName >> &IsWin;
	if (IsWin)
	{
		CString Sql(_T("UPDATE USERDATA SET currentexp = currentexp + 100, point = point + 10 where ID = '"));
		Sql += CString(CharacterName.c_str());
		Sql += CString(_T("';"));
		FaceTheSunDB.BeginTrans();
		FaceTheSunDB.ExecuteSQL(Sql);
		FaceTheSunDB.CommitTrans();
	}
	else
	{
		CString Sql(_T("UPDATE USERDATA SET currentexp = currentexp + 50, point = point + 5 where ID = '"));
		Sql += CString(CharacterName.c_str());
		Sql += CString(_T("';"));
		FaceTheSunDB.BeginTrans();
		FaceTheSunDB.ExecuteSQL(Sql);
		FaceTheSunDB.CommitTrans();
	}
}

void CFaceTheSunServerGUIDlg::OnBnClickedButtonModify()
{
	// TODO: Add your control notification handler code here
	ListUserData.ResetContent();
	CString temp = _T("SELECT * FROM USERDATA WHERE ID = '");
	CString cs;
	ConnectUserList.GetText(ConnectUserList.GetCurSel(),cs);
	temp += cs + _T("'");
	if (FaceTheSunRecordSet->Open(CRecordset::dynamic, temp))
	{
		for (int i = 0; i < FaceTheSunRecordSet->GetODBCFieldCount(); ++i)
		{
			CString stemp;
			FaceTheSunRecordSet->GetFieldValue(i, stemp);
			ListUserData.AddString(UserDataField[i] + stemp);
		}
	}
	FaceTheSunRecordSet->Close();
}


void CFaceTheSunServerGUIDlg::OnBnClickedButtonuserdata() // 앞으로 추가될 목록이 많으므로 지속적으로 각 상황에 맞게 추가하도록 유도
{
	// TODO: Add your control notification handler code here
	CString temp;
	temp = UserDataField[ListUserData.GetCurSel()];
	temp.Replace(':','=');
	CString ttemp;
	EditUserData.GetWindowTextW(ttemp);
	int index = ListUserData.GetCurSel();
	if (index == 0) // ID
	{
		temp += "'";
		temp += ttemp;
		temp += "'";
		CString Sql(_T("UPDATE USERDATA SET "));
		Sql += temp;
		CString Sql2(_T("Where ID = '"));
		Sql += Sql2;
		CString cs;
		ConnectUserList.GetText(ConnectUserList.GetCurSel(), cs);
		Sql += cs;
		Sql += _T("'");
		FaceTheSunDB.BeginTrans();
		FaceTheSunDB.ExecuteSQL(Sql);
		FaceTheSunDB.CommitTrans();
	}
	else if (index == 4 || index == 5) // IP or PassWord
	{
		AfxMessageBox(_T("Ip 혹은 비밀번호는 수정 할 수 없습니다."));
	}
	else // etc
	{
		temp += ttemp;
		CString Sql(_T("UPDATE USERDATA SET "));
		Sql += temp;
		FaceTheSunDB.BeginTrans();
		FaceTheSunDB.ExecuteSQL(Sql);
		FaceTheSunDB.CommitTrans();
	}
	AfxMessageBox(_T("처리 완료!"));
	ListUserData.ResetContent();
}

void CFaceTheSunServerGUIDlg::InsertDBField()
{
	UserDataField.insert(std::make_pair(0, _T("ID : ")));
	UserDataField.insert(std::make_pair(1, _T("UserLevel : ")));
	UserDataField.insert(std::make_pair(2, _T("CurrentEXP : ")));
	UserDataField.insert(std::make_pair(3, _T("Point : ")));
	UserDataField.insert(std::make_pair(4, _T("IP : ")));
	UserDataField.insert(std::make_pair(5, _T("Password : ")));
	UserDataField.insert(std::make_pair(6, _T("현재 장착한 아이템 번호 : ")));
}


void CFaceTheSunServerGUIDlg::OnLbnSelchangeListuserdata()
{
	CString temp;
	ListUserData.GetText(ListUserData.GetCurSel(), temp);
	if (temp.Find(UserDataField[ListUserData.GetCurSel()]) != -1)
	{
		temp.Delete(0, UserDataField[ListUserData.GetCurSel()].GetLength());
		EditUserData.SetWindowTextW(temp);
	}
	else
	{
		AfxMessageBox(_T("찾기 실패"));
	}
}


void CFaceTheSunServerGUIDlg::OnBnClickedRobbycancel()
{
	// TODO: Add your control notification handler code here
	int index = ListLobby.GetCurSel();
	auto IterPair = RoomUsers.equal_range(RoomList[index].HostName);
	for (auto iter = IterPair.first; iter != IterPair.second; ++iter)
	{
		PackToBuffer pb(32);
		pb << PacketID::DeleteRoomHost;
		if(send(ConnectedSocketSet[CString(iter->second.c_str())],pb.GetBuffer(),pb.GetBufferSize(),0) == SOCKET_ERROR)
			std::cout << WSAGetLastError() << std::endl;
	}
	RoomUsers.erase(RoomList[0].HostName);
	RoomList.erase(RoomList.begin() + index);
	ListLobby.DeleteString(index);
}
