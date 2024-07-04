#ifndef __SPHERE_LIGHT_HPP__
#define __SPHERE_LIGHT_HPP__

#include "volumic_light.hpp"
#include "utils/random.hpp"

namespace RT_CPU
{

	class SphereLight : public VolumicLight
	{
	  public:
		SphereLight( const Vec3f & p_position, const Vec3f& p_direction, const Vec3f& p_color, float p_power, float p_radius, int p_nbShadowRay = 4)
			: VolumicLight( p_position, p_direction, p_color, p_power, 4.f*PIf*p_radius*p_radius*p_radius/3.f, p_nbShadowRay ), _radius( p_radius ) {}
		~SphereLight() {}

		inline virtual LightSample sample(const Vec3f& p_point, int p_id) const override {
			float phi = randomFloat() * TWO_PIf;
			float theta = randomFloat() * TWO_PIf;
			float r = _radius * glm::max(1e-5f, glm::sqrt(randomFloat()));
			Vec3f frag = r * Vec3f(glm::sin(theta)*glm::cos(phi), glm::sin(theta)*glm::sin(phi), glm::cos(theta)) + _position;
			
			// todo sample stratified !

			Vec3f vec  = frag-p_point;
			float dist_sq = glm::dot(vec,vec);
			Vec3f dir  = glm::normalize(vec);

			float pdf = dist_sq/(glm::abs(glm::dot(glm::normalize(frag-_position),dir))*_volume);

			return LightSample(_color*_power, dir, glm::sqrt(dist_sq), pdf);
		}

		inline LightSample sample(const Vec3f& p_point) const override {
			float phi = randomFloat() * TWO_PIf;
			float theta = randomFloat() * TWO_PIf;
			float r = _radius * glm::max(1e-5f, glm::sqrt(randomFloat()));
			Vec3f frag = r * Vec3f(glm::sin(theta) * glm::cos(phi), glm::sin(theta) * glm::sin(phi), glm::cos(theta)) + _position;

			Vec3f vec = frag-p_point;
			float dist_sq = glm::dot(vec,vec);
			Vec3f dir = glm::normalize(vec);
			
			float pdf = dist_sq;
			float intensity = glm::abs(glm::dot(glm::normalize(frag - _position), dir)) * _volume;
			
			return LightSample(_color*_power*intensity, dir, glm::sqrt(dist_sq), pdf);
		}

	  private:
		//float _cosInnerAngle;
		//float _cosOuterAngle;
		float _radius;
	};

}

#endif