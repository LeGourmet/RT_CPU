#ifndef __SPHERE_IMPLICIT_HPP__
#define __SPHERE_IMPLICIT_HPP__

#include "scene/meshes/geometry/implicit/mesh_implicit.hpp"

namespace RT_CPU
{
	class SphereImplicit : public MeshImplicit
	{
	public:
		SphereImplicit(const Vec3f p_center, const float p_scale, const float p_radius)
			: MeshImplicit(p_center,p_scale), _radius(p_radius) {
			_aabb.extend(p_center + p_radius);
			_aabb.extend(p_center - p_radius);
		}
		~SphereImplicit() { }

	protected:
		float _evaluate(const Vec3f& p_point) const override {
			return length(p_point) - _radius;
		}

	private:
		float _radius;
	};
}

#endif