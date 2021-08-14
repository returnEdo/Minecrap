#pragma once

#include "vec3.hpp"


namespace Math
{

struct Plane
{
	vec3	m_point		{0.0f, 0.0f, 0.0f};
	vec3	m_normal	{1.0f, 0.0f, 0.0f};

};

float plane_sdf(const Math::vec3& t_x, const Plane& t_plane);
bool isPointAbovePlane(const Math::vec3& t_x, const Plane& t_plane);

}
