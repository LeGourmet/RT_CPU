#ifndef __OBJECT_HPP__
#define __OBJECT_HPP__

#include "material.hpp"
#include "ray/hit_record.hpp"
#include "accelerators/aabb.hpp"

namespace RT_CPU
{
	class Object
	{
	public:
		Object() {}
		~Object() {}

		inline const AABB& getAABB() const { return _aabb; }
		inline Material* getMaterial() const { return _material; }
		
		inline void setMaterial(Material* p_material) { _material = p_material; }

		virtual bool intersect(const Ray& p_ray, const float p_tMin, const float p_tMax, HitRecord& p_hitRecord) const = 0;

		virtual bool intersectAny(const Ray& p_ray, const float ptMin, const float p_tMax) const = 0;

	protected:
		inline void _hitRegister(HitRecord& p_hitRecord, const Ray& p_ray, Vec3f p_point, Vec3f p_normal, float p_t) const {
			p_hitRecord._point = p_point;
			p_hitRecord._normal = p_normal;
			p_hitRecord.faceNormal(p_ray.getDirection());
			p_hitRecord._distance = p_t;
			p_hitRecord._object = this;
		}

	protected:
		AABB		_aabb;
		Material*	_material = nullptr;
	};

}

#endif