#pragma once

#include "vec2.hpp"
#include "vec3.hpp"
#include "Rotor.hpp"

#include "Constants.hpp"

namespace Minecrap
{

// Position + rotor for attitude
struct Transform
{
	Math::vec3	mPosition;
	Math::Rotor	mRotor;
};


// I usually attach it to the cam component
struct View
{
	float	m_nearPlane 	{ C_NEAR_PLANE };
	float	m_farPlane 	{ C_FAR_PLANE };
	float 	m_fov 		{ C_FOV };
	float	m_alpha 	{ C_ASPECT_RATIO };
};


}
