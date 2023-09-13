#ifndef __CAMERA_ORTHOGONAL_HPP__
#define __CAMERA_ORTHOGONAL_HPP__

#include "camera.hpp"
#include "ray/ray.hpp"

namespace RT_CPU
{
	class CameraOrthogonal : public Camera
	{
	public:
		// ----------------------------------------------- DESTRUCTOR / CONSTRUCTOR ----------------------------------------------
		CameraOrthogonal(const Vec3f& p_position, const Vec3f& p_lookAt, float p_xmag, float p_ymag) 
			: Camera(p_position,p_lookAt,p_xmag,p_ymag) { }
		~CameraOrthogonal() { }

		inline Ray generateRay(const float p_sx, const float p_sy) const override { 
			return Ray((p_sx-0.5f)*_u*_xSize + (p_sy-0.5f)*_v*_ySize + _position, _w); 
		}
	};
}

#endif 