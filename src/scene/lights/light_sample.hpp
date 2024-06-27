#ifndef __LIGHT_SAMPLE_HPP__
#define __LIGHT_SAMPLE_HPP__

#include "utils/defines.hpp"

namespace RT_CPU
{
	struct LightSample
	{
		LightSample(const Vec3f& p_radiance, const Vec3f& p_direction, const float p_distance, const float p_pdf)
			: _radiance(p_radiance) ,_direction(p_direction), _distance(p_distance), _pdf(p_pdf) {}

		Vec3f _radiance		= VEC3F_ONE;
		Vec3f _direction	= VEC3F_X;
		float _distance		= 0.f;
		float _pdf			= 1.f;
	};

}

#endif