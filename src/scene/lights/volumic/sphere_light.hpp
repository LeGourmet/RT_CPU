#ifndef __SPHERE_LIGHT_HPP__
#define __SPHERE_LIGHT_HPP__

#include "volumic_light.hpp"
#include "utils/random.hpp"

namespace RT_CPU
{
	class SphereLight : public VolumicLight
	{
	  public:
		SphereLight( const Vec3f & p_position, const Vec3f& p_direction, const Vec3f& p_color, float p_power, float p_radius, float p_innerAngle, float p_outerAngle, int p_nbShadowRay = 3)
			: VolumicLight( p_position, p_direction, p_color, p_power, 4.f*PIf*p_radius*p_radius*p_radius/3.f, p_nbShadowRay ), _radius(p_radius), _cosInnerAngle(glm::cos(p_innerAngle)), _cosOuterAngle(glm::cos(p_outerAngle)) {}
		~SphereLight() {}

		inline virtual LightSample sample(const Vec3f& p_point, int p_id) const override {
			float phi = randomFloat() * TWO_PIf;
			float theta = randomFloat() * TWO_PIf;
			float r = _radius * glm::max(1e-5f, glm::sqrt(randomFloat()));
			Vec3f frag = r * Vec3f(glm::sin(theta)*glm::cos(phi), glm::sin(theta)*glm::sin(phi), glm::cos(theta));
			
			Vec3f rot_axis = glm::cross(_direction, -VEC3F_Z);
			if (glm::dot(rot_axis, rot_axis) != 0.f)
				frag = Vec3f(glm::rotate(glm::acos(glm::dot(_direction, -VEC3F_Z)), glm::normalize(rot_axis)) * Vec4f(frag, 1.f));
			frag += _position;

			// todo sample stratified !

			Vec3f vec  = frag-p_point;
			float dist_sq = glm::dot(vec,vec);
			Vec3f dir  = glm::normalize(vec);

			float intensity = glm::abs(glm::dot(glm::normalize(frag-_position),dir))*_volume
							* glm::clamp((glm::dot(-dir,_direction)-_cosOuterAngle)/(_cosInnerAngle-_cosOuterAngle), 0.f, 1.f);
			
			return LightSample(_color*_power*intensity, dir, glm::sqrt(dist_sq), dist_sq);
		}

		inline LightSample sample(const Vec3f& p_point) const override {
			float phi = randomFloat() * TWO_PIf;
			float theta = randomFloat() * TWO_PIf;
			float r = _radius * glm::max(1e-5f, glm::sqrt(randomFloat()));
			Vec3f frag = r * Vec3f(glm::sin(theta) * glm::cos(phi), glm::sin(theta) * glm::sin(phi), glm::cos(theta));

			Vec3f rot_axis = glm::cross(_direction, -VEC3F_Z);
			if (glm::dot(rot_axis, rot_axis) != 0.f)
				frag = Vec3f(glm::rotate(glm::acos(glm::dot(_direction, -VEC3F_Z)), glm::normalize(rot_axis)) * Vec4f(frag, 1.f));
			frag += _position;

			Vec3f vec = frag-p_point;
			float dist_sq = glm::dot(vec,vec);
			Vec3f dir = glm::normalize(vec);
			
			float intensity = glm::abs(glm::dot(glm::normalize(frag-_position),dir))*_volume
							* glm::clamp((glm::dot(-dir,_direction)-_cosOuterAngle)/(_cosInnerAngle-_cosOuterAngle), 0.f, 1.f);

			return LightSample(_color*_power*intensity, dir, glm::sqrt(dist_sq), dist_sq);
		}

	  private:
		float _cosInnerAngle;
		float _cosOuterAngle;
		float _radius;
	};

}

#endif