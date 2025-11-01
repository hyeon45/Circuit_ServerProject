#include "WorldState.h"

WorldState::WorldState()
    : player(0) // Player ID
{
}

// ----------------------------------------
// ���� ���� �� �ʱ�ȭ
// ----------------------------------------
void WorldState::Initialize()
{
    // �÷��̾� �ʱ� ��ġ
    player.x = 100.0f;
    player.y = 10.0f;
    player.z = 1780.0f;

    // ������ ��ġ
    float ItemPosy = 5.0f; // ������ ��ġ ���̰��� ����

    items = {
        {0, 270.0f, ItemPosy, 1100.0f},
        {1, 300.0f, ItemPosy, 1100.0f},
        {2, 330.0f, ItemPosy, 1100.0f},
        {3, 360.0f, ItemPosy, 1100.0f},

        {4, 1000.0f, ItemPosy, 1700.0f},
        {5, 1000.0f, ItemPosy, 1730.0f},
        {6, 1000.0f, ItemPosy, 1760.0f},
        {7, 1000.0f, ItemPosy, 1790.0f},

        {8, 1840.0f, ItemPosy, 900.0f},
        {9, 1870.0f, ItemPosy, 900.0f},
        {10, 1900.0f, ItemPosy, 900.0f},
        {11, 1930.0f, ItemPosy, 900.0f},

        {12, 810.0f, ItemPosy, 600.0f},
        {13, 840.0f, ItemPosy, 600.0f},
        {14, 870.0f, ItemPosy, 600.0f},
        {15, 900.0f, ItemPosy, 600.0f}
    };

}

// ----------------------------------------
// ������ ���� ó�� (�浹 �� ȣ��)
// ----------------------------------------
void WorldState::RemoveItem(int itemID)
{
    
}


// ----------------------------------------
// Ŭ���̾�Ʈ ��ġ, ȸ��, �ӵ� ������ ����ȭ �ݿ�
// ----------------------------------------
void WorldState::SyncPlayers()
{

}