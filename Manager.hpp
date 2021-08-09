#pragma once

#include "Entity.hpp"
#include "EntityManager.hpp"
#include "Mask.hpp"
#include "ComponentManager.hpp"
#include "SystemManager.hpp"



namespace ecs
{


class Manager
{
	private:
	
	EntityManager		mEntityManager;
	ComponentManager	mComponentManager;
	SystemManager		mSystemManager;

	public:

	void init(void)
	{	
		mEntityManager.init();
		mSystemManager.init();
	};

	Entity createEntity(void)
	{	
		return mEntityManager.createEntity();
	};

	void destroyEntity(Entity tEntity)
	{
		mEntityManager.destroyEntity(tEntity);
	};

	template <typename T>
	T& addComponent(Entity tEntity, T tComponent)
	{
		int lComponentId = mComponentManager.getComponentId<T>();
		Mask& lEntityMask = mEntityManager.getMask(tEntity);

		MaskUtils::setComponentById(lEntityMask, lComponentId);

		return mComponentManager.addComponent<T>(tEntity, tComponent);
	};

	template <typename T>
	T& getComponent(Entity tEntity)
	{
		int lComponentId = mComponentManager.getComponentId<T>();
		Mask lEntityMask = mEntityManager.getMask(tEntity);
		
		if (not MaskUtils::hasComponentById(lEntityMask, lComponentId))
		{
			WARNING(true, "Entity does not have component. Component created!!");
			mComponentManager.addComponent<T>(tEntity, {});
		};
			
		return mComponentManager.getComponent<T>(tEntity);
	};

	template <typename T>
	void removeComponent(Entity tEntity)
	{
		int lComponentId = mComponentManager.getComponentId<T>();
		Mask& lEntityMask = mEntityManager.getMask(tEntity);

		if (not MaskUtils::hasComponentById(lEntityMask, lComponentId))
		{		
			WARNING(true, "The entity does not have the compoent required!!");
		};

		MaskUtils::removeComponentById(lEntityMask, lComponentId);

		mComponentManager.removeComponent<T>(tEntity);
	};

	template <typename T, class ... Types>
	T& createSystem(void)
	{
		Mask lMask;	
		MaskUtils::reset(lMask);
		(MaskUtils::setComponentById(lMask, mComponentManager.getComponentId<Types>()), ...);		
		
		Mask& lSystemMask = mSystemManager.getMask<T>();
		lSystemMask = lMask;
		
		updateSystemEntities<T>();

		return mSystemManager.getSystem<T>();
	};

	// You decide when calling it
	template <typename T>
	void updateSystemEntities(void)
	{	
		mSystemManager.resetEntities<T>();

		Mask lSystemMask = mSystemManager.getMask<T>();
		
		for (int i = 0; i < C_MAX_ENTITIES; i++)
		{
			Mask lEntityMask = mEntityManager.getMask(i);
			if (MaskUtils::hasComponent(lEntityMask, lSystemMask))
			{
				mSystemManager.addEntityToSystem<T>(i);
			}
		}
	};	

	template <typename T>
	const std::set<Entity>& getSystemEntities(void)
	{
		return mSystemManager.getSystemEntities<T>();	
	};
};

}
