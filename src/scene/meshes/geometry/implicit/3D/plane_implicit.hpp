#ifndef __PLANE_IMPLICIT_HPP__
#define __PLANE_IMPLICIT_HPP__

#include "scene/meshes/geometry/implicit/mesh_implicit.hpp"

namespace RT_CPU
{
	class PlaneImplicit : public MeshImplicit
	{
	public:
		PlaneImplicit(const Vec3f p_center, const float p_scale, const Vec3f p_normal)
			: MeshImplicit(p_center,p_scale), _normal(p_normal) {
			_aabb.extend(Vec3f(-FLT_MAX));
			_aabb.extend(Vec3f(FLT_MAX));
		}
		~PlaneImplicit() { }

	protected:
		float _evaluate(const Vec3f& p_point) const override {
			return glm::dot(p_point, _normal);
		}

	private:
		Vec3f _normal;
	};
}

#endif