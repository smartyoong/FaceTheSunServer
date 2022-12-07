
// FaceTheSunServerGUIDlg.h: 헤더 파일
//
#pragma once
#include <map>
#include <vector>
#include <queue>
#include <set>
#include "MakePackToBuffer.h"
#include "Packet.h"
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
class UserDataStream : OVERLAPPED // overlap
{
public: 
	SOCKET sock; // accept로 받은 소켓
	PVOID CustomOrder = nullptr; //명령 및 명령 길이
	int OrderLen = 0;
	char addr[INET_ADDRSTRLEN]; //주소
	char buffer[4096]; // 명령을 받는 버퍼
	char ID[4096]; // ID 연결 끊긴 소켓 관리용
	PTP_IO ptpRecvSend; // SendRecv 전용 스레드풀
	bool Reuse = false;
	bool Error = false;
	bool Stop = false;
public:
	void Initialize() { ZeroMemory(this, sizeof(UserDataStream)); }
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

private: //socket, iocp, threadpool 관련 변수들
	PTP_IO acceptTPIO = nullptr; //송수신용 스레드풀 객체
	LPFN_ACCEPTEX pAcceptEX = nullptr; // acceptEx
	LPFN_GETACCEPTEXSOCKADDRS pAcceptAddrs = nullptr; // GetAccpetExSockAddrs
	LPFN_DISCONNECTEX pDisconnect = nullptr; // DisconnectedEX
	SOCKET ListenSock = INVALID_SOCKET; // 소켓
	CRITICAL_SECTION SyncroData; // 소켓이나 UserDataStream객체 삽입 삭제시 에러방지를 위한 임계영역

private: // db 관련 변수
	CDatabase FaceTheSunDB; // db
	CRecordset* FaceTheSunRecordSet; // record

private: // MFC관련 변수
	CEdit EditServerStatus; // 서버 상태 설정
	bool IsServerOn = false; // 서버 ON OFF 판별용
	CListBox ConnectUserList; // 접속된 유저 정보
	CButton ServerOnOffButton; // 서버 온오프
	CListBox ListUserData; // 유저 상세정보를 출력하는 리스트 박스
	CEdit EditUserData;  // 유저 데이터를 수정하기 위한 값을 받는 변수

private: // 객체 컨테이너
	std::map<CString, SOCKET> ConnectedSocketSet; // 연결된 소켓들 관리용 키는 ID, 값은 소켓
	std::vector<UserDataStream*>USArray; // UserData보관하다가 나중에 TP관리용
	std::queue<SOCKET>DisconnectedSocket; // 연결이 끊긴 소켓들을 보관했다가 재사용하는데 초점을 둔다.
	std::set<CString>OnlineUsers; // ID는 유니크하므로 SET으로 객체 생성
	std::map<int, CString> UserDataField; // db 필드명
	std::map<CString, CString> UserIPField; // ID당 IP 수집용

private:
	

public: //실행 함수들
	void BeginAcceptStart(); // accept 시키는 함수
	void BeginRecvStart(UserDataStream* us); // Recv시키는 함수
	void SendKindOfData(UserDataStream* us); // 상황별Send
	void CleanUpAllSocketAndTP(); // 모든 클라이언트 소켓 및 데이터, 스레드풀 삭제용
	void InsertDBField(); // db필드명 초기화
	void SignInDB(PackToBuffer* pb, UserDataStream* us); // 회원가입 함수
	void LogIn(PackToBuffer* pb, UserDataStream* us); // 로그인
	void IDCheck(PackToBuffer* pb, UserDataStream* us); //ID체크

public: // 메세지 함수들
	afx_msg void OnClickedIdserveronoff(); // 서버 온오프
	afx_msg void OnBnClickedButtonuserdata(); //유저 데이터 값 수정
	afx_msg void OnDestroy(); // 파괴될때 객체 정리용
	afx_msg void OnBnClickedButtonModify(); // 유저 상세정보 보기
	afx_msg void OnBnClickedButtonShutdown(); // 상대방 접속 종료
	afx_msg void OnBnClickedRfreshuser(); // 수동조작으로 접속 유저 목록 동기화
	afx_msg void OnTimer(UINT_PTR nIDEvent); //유저목록 자동 갱신을 위한 타이머 1분마다

public: // 콜백 함수들
	// accept 콜백함수
	static void CALLBACK TPAcceptCallBackFunc(PTP_CALLBACK_INSTANCE instance, PVOID context, PVOID overlapped, ULONG result, ULONG_PTR NumOfBytesTrans, PTP_IO tio);
	// recv 콜백함수
	static void CALLBACK TPRecvSendCallBackFunc(PTP_CALLBACK_INSTANCE instance, PVOID context, PVOID overlapped, ULONG result, ULONG_PTR NumOfBytesTrans, PTP_IO tio);
	afx_msg void OnLbnSelchangeListuserdata();
};
