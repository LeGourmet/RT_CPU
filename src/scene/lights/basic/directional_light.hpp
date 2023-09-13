#ifndef __DIRECTIONAL_LIGHT_HPP__
#define __DIRECTIONAL_LIGHT_HPP__

#include "basic_light.hpp"

namespace RT_CPU
{

	class DirectionalLight : public BasicLight
	{
	  public:
		DirectionalLight(const Vec3f& p_position, const Vec3f& p_direction, const Vec3f& p_color, float p_power = 1.f)
			: BasicLight( p_position, p_direction, p_color, p_power ){}
		~DirectionalLight() {}

		LightSample sample(const Vec3f& p_point, int p_id) const override {
			return LightSample(_color*_power, -_direction, FLT_MAX);
		}

	};

}

#endif