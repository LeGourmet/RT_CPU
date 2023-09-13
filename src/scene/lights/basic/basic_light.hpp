#ifndef __BASIC_LIGHT_HPP__
#define __BASIC_LIGHT_HPP__

#include "scene/lights/light.hpp"

namespace RT_CPU
{

	class BasicLight : public Light
	{
	public:
		BasicLight(const Vec3f& p_position, const Vec3f& p_direction, const Vec3f& p_color, float p_power)
			: Light(p_position, p_direction, p_color, p_power, 1) {}
		virtual ~BasicLight() = default;

		virtual LightSample sample( const Vec3f & p_point, int p_id ) const = 0;
	};

}

#endif