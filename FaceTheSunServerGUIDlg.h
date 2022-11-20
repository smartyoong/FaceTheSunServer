
// FaceTheSunServerGUIDlg.h: 헤더 파일
//

#pragma once
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
class UserDataStream
{
public:
	enum class SendOrRecv : DWORD
	{
		Recv = 0,
		Send = 1
	};
	PVOID CustomOrder = nullptr; //명령 및 명령 길이
	int OrderLen = 0;
	char addr[512]; // 주소
	DWORD addrlen = 0;
	int Category = 0; // 명령 분류
	SendOrRecv IsItSendOrRecv = SendOrRecv::Recv; // 전송 혹은 수신 소켓 구별용 
	SOCKET sock; // accept로 받은 소켓
	SOCKET liSock; // listenSock
	WSAEVENT accpetNoti;
	OVERLAPPED ov; // overlap 구조체
public:
	UserDataStream(SOCKET ssock) // 초기화
	{
		ZeroMemory(this, sizeof(this));
		sock = ssock;
	}
};

// CFaceTheSunServerGUIDlg 대화 상자
class CFaceTheSunServerGUIDlg : public CDialogEx
{
// 생성입니다.
public:
	CFaceTheSunServerGUIDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FACETHESUNSERVERGUI_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClickedIdserveronoff();
	CEdit EditServerStatus; // 서버 상태 설정
	bool IsServerOn = false; // 서버 ON OFF 판별용
	HANDLE hIocp = INVALID_HANDLE_VALUE; // IOCP 생성 핸들
	HANDLE Liocp = INVALID_HANDLE_VALUE; // 리슨 소켓용 iocp 핸들
	SOCKET ListenSock = INVALID_SOCKET; // 소켓
	UserDataStream* us = nullptr; // Overlap 구조체 및 데이터 식별용
	PTP_IO acceptTPIO = nullptr; // 스레드풀 객체
	PTP_WAIT ListenTPWait; //대기용 객체
	WSAEVENT acceptNotify;
	static void CALLBACK TPAcceptCallBackFunc(PTP_CALLBACK_INSTANCE instance, PVOID context, PVOID overlapped, ULONG result, ULONG_PTR NumOfBytesTrans, PTP_IO tio);
	static void CALLBACK TPAcceptWaitCallBackFunc(PTP_CALLBACK_INSTANCE instance, PVOID context, PTP_WAIT wait, TP_WAIT_RESULT result);
};
