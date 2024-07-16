#ifndef __MENGER_IMPLICIT_HPP__
#define __MENGER_IMPLICIT_HPP__

#include "scene/meshes/geometry/implicit/mesh_implicit.hpp"

namespace RT_CPU
{
	class MengerImplicit : public MeshImplicit
	{
	public:
		MengerImplicit(const Vec3f p_center, const float p_scale)
			: MeshImplicit(p_center, p_scale*0.5f) {
			_aabb.extend(p_center+Vec3f(0.5f)*p_scale);
			_aabb.extend(p_center-Vec3f(0.5f)*p_scale);
		}
		~MengerImplicit() { }

	protected:
		float _evaluate(const Vec3f& p_point) const override {
			Vec3f p = p_point;

			Vec3f q = abs(p) - Vec3f(1.f);
			float d = glm::length(glm::max(q, 0.f)) + glm::min(glm::max(q.x, glm::max(q.y, q.z)), 0.f);

			float s = 1.f;
			for (int i=0; i<6 ;i++) {
				Vec3f r = glm::abs(1.f-3.f*abs(glm::mod(p * s, 2.f) - 1.f));
				s *= 3.f;
				d = glm::max(d, (glm::min(glm::max(r.x, r.y), glm::min(glm::max(r.y, r.z), glm::max(r.z, r.x))) - 1.f) / s); // 2*s
			}

			return d;
		}
	};
}

#endif