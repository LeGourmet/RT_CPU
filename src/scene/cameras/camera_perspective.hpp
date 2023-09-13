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
		CameraPerspective(const Vec3f& p_position, const Vec3f& p_lookAt, float p_fovy, float p_focalDistance, float p_aspectRatio)
			: Camera(p_position, p_lookAt, 2.f*glm::tan(p_fovy*0.5f)*p_focalDistance*p_aspectRatio, 2.f*glm::tan(p_fovy*0.5f)*p_focalDistance),
			  _fovy(p_fovy), _focalDistance(p_focalDistance), _aspectRatio(p_aspectRatio) { }

		~CameraPerspective() { }

		inline Ray generateRay(const float p_sx, const float p_sy) const override {
			Vec3f fragPos = (p_sx-0.5f)*_u*_xSize + (p_sy-0.5f)*_v*_ySize + _position;
			return Ray(fragPos,glm::normalize(fragPos-(_position-_w*_focalDistance)));
		}

	private:
		float _fovy = 0.f;
		float _focalDistance = 1.f;
		float _aspectRatio = 1.f;
	};
} 

#endif 