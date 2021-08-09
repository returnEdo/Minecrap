#include "EntityManager.hpp"

#include "Macros.hpp"

namespace ecs
{



void EntityManager::init(void)
{
	for (int i = 0; i < C_MAX_ENTITIES; i++)
	{
		mAvailable.push(i);
		MaskUtils::reset(mMasks[i]);
	}
};


Entity EntityManager::createEntity(void)
{
	ASSERT(not mAvailable.empty(), "Reached max number of entities!!");

	Entity lEntity = mAvailable.front();
	mAvailable.pop();

	return lEntity;
};


void EntityManager::destroyEntity(Entity tEntity)
{
	ASSERT(tEntity >= 0 and tEntity < C_MAX_ENTITIES, "Invalid Entity. Could not destroy!!");
	
	mAvailable.push(tEntity);
	MaskUtils::reset(mMasks[tEntity]);
};


Mask& EntityManager::getMask(Entity tEntity)
{
	return mMasks[tEntity];
};



}
