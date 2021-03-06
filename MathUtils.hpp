#pragma once

#include "vec2.hpp"
#include "vec3.hpp"
#include "mat3.hpp"


namespace Math
{
	
float sign(float tValue);

template <typename T>
T sign(T tValue)
{
	return (T)((tValue > T(0)) - (tValue < T(0)));
}

// if negative no overlap
float segmentOverlap(float tCenterA, float tDimensionA,
		     float tCenterB, float tDimensionB);


bool areRectanglesIntersecting(const Math::vec2& tCenterA, const Math::vec2& tDimensionsA,
			       const Math::vec2& tCenterB, const Math::vec2& tDimensionsB);


bool near(float tValue, float tReference, float tTollerance);

Math::mat3 cross(const Math::vec3& tV);

float randf(float tA, float tB);

float clamp(float tX, float tLower, float tUpper);

template <typename T>
T clamp(T tValue, T tLower, T tUpper)
{
	return std::max(std::min(tUpper, tValue), tLower);
}


Math::vec3 vLerp(const Math::vec3& tA, const Math::vec3& tB, float tAlpha);
Math::vec3 randNormal(void);


// Barycentric coordinates

Math::vec3 findBarycentric(const Math::vec3& x, const Math::vec3& u,
			   const Math::vec3& v, const Math::vec3& w);

bool isInsideBarycentric(const Math::vec3& tB);


template <typename T>
T interpolateBarycentric(const Math::vec3& tBarycentric, T u, T v, T w)
{
	return (tBarycentric.x * u + tBarycentric.y * v + tBarycentric.z * w);
}

Math::vec3 reflect(const Math::vec3& tReflect, const Math::vec3& tAxis);


float squishify(float t_value, float t_k = 15.0f);
float quantize(float t_value, float t_stepCount); 


};
