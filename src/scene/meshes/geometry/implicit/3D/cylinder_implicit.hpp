#ifndef __CYLINDER_IMPLICIT_HPP__
#define __CYLINDER_IMPLICIT_HPP__

#include "scene/meshes/geometry/implicit/mesh_implicit.hpp"

namespace RT_CPU
{
	class CylinderImplicit : public MeshImplicit
	{
	public:
		CylinderImplicit(const Vec3f p_center, const float p_scale, const float p_radius, const float p_h)
			: MeshImplicit(p_center,p_scale), _radius(p_radius), _h(p_h) {
			_aabb.extend(p_center-Vec3f(p_radius,p_radius,p_h*0.5f));
			_aabb.extend(p_center+Vec3f(p_radius,p_radius,p_h*0.5f));
		}
		~CylinderImplicit() { }

	protected:
		float _evaluate(const Vec3f& p_point) const override {
			Vec2f tmp = Vec2f(glm::length(Vec2f(p_point.x, p_point.y)) - _radius, abs(p_point.z) - _h);
			return glm::min(glm::max(tmp.x, tmp.y), 0.f) + glm::length(glm::max(tmp, 0.f));
		}

	private:
		float _radius;
		float _h;
	};
}

#endif