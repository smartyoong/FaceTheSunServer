
// FaceTheSunServerGUIDlg.h: 헤더 파일
//
#pragma once
#include <map>
#include <vector>
#include <queue>
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
enum class SendOrRecv : DWORD
{
	Accept = 0,
	Recv = 1,
	Send = 2
};
class UserDataStream : OVERLAPPED // overlap
{
public: 
	SOCKET sock; // accept로 받은 소켓
	PVOID CustomOrder = nullptr; //명령 및 명령 길이
	int OrderLen = 0;
	char buffer[4096]; // 명령을 받는 버퍼
	char ID[4096]; // ID 연결 끊긴 소켓 관리용
	int Category = 0; // 명령 분류
	SendOrRecv IsItSendOrRecv = SendOrRecv::Accept; // 전송 혹은 수신 소켓 구별용
	PTP_IO ptpRecvSend; // SendRecv 전용 스레드풀
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
	static void CALLBACK TPRecvSendCallBackFunc(PTP_CALLBACK_INSTANCE instance, PVOID context, PVOID overlapped, ULONG result, ULONG_PTR NumOfBytesTrans, PTP_IO tio);
	LPFN_ACCEPTEX pAcceptEX = nullptr; // acceptEx
	LPFN_GETACCEPTEXSOCKADDRS pAcceptAddrs = nullptr; // GetAccpetExSockAddrs
	CListBox ConnectUserList; // 접속된 유저 정보
	SOCKET ListenSock = INVALID_SOCKET; // 소켓
	void BeginAcceptStart(); // accept 시키는 함수
	void BeginRecvStart(UserDataStream* us); // Recv시키는 함수
	void SendKindOfData(UserDataStream* us); // 상황별Send
	void CleanUpAllSocketAndTP();
	std::map<char*, SOCKET> ConnectedSocketSet; // 연결된 소켓들 관리용 키는 ID, 값은 소켓
	std::vector<UserDataStream*>USArray; // UserData보관하다가 나중에 TP관리용
	std::queue<SOCKET>DisconnectedSocket; // 연결이 끊긴 소켓들을 보관했다가 재사용하는데 초점을 둔다.
};
