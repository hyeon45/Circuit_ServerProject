#pragma once
#include <cstdint>
#pragma pack(push,1)
// 패킷 타입 
//패킷 보낼 때 맨 앞에 붙여서 어떤 데이터인지 구분해줌
enum : uint16_t {
	PKT_GAME_START  = 0x1001,	//2바이트 정수로 패킷 헤더의 고정 크기 사용
	PKT_CAR_MOVE    = 0x1002,   //고유 패킷 아이디 0x1001, 0x1002 ...
	PKT_ITEM_DELETE = 0x1003,
	PKT_WORLD_SYNC  = 0x1004,
	PKT_GAME_RESULT = 0x1005
};

// =======================
// [ Server → Client ]
// =======================

struct PKT_GameStart {
	uint16_t type = PKT_GAME_START;
	int playerID;
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
	float    scale;     // 추가
	uint8_t  shield;    // 추가 (0/1)
};

struct PKT_GameResult {
	uint16_t type = PKT_GAME_RESULT;
	int winnerID;
};

// =======================
// [ Client → Server ]
// =======================

struct PKT_CarMove {
	uint16_t type = PKT_CAR_MOVE;
	int playerID;
	uint8_t button;
};

#pragma pack(pop)