#pragma once
#include <vector>
#include "Player.h"
#include "Item.h"

class WorldState
{
public:
    Player player;           // ���� 1�ο� ����
    std::vector<Item> items; // �� �� �����ϴ� �����۵�

    WorldState();

    void Initialize();           // �ʱ� ����
    void RemoveItem(int itemID); // ������ ���� ó��
    void SyncPlayers();          // Ŭ���̾�Ʈ�� �޾� ��ġ, �ӵ�, �����͸� ����ȭ�� ����ü�� �ݿ�
};
