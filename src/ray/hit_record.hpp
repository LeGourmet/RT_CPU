#ifndef __HIT_RECORD_HPP__
#define __HIT_RECORD_HPP__

#include "utils/defines.hpp"

namespace RT_CPU
{
	class Mesh;

	struct HitRecord
	{
		void faceNormal( const Vec3f p_direction ) { if(glm::dot(p_direction,_normal)>0.f) _normal = -_normal; }

		Vec3f			_point		= VEC3F_ZERO;
		Vec3f			_normal		= VEC3F_Z;
		Vec3f			_tangent	= VEC3F_Y;
		Vec3f			_bitangent	= VEC3F_X; 
		float			_distance	= FLOAT_MAX;
		const Mesh*		_mesh		= nullptr;
	};
}

#endif