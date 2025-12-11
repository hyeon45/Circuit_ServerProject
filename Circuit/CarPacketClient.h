#pragma once
#include <cstdint>
#pragma pack(push,1)

enum : uint16_t {
	PKT_GAME_START = 0x1001,	//2바이트 정수로 패킷 헤더의 고정 크기 사용
	PKT_CAR_MOVE = 0x1002,   //고유 패킷 아이디 0x1001, 0x1002 ...
	PKT_ITEM_DELETE = 0x1003,
	PKT_WORLD_SYNC = 0x1004,
	PKT_GAME_RESULT = 0x1005
};

// 서버가 쓰는 것과 동일하게 맞춰야 함
struct PKT_HEADER {
	uint16_t type;
	uint16_t size;
};
// 임시 확인용
struct PKT_CarMoveBody {
	uint8_t button;
};

// =======================
// [ Client → Server ]
// =======================

struct PKT_CarMove {
	uint16_t type = PKT_CAR_MOVE;
	int playerID;
	uint8_t button;
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

#pragma pack(pop)