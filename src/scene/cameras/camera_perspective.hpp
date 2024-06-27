#ifndef __CAMERA_PERSPECTIVE_HPP__
#define __CAMERA_PERSPECTIVE_HPP__

#include "camera.hpp"
#include "ray/ray.hpp"

namespace RT_CPU
{
	class CameraPerspective : public Camera
	{
	public:
		// ----------------------------------------------- DESTRUCTOR / CONSTRUCTOR ----------------------------------------------
		CameraPerspective(const Vec3f& p_position, const Vec3f& p_lookAt, float p_fovy, float p_focalDistance, float p_aspectRatio) : Camera(p_position, p_lookAt), _fovy(p_fovy), _focalDistance(p_focalDistance), _aspectRatio(p_aspectRatio) {
			float tmp = 2.f*glm::tan(p_fovy*0.5f)*p_focalDistance;
			_xSize = tmp*p_aspectRatio;
			_ySize = tmp;
		}
		~CameraPerspective() { }

		inline Ray generateRay(const float p_sx, const float p_sy) const override {
			return Ray(_position,glm::normalize((p_sx-0.5f)*_u*_xSize + (p_sy-0.5f)*_v*_ySize + _w*_focalDistance));
		}

	private:
		float _fovy				= 1e-5f;
		float _focalDistance	= 1.f;
		float _aspectRatio		= 1.f;
		float _xSize			= 1.f;
		float _ySize			= 1.f;
	};
} 

#endif 