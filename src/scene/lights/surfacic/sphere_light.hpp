#ifndef __SPHERE_LIGHT_HPP__
#define __SPHERE_LIGHT_HPP__

#include "surfacic_light.hpp"
#include "utils/random.hpp"

namespace RT_CPU
{

	class SphereLight : public SurfacicLight
	{
	  public:
		SphereLight( const Vec3f & p_position, const Vec3f& p_direction, const Vec3f& p_color, float p_radius, float p_power = 1.f, int p_nbShadowRay = 4)
			: SurfacicLight( p_position, p_direction, p_color, 4.f*PIf*_radius*_radius, p_power, p_nbShadowRay ), _radius( p_radius ) {}
		~SphereLight() {}

		inline virtual LightSample sample(const Vec3f& p_point, int p_id) const override {
			// sample inside
			const float phi   = randomFloat() * TWO_PIf;
			const float theta = randomFloat() * TWO_PIf - PIf;
			const float r	  = _radius * glm::max(1e-3f,sqrt( randomFloat() ));
			Vec3f frag = r * Vec3f(glm::sin(theta)*glm::cos(phi), glm::sin(theta)*glm::sin(phi), glm::cos(theta)) + _position;

			// sample outside
			/*const float phi = randomFloat() * TWO_PIf;
			const float z = randomFloat() * (2.f * _radius) - _radius;
			const float x = glm::sqrt(_radius * _radius - z * z) * glm::cos(phi);
			const float y = glm::sqrt(_radius * _radius - z * z) * glm::sin(phi);
			Vec3f frag = Vec3f(x, y, z) + _position;*/

			// todo sample stratified !

			Vec3f vec  = frag - p_point;
			Vec3f norm = glm::normalize( frag - _position );
			float dist_sq = glm::dot( vec, vec );
			Vec3f dir  = glm::normalize( vec );

			float pdf = dist_sq / ( glm::abs(glm::dot( norm, dir )) * _area );

			return LightSample( (_color*_power)/pdf, dir, glm::sqrt(dist_sq) );
		}

	  private:
		float _radius;
	};

}

#endif