#pragma once

#include <array>

#include "Plane.hpp"
#include "vec3.hpp"
#include "RenderingComponents.hpp"


namespace Minecrap
{


struct Frustum
{
	Math::Plane	m_front;
	Math::Plane	m_back;

	Math::Plane	m_right;
	Math::Plane 	m_left;

	Math::Plane	m_top;
	Math::Plane	m_bottom;
};

using fbbox = std::array<Math::vec3, 8>;


void updateFrustum(const Transform& t_transform, const View& t_view, Frustum& t_frustum);
bool isPointInsideFrustum(const Math::vec3& t_point, const Frustum& t_frustum);
bool isBoundingBoxBelowPlane(const fbbox& t_points, const Math::Plane& t_plane);
bool isBoundingBoxIntersecting(const Frustum& t_frustum, int t_x, int t_y, int t_z,
			       int t_width, int t_height, int t_depth);


}
