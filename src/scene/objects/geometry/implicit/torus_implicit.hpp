#ifndef __TORUS_IMPLICIT_HPP__
#define __TORUS_IMPLICIT_HPP__

#include "object_implicit.hpp"

namespace RT_CPU
{
	class TorusImplicit : public ObjectImplicit
	{
	public:
		TorusImplicit(const Vec3f p_center, const float p_radius, const float p_thickness)
			: ObjectImplicit(p_center), _minRadius(p_thickness*0.5f), _maxRadius(p_radius-p_thickness*0.5f) {
			_aabb.extend(p_center+Vec3f(_maxRadius+_minRadius, _minRadius, _maxRadius+_minRadius));
			_aabb.extend(p_center-Vec3f(_maxRadius+_minRadius, _minRadius, _maxRadius+_minRadius));
		}
		~TorusImplicit() { }

		float evaluateSDF(const Vec3f& p_point) const override {
			Vec3f p = Vec3f(p_point - _center);

			Vec2f q = Vec2f(glm::length(Vec2f(p.x, p.z)) - _maxRadius, p.y);
			return glm::length(q) - _minRadius;
		}

	private:
		float _minRadius;
		float _maxRadius;
	};
}

#endif