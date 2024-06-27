#ifndef __CAMERA_ORTHOGRAPHIC_HPP__
#define __CAMERA_ORTHOGRAPHIC_HPP__

#include "camera.hpp"
#include "ray/ray.hpp"

namespace RT_CPU
{
	class CameraOrthographic : public Camera
	{
	public:
		// ----------------------------------------------- DESTRUCTOR / CONSTRUCTOR ----------------------------------------------
		CameraOrthographic(const Vec3f& p_position, const Vec3f& p_lookAt, float p_xmag, float p_ymag) : Camera(p_position,p_lookAt), _xmag(p_xmag), _ymag(p_ymag) { }
		~CameraOrthographic() { }

		inline Ray generateRay(const float p_sx, const float p_sy) const override { 
			return Ray((p_sx-0.5f)*_u*_xmag + (p_sy-0.5f)*_v*_ymag + _position, _w);
		}

	private:
		float _xmag = 1.f;
		float _ymag = 1.f;
	};
}

#endif 