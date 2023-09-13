#ifndef __LIGHT_SAMPLE_HPP__
#define __LIGHT_SAMPLE_HPP__

#include "utils/defines.hpp"

namespace RT_CPU
{
	struct LightSample
	{
		LightSample(const Vec3f& p_radiance, const Vec3f& p_direction, const float p_distance)
			: _radiance(p_radiance) ,_direction(p_direction), _distance(p_distance) {}

		Vec3f _radiance = VEC3F_ONE; // radiance at given point divided by pdf
		Vec3f _direction = VEC3F_X; // normalized direction toward light source
		float _distance = 0.f;
	};

}

#endif