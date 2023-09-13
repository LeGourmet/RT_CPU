#ifndef __SPHERE_HPP__
#define __SPHERE_HPP__

#include "scene/objects/object.hpp"

namespace RT_CPU
{
	class Sphere : public Object
	{
	public:
		Sphere(Vec3f p_center, const float p_radius) : Object(), _center(p_center), _radius(p_radius) {
			_aabb.extend(p_center-Vec3f(p_radius));
			_aabb.extend(p_center+Vec3f(p_radius));
		}

		~Sphere() {}

		bool intersect(const Ray& p_ray, const float p_tMin, const float p_tMax, HitRecord& p_hitRecord) const override {
			float t1, t2;
			if (_intersectGeometry(p_ray, t1, t2)) {
				if (t1 > p_tMax) return false;						// first intersection too far
				if (t1 < p_tMin) t1 = t2;							// first intersection too near, check second one
				if (t1 < p_tMin || t1 > p_tMax) return false;		// not in range

				Vec3f point = p_ray.pointAtT(t1);
				_hitRegister(p_hitRecord,p_ray, point, glm::normalize(point-_center), t1);
				return true;
			}
			return false;
		}

		bool intersectAny(const Ray& p_ray, const float p_tMin, const float p_tMax) const override {
			float t1, t2;
			return _intersectGeometry(p_ray, t1, t2) && ((t1 >= p_tMin && t1 <= p_tMax) || (t2 >= p_tMin && t2 <= p_tMax));
		}

	private:
		bool _intersectGeometry(const Ray& p_ray, float& p_t1, float& p_t2) const {
			p_t1 = -1.f;
			p_t2 = -1.f;

			Vec3f centerToCam = p_ray.getOrigin() - _center;

			float b = 2.f * glm::dot(p_ray.getDirection(), centerToCam);
			float c = glm::dot(centerToCam, centerToCam) - (_radius*_radius);
			float delta = b * b - 4.f * c;

			if (delta < 0.f) { return false; }
			if (delta == 0.f) { p_t1 = -0.5f * b; }
			else {
				float tmp = glm::sqrt(delta);
				p_t1 = -0.5f * (b + tmp);
				p_t2 = 0.5f * (tmp - b);
			}

			return true;
		}

	private:
		Vec3f _center;
		float _radius;
	};

}

#endif