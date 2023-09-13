#ifndef __APPOLONIUS_IMPLICIT_HPP__
#define __APPOLONIUS_IMPLICIT_HPP__

#include "object_implicit.hpp"

namespace RT_CPU
{
	class AppoloniusImplicit : public ObjectImplicit
	{
	public:
		AppoloniusImplicit(const Vec3f p_center)
			: ObjectImplicit(p_center) {
			_aabb.extend(Vec3f(FLT_MAX));
			_aabb.extend(Vec3f(-FLT_MAX));
		}
		~AppoloniusImplicit() { }

		float evaluateSDF(const Vec3f& p_point) const override {
			Vec3f p = (p_point - _center);

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