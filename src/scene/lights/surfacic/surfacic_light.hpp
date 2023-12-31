#ifndef __SURFACIC_LIGHT_HPP__
#define __SURFACIC_LIGHT_HPP__

#include "scene/lights/light.hpp"

namespace RT_CPU
{

	class SurfacicLight : public Light
	{
	public:
		SurfacicLight(const Vec3f& p_position, const Vec3f& p_direction, const Vec3f& p_color, float _area, float p_power, int p_nbShadowRay)
			: Light(p_position, p_direction, p_color, p_power, p_nbShadowRay) {}
		virtual ~SurfacicLight() = default;

		virtual LightSample sample(const Vec3f & p_point, int p_id) const = 0;

	protected:
		float _area = 1.f;
	};

}

#endif