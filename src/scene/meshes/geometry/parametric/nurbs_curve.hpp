#ifndef __NURBS_CURVE_HPP__
#define __NURBS_CURVE_HPP__

#include "scene/meshes/mesh.hpp"

namespace RT_CPU
{
	class NurbsCurve : public Mesh
	{
	public:
		NurbsCurve(std::vector<Vec3f>& p_vertices) : Mesh() {
			_vertices = p_vertices;
			_aabb.extend(Vec3f(-FLT_MAX));
			_aabb.extend(Vec3f(FLT_MAX));
		}

		~NurbsCurve() {}

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
		std::vector<Vec3f>	_vertices;
	};

}

#endif