#ifndef __TORUS_IMPLICIT_HPP__
#define __TORUS_IMPLICIT_HPP__

#include "scene/meshes/geometry/implicit/mesh_implicit.hpp"

namespace RT_CPU
{
	class TorusImplicit : public MeshImplicit
	{
	public:
		TorusImplicit(const Vec3f p_center, const float p_scale, const float p_radius, const float p_thickness)
			: MeshImplicit(p_center,p_scale), _minRadius(p_thickness*0.5f), _maxRadius(p_radius-p_thickness*0.5f) {
			_aabb.extend(p_center+Vec3f(_maxRadius+_minRadius, _minRadius, _maxRadius+_minRadius));
			_aabb.extend(p_center-Vec3f(_maxRadius+_minRadius, _minRadius, _maxRadius+_minRadius));
		}
		~TorusImplicit() { }

	protected:
		float _evaluate(const Vec3f& p_point) const override {
			Vec3f p = p_point;

			Vec2f q = Vec2f(glm::length(Vec2f(p_point.x, p_point.z)) - _maxRadius, p_point.y);
			return glm::length(q) - _minRadius;
		}

	private:
		float _minRadius;
		float _maxRadius;
	};
}

#endif