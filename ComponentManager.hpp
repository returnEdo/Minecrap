#pragma once

#include "ecsConstants.hpp"
#include "ComponentPool.hpp"
#include "Unique.hpp"


namespace ecs
{

class ComponentManager
{
	private:
	
	std::array<Memory::Unique<IComponentPool>, C_MAX_COMPONENTS>	mPools;

	int mComponentCounter { 0 };

	public:

	template <typename T>
	int getComponentId(void)
	{
		static int sThisComponentCounter = 0;
		static int sThisComponentIndex;

		if (not sThisComponentCounter)
		{
			mPools[mComponentCounter].allocate<ComponentPool<T>>();

			sThisComponentIndex = mComponentCounter++;
		}
		sThisComponentCounter ++;

		return sThisComponentIndex;
	};

	template <typename T>
	ComponentPool<T>* getPool(void)
	{	
		return (ComponentPool<T>*) mPools[getComponentId<T>()].get();
	};

	template <typename T>
	T& addComponent(Entity tEntity, T tComponent)
	{
		return getPool<T>() -> addComponent(tEntity, tComponent);
	};
	
	template <typename T>
	T& getComponent(Entity tEntity)
	{
		return getPool<T>() -> getComponent(tEntity);
	};

	template <typename T>
	void removeComponent(Entity tEntity)
	{
		getPool<T>() -> removeComponent(tEntity);
	};
};

}
