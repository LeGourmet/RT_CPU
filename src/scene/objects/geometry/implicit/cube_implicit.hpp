#ifndef __CUBE_IMPLICIT_HPP__
#define __CUBE_IMPLICIT_HPP__

#include "object_implicit.hpp"

namespace RT_CPU
{
	class CubeImplicit : public ObjectImplicit
	{
	public:
		CubeImplicit(const Vec3f p_center, const Vec3f p_bound)
			: ObjectImplicit(p_center), _bound(p_bound) {
			_aabb.extend(p_center+p_bound);
			_aabb.extend(p_center-p_bound);
		}
		~CubeImplicit() { }

		float evaluateSDF(const Vec3f& p_point) const override {
			Vec3f p = Vec3f(p_point - _center);

			Vec3f q = abs(p) - _bound;
			return glm::length(glm::max(q, 0.f)) + glm::min(glm::max(q.x, glm::max(q.y, q.z)), 0.f);
		}

	private:
		Vec3f _bound;
	};
}

#endif