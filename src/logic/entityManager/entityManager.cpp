#include "entityManager.h"

void EntityManager::registerEntitys(const EntityID id)
{
	//entitys_.clear();

	//临时构造
	Entity e;
	e.ID = id;
	e.pos = { 960.f, 540.f };              // 屏幕正中
	e.vel = { 0.f, 0.f };
	e.w = 100.f;
	e.h = 100.f;
	e.speed = 300.f;                         // 像素/秒

	//放入
	int i = entitys_.size();
	if (freeList_.empty())
	{
		entitys_.push_back(e);
	}
	else
	{
		i = freeList_.front();
		freeList_.pop();
		entitys_[i] = e;
	}
	setoff[e.ID] = i;//建立索引

	//entitys_.push_back(e);//临时构造
}

void EntityManager::deleteEntitys(const EntityID id)
{
	auto it = setoff.find(id);
	if (it != setoff.end()) 
	{
		int i = it->second;
		freeList_.push(i);
		entitys_[i].ID = EntityID::Null;
		setoff.erase(id);
		//return true;
	}
	//return false;
}

const std::vector<Entity>& EntityManager::commitEntitys() const
{
	return entitys_;
}

std::vector<Entity>& EntityManager::commitEntitys()
{
	return entitys_;
}

void EntityManager::tick(Data& data, double dt)
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

	for (auto& e : entitys_) {
		e.vel = { dx * e.speed, dy * e.speed };
		e.updateMove(dt);                    // 只传"时长"
	}
}

void EntityManager::initEntityManager()
{
	//？？？
	entitys_.clear();
	while (!freeList_.empty())freeList_.pop();
	setoff.clear();
}
