#ifndef __MANDELBULB_IMPLICIT_HPP__
#define __MANDELBULB_IMPLICIT_HPP__

#include "object_implicit.hpp"

namespace RT_CPU
{
	class MandelbulbImplicit : public ObjectImplicit
	{
	public:
		MandelbulbImplicit(const Vec3f p_center, const float p_power)
			: ObjectImplicit(p_center), _power(p_power) {
			_aabb.extend(p_center+Vec3f(1.5f));
			_aabb.extend(p_center-Vec3f(1.5f));
		}
		~MandelbulbImplicit() { }

		float evaluateSDF(const Vec3f& p_point) const override {
			Vec3f pos = p_point - _center;
			Vec3f p = pos;

			float len;
			float fact = 1.f;

			for(int i=0; i<10 ;i++) {
				len = glm::length(p);
				if (len > 1.5f) break;

				fact = _power * glm::pow(len, _power - 1.f) * fact + 1.f;

				float theta = _power * glm::acos(p.z / len);
				float phi = _power * glm::atan(p.y, p.x);

				p = pos + glm::pow(len, _power) * Vec3f(glm::sin(theta) * glm::cos(phi), glm::sin(theta) * glm::sin(phi), glm::cos(theta));
			}

			return 0.25f * glm::log(len) * len / fact;
		}

	private:
		float _power;
	};
}

#endif