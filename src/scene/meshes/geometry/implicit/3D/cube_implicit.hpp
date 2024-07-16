#ifndef __CUBE_IMPLICIT_HPP__
#define __CUBE_IMPLICIT_HPP__

#include "scene/meshes/geometry/implicit/mesh_implicit.hpp"

namespace RT_CPU
{
	class CubeImplicit : public MeshImplicit
	{
	public:
		CubeImplicit(const Vec3f p_center, const float p_scale, const Vec3f p_bounds)
			: MeshImplicit(p_center, p_scale), _bounds(p_bounds) {
			_aabb.extend(p_center+p_bounds);
			_aabb.extend(p_center-p_bounds);
		}
		~CubeImplicit() { }

	protected:
		float _evaluate(const Vec3f& p_point) const override {
			Vec3f q = abs(p_point) - _bounds;
			return glm::length(glm::max(q, 0.f)) + glm::min(glm::max(q.x, glm::max(q.y, q.z)), 0.f);
		}

	private:
		Vec3f _bounds;
	};
}

#endif