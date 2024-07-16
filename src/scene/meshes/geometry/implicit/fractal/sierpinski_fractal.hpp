#ifndef __SIERPINSKI_IMPLICIT_HPP__
#define __SIERPINSKI_IMPLICIT_HPP__

#include "scene/meshes/geometry/implicit/mesh_implicit.hpp"

namespace RT_CPU
{
	class SierpinskiImplicit : public MeshImplicit
	{
	public:
		SierpinskiImplicit(const Vec3f p_center, const float p_scale)
			: MeshImplicit(p_center, p_scale*0.434f) {
			_aabb.extend(p_center+Vec3f(0.5f)*p_scale);
			_aabb.extend(p_center-Vec3f(0.5f)*p_scale);
		}
		~SierpinskiImplicit() { }

	protected:
		float _evaluate(const Vec3f& p_point) const override {
			Vec3f p = p_point;
			
			Vec3f va = Vec3f(0.f, 0.57735f, 0.f);
			Vec3f vb = Vec3f(0.f, -1.f, 1.15470f);
			Vec3f vc = Vec3f(1.f, -1.f, -0.57735f);
			Vec3f vd = Vec3f(-1.f, -1.f, -0.57735f);

			float t, d, dm;
			float r = 1.f;
			Vec3f v;

			for (int i=0; i<10; i++) {
				d = glm::dot(p-va, p-va);		   { v=va; dm=d; t=0.f; }
				d = glm::dot(p-vb, p-vb); if(d<dm) { v=vb; dm=d; t=1.f; }
				d = glm::dot(p-vc, p-vc); if(d<dm) { v=vc; dm=d; t=2.f; }
				d = glm::dot(p-vd, p-vd); if(d<dm) { v=vd; dm=d; t=3.f; }
				p = v + 2.f*(p-v); 
				r *= 2.f;
			}

			return (glm::sqrt(dm)-1.f)/r;
		}
	};
}

#endif