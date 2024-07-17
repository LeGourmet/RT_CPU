#ifndef __QUAD_LIGHT_HPP__
#define __QUAD_LIGHT_HPP__

#include "surfacic_light.hpp"
#include "utils/random.hpp"

namespace RT_CPU
{

	/*
	float phi = (_direction.x==0.f) ? PI_2f : glm::atan(_direction.y/_direction.x);
	float theta = glm::acos(_direction.z);
			
	Vec3f dir = Vec3f(glm::sin(theta) * glm::cos(phi), glm::sin(theta) * glm::sin(phi), glm::cos(theta));
	*/

	class QuadLight : public SurfacicLight{
	  public:
		// len(cross(a,b)) = len(a) * len(b) * sin(a^b)   ==>   area = L * H * sin(u^v)
		QuadLight( const Vec3f& p_position, const Vec3f& p_direction, const Vec3f& p_color, float p_power, const Vec3f& p_u, const Vec3f& p_v, int p_nbShadowRay = 4)
			: SurfacicLight( p_position, p_direction, p_color, p_power, glm::length(glm::cross(p_u, p_v)), p_nbShadowRay ), _u( p_u ), _v( p_v ), _nbRow(p_nbShadowRay) {}
		~QuadLight() {}

		inline virtual LightSample sample(const Vec3f& p_point, const int p_i) const override {
			Vec3f frag = (((p_i/_nbRow)+randomFloat())/float(_nbRow)-0.5f)*_u +
						 (((p_i%_nbRow)+randomFloat())/float(_nbRow)-0.5f)*_v + 
						 _position;

			// todo rotate frag with direction

			Vec3f vec = frag-p_point;
			float dist_sq = glm::dot(vec,vec);
			Vec3f dir = glm::normalize(vec);

			float pdf = dist_sq;
			float intensity = glm::abs(glm::dot(_direction, dir)) * _area;

			return LightSample(_color * _power * intensity, dir, glm::sqrt(dist_sq), pdf);
		}

		inline LightSample sample(const Vec3f& p_point) const override {
			Vec3f frag = (randomFloat()-0.5f)*_u + (randomFloat()-0.5f)*_v + _position;
			
			// todo rotate frag with direction

			Vec3f vec = frag-p_point;
			float dist_sq = glm::dot(vec,vec);
			Vec3f dir = glm::normalize(vec);

			float pdf = dist_sq;
			float intensity = glm::abs(glm::dot(_direction, dir)) * _area;

			return LightSample(_color*_power*intensity, dir, glm::sqrt(dist_sq), pdf);
		}

	  private:
		int _nbRow;
		Vec3f _u;
		Vec3f _v;
	};

}

#endif