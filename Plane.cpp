#include "Plane.hpp"

namespace Math
{


float plane_sdf(const Math::vec3& t_x, const Plane& t_plane)
{
	return (t_plane.m_normal * (t_x - t_plane.m_point));
}

// Above = in the direction of the normal
bool isPointAbovePlane(const Math::vec3& t_x, const Plane& t_plane)
{
	return (plane_sdf(t_x, t_plane) > 0.0f);
}

}
