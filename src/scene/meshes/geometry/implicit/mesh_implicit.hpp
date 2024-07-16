#ifndef __MESH_IMPLICIT_HPP__
#define __MESH_IMPLICIT_HPP__

#include "scene/meshes/mesh.hpp"

namespace RT_CPU
{
	class MeshImplicit : public Mesh
	{
	public:
		MeshImplicit(const Vec3f p_center, const float p_scaling) : Mesh(), _center(p_center), _scaling(p_scaling) {}
		~MeshImplicit() {}
		
		bool intersect(const Ray& p_ray, const float p_tMin, const float p_tMax, HitRecord& p_hitRecord) const override {
			if (!_aabb.intersect(p_ray,p_tMin,p_tMax)) return false;
			
			float t=p_tMin;
			for(int i=0; i<_maxStep ;i++){
				Vec3f point = p_ray.pointAtT(t);
				float dist = _evaluateSDF(point);
				if (glm::abs(dist)<_epsDistance) {
					_hitRegister(p_hitRecord,p_ray,point,_evaluateNormal(point,dist),t);
					return true;
				}
				t += glm::abs(dist);
				if(t>p_tMax) break;
			}
			return false;
		}

		bool intersectAny(const Ray& p_ray, const float p_tMin, const float p_tMax) const override {
			if (!_aabb.intersect(p_ray, p_tMin, p_tMax)) return false;
			
			float t=p_tMin;
			for(int i=0; i<_maxStep ;i++){
				float absDist = glm::abs(_evaluateSDF(p_ray.pointAtT(t)));
				if (absDist <_epsDistance) return true;
				t += absDist;
				if(t>p_tMax) break;
			}
			return false;
		}

	protected:
		virtual float _evaluate(const Vec3f& p_point) const = 0;
		
		float _evaluateSDF(const Vec3f& p_point) const { return (glm::abs(_scaling) == 0.f) ? 1e10f : _evaluate((p_point - _center) / _scaling)* _scaling; }

		Vec3f _evaluateNormal(const Vec3f& p_point, const float p_dist) const {
			float x = _evaluateSDF(p_point + Vec3f(_epsNormal, 0.f, 0.f));
			float y = _evaluateSDF(p_point + Vec3f(0.f, _epsNormal, 0.f));
			float z = _evaluateSDF(p_point + Vec3f(0.f, 0.f, _epsNormal));
			return glm::normalize(Vec3f(x, y, z) - p_dist);
		}

		Vec3f _center;
		float _scaling;
		// Quatf _rotation; 

		const float _epsNormal		= 1e-3f; // 1e-2f
		const float _epsDistance	= 1e-4f; // 1e-3f
		const int	_maxStep		= 10000; // 1000
	};

}

#endif