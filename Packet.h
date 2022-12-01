#pragma once
#include "MakePackToBuffer.h"

enum PacketID : int // 패킷 처리 헤더
{
	TryLogIn = 0,
	LogInResult = 1,
	AskLobby = 2, // 요청만 보내는것이기 때문에 따로 패킷은 필요없음
	SendLobby = 3,
	AskToRoom = 4, // 요청만 보내는것이시 때문에 따로 패킷은 필요없음
	SendRoomInfo = 5,
	AskShop = 6, // 요청만 보내는것이기 때문에 따로 패킷은 필요없음
	SendShop = 7,
	PurchaseItem = 8,
	SellItem = 9,
	GameStart = 10,
	SendGameResult = 11,
	RecvGameResult = 12
};