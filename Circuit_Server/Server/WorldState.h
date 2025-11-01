#pragma once
#include <vector>
#include "Player.h"
#include "Item.h"
#include "Obstacle.h"

class WorldState
{
public:
    Player player;           // ���� 1�ο� ����
    std::vector<Item> items; // �� �� �����ϴ� �����۵�
    std::vector<Obstacle> obstacles;    // �ʿ� �����ϴ� ��ֹ���� �浹���� ��ġ��

    WorldState();

    void Initialize();           // �ʱ� ����
    void RemoveItem(int itemID); // ������ ���� ó��
    void SyncPlayers();          // Ŭ���̾�Ʈ�� �޾� ��ġ, �ӵ�, �����͸� ����ȭ�� ����ü�� �ݿ�
};
