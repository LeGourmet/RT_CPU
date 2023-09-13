#ifndef __AABB_HPP__
#define __AABB_HPP__

#include "ray/ray.hpp"

namespace RT_CPU
{
	class AABB
	{
	  public:
		AABB( const Vec3f& p_min, const Vec3f& p_max ) : _min( p_min ), _max( p_max ) {}
		AABB() {}
		~AABB() {}

		inline const Vec3f& getMin() const { return _min; }
		inline const Vec3f& getMax() const { return _max; }

		// Returns true if the AABB is degenerated.
		inline bool isValid() const { return ( ( _min.x <= _max.x ) && ( _min.y <= _max.y ) && ( _min.z <= _max.z ) ); }

		// Extends the AABB with a point
		inline void extend( const Vec3f& p_v ) {
			_min = glm::min( _min, p_v );
			_max = glm::max( _max, p_v );
		}

		// Extends the AABB with another AABB
		inline void extend( const AABB& p_aabb ) {
			_min = glm::min( _min, p_aabb.getMin() );
			_max = glm::max( _max, p_aabb.getMax() );
		}

		inline Vec3f diagonal() const { return _max - _min; }

		inline Vec3f centroid() const { return ( _min + _max ) * 0.5f; }

		inline float surface() const {
			const Vec3f d = diagonal();
			return 2.f * ( d.x*d.y + d.x*d.z + d.y*d.z ); 
		}

		// Returns the largest axis, 0 -> x, 1 -> y, 2 -> z
		inline size_t largestAxis() const {
			const Vec3f d = diagonal();
			if (d.x >= d.y && d.x >= d.z) return 0;
			if ( d.y >= d.z ) return 1;
			return 2;
		}

		// slabs method
		bool intersect(const Ray& p_ray, const float p_tMin, const float p_tMax) const {
			Vec3f tmin = (_min - p_ray.getOrigin()) * p_ray.getInvDirection();
			Vec3f tmax = (_max - p_ray.getOrigin()) * p_ray.getInvDirection();

			Vec3f a = glm::min(tmin, tmax);
			Vec3f b = glm::max(tmin, tmax);

			float tnear = glm::max(glm::max(a.x, a.y), a.z);
			float tfar = glm::min(glm::min(b.x, b.y), b.z);

			return tnear <= tfar && tfar >= p_tMin && tnear <= p_tMax;
		}

	  private:
		Vec3f _min = Vec3f( FLT_MAX );
		Vec3f _max = Vec3f( -FLT_MAX );
	};
}

#endif 