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

// =======================
// [ Client → Server ]
// =======================

struct PKT_CarMove {
	uint16_t type = PKT_CAR_MOVE;
	int playerID;
	uint8_t button;
};

#pragma pack(pop)