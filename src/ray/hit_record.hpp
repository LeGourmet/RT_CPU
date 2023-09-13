#ifndef __HIT_RECORD_HPP__
#define __HIT_RECORD_HPP__

#include "utils/defines.hpp"

namespace RT_CPU
{
	class Object;

	struct HitRecord
	{
		void faceNormal( const Vec3f p_direction ) { _normal = glm::dot( p_direction, _normal ) < 0. ? _normal : -_normal; }

		Vec3f			_point		= VEC3F_ZERO;
		Vec3f			_normal		= VEC3F_Y;
		float			_distance	= FLOAT_MAX;
		const Object*	_object		= nullptr;
	};
}

#endif