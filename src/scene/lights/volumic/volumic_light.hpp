#ifndef __VOLUMIC_LIGHT_HPP__
#define __VOLUMIC_LIGHT_HPP__

#include "scene/lights/light.hpp"

namespace RT_CPU
{

	class VolumicLight : public Light
	{
	public:
		VolumicLight(const Vec3f& p_position, const Vec3f& p_direction, const Vec3f& p_color, float p_power, float p_area, int p_nbShadowRay)
			: Light(p_position, p_direction, p_color, p_power, p_nbShadowRay*p_nbShadowRay*p_nbShadowRay), _area(p_area) {}
		virtual ~VolumicLight() = default;

		virtual LightSample sample(const Vec3f& p_point, int p_id) const = 0;
		virtual LightSample sample(const Vec3f& p_point) const = 0;

	protected:
		float _area = 1.f;
	};

}

#endif