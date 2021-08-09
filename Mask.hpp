#pragma once

#include <cstdint>

namespace ecs
{

using Mask = uint64_t;

namespace MaskUtils
{

void reset(Mask& tMask);
void setComponent(Mask& tMask, Mask tComponent);
void setComponentById(Mask& tMask, int lComponentId);
void removeComponent(Mask& tMask, Mask tComponent);
void removeComponentById(Mask& tMask, int lComponentId);
bool hasComponent(Mask tMask, Mask tComponent);
bool hasComponentById(Mask tMask, int lComponentId);


}

}
