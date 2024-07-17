#ifndef __DIRECTIONAL_LIGHT_HPP__
#define __DIRECTIONAL_LIGHT_HPP__

#include "basic_light.hpp"

namespace RT_CPU
{
	// blender export or gltf need multiply by 1.f/683.f

	class DirectionalLight : public BasicLight
	{
	  public:
		DirectionalLight(const Vec3f& p_position, const Vec3f& p_direction, const Vec3f& p_color, float p_power)
			: BasicLight( p_position, p_direction, p_color, p_power ){}
		~DirectionalLight() {}

		LightSample sample(const Vec3f& p_point) const override {
			float phi = (_direction.x==0.f) ? PI_2f : glm::atan(_direction.y/_direction.x);
			float theta = glm::acos(-_direction.z);
			

			phi += rand()* _jittering;
			theta += rand()* _jittering;

			Vec3f dir = Vec3f(glm::sin(theta) * glm::cos(phi), glm::sin(theta) * glm::sin(phi), glm::cos(theta));

			return LightSample(_color*_power, dir, FLT_MAX, 1.f);
		}

	  private:
		float _jittering = 0.f * 0.00001f;
	};
}

#endif