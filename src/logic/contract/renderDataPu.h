#pragma once

//#include "logic/entity/entityKind.h"
#include "logic/entity/entity.h"

#include <vector>
#include <cstdint>//为了uint32_t

struct RenderDataPu {
	std::vector<Entity> entitys_;
	uint32_t selectedID_ = 0;//被选中的id
	//uint32_t selectedHoverID_ = 0;//悬停的id
	//bool loogTimeHover_ = false;
};
