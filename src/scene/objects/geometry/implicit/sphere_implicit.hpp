#ifndef __SPHERE_IMPLICIT_HPP__
#define __SPHERE_IMPLICIT_HPP__

#include "object_implicit.hpp"

namespace RT_CPU
{
	class SphereImplicit : public ObjectImplicit
	{
	public:
		SphereImplicit(const Vec3f p_center, const float p_radius)
			: ObjectImplicit(p_center), _radius(p_radius) {
			_aabb.extend(p_center + p_radius);
			_aabb.extend(p_center - p_radius);
		}
		~SphereImplicit() { }

		float evaluateSDF(const Vec3f& p_point) const override {
			Vec3f p = Vec3f(p_point - _center);

			return length(p) - _radius;
		}

	private:
		float _radius;
	};
}

#endif