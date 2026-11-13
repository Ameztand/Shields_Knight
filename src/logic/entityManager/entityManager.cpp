#include "entityManager.h"

void EntityManager::registerEntitys(const IGameState::GameSta nowSta, const EntityID id)
{
	size_t sta = (size_t)nowSta;
	entitys[sta].clear();

	Entity e;
	e.ID = id;
	e.pos = { 960.f, 540.f };              // 屏幕正中
	e.vel = { 0.f, 0.f };
	e.w = 100.f;
	e.h = 100.f;
	e.speed = 300.f;                         // 像素/秒

	entitys[sta].push_back(e);//临时构造
}

void EntityManager::deleteEntitys(const IGameState::GameSta nowSta, const EntityID id)
{
	size_t sta = (size_t)nowSta;
	for (auto& ety : entitys[sta]) 
	{
		//if (ety.status.ID == id)
		//{
		//	ety.status.ID = EntityID::Null;//后续做真删除（换unorther_map）
		//}
	}
}

const std::array<std::vector<Entity>, kStateCount>& EntityManager::commitEntitys()
{
	return entitys;
}

void EntityManager::tick(Data& data, const IGameState::GameSta nowSta, double dt)
{
	//?仲裁？还是统一更新？
	//临时实体控制
	const auto& msgData = data.getMsgData();

	/*
	size_t sta = (size_t)nowSta;
	for (auto& ety : entitys[sta]) {

		//临时运动检测代码
		if (IInputLayer::held(msgData.W)) ety.Vy -= 2;
		if (IInputLayer::held(msgData.S)) ety.Vy += 2;
		if (IInputLayer::held(msgData.A)) ety.Vx -= 2;
		if (IInputLayer::held(msgData.D)) ety.Vx += 2;

		if (ety.Vx > 0) ety.Vx--; 
		else if (ety.Vx < 0) ety.Vx++;
		if (ety.Vy > 0) ety.Vy--;
		else if (ety.Vy < 0) ety.Vy++;

		ety.updateMove();
	}
	*/
	float dx = 0.f, dy = 0.f;
	if (IInputLayer::held(msgData.W)) dy -= 1.f;
	if (IInputLayer::held(msgData.S)) dy += 1.f;
	if (IInputLayer::held(msgData.A)) dx -= 1.f;
	if (IInputLayer::held(msgData.D)) dx += 1.f;

	if (dx != 0.f || dy != 0.f) {
		const float len = std::sqrt(dx * dx + dy * dy);
		dx /= len;
		dy /= len;                           // 斜走不加速
	}

	size_t sta = (size_t)nowSta;
	for (auto& e : entitys[sta]) {
		e.vel = { dx * e.speed, dy * e.speed };
		e.updateMove(dt);                    // 只传"时长"
	}
}

void EntityManager::initEntityManager()
{
	//？？？
}
