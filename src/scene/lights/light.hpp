#ifndef __LIGHT_HPP__
#define __LIGHT_HPP__

#include "utils/defines.hpp"
#include "light_sample.hpp"

namespace RT_CPU
{
	class Light
	{
	public:
		Light(const Vec3f& p_position, const Vec3f& p_direction, const Vec3f& p_color, float p_power, int p_nbShadowRay)
			: _position(p_position), _direction(p_direction), _color(p_color), _power(p_power), _nbShadowRay(p_nbShadowRay*p_nbShadowRay) {}
		virtual ~Light() = default;

		inline const int& getNbShadowRay() const { return _nbShadowRay; }

		virtual LightSample sample(const Vec3f& p_point, int p_id) const = 0;

	protected:
		Vec3f _position = VEC3F_ZERO;
		Vec3f _direction = VEC3F_X;
		Vec3f _color = VEC3F_ONE;
		float _power = 1.f;
		int	  _nbShadowRay = 1;
	};

}

#endif