#ifndef __RAY_HPP__
#define __RAY_HPP__

#include "utils/defines.hpp"

namespace RT_CPU
{
	class Ray
	{
	public:
		Ray() = delete;
		// p_direction must be normalized
		Ray(const Vec3f& p_origin, const Vec3f& p_direction) : _origin(p_origin), _direction(p_direction), _invDir(1.f / p_direction) {}

		inline const Vec3f& getOrigin() const { return _origin; }
		inline const Vec3f& getDirection() const { return _direction; }
		inline const Vec3f& getInvDirection() const { return _invDir; }

		// Return the point along _direction at distance 'p_t'
		inline Vec3f pointAtT(const float p_t) const { return _origin + _direction * p_t; }

		// This function add an offset to the ray origin to avoid self-intersection when computing shadows
		// See:
		// Wächter C., Binder N. (2019)
		// A Fast and Robust Method for Avoiding Self-Intersection.
		// In: Haines E., Akenine-Möller T. (eds) Ray Tracing Gems. Apress, Berkeley, CA
		inline void offset(const Vec3f p_normal)
		{
			Vec3i of_i(p_normal * _intScale());
			Vec3i ip_i(*reinterpret_cast<const int*>(&_origin.x) + ((_origin.x < 0.f) ? -of_i.x : of_i.x),
				*reinterpret_cast<const int*>(&_origin.y) + ((_origin.y < 0.f) ? -of_i.y : of_i.y),
				*reinterpret_cast<const int*>(&_origin.z) + ((_origin.z < 0.f) ? -of_i.z : of_i.z));

			Vec3f p_i(*reinterpret_cast<const float*>(&ip_i[0]),
				*reinterpret_cast<const float*>(&ip_i[1]),
				*reinterpret_cast<const float*>(&ip_i[2]));
			_origin = Vec3f(fabsf(_origin.x) < _originScale() ? _origin.x + _floatScale() * p_normal.x : p_i.x,
				fabsf(_origin.y) < _originScale() ? _origin.y + _floatScale() * p_normal.y : p_i.y,
				fabsf(_origin.z) < _originScale() ? _origin.z + _floatScale() * p_normal.z : p_i.z);
		}

	private:
		static constexpr float _originScale() { return 1.f / 32.f; }
		static constexpr float _floatScale() { return 1.f / 65536.f; }
		static constexpr float _intScale() { return 256.f; }

	private:
		Vec3f _origin = VEC3F_ZERO;
		Vec3f _direction = VEC3F_ZERO;
		Vec3f _invDir = VEC3F_ZERO;
	};
}

#endif