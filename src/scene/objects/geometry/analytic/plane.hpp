#ifndef __PLANE_HPP__
#define __PLANE_HPP__

#include "scene/objects/object.hpp"

namespace RT_CPU
{
	class Plane : public Object
	{
	public:
		Plane(Vec3f p_origine, const Vec3f p_normal) : Object(), _origine(p_origine), _normal(p_normal) {
			_aabb.extend(Vec3f(-FLT_MAX));
			_aabb.extend(Vec3f(FLT_MAX));
		}
		
		~Plane() {}

		bool intersect(const Ray& p_ray, const float p_tMin, const float p_tMax, HitRecord& p_hitRecord) const override {
			float t;
			if (_intersectGeometry(p_ray, t) && t<=p_tMax && t>=p_tMin) {
				_hitRegister(p_hitRecord,p_ray,p_ray.pointAtT(t),_normal,t);
				return true;
			}
			return false;
		}

		bool intersectAny(const Ray& p_ray, const float p_tMin, const float p_tMax) const override {
			float t;
			return _intersectGeometry(p_ray, t) && t <= p_tMax && t >= p_tMin;
		}
		
	private:
		bool _intersectGeometry(const Ray& p_ray, float& p_t) const {
			float d = glm::dot(p_ray.getDirection(), _normal);
			if (glm::abs(d) > 0.f) {
				p_t = dot(_origine - p_ray.getOrigin(), _normal) / d;
				return p_t >= 0.f;
			}
			return false;
		}

	private:
		Vec3f _origine;
		Vec3f _normal;
	};

}

#endif