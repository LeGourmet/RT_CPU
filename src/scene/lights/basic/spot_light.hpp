#ifndef __SPOT_LIGHT_HPP__
#define __SPOT_LIGHT_HPP__

#include "basic_light.hpp"

namespace RT_CPU
{

	class SpotLight : public BasicLight
	{
	  public:
		SpotLight(const Vec3f& p_position, const Vec3f& p_direction, const Vec3f& p_color, float p_innerAngle, float p_outerAngle, float p_power = 1.f )
			: BasicLight( p_position, p_direction, p_color, p_power ), _cosInnerAngle(glm::cos(p_innerAngle)), _cosOuterAngle(glm::cos(p_outerAngle)) { }
		~SpotLight() {}

		LightSample sample(const Vec3f& p_point, const int p_id) const override {
			Vec3f vec  = _position - p_point;
			float dist_sq = glm::dot(vec,vec);
			Vec3f dir = glm::normalize( vec );
			float intensity = glm::clamp((glm::dot(-dir,_direction)-_cosOuterAngle)/(_cosInnerAngle-_cosOuterAngle), 0.f, 1.f);

			return LightSample( intensity*(_color*_power)/dist_sq, dir, glm::sqrt( dist_sq ) );
		}

	  private:
		float _cosInnerAngle;
		float _cosOuterAngle;
	};

}

#endif