#ifndef __OBJECT_IMPLICIT_HPP__
#define __OBJECT_IMPLICIT_HPP__

#include "scene/objects/object.hpp"

namespace RT_CPU
{
	class ObjectImplicit : public Object
	{
	public:
		ObjectImplicit(const Vec3f p_center) : Object(), _center(p_center) {}
		~ObjectImplicit() {}
		
		bool intersect(const Ray& p_ray, const float p_tMin, const float p_tMax, HitRecord& p_hitRecord) const override {
			if (!_aabb.intersect(p_ray,p_tMin,p_tMax)) return false;
			
			float t=p_tMin;
			for(int i=0; i<_maxStep ;i++){
				Vec3f point = p_ray.pointAtT(t);
				float dist = evaluateSDF(point);
				if (dist<_minDistance) {
					_hitRegister(p_hitRecord,p_ray,point,evaluateNormal(point,dist),t);
					return true;
				}
				t += dist;
				if(t>p_tMax) break;
			}
			return false;
		}

		bool intersectAny(const Ray& p_ray, const float p_tMin, const float p_tMax) const override {
			if (!_aabb.intersect(p_ray, p_tMin, p_tMax)) return false;
			
			float t=p_tMin;
			for(int i=0; i<_maxStep ;i++){
				float dist = evaluateSDF(p_ray.pointAtT(t));
				if (dist<_minDistance) return true;
				t += dist;
				if(t>p_tMax) break;
			}
			return false;
		}

		virtual float evaluateSDF(const Vec3f& p_point) const = 0;

		Vec3f evaluateNormal(const Vec3f& p_point, const float p_dist) const {
			float eps = 0.001f;
			float x = evaluateSDF(p_point + Vec3f(eps, 0.f, 0.f));
			float y = evaluateSDF(p_point + Vec3f(0.f, eps, 0.f));
			float z = evaluateSDF(p_point + Vec3f(0.f, 0.f, eps));
			return glm::normalize(Vec3f(x, y, z) - p_dist);
		}

	protected:
		Vec3f _center;

		const float _minDistance = 1e-4f;
		const int	_maxStep = 10000;
	};

}

#endif