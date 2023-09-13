#ifndef __MENGER_IMPLICIT_HPP__
#define __MENGER_IMPLICIT_HPP__

#include "object_implicit.hpp"

namespace RT_CPU
{
	class MengerImplicit : public ObjectImplicit
	{
	public:
		MengerImplicit(const Vec3f p_center, const Vec3f p_bound)
			: ObjectImplicit(p_center), _bound(p_bound) {
			_aabb.extend(p_center + p_bound);
			_aabb.extend(p_center - p_bound);
		}
		~MengerImplicit() { }

		float evaluateSDF(const Vec3f& p_point) const override {
			Vec3f p = p_point - _center;

			Vec3f q = abs(p) - _bound;
			float d = glm::length(glm::max(q, 0.f)) + glm::min(glm::max(q.x, glm::max(q.y, q.z)), 0.f);

			float s = 1.f;
			for(int i=0; i<5 ;i++) {
				s *= 3.f;
				Vec3f r = glm::abs(1.f-3.f*abs(glm::mod(p * s, 2.f) - 1.f));
				d = glm::max(d, (glm::min(glm::max(r.x, r.y), glm::min(glm::max(r.y, r.z), glm::max(r.z, r.x))) - 1.f) / s);
			}

			return d;
		}

	private:
		Vec3f _bound;
	};
}

#endif