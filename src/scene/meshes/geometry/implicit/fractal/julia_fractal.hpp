#ifndef __JULIA_IMPLICIT_HPP__
#define __JULIA_IMPLICIT_HPP__

#include "scene/meshes/geometry/implicit/mesh_implicit.hpp"

namespace RT_CPU
{
	class JuliaImplicit : public MeshImplicit
	{
	public:
		JuliaImplicit(const Vec3f p_center, const float p_scale, const Vec4f p_c)
			: MeshImplicit(p_center,p_scale), _c(p_c) {
			//_aabb.extend(p_center+Vec3f(1.5f));
			//_aabb.extend(p_center-Vec3f(1.5f));
			_aabb.extend(Vec3f(FLT_MAX));
			_aabb.extend(Vec3f(-FLT_MAX));
		}
		~JuliaImplicit() { }

	protected:
		float _evaluate(const Vec3f& p_point) const override {
			Vec4f z = Vec4f(p_point, 0.f);

			float len;
			float fact = 1.f;

			for (int i=0; i<20 ;i++) {
				len = glm::length(z);
				if (len > 2.f) break;

				fact *= 2.f * len;
				z = Vec4f(z.x * z.x - z.y * z.y - z.z * z.z - z.w * z.w,
					2.f * z.x * z.y,
					2.f * z.x * z.z,
					2.f * z.x * z.w) + _c;
			}

			return 0.5f * glm::log(len) * len / fact;

		}

	private:
		Vec4f _c;
	};
}

#endif