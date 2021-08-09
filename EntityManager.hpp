#pragma once

#include <array>
#include <queue>

#include "ecsConstants.hpp"
#include "Entity.hpp"
#include "Mask.hpp"

namespace ecs
{


class EntityManager
{
	private:

	std::queue<Entity>			mAvailable;
	std::array<Mask, C_MAX_ENTITIES>	mMasks;

	public:

	void init(void);

	Entity createEntity(void);

	void destroyEntity(Entity tEntity);

	Mask& getMask(Entity tEntity);
};

}
