#pragma once
#include <cstdint>
#pragma pack(push,1)
// ��Ŷ Ÿ�� 
//��Ŷ ���� �� �� �տ� �ٿ��� � ���������� ��������
enum : uint16_t {
	PKT_GAME_START  = 0x1001,	//2����Ʈ ������ ��Ŷ ����� ���� ũ�� ���
	PKT_CAR_MOVE    = 0x1002,   //���� ��Ŷ ���̵� 0x1001, 0x1002 ...
	PKT_ITEM_DELETE = 0x1003,
	PKT_WORLD_SYNC  = 0x1004,
	PKT_GAME_RESULT = 0x1005
};

// =======================
// [ Server �� Client ]
// =======================

struct PKT_GameStart {
	uint16_t type = PKT_GAME_START;
};

struct PKT_ItemDelete {
	uint16_t type = PKT_ITEM_DELETE;
	int itemID;
	float posx, posy, posz;
};

struct PKT_WorldSync {
	uint16_t type = PKT_WORLD_SYNC;
	int playerID;
	float posx, posy, posz;
	float yaw;
};

struct PKT_GameResult {
	uint16_t type = PKT_GAME_RESULT;
	int winnerID;
};

// =======================
// [ Client �� Server ]
// =======================

struct PKT_CarMove {
	uint16_t type = PKT_CAR_MOVE;
	int playerID;
	uint8_t button;
};

#pragma pack(pop)