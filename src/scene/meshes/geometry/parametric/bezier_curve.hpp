#ifndef __BEZIER_CURVE_HPP__
#define __BEZIER_CURVE_HPP__

#include "scene/meshes/mesh.hpp"

namespace RT_CPU
{
	class BezierCurve : public Mesh
	{
	public:
		BezierCurve(const Vec3f p_v1, const Vec3f p_control, const Vec3f p_v2 ) : Mesh(), _v1(p_v1), _control(p_control), _v2(p_v2) {
			_aabb.extend(Vec3f(-FLT_MAX));
			_aabb.extend(Vec3f(FLT_MAX));
		}

		~BezierCurve() {}

		bool intersect(const Ray& p_ray, const float p_tMin, const float p_tMax, HitRecord& p_hitRecord) const override {
			return false;
		}

		bool intersectAny(const Ray& p_ray, const float p_tMin, const float p_tMax) const override {
			return false;
		}

	private:
		bool _intersectGeometry(const Ray& p_ray, float& p_t) const {
			return false;
		}

	private:
		Vec3f _v1;
		Vec3f _control;
		Vec3f _v2;
	};

}

#endif