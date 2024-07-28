#ifndef __PLANAR_LIGHT_HPP__
#define __PLANAR_LIGHT_HPP__

#include "scene/lights/light.hpp"

namespace RT_CPU
{

	class PlanarLight : public Light
	{
	public:
		PlanarLight(const Vec3f& p_position, const Vec3f& p_direction, const Vec3f& p_color, float p_power, float p_area, int p_nbShadowRay)
			: Light(p_position, p_direction, p_color, p_power, p_nbShadowRay*p_nbShadowRay), _area(p_area) {}
		virtual ~PlanarLight() = default;

		virtual LightSample sample(const Vec3f& p_point, int p_id) const = 0;
		virtual LightSample sample(const Vec3f& p_point) const = 0;

	protected:
		LightSample _evaluate(const Vec3f& p_point, const Vec2f& p_frag) const {
			Vec3f frag = Vec3f(p_frag,0.f);

			Vec3f rot_axis = glm::cross(_direction, -VEC3F_Z);
			if (glm::dot(rot_axis, rot_axis) != 0.f)
				frag = Vec3f(glm::rotate(glm::acos(glm::dot(_direction, -VEC3F_Z)), glm::normalize(rot_axis)) * Vec4f(frag, 1.f));
			frag += _position;

			Vec3f vec = frag - p_point;
			float dist_sq = glm::dot(vec, vec);
			Vec3f dir = glm::normalize(vec);

			return LightSample(_color * _power, dir, glm::sqrt(dist_sq), dist_sq / glm::max(1e-5f, glm::abs(glm::dot(_direction, dir)) * _area));
		}

	protected:
		float _area = 1.f;
	};

}

#endif