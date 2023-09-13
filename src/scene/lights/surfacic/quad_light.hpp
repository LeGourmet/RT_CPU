#ifndef __QUAD_LIGHT_HPP__
#define __QUAD_LIGHT_HPP__

#include "surfacic_light.hpp"

namespace RT_CPU
{

	class QuadLight : public SurfacicLight{
	  public:
		// len(cross(a,b)) = len(a) * len(b) * sin(a^b)   ==>   area = L * H * sin(u^v)
		QuadLight( const Vec3f& p_position, const Vec3f& p_direction, const Vec3f& p_color, const Vec3f& p_u, const Vec3f& p_v, float p_power = 1.f, int p_nbShadowRay = 4)
			: SurfacicLight( p_position, p_direction, p_color, glm::length(glm::cross(p_u, p_v)), p_power, p_nbShadowRay ), _u( p_u ), _v( p_v ) {
			//_nbRow = _nbShadowRay; 
			//_sizeRow = 1.f / _nbRow ;
			//_linespace.push_back((_sizeRow-1.f)*0.5f);
			//for(int i=0; i<_nbRow-1 ;i++) _linespace.push_back(_linespace[i]+_sizeRow);
		}
		~QuadLight() {}

		inline virtual LightSample sample(const Vec3f& p_point, const int p_i) const override {
			/*Vec3f frag = (_linespace[p_i / _nbRow] + _sizeRow * (randomFloat() - 0.5f)) * _u
					   + ( _linespace[ p_i % _nbRow ] + _sizeRow * ( randomFloat() - 0.5f ) ) * _v + _position;*/

			Vec3f frag = VEC3F_ONE;
			Vec3f vec  = frag - p_point;
			float dist_sq = glm::dot( vec, vec );
			Vec3f dir  = glm::normalize( vec );

			float pdf = dist_sq / ( glm::abs(glm::dot( _direction, dir )) * _area );

			return LightSample( (_color*_power)/pdf, dir, glm::sqrt( dist_sq ));
		}

	  private:
		Vec3f _u;
		Vec3f _v;

		//int _nbRow;
		//float _sizeRow;
		//std::vector<float> _linespace;
	};

}

#endif