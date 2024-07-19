#ifndef __QUAD_LIGHT_HPP__
#define __QUAD_LIGHT_HPP__

#include "surfacic_light.hpp"
#include "utils/random.hpp"
#include "glm/gtx/transform.hpp"

namespace RT_CPU
{
	class QuadLight : public SurfacicLight{
	  public:
		// len(cross(a,b)) = len(a) * len(b) * sin(a^b)   ==>   area = L * H * sin(u^v)
		QuadLight( const Vec3f& p_position, const Vec3f& p_direction, const Vec3f& p_color, float p_power, const Vec2f& p_u, const Vec2f& p_v, int p_nbShadowRay = 4)
			: SurfacicLight( p_position, p_direction, p_color, p_power, glm::length(glm::cross(Vec3f(p_u,0.f), Vec3f(p_v,0.f))), p_nbShadowRay ), _u( p_u ), _v( p_v ), _nbRow(p_nbShadowRay) {}
		~QuadLight() {}

		inline virtual LightSample sample(const Vec3f& p_point, const int p_i) const override {
			Vec3f frag = Vec3f( (((p_i/_nbRow)+randomFloat())/float(_nbRow)-0.5f)*_u +
								(((p_i%_nbRow)+randomFloat())/float(_nbRow)-0.5f)*_v , 0.f);

			Vec3f rot_axis = glm::cross(_direction, -VEC3F_Z);
			if (glm::dot(rot_axis, rot_axis) != 0.f)
				frag = Vec3f(glm::rotate(glm::acos(glm::dot(_direction, -VEC3F_Z)), glm::normalize(rot_axis)) * Vec4f(frag, 1.f));
			frag += _position;

			Vec3f vec = frag-p_point;
			float dist_sq = glm::dot(vec,vec);
			Vec3f dir = glm::normalize(vec);

			float intensity = glm::abs(glm::dot(_direction,dir))*_area;

			return LightSample(_color*_power*intensity, dir, glm::sqrt(dist_sq), dist_sq);
		}

		inline LightSample sample(const Vec3f& p_point) const override {
			Vec3f frag = Vec3f((randomFloat()-0.5f)*_u + (randomFloat()-0.5f)*_v , 0.f);
			
			Vec3f rot_axis = glm::cross(_direction, -VEC3F_Z);
			if (glm::dot(rot_axis, rot_axis) != 0.f)
				frag = Vec3f(glm::rotate(glm::acos(glm::dot(_direction, -VEC3F_Z)), glm::normalize(rot_axis)) * Vec4f(frag, 1.f));
			frag += _position;

			Vec3f vec = frag-p_point;
			float dist_sq = glm::dot(vec,vec);
			Vec3f dir = glm::normalize(vec);

			float intensity = glm::abs(glm::dot(_direction,dir))*_area;

			return LightSample(_color*_power*intensity, dir, glm::sqrt(dist_sq), dist_sq);
		}

	  private:
		int _nbRow;
		Vec2f _u;
		Vec2f _v;
	};

}

#endif