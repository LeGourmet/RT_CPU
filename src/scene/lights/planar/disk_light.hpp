#ifndef __DISK_LIGHT_HPP__
#define __DISK_LIGHT_HPP__

#include "planar_light.hpp"
#include "utils/random.hpp"
#include "glm/gtx/transform.hpp"

namespace RT_CPU
{

	class DiskLight : public PlanarLight {
	public:
		DiskLight(const Vec3f& p_position, const Vec3f& p_direction, const Vec3f& p_color, float p_power, float p_radius, int p_nbShadowRay = 4)
			: PlanarLight(p_position, p_direction, p_color, p_power, PIf*p_radius*p_radius, p_nbShadowRay), _radius(p_radius) {}
		~DiskLight() {}

		inline virtual LightSample sample(const Vec3f& p_point, const int p_i) const override {
			float phi = (randomFloat()+p_i) * TWO_PIf/_nbShadowRay;
			float r = _radius * glm::max(1e-5f, glm::sqrt(randomFloat()));
			Vec2f frag = r*Vec2f(glm::cos(phi), glm::sin(phi));
			return _evaluate(p_point, frag);
		}

		inline LightSample sample(const Vec3f& p_point) const override {
			float phi = randomFloat()* TWO_PIf;
			float r = _radius * glm::max(1e-5f, glm::sqrt(randomFloat()));
			Vec2f frag = r*Vec2f(glm::cos(phi), glm::sin(phi));
			return _evaluate(p_point, frag);
		}

	private:
		float _radius;
	};

}

#endif