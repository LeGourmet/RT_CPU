#ifndef __DISK_LIGHT_HPP__
#define __DISK_LIGHT_HPP__

#include "surfacic_light.hpp"
#include "utils/random.hpp"
#include "glm/gtx/transform.hpp"

namespace RT_CPU
{

	class DiskLight : public SurfacicLight {
	public:
		DiskLight(const Vec3f& p_position, const Vec3f& p_direction, const Vec3f& p_color, float p_power, float p_radius, int p_nbShadowRay = 4)
			: SurfacicLight(p_position, p_direction, p_color, p_power, PIf*p_radius*p_radius, p_nbShadowRay), _radius(p_radius) {}
		~DiskLight() {}

		inline virtual LightSample sample(const Vec3f& p_point, const int p_i) const override {
			float phi = (randomFloat()+p_i) * TWO_PIf/_nbShadowRay;
			float r = _radius * glm::max(1e-5f, glm::sqrt(randomFloat()));
			Vec3f frag = r*Vec3f(glm::cos(phi), glm::sin(phi), 0.f);
			
			Vec3f rot_axis = glm::cross(_direction, -VEC3F_Z);
			if (glm::dot(rot_axis, rot_axis) != 0.f)
				frag = Vec3f(glm::rotate(glm::acos(glm::dot(_direction, -VEC3F_Z)), glm::normalize(rot_axis))*Vec4f(frag, 1.f));
			frag += _position;

			Vec3f vec = frag - p_point;
			float dist_sq = glm::dot(vec, vec);
			Vec3f dir = glm::normalize(vec);

			return LightSample(_color*_power, dir, glm::sqrt(dist_sq), dist_sq/glm::max(1e-5f,glm::abs(glm::dot(_direction,dir))*_area));
		}

		inline LightSample sample(const Vec3f& p_point) const override {
			float phi = randomFloat()* TWO_PIf;
			float r = _radius * glm::max(1e-5f, glm::sqrt(randomFloat()));
			Vec3f frag = r*Vec3f(glm::cos(phi), glm::sin(phi), 0.f);
			
			Vec3f rot_axis = glm::cross(_direction, -VEC3F_Z);
			if (glm::dot(rot_axis, rot_axis) != 0.f)
				frag = Vec3f(glm::rotate(glm::acos(glm::dot(_direction, -VEC3F_Z)), glm::normalize(rot_axis)) * Vec4f(frag, 1.f));
			frag += _position;

			Vec3f vec = frag - p_point;
			float dist_sq = glm::dot(vec, vec);
			Vec3f dir = glm::normalize(vec);

			return LightSample(_color*_power, dir, glm::sqrt(dist_sq), dist_sq/glm::max(1e-5f,glm::abs(glm::dot(_direction,dir))*_area));
		}

	private:
		float _radius;
	};

}

#endif