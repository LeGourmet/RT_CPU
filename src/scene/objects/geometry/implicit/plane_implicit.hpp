#ifndef __PLANE_IMPLICIT_HPP__
#define __PLANE_IMPLICIT_HPP__

#include "object_implicit.hpp"

namespace RT_CPU
{
	class PlaneImplicit : public ObjectImplicit
	{
	public:
		PlaneImplicit(const Vec3f p_center, const Vec3f p_normal)
			: ObjectImplicit(p_center), _normal(p_normal) {
			_aabb.extend(Vec3f(-FLT_MAX));
			_aabb.extend(Vec3f(FLT_MAX));
		}
		~PlaneImplicit() { }

		float evaluateSDF(const Vec3f& p_point) const override {
			Vec3f p = Vec3f(p_point - _center);

			return glm::dot(p, _normal);
		}

	private:
		Vec3f _normal;
	};
}

#endif