#pragma once
#include "MakePackToBuffer.h"

enum PacketID : int // ��Ŷ ó�� ���
{
	TryLogIn = 0,
	LogInResult = 1,
	AskLobby = 2, // ��û�� �����°��̱� ������ ���� ��Ŷ�� �ʿ����
	SendLobby = 3,
	AskToRoom = 4, // ��û�� �����°��̽� ������ ���� ��Ŷ�� �ʿ����
	SendRoomInfo = 5,
	AskShop = 6, // ��û�� �����°��̱� ������ ���� ��Ŷ�� �ʿ����
	SendShop = 7,
	PurchaseItem = 8,
	SellItem = 9,
	GameStart = 10,
	SendGameResult = 11,
	RecvGameResult = 12
};