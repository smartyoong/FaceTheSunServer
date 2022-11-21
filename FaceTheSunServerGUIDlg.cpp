
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
	acceptNotify = WSACreateEvent();
	if (acceptNotify == NULL)
	{
		AfxMessageBox(L"접속 신호용 이벤트 생성 실패");
		std::cout << WSAGetLastError() << std::endl;
		exit(EXIT_FAILURE);
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
		us = &UserDataStream(ListenSock);
		if (us == nullptr)
		{
			AfxMessageBox(L"UserDataStream이 nullptr입니다.");
			exit(EXIT_FAILURE);
		}
		us->accpetNoti = acceptNotify;
		us->FTSp = this;
		ListenTPWait = CreateThreadpoolWait(TPAcceptWaitCallBackFunc, us, NULL);
		if (ListenTPWait == NULL)
		{
			AfxMessageBox(L"접속 대기용 스레드풀 생성 실패");
			std::cout << WSAGetLastError() << std::endl;
			exit(EXIT_FAILURE);
		}
		SetThreadpoolWait(ListenTPWait, acceptNotify, NULL);
		EditServerStatus.SetWindowTextW(L"스레드풀 생성 완료");
		WSAEventSelect(ListenSock, acceptNotify, FD_ACCEPT);
		EditServerStatus.SetWindowTextW(L"서버대기 준비 완료");
		IsServerOn = true;
	}
}

void CALLBACK CFaceTheSunServerGUIDlg::TPAcceptCallBackFunc(PTP_CALLBACK_INSTANCE instance, PVOID context, PVOID overlapped, ULONG result, ULONG_PTR NumOfBytesTrans, PTP_IO tio)
{
	/* 다음번 송수신 참고용
	 PIOCP_ENV  pie = (PIOCP_ENV )pCtx;
	PSOCK_ITEM psi = (PSOCK_ITEM)pov;

	if (dwTrBytes > 0 && dwErrCode == NO_ERROR)
	{
		if ((int)dwTrBytes > 0)
		{
			psi->_buff[dwTrBytes] = 0;
			cout << " *** Client(" << psi->_sock << ") sent : " << psi->_buff << endl;

			int lSockRet = send(psi->_sock, psi->_buff, dwTrBytes, 0);
			if (lSockRet == SOCKET_ERROR)
			{
				dwErrCode = WSAGetLastError();
				goto $LABEL_CLOSE;
			}
		}

		StartThreadpoolIo(ptpIo);
		WSABUF wb; DWORD dwFlags = 0;
		wb.buf = psi->_buff, wb.len = sizeof(psi->_buff);
		int nSockRet = WSARecv(psi->_sock, &wb, 1, NULL, &dwFlags, psi, NULL);
		if (nSockRet == SOCKET_ERROR)
		{
			dwErrCode = WSAGetLastError();
			if (dwErrCode != WSA_IO_PENDING)
			{
				CancelThreadpoolIo(ptpIo);
				goto $LABEL_CLOSE;
			}
		}
		return;
	}

$LABEL_CLOSE:
	if (dwErrCode == ERROR_OPERATION_ABORTED)
		return;

	if (dwErrCode == ERROR_SUCCESS || dwErrCode == ERROR_NETNAME_DELETED)
		cout << " ==> Client " << psi->_sock << " disconnected..." << endl;
	else
		cout << " ==> Error occurred, code = " << dwErrCode << endl;
	EnterCriticalSection(&pie->_cs);
	pie->_conn.erase(psi);
	LeaveCriticalSection(&pie->_cs);

	closesocket(psi->_sock);
	delete psi;
	CloseThreadpoolIo(ptpIo);
	*/
}

void CFaceTheSunServerGUIDlg::TPAcceptWaitCallBackFunc(PTP_CALLBACK_INSTANCE instance, PVOID context, PTP_WAIT wait, TP_WAIT_RESULT result)
{
	UserDataStream* us = (UserDataStream*)context;
	WSANETWORKEVENTS ne;
	WSAEnumNetworkEvents(us->liSock, us->accpetNoti, &ne);
	if (!(ne.lNetworkEvents & FD_ACCEPT))
	{
		return;
	}
	int err = 0;
	if (ne.iErrorCode[FD_ACCEPT_BIT] != 0)
	{
		AfxMessageBox(L"네트워크 이벤트 설정 오류");
		std::cout << "ListenFail" << ne.iErrorCode[FD_ACCEPT_BIT] <<std::endl;
		LPVOID lpMsgBuf;
		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM,
			NULL, ne.iErrorCode[FD_ACCEPT_BIT],
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf, 0, NULL);
		printf("[오류] %s", (LPCTSTR)lpMsgBuf);
		LocalFree(lpMsgBuf);
		//return;
	}
	sockaddr_in addrInfo;
	ZeroMemory(&addrInfo, sizeof(sockaddr_in));
	int addrlen =0;
	SOCKET sock = accept(us->liSock, (sockaddr*)&addrInfo, &addrlen);
	if (sock == INVALID_SOCKET)
	{
		{
			err = WSAGetLastError();
			if (err != WSAEINTR)
			{
				AfxMessageBox(L"accept 오류");
				LPVOID lpMsgBuf;
				FormatMessage(
					FORMAT_MESSAGE_ALLOCATE_BUFFER |
					FORMAT_MESSAGE_FROM_SYSTEM,
					NULL, err,
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
					(LPTSTR)&lpMsgBuf, 0, NULL);
				printf("[오류] %s", (LPCTSTR)lpMsgBuf);
				LocalFree(lpMsgBuf);
				std::cout << err << std::endl;
				return;
			}
		}
	}
	char addrbuf[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &addrInfo.sin_addr.S_un.S_addr, addrbuf, sizeof(addrbuf));
	CString a = L"클라이언트 접속 주소 : ";
	a += (LPSTR)addrbuf;
	us->FTSp->EditServerStatus.SetWindowTextW(a); // 다음번 코딩때는 ListBox에 해당내용을 추가하고 여기에는 수신중을 입력

	/* 다음번 참고용 송수신 스레드풀 생성시의 참고용
	* PSOCK_ITEM psi = new SOCK_ITEM(sock);
	psi->_ptpIo = CreateThreadpoolIo((HANDLE)psi->_sock, Handler_SockChild, pie, NULL);

	EnterCriticalSection(&pie->_cs);
	pie->_conn.insert(psi);
	LeaveCriticalSection(&pie->_cs);

	Handler_SockChild(pInst, pie, psi, NO_ERROR, -1, psi->_ptpIo);
	*/
	SetThreadpoolWait(wait, us->accpetNoti, NULL);
	WSAEventSelect(us->liSock, us->accpetNoti, FD_ACCEPT);
}
