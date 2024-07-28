#ifndef __MESH_HPP__
#define __MESH_HPP__

#include "scene/meshes/material/material.hpp"
#include "ray/hit_record.hpp"
#include "accelerators/aabb.hpp"

namespace RT_CPU
{
	class Mesh
	{
	public:
		Mesh() {}
		~Mesh() {}

		inline const AABB& getAABB() const { return _aabb; }
		inline Material* getMaterial() const { return _material; }
		
		inline void setMaterial(Material* p_material) { _material = p_material; }

		virtual bool intersect(const Ray& p_ray, const float p_tMin, const float p_tMax, HitRecord& p_hitRecord) const = 0;

		virtual bool intersectAny(const Ray& p_ray, const float ptMin, const float p_tMax) const = 0;

	protected:
		// https://marti.works/posts/post-calculating-tangents-for-your-mesh/post/
		inline void _hitRegister(HitRecord& p_hitRecord, const Ray& p_ray, Vec3f p_point, Vec3f p_normal, float p_t) const {
			p_hitRecord._point = p_point;
			p_hitRecord._normal = p_normal;
			p_hitRecord.faceNormal(p_ray.getDirection());
			p_hitRecord._distance = p_t;
			p_hitRecord._mesh = this;

			float s = (p_hitRecord._normal.z >= 0.) ? 1.f : -1.f;
			float a = -1.f / (s + p_hitRecord._normal.z);
			float b = p_hitRecord._normal.x * p_hitRecord._normal.y * a;
			p_hitRecord._tangent = Vec3f(1.f + s * p_hitRecord._normal.x * p_hitRecord._normal.x * a, s * b, -s * p_hitRecord._normal.x);
			p_hitRecord._bitangent = Vec3f(b, s + p_hitRecord._normal.y * p_hitRecord._normal.y * a, -p_hitRecord._normal.y);
		}

	protected:
		AABB		_aabb;
		Vec3f		_translation	= VEC3F_ZERO;
		float		_scale			= 1.f;
		Quatf		_rotation		= QUAT_ID;
		Material*	_material		= nullptr;
	};

}

#endif