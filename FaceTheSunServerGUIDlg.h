
// FaceTheSunServerGUIDlg.h: 헤더 파일
//
#pragma once
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
class UserDataStream : OVERLAPPED // overlap
{
public: 
	enum class SendOrRecv : DWORD
	{
		Accept = 0,
		Recv = 1,
		Send = 2
	};
	SOCKET sock; // accept로 받은 소켓
	PVOID CustomOrder = nullptr; //명령 및 명령 길이
	int OrderLen = 0;
	char buffer[4096]; // 명령을 받는 버퍼
	int Category = 0; // 명령 분류
	SendOrRecv IsItSendOrRecv = SendOrRecv::Recv; // 전송 혹은 수신 소켓 구별용
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
	PTP_IO acceptTPIO = nullptr; //송수신용 스레드풀 객체
	static void CALLBACK TPAcceptCallBackFunc(PTP_CALLBACK_INSTANCE instance, PVOID context, PVOID overlapped, ULONG result, ULONG_PTR NumOfBytesTrans, PTP_IO tio);
	LPFN_ACCEPTEX pAcceptEX = nullptr; // acceptEx
	LPFN_GETACCEPTEXSOCKADDRS pAcceptAddrs = nullptr; // GetAccpetExSockAddrs
	CListBox ConnectUserList; // 접속된 유저 정보
	SOCKET ListenSock = INVALID_SOCKET; // 소켓
	void BeginAcceptStart();
};
