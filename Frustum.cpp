#include "Frustum.hpp"

#include <cmath>

#include "Macros.hpp"
#include "MathUtils.hpp"
#include "mat3.hpp"


namespace Minecrap
{

// Updates planes definifing the frustum
void updateFrustum(const Transform& t_transform, const View& t_view, Frustum& t_frustum)
{
//	float w_near2 	= t_view.m_nearPlane * std::tan(M_PI/6.0f);	// half w_near
	float w_near2 	= t_view.m_nearPlane * std::tan(t_view.m_fov / 2.0f);	// half w_near
	float h_near2	= w_near2 / t_view.m_alpha;

	Math::mat3 l_camR 	= t_transform.mRotor.getMatrixForm();
	Math::vec3 l_p		= t_transform.mPosition;

	// Find the axis in world coordinates
	Math::vec3 x_cam = l_camR.col1;
	Math::vec3 y_cam = l_camR.col2;
	Math::vec3 z_cam = l_camR.col3;

	// Front & back
	t_frustum.m_front.m_point	= l_p - z_cam * t_view.m_nearPlane;
	t_frustum.m_front.m_normal 	= (-1.0f) * z_cam;

	t_frustum.m_back.m_point	= l_p - z_cam * t_view.m_farPlane;
	t_frustum.m_back.m_normal	= z_cam;
	
	Math::vec3 l_tangent;

	// Right & left
	t_frustum.m_right.m_point 	= t_frustum.m_front.m_point + x_cam * w_near2;
	l_tangent 			= t_frustum.m_right.m_point - l_p;
	// TODO: normalization is not strictly needed here
	l_tangent.normalize();
	t_frustum.m_right.m_normal	= y_cam ^ l_tangent;

	t_frustum.m_left.m_point	= t_frustum.m_front.m_point - x_cam * w_near2;
	t_frustum.m_left.m_normal	= Math::reflect(t_frustum.m_right.m_normal, z_cam);

	// Top & bottom
	t_frustum.m_top.m_point		= t_frustum.m_front.m_point + y_cam * h_near2;
	l_tangent			= t_frustum.m_top.m_point - l_p;
	l_tangent.normalize();
	t_frustum.m_top.m_normal	= l_tangent ^ x_cam;

	t_frustum.m_bottom.m_point	= t_frustum.m_front.m_point - y_cam * h_near2;
	t_frustum.m_bottom.m_normal	= Math::reflect(t_frustum.m_top.m_normal, z_cam);
}


// short circuiting
bool isPointInsideFrustum(const Math::vec3& t_point, const Frustum& t_frustum)
{
	return (Math::isPointAbovePlane(t_point, t_frustum.m_right) and
		Math::isPointAbovePlane(t_point, t_frustum.m_left) and
		Math::isPointAbovePlane(t_point, t_frustum.m_top) and
		Math::isPointAbovePlane(t_point, t_frustum.m_bottom) and
		Math::isPointAbovePlane(t_point, t_frustum.m_front) and
		Math::isPointAbovePlane(t_point, t_frustum.m_back));
}


bool isBoundingBoxBelowPlane(const fbbox& t_bbox, const Math::Plane& t_plane)
{
	for (const auto& t_point: t_bbox)
	{
		if (Math::isPointAbovePlane(t_point, t_plane))
		{
			return false;
		}
	}
	return true;
}

// int because Minecrap
bool isBoundingBoxIntersecting(const Frustum& t_frustum, int t_x, int t_y, int t_z,
			       int t_width, int t_height, int t_depth)
{
	float l_xf = static_cast<float>(t_x);
	float l_yf = static_cast<float>(t_y);
	float l_zf = static_cast<float>(t_z);
	float l_widthf 	= static_cast<float>(t_width);
	float l_heightf = static_cast<float>(t_height);
	float l_depthf 	= static_cast<float>(t_depth);

	fbbox l_bbox;
	l_bbox[0] = {l_xf		, l_yf		, l_zf};
	l_bbox[1] = {l_xf		, l_yf		, l_zf + l_depthf};
	l_bbox[2] = {l_xf + l_widthf, l_yf		, l_zf + l_depthf};
	l_bbox[3] = {l_xf + l_widthf, l_yf		, l_zf};
	l_bbox[4] = {l_xf		, l_yf + l_heightf, l_zf};
	l_bbox[5] = {l_xf		, l_yf + l_heightf, l_zf + l_depthf};
	l_bbox[6] = {l_xf + l_widthf, l_yf + l_heightf, l_zf + l_depthf};
	l_bbox[7] = {l_xf + l_widthf, l_yf + l_heightf, l_zf};
	
	if (isBoundingBoxBelowPlane(l_bbox, t_frustum.m_right))	{ return false; }
	if (isBoundingBoxBelowPlane(l_bbox, t_frustum.m_left))	{ return false; }
	if (isBoundingBoxBelowPlane(l_bbox, t_frustum.m_top))	{ return false; }
	if (isBoundingBoxBelowPlane(l_bbox, t_frustum.m_bottom)){ return false; }
	if (isBoundingBoxBelowPlane(l_bbox, t_frustum.m_front))	{ return false; }
	if (isBoundingBoxBelowPlane(l_bbox, t_frustum.m_back))	{ return false; }

	return true;
}




}
