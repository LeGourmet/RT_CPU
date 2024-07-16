#ifndef __APOLLONIUS_IMPLICIT_HPP__
#define __APOLLONIUS_IMPLICIT_HPP__

#include "scene/meshes/geometry/implicit/mesh_implicit.hpp"

namespace RT_CPU
{
	class ApolloniusImplicit : public MeshImplicit
	{
	public:
		ApolloniusImplicit(const Vec3f p_center, const float p_scaling)
			: MeshImplicit(p_center, p_scaling) {
			_aabb.extend(Vec3f(FLT_MAX));
			_aabb.extend(Vec3f(-FLT_MAX));
		}
		~ApolloniusImplicit() { }

	protected:
		float _evaluate(const Vec3f& p_point) const override {
			Vec3f p = p_point;

			float s = 3.f;
			for(int i=0; i<10 ;i++) {
				p = glm::mod(p-1.f,2.f)-1.f;
				float e = 1.4f / glm::dot(p, p);
				s *= e;
				p *= e;
			}
			return glm::length(Vec2f(p.y,p.z))/s;
		}
	};
}

#endif