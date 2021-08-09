#pragma once

#include <set>
#include <array>

#include "ISystem.hpp"
#include "Mask.hpp"
#include "Unique.hpp"
#include "ecsConstants.hpp"


namespace ecs
{


class SystemManager
{
	private:

	std::array<Memory::Unique<ISystem>,	C_MAX_SYSTEMS>	mSystems;
	std::array<std::set<Entity>, 		C_MAX_SYSTEMS>	mEntities;
	std::array<Mask, 			C_MAX_SYSTEMS>	mMasks;

	int mSystemCount { 0 };

	public:

	void init(void)
	{
		for (Mask& lMask: mMasks)
		{
			MaskUtils::reset(lMask);
		}

	};

	template <typename T>
	int getSystemId(void)
	{
		static int sThisSystemCount = 0;
		static int sThisSystemId;

		if (not sThisSystemCount)
		{	
			sThisSystemId = mSystemCount++;
			mSystems[sThisSystemId].allocate<T>();
		}
		
		sThisSystemCount++;

		return sThisSystemId;
	};

	template <typename T>
	T& getSystem(void)
	{
		return *((T*) mSystems[getSystemId<T>()].get());
	};

	// Check the mask before
	template <typename T>	
	void addEntityToSystem(Entity tEntity)
	{
		mEntities[getSystemId<T>()].insert(tEntity);
	};

	template <typename T>
	void removeEntityFromSystem(Entity tEntity)
	{
		mEntities[getSystemId<T>()].erase(tEntity);
	};

	template <typename T>
	void resetEntities(void)
	{	
		int lSystemId = getSystemId<T>();
		mEntities[lSystemId].erase(mEntities[lSystemId].begin(),
					   mEntities[lSystemId].end());
	};

	template <typename T>
	Mask& getMask(void)
	{
		return mMasks[getSystemId<T>()];
	};

	template <typename T>
	const std::set<Entity>& getSystemEntities(void)
	{
		return mEntities[getSystemId<T>()];
	};
};



}
