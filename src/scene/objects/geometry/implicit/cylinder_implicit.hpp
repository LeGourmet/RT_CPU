#ifndef __CYLINDER_IMPLICIT_HPP__
#define __CYLINDER_IMPLICIT_HPP__

#include "object_implicit.hpp"

namespace RT_CPU
{
	class CylinderImplicit : public ObjectImplicit
	{
	public:
		CylinderImplicit(const Vec3f p_center, const float p_radius, const float p_h)
			: ObjectImplicit(p_center), _radius(p_radius), _h(p_h) {
			_aabb.extend(p_center-Vec3f(p_radius,p_radius,p_h*0.5f));
			_aabb.extend(p_center+Vec3f(p_radius,p_radius,p_h*0.5f));
		}
		~CylinderImplicit() { }

		float evaluateSDF(const Vec3f& p_point) const override {
			Vec3f p = Vec3f(p_point - _center);

			Vec2f tmp = Vec2f(glm::length(Vec2f(p.x, p.y)) - _radius, abs(p.z) - _h);
			return glm::min(glm::max(tmp.x, tmp.y), 0.f) + glm::length(glm::max(tmp, 0.f));
		}

	private:
		float _radius;
		float _h;
	};
}

#endif