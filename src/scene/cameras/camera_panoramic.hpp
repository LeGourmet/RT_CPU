#ifndef __CAMERA_PANORAMIC_HPP__
#define __CAMERA_PANORAMIC_HPP__

#include "camera.hpp"
#include "ray/ray.hpp"

namespace RT_CPU
{
	class CameraPanoramic : public Camera
	{
	public:
		// ----------------------------------------------- DESTRUCTOR / CONSTRUCTOR ----------------------------------------------
		CameraPanoramic(const Vec3f& p_position, const Vec3f& p_lookAt, float p_focalDistance) : Camera(p_position, p_lookAt), _focalDistance(p_focalDistance) { }
		~CameraPanoramic() { }

		inline Ray generateRay(const float p_sx, const float p_sy) const override {
			// convert w to be center in screen
			// theta => rotate arround v axis
			// phi => rotate arround u axis
			float W_theta = glm::atan(_w.y/_w.x);
			float W_phi = glm::acos(_w.z);

			float theta = TWO_PIf * p_sx - W_theta;
			float phi	= PIf * p_sy;// + W_phi;
			Vec3f dir = Vec3f(glm::sin(phi)*glm::cos(theta), glm::sin(phi)*glm::sin(theta), glm::cos(phi));
			
			return Ray(_position, glm::normalize(dir));
		}

	private:
		float _focalDistance = 1.f;
	};
}

#endif