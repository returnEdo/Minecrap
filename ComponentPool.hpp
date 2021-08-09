#pragma once

#include "ecsConstants.hpp"
#include "Macros.hpp"


namespace ecs
{

class IComponentPool
{
	public:
	
	virtual ~IComponentPool(void) 			{};
	virtual void removeComponent(Entity tEntity) 	{};
};


template <typename T>
class ComponentPool: public IComponentPool
{
	private:

	T*	mHead { nullptr };
	
	public:

	ComponentPool(void)
	{
		mHead = (T*)(operator new(C_MAX_ENTITIES * sizeof(T)));
	};

	virtual ~ComponentPool(void)
	{	
		operator delete(mHead);
	};

	T& addComponent(Entity tEntity, T tComponent)
	{
		return *(new(mHead + tEntity) T(tComponent));
	};

	virtual void removeComponent(Entity tEntity)
	{
		(mHead + tEntity) -> ~T();
	};

	T& getComponent(Entity tEntity)
	{
		return *(mHead + tEntity);
	};

};

}
