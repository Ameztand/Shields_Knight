#include "entityManager.h"

void EntityManager::registerEntitys(const IGameState::GameSta nowSta, const EntityID id)
{
	size_t sta = (size_t)nowSta;
	entitys[sta].clear();
	EntityConfig tempEntityConfig = { { 100 , 500 } , { { 50, 450 } , 100 , 100 } , EntityID::testEntity };
	Entity tempEntity(tempEntityConfig);
	entitys[sta].push_back(tempEntity);//临时构造
}

void EntityManager::deleteEntitys(const IGameState::GameSta nowSta, const EntityID id)
{
	size_t sta = (size_t)nowSta;
	for (auto& ety : entitys[sta]) 
	{
		if (ety.status.ID == id)
		{
			ety.status.ID = EntityID::Null;//后续做真删除（换unorther_map）
		}
	}
}

const std::array<std::vector<Entity>, kStateCount>& EntityManager::commitEntitys()
{
	return entitys;
}

void EntityManager::tick(Data& data, const IGameState::GameSta nowSta)
{
	//?仲裁？还是统一更新？
}

void EntityManager::initEntityManager()
{
	//？？？
}
