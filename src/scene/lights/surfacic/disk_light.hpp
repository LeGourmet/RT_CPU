#ifndef __DISK_LIGHT_HPP__
#define __DISK_LIGHT_HPP__

#include "surfacic_light.hpp"
#include "utils/random.hpp"

namespace RT_CPU
{

	class DiskLight : public SurfacicLight {
	public:
		DiskLight(const Vec3f& p_position, const Vec3f& p_direction, const Vec3f& p_color, float p_power, float p_radius, int p_nbShadowRay = 4)
			: SurfacicLight(p_position, p_direction, p_color, p_power, PIf*p_radius*p_radius, p_nbShadowRay), _radius(p_radius) {}
		~DiskLight() {}

		inline virtual LightSample sample(const Vec3f& p_point, const int p_i) const override {
			// todo stratified sampling
			
			//float phi = randomFloat() * TWO_PIf; // twoPI/sqrt(nbShadowRay)
			float phi = (randomFloat()+p_i) * TWO_PIf/_nbShadowRay;
			float r = _radius * glm::max(1e-5f, glm::sqrt(randomFloat()));

			Vec3f frag = r*Vec3f(glm::cos(phi), glm::sin(phi), 0.f)+_position;
			
			// todo rotate with direction

			Vec3f vec = frag - p_point;
			float dist_sq = glm::dot(vec, vec);
			Vec3f dir = glm::normalize(vec);

			float pdf = dist_sq/(glm::abs(glm::dot(_direction,dir))*_area);

			return LightSample(_color*_power, dir, glm::sqrt(dist_sq), pdf);
		}

		inline LightSample sample(const Vec3f& p_point) const override {
			float phi = randomFloat()* TWO_PIf;
			float r = _radius * glm::max(1e-5f, glm::sqrt(randomFloat()));
			
			Vec3f frag = r*Vec3f(glm::cos(phi), glm::sin(phi), 0.f)+_position;
			
			// todo rotate with direction

			Vec3f vec = frag - p_point;
			float dist_sq = glm::dot(vec, vec);
			Vec3f dir = glm::normalize(vec);

			float pdf = dist_sq/(glm::abs(glm::dot(_direction,dir))*_area);

			return LightSample(_color*_power, dir, glm::sqrt(dist_sq), pdf);
		}

	private:
		float _radius;
	};

}

#endif