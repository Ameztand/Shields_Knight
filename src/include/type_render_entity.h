#pragma once

#include "../logic/entityManager/entityKind.h"
#include "../logic/entityManager/entity.h"

#include <vector>
#include <cstdint>//为了uint32_t

struct RenderDataEntity {
	std::vector<Entity> entitys_;
	uint32_t selectedID_ = 0;//被选中的id
	//uint32_t selectedHoverID_ = 0;//悬停的id
	//bool loogTimeHover_ = false;
};
