#ifndef __DIRECTIONAL_LIGHT_HPP__
#define __DIRECTIONAL_LIGHT_HPP__

#include "basic_light.hpp"
#include "glm/gtx/transform.hpp"

namespace RT_CPU
{
	// blender export or gltf need multiply by 1.f/683.f
	// NEE => eclipse wtf ??

	class DirectionalLight : public BasicLight
	{
	  public:
		DirectionalLight(const Vec3f& p_position, const Vec3f& p_direction, const Vec3f& p_color, float p_power, float p_angle)
			: BasicLight( p_position, p_direction, p_color, p_power ), _angle(p_angle) {}
		~DirectionalLight() {}

		LightSample sample(const Vec3f& p_point) const override {
			Vec3f rot_axis = glm::cross(VEC3F_X,_direction);
			if(glm::dot(rot_axis, rot_axis)<0.01f) rot_axis = glm::cross(VEC3F_Y,_direction);
			rot_axis = Vec3f(glm::rotate(TWO_PIf*randomFloat(), _direction) * Vec4f(glm::normalize(rot_axis), 0.f));

			Vec3f dir = Vec3f(glm::rotate(_angle*randomFloat(), rot_axis) * Vec4f(-_direction, 0.f));

			return LightSample(_color*_power, dir, FLT_MAX, 1.f);
		}
	  private:
		float _angle;
	};
}

#endif