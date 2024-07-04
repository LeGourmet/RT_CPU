#ifndef __PONCTUAL_LIGHT_HPP__
#define __PONCTUAL_LIGHT_HPP__

#include "basic_light.hpp"

namespace RT_CPU
{

	class PonctualLight : public BasicLight
	{
	  public:
		PonctualLight(const Vec3f& p_position, const Vec3f& p_direction, const Vec3f& p_color, float p_power, float p_innerAngle, float p_outerAngle) // +texture
			: BasicLight( p_position, p_direction, p_color, p_power ), _cosInnerAngle(glm::cos(p_innerAngle)), _cosOuterAngle(glm::cos(p_outerAngle)) {}
		~PonctualLight() {}

		LightSample sample(const Vec3f& p_point) const override {
			Vec3f vec  = _position - p_point;
			float dist_sq = glm::dot( vec, vec );
			Vec3f dir = glm::normalize(vec);

			float intensity = glm::clamp((glm::dot(-dir,_direction)-_cosOuterAngle)/(_cosInnerAngle-_cosOuterAngle), 0.f, 1.f);
			//float profil = texture2DLodEXT(_profil, Vec2f(glm::acos(glm::dot(-dir,_direction))*INV_PIf,0.f), 0.f).r;

			return LightSample(intensity*_color*_power, dir, glm::sqrt(dist_sq), dist_sq);
		}
	  
	  private:
		float _cosInnerAngle;
		float _cosOuterAngle;
		// texture _profil
	};

}

#endif