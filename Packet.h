#pragma once
#include <string>
#include <vector>


enum PacketID : int // ��Ŷ ó�� ���
{
	TryLogIn = 0, // �α��� �õ�
	LogInResult = 1, // �α��� ���
	AskLobby = 2, // ��û�� �����°��̱� ������ ���� ��Ŷ�� �ʿ���� (�κ� ��� ��û)
	SendLobby = 3, // �κ� ��� ����
	AskToRoom = 4, // ��û�� �����°��̱� ������ ���� ��Ŷ�� �ʿ���� (�κ񿡼� ������ �����)
	SendRoomInfo = 5, // �� ����� ���� ���� (������ �ִ� ����� ��)
	AskShop = 6, // ��û�� �����°��̱� ������ ���� ��Ŷ�� �ʿ���� (���� ���� ��û)
	SendShop = 7, // ���� ����
	PurchaseItem = 8, // ������ ���� ��û
	SellItem = 9, // �Ǹ�(���������� ������ ������Է� ����)
	GameStart = 10, // ���� ���� ��¼�� �� ����ú��� ���������� ��ȯ��ų���� �����Ƿ� �Ȼ���Ҽ��� ����
	SendGameResult = 11, // ���� ��� ����
	RecvGameResult = 12, // ���� ��� ���� ��� ����
	TrySignIn =13, // ȸ�� ����
	SignInResult =14, // ȸ������ ���
	TryID = 15, // ID �ߺ� üũ
	IDResult =16, // ID �ߺ� ���
	AskCreateRoom = 17, // �����
	CreateRoomResult = 18, // ����� ���
	DeleteRoom =19, // �� ����
	SomeBodyJoin = 20, // ������ �� ����� ���� �����鿡�� ����
	SendChat = 21, // �濡�� ä�� ������
	RecvChat = 22 // ������ ���� ä�� �ޱ�
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