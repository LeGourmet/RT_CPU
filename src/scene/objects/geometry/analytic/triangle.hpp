#ifndef __TRIANGLE_HPP__
#define __TRIANGLE_HPP__

#include "scene/objects/object.hpp"

namespace RT_CPU
{
	class Triangle : public Object
	{
	public:
		Triangle(Vec3f& p_v0, Vec3f& p_v1, Vec3f& p_v2, Vec3f& p_n0, Vec3f& p_n1, Vec3f& p_n2) : Object() {
			_vertices[0] = p_v0; _aabb.extend(p_v0); _normals[0] = p_n0;
			_vertices[1] = p_v1; _aabb.extend(p_v1); _normals[1] = p_n1;
			_vertices[2] = p_v2; _aabb.extend(p_v2); _normals[2] = p_n2;
		}
		
		~Triangle() {}

		bool intersect(const Ray& p_ray, const float p_tMin, const float p_tMax, HitRecord& p_hitRecord) const override {
			Vec2f uv; float t;
			if (_intersectGeometry(p_ray, uv, t) && t <= p_tMax && t >= p_tMin) {
				_hitRegister(p_hitRecord,p_ray, p_ray.pointAtT(t), glm::normalize((1.f - uv[0] - uv[1]) * _normals[0] + uv[0] * _normals[1] + uv[1] * _normals[2]), t);
				return true;
			}
			return false;
		}

		bool intersectAny(const Ray& p_ray, const float p_tMin, const float p_tMax) const override {
			Vec2f uv; float t;
			return _intersectGeometry(p_ray, uv, t) && t <= p_tMax && t >= p_tMin;
		}

	private:
		bool _intersectGeometry(const Ray& p_ray, Vec2f& p_uv, float& p_t) const {
			const Vec3f& o = p_ray.getOrigin();
			const Vec3f& dir = p_ray.getDirection();

			Vec3f x = _vertices[1] - _vertices[0];
			Vec3f y = _vertices[2] - _vertices[0];
			Vec3f p = glm::cross(dir, y);
			float det = glm::dot(x, p);

			if (det == 0.f) return false;

			float invDet = 1.f / det;

			Vec3f t = o - _vertices[0];
			float u = glm::dot(t, p) * invDet;
			if (u < 0.f || u > 1.f) return false;

			Vec3f q = glm::cross(t, x);
			float v = glm::dot(dir, q) * invDet;
			if (v < 0.f || u + v > 1.f) return false;

			p_t = glm::dot(y, q) * invDet;
			p_uv = Vec2f(u, v);

			return true;
		}

	private:
		Vec3f _vertices[3];
		Vec3f _normals[3];
	};
}

#endif