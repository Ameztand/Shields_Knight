#include "entityStore.h"

int EntityStore::findIndex(const uint32_t id) const
{
	if (id <= 0) return -1;
	auto it = setoff.find(id);
	if (it != setoff.end()) return it->second;
	return -1;
}

void EntityStore::registerEntitys(const EntityKind kind)
{
	//entitys_.clear();

	//临时构造
	static constexpr Vec2 kSpawns[] = { {960.f, 540.f}, {100.f, 100.f} };
	for (const Vec2 p : kSpawns) {
		Entity e;
		e.handle.id = ++id;          // ← 用你自己的计数器，别再写死
		e.kind = kind;
		e.pos = p;
		e.vel = { 0.f, 0.f };
		e.w = 100.f; e.h = 100.f; e.speed = 300.f;

		int i = entitys_.size();
		if (freeList_.empty()) entitys_.push_back(e);
		else { i = freeList_.front(); freeList_.pop(); entitys_[i] = e; }
		setoff[e.handle.id] = i;
	}
	//再调用一次，理应id不是1/2

	//entitys_.push_back(e);//临时构造
	//返回 handle.id ？
}

void EntityStore::deleteEntitys(const uint32_t id)
{
	if (id == 0)return;
	auto it = setoff.find(id);
	if (it != setoff.end()) 
	{
		int i = it->second;
		freeList_.push(i);
		entitys_[i].kind = EntityKind::Null;
		setoff.erase(id);
		//return true;
	}
	//return false;
}

const std::vector<Entity>& EntityStore::commitEntitys() const
{
	return entitys_;
}

const Entity& EntityStore::getEntity(const uint32_t id) const
{
	if (id == 0)return Eentity;
	auto it = setoff.find(id);
	if (it != setoff.end())
	{
		int i = it->second;
		return entitys_[i];
	}
	return Eentity;
}

void EntityStore::setEntity(const Entity& ety, const uint32_t id)
{
	if (id == 0)return;
	auto it = setoff.find(id);
	if (it != setoff.end())
	{
		int i = it->second;
		entitys_[i] = ety;
	}
}

void EntityStore::updateHover(const uint32_t id, const double dt)
{
	int i = findIndex(id);
	if (i != -1)
	{
		//不判断是否为墓碑
		entitys_[i].hoverTime += dt;
		entitys_[i].isHovered = entitys_[i].hoverTime > 0;
		entitys_[i].isLongHover = entitys_[i].hoverTime > entitys_[i].hoverThreshold;
	}
}

void EntityStore::clearHover(const uint32_t id)
{
	int i = findIndex(id);
	if (i != -1)
	{
		//不判断是否为墓碑
		entitys_[i].hoverTime = 0;
		entitys_[i].isHovered = entitys_[i].isLongHover = false;
	}
}

void EntityStore::clearAllHover()
{
	for (auto& ety : entitys_) {
		if (ety.kind == EntityKind::Null) continue;
		ety.hoverTime = 0;
		ety.isHovered = ety.isLongHover = false;
	}
}

void EntityStore::initEntityStore()
{
	//？？？
	entitys_.clear();
	while (!freeList_.empty())freeList_.pop();
	setoff.clear();
	id = 0;//牌号清空
}
