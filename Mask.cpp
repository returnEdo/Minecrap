#include "Mask.hpp"

#include "Macros.hpp"

namespace ecs
{

namespace MaskUtils
{

void reset(Mask& tMask)
{
	tMask = 0;
};

void setComponent(Mask& tMask, Mask tComponent)
{
	tMask |= tComponent;
};

void setComponentById(Mask& tMask, int lComponentId)
{
	tMask |= (1 << lComponentId);
};


void removeComponent(Mask& tMask, Mask tComponent)
{
	Mask lComplementary = ~tComponent;

	tMask &= lComplementary;
};

void removeComponentById(Mask& tMask, int lComponentId)
{
	Mask lComplementary = ~(1 << lComponentId);
	tMask &= lComplementary;
};

bool hasComponent(Mask tMask, Mask tComponent)
{
	return (tMask & tComponent) == tComponent;
};

bool hasComponentById(Mask tMask, int lComponentId)
{
	Mask lComponent = (1 << lComponentId);
	return  (tMask & lComponent) == lComponent;
};


}

}
