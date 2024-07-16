#ifndef __MANDELBULB_IMPLICIT_HPP__
#define __MANDELBULB_IMPLICIT_HPP__

#include "scene/meshes/geometry/implicit/mesh_implicit.hpp"

namespace RT_CPU
{
	class MandelbulbImplicit : public MeshImplicit
	{
	public:
		// p_power should be greater or equal two 
		MandelbulbImplicit(const Vec3f p_center, const float p_scale, const float p_power)
			: MeshImplicit(p_center, p_scale*0.33f), _power(p_power) {
			_aabb.extend(p_center+Vec3f(0.5f)*p_scale);
			_aabb.extend(p_center-Vec3f(0.5f)*p_scale);
		}
		~MandelbulbImplicit() { }

	protected:
		float _evaluate(const Vec3f& p_point) const override {
			Vec3f p = p_point;

			float len;
			float fact = 1.f;

			for(int i=0; i<10 ;i++) {
				len = glm::length(p);
				if (len > 1.5f) break;

				fact = _power * glm::pow(len, _power - 1.f) * fact + 1.f;

				float theta = _power * glm::acos(p.z / len);
				float phi = _power * glm::atan(p.y, p.x);

				p = p_point + glm::pow(len, _power) * Vec3f(glm::sin(theta) * glm::cos(phi), glm::sin(theta) * glm::sin(phi), glm::cos(theta));
			}

			return 0.25f * glm::log(len) * len / fact;
		}

	private:
		float _power;
	};
}

#endif