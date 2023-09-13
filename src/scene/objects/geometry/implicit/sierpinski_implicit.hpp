#ifndef __SIERPINSKI_IMPLICIT_HPP__
#define __SIERPINSKI_IMPLICIT_HPP__

#include "object_implicit.hpp"

namespace RT_CPU
{
	class SierpinskiImplicit : public ObjectImplicit
	{
	public:
		SierpinskiImplicit(const Vec3f p_center, const float p_scale)
			: ObjectImplicit(p_center), _scale(p_scale) {
			_aabb.extend(Vec3f(FLT_MAX));
			_aabb.extend(Vec3f(-FLT_MAX));
		}
		~SierpinskiImplicit() { }

		float evaluateSDF(const Vec3f& p_point) const override {
			Vec3f p = p_point-_center;
			
			Vec3f a1 = Vec3f(1, 1, 1);
			Vec3f a2 = Vec3f(-1, -1, 1);
			Vec3f a3 = Vec3f(1, -1, -1);
			Vec3f a4 = Vec3f(-1, 1, -1);
			Vec3f c;

			float d;
			int i;

			for(i=0; i<5 ;i++){
				c = a1; 
				float dist = glm::length(p - a1);
				d = glm::length(p - a2); if (d<dist) {c=a2; dist=d;}
				d = glm::length(p - a3); if (d<dist) {c=a3; dist=d;}
				d = glm::length(p - a4); if (d<dist) {c=a4; dist=d;}
				p = p*_scale - c*(_scale-1.f);
			}

			return glm::length(p) * glm::pow(_scale, float(-i));	
		}

	private:
		float _scale;
	};
}

#endif