#pragma once
#include <string>
#include <vector>


enum PacketID : int // 패킷 처리 헤더
{
	TryLogIn = 0, // 로그인 시도
	LogInResult = 1, // 로그인 결과
	AskLobby = 2, // 요청만 보내는것이기 때문에 따로 패킷은 필요없음 (로비 목록 요청)
	SendLobby = 3, // 로비 목록 전송
	AskToRoom = 4, // 요청만 보내는것이기 때문에 따로 패킷은 필요없음 (로비에서 방으로 입장시)
	SendRoomInfo = 5, // 방 입장시 정보 전달 (기존에 있던 사람들 등)
	AskShop = 6, // 요청만 보내는것이기 때문에 따로 패킷은 필요없음 (상점 정보 요청)
	SendShop = 7, // 상점 정보
	PurchaseItem = 8, // 아이템 구매 요청
	SellItem = 9, // 판매(서버측에서 구매한 사람에게로 전송)
	GameStart = 10, // 게임 시작 어쩌면 방 입장시부터 리슨서버로 전환시킬수도 있으므로 안사용할수도 있음
	SendGameResult = 11, // 게임 결과 전송
	RecvGameResult = 12, // 게임 결과 적용 결과 받음
	TrySignIn =13, // 회원 가입
	SignInResult =14, // 회원가입 결과
	TryID = 15, // ID 중복 체크
	IDResult =16, // ID 중복 결과
	AskCreateRoom = 17, // 방생성
	CreateRoomResult = 18, // 방생성 결과
	DeleteRoom =19, // 방 삭제
	SomeBodyJoin = 20, // 누군가 방 입장시 기존 유저들에게 갱신
	SendChat = 21, // 방에서 채팅 보내기
	RecvChat = 22 // 누군가 보낸 채팅 받기
};

struct RoomInfo
{
	std::string RoomName;
	std::string HostName;
	int CurrentPlayer = 1;
	bool CanJoin = true;
	RoomInfo() = default;
	RoomInfo(std::string Room, std::string Host)
	{
		RoomName = Room;
		HostName = Host;
	}
};