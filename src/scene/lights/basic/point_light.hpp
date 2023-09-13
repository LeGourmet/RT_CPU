#ifndef __POINT_LIGHT_HPP__
#define __POINT_LIGHT_HPP__

#include "basic_light.hpp"

namespace RT_CPU
{

	class PointLight : public BasicLight
	{
	  public:
		PointLight(const Vec3f& p_position, const Vec3f& p_direction, const Vec3f& p_color, float p_power = 1.f )
			: BasicLight( p_position, p_direction, p_color, p_power ){}
		~PointLight() {}

		LightSample sample(const Vec3f& p_point, const int p_id) const override {
			Vec3f vec  = _position - p_point;
			float dist_sq = glm::dot( vec, vec );

			return LightSample( (_color*_power)/dist_sq, glm::normalize(vec), glm::sqrt( dist_sq ) );
		}
	};

}

#endif