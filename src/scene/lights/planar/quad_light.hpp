#ifndef __QUAD_LIGHT_HPP__
#define __QUAD_LIGHT_HPP__

#include "planar_light.hpp"
#include "utils/random.hpp"
#include "glm/gtx/transform.hpp"

namespace RT_CPU
{
	class QuadLight : public PlanarLight {
	  public:
		// len(cross(a,b)) = len(a) * len(b) * sin(a^b)   ==>   area = L * H * sin(u^v)
		QuadLight( const Vec3f& p_position, const Vec3f& p_direction, const Vec3f& p_color, float p_power, const Vec2f& p_u, const Vec2f& p_v, int p_nbShadowRay = 4)
			: PlanarLight( p_position, p_direction, p_color, p_power, glm::length(glm::cross(Vec3f(p_u, 0.f), Vec3f(p_v, 0.f))), p_nbShadowRay), _u(p_u), _v(p_v), _nbRow(p_nbShadowRay) {}
		~QuadLight() {}

		inline virtual LightSample sample(const Vec3f& p_point, const int p_i) const override {
			Vec2f frag = Vec2f( (((p_i/_nbRow)+randomFloat())/float(_nbRow)-0.5f)*_u +
								(((p_i%_nbRow)+randomFloat())/float(_nbRow)-0.5f)*_v);
			return _evaluate(p_point, frag);
		}

		inline LightSample sample(const Vec3f& p_point) const override {
			Vec2f frag = Vec2f((randomFloat()-0.5f)*_u + (randomFloat()-0.5f)*_v);
			return _evaluate(p_point, frag);
		}

	  private:
		int _nbRow;
		Vec2f _u;
		Vec2f _v;
	};

}

#endif