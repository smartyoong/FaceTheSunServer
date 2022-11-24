
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
}

BEGIN_MESSAGE_MAP(CFaceTheSunServerGUIDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDServerOnOff, &CFaceTheSunServerGUIDlg::OnClickedIdserveronoff)
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
		CleanUpAllSocketAndTP();
		WaitForThreadpoolIoCallbacks(acceptTPIO, TRUE); // 콜백이 끝나기를 기다린다.
		CloseThreadpoolIo(acceptTPIO);
		closesocket(ListenSock);
		WSACleanup();
		//추후에 버튼 글씨 바뀌는것도 추가할것
	}
	else
	{
		ListenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // 소켓 생성
		sockaddr_in ServerAddr;
		ServerAddr.sin_family = AF_INET;
		ServerAddr.sin_port = htons(18891);
		ServerAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
		int retval = 0;
		retval = bind(ListenSock, (sockaddr*)&ServerAddr, sizeof(ServerAddr));
		if (retval == SOCKET_ERROR)
		{
			AfxMessageBox(L"바인드 실패");
			exit(EXIT_FAILURE);
		}
		retval =  listen(ListenSock, SOMAXCONN);
		if (retval == SOCKET_ERROR)
		{
			AfxMessageBox(L"리슨 소켓 생성 실패");
			exit(EXIT_FAILURE);
		}
		EditServerStatus.SetWindowTextW(L"서버설정 완료");
		UserDataStream us; // OverLadpped확장 구조체로 필요한 정보를 송수신할 예정 자세한건 헤더파일 참고
		ZeroMemory(&us, sizeof(UserDataStream));
		// 위의 변수 생성자에 초기화 함수가 존재하므로 참고할때에 반드시 OVERLAPPED구조체를 초기화 시켜주지 않으면 에러가 남
		acceptTPIO = CreateThreadpoolIo((HANDLE)ListenSock, TPAcceptCallBackFunc, this, NULL); //Dlg 갱신용으로 this 포인터 전송
		if (acceptTPIO == NULL)
		{
			AfxMessageBox(L"스레드풀 생성 실패");
			std::cout << WSAGetLastError() << std::endl;
			exit(EXIT_FAILURE);
		}
		StartThreadpoolIo(acceptTPIO);
		EditServerStatus.SetWindowTextW(L"스레드풀 생성 완료");
		GUID guid = WSAID_ACCEPTEX;
		DWORD dwb = 0;
		if (WSAIoctl(ListenSock, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid), &pAcceptEX, sizeof(pAcceptEX), &dwb, nullptr, nullptr) == SOCKET_ERROR) // acceptex함수획득
		{
			AfxMessageBox(L"accpet함수 획득 실패");
			std::cout << WSAGetLastError() << std::endl;
			exit(EXIT_FAILURE);
		}
		guid = WSAID_GETACCEPTEXSOCKADDRS;
		dwb = 0;
		if (WSAIoctl(ListenSock, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid), &pAcceptAddrs, sizeof(pAcceptAddrs), &dwb, nullptr, nullptr) == SOCKET_ERROR) // getacceptexsockaddr함수획득
		{
			AfxMessageBox(L"sockaddr함수 획득 실패");
			std::cout << WSAGetLastError() << std::endl;
			exit(EXIT_FAILURE);
		}
		SOCKET ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // 이 소켓은 절대 바인드시키거나 연결시키지 말것
		if (ClientSocket == INVALID_SOCKET)
		{
			std::cout << WSAGetLastError() << std::endl;
		}
		us.sock = ClientSocket;
		if (!pAcceptEX(ListenSock, ClientSocket, us.ID, 16, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, nullptr, (LPOVERLAPPED)&us)) 
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
		EditServerStatus.SetWindowTextW(L"서버대기 준비 완료");
		IsServerOn = true;
	}
}

void CALLBACK CFaceTheSunServerGUIDlg::TPAcceptCallBackFunc(PTP_CALLBACK_INSTANCE instance, PVOID context, PVOID overlapped, ULONG result, ULONG_PTR NumOfBytesTrans, PTP_IO tio)
{
	UserDataStream* us = (UserDataStream*)overlapped;
	CFaceTheSunServerGUIDlg* dlg = (CFaceTheSunServerGUIDlg*)context;
	//dlg->USArray.push_back(us); 크리티컬 섹션 추후에 선언하고 추가할것
	PSOCKADDR lsm, rsm;
	int nsiloc, nsirem = 0;
	dlg->pAcceptAddrs(us->ID, 16, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN)+16, &lsm, &nsiloc, &rsm, &nsirem);
	dlg->EditServerStatus.SetWindowTextW(L"클라이언트 수신 중");
	SOCKADDR_IN saLoc;
	SOCKADDR_IN saRem;
	ZeroMemory(&saLoc, sizeof(SOCKADDR_IN));
	ZeroMemory(&saRem, sizeof(SOCKADDR_IN));
	saLoc = *((PSOCKADDR_IN)lsm);
	saRem = *((PSOCKADDR_IN)rsm);
	us->ID[NumOfBytesTrans] = '\0';
	CString a;
	char addr[INET_ADDRSTRLEN];
	a += inet_ntop(AF_INET, &saRem.sin_addr.S_un.S_addr, addr, sizeof(addr));
	a += " ";
	a += us->ID;
	dlg->ConnectUserList.AddString(a);
	if (us->sock == INVALID_SOCKET)
		AfxMessageBox(_T("SEX"));
	us->IsItSendOrRecv = SendOrRecv::Accept;
	us->ptpRecvSend = CreateThreadpoolIo((HANDLE)us->sock, TPRecvSendCallBackFunc,dlg,NULL);
	if (us->ptpRecvSend == NULL)
	{
		AfxMessageBox(L"데이터 송수신 스레드풀 생성 실패");
		std::cout << WSAGetLastError() << std::endl;
		exit(EXIT_FAILURE);
	}
	StartThreadpoolIo(tio);
	StartThreadpoolIo(us->ptpRecvSend);
	TPRecvSendCallBackFunc(instance, dlg, us, NO_ERROR, -19, us->ptpRecvSend); // recv를 실행하도록 임의로 1번 호출
	dlg->BeginAcceptStart(); 
	// acceptEX를 호출할때는 반드시 함수형식으로 선언해야한다. 여기서는 thread가 호출하게되므로, 함수형식으로 묶어서 한번에 호출시키지않으면 204.204.204.204 Overflow를 보게된다.
	// 이거 진짜 왜 안되는지 자료도 없어서 혼자 알아내는데 너무 고생했다. ㅠㅠㅠ
}

void CFaceTheSunServerGUIDlg::TPRecvSendCallBackFunc(PTP_CALLBACK_INSTANCE instance, PVOID context, PVOID overlapped, ULONG result, ULONG_PTR NumOfBytesTrans, PTP_IO tio)
{
	UserDataStream* us = (UserDataStream*)overlapped;
	CFaceTheSunServerGUIDlg* dlg = (CFaceTheSunServerGUIDlg*)context;
	dlg->EditServerStatus.SetWindowTextW(L"데이터 송수신 중");
	std::cout << "RecvFunc" << (int)us->IsItSendOrRecv <<std::endl;
	if (NumOfBytesTrans == -19) // accept를 시켰다. 나중에 로그인을 구현할경우 send함수를 설정할것
	{
		std::cout << "AccFunc" << std::endl;
		StartThreadpoolIo(tio);
		dlg->BeginRecvStart(us);
	}
	else if (NumOfBytesTrans > 0 && result == NO_ERROR)
	{
		std::cout << "RecvOrSend" << std::endl;
		if (NumOfBytesTrans>0) //바로 이전에 데이터를 전송했으니 받아야한다.
		{
			std::cout << "SendFunc" << std::endl;
			us->buffer[NumOfBytesTrans] = '\0';
			CString a;
			a += us->buffer;
			dlg->ConnectUserList.AddString(a); // 테스트용으로 여기에 글을 보여주지만 나중에는 데이터 송수신 함수를 작성해야함
			dlg->SendKindOfData(us);
			StartThreadpoolIo(tio);
			dlg->BeginRecvStart(us);
		}
	}
	else if (result == ERROR_OPERATION_ABORTED) // 작업을 중단할때 발생하는 오류
		return;
	else if (result == ERROR_SUCCESS || result == ERROR_NETNAME_DELETED) // 상대방이 연결을 끊음
	{
		std::cout << "Client Disconnected" << std::endl;
		//추후 소켓 닫고 스레드풀을 해제할수 있도록 추가할것
	}
	else
	{
		std::cout << "Error" << std::endl;
		closesocket(us->sock);
		CloseThreadpoolIo(tio);
	}
}

void CFaceTheSunServerGUIDlg::BeginAcceptStart()
{
	UserDataStream uss;
	ZeroMemory(&uss, sizeof(UserDataStream));
	SOCKET ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // 이 소켓은 절대 바인드시키거나 연결시키지 말것 (추후 해당 소켓을 DisconnectEX 함수를 통해서 재사용할 예정)
	if (ClientSocket == INVALID_SOCKET)
	{
		std::cout << WSAGetLastError() << std::endl;
	}
	uss.sock = ClientSocket;
	if (!pAcceptEX(ListenSock, ClientSocket, uss.ID, 16, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, nullptr, (LPOVERLAPPED)&uss))
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

void CFaceTheSunServerGUIDlg::BeginRecvStart(UserDataStream* us)
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
			CancelThreadpoolIo(us->ptpRecvSend);
		}
	}
}

void CFaceTheSunServerGUIDlg::SendKindOfData(UserDataStream* us)
{
	int err = send(us->sock, us->buffer, sizeof(us->buffer), 0);
	if (err == SOCKET_ERROR)
	{
		std::cout << WSAGetLastError() << std::endl;
		CancelThreadpoolIo(us->ptpRecvSend);
	}
}

void CFaceTheSunServerGUIDlg::CleanUpAllSocketAndTP()
{
	//CancelIoEx(소켓, 오버랩);
	//WaitForThreadpoolIoCallbacks(TP객체, TRUE);
	//closesocket();
	//CloseThreadpoolIo();
}
