#ifndef __CAMERA_FISHEYE_HPP__
#define __CAMERA_FISHEYE_HPP__

#include "camera.hpp"
#include "ray/ray.hpp"

namespace RT_CPU
{
	class CameraFisheye : public Camera
	{
	public:
		// ----------------------------------------------- DESTRUCTOR / CONSTRUCTOR ----------------------------------------------
		CameraFisheye(const Vec3f& p_position, const Vec3f& p_lookAt, float p_focalDistance) : Camera(p_position, p_lookAt), _focalDistance(p_focalDistance) { }
		~CameraFisheye() { }

		inline Ray generateRay(const float p_sx, const float p_sy) const override {
			// sould be less compressed in the center

			/*Point2D pn(2.0 / (s * hres) * pp.x, 2.0 / (s * vres) * pp.y);
			r_squared = pn.x * pn.x + pn.y * pn.y;

			if (r_squared <= 1.0) {
				float r = sqrt(r_squared);
				float psi = r * psi_max * PI_ON_180;
				float sin_psi = sin(psi);
				float cos_psi = cos(psi);
				float sin_alpha = pn.y / r;
				float cos_alpha = pn.x / r;
				Vector3D dir = sin_psi * cos_alpha * u + sin_psi * sin_alpha * v - cos_psi * w;

				return (dir);
			}
			else
				return (Vector3D(0.0));*/
			
			//float theta = PIf * (p_sx*2.f-1.f);
			//float phi	= PIf * (p_sy*2.f-1.f);
			//Vec3f dir = glm::sin(phi)*glm::cos(theta)*_u + glm::sin(phi)*glm::sin(theta)*_v - glm::cos(phi)*_w;

			float r = glm::sqrt(p_sx*p_sx+p_sy*p_sy);
			float phi = r * PIf;
			Vec3f dir = glm::sin(phi)*(p_sx/r)*_u + glm::sin(phi)*(p_sy/r)*_v - glm::cos(phi)*_w;

			return Ray(_position, glm::normalize(dir));
		}

	private:
		float _focalDistance = 1.f;
	};
}

#endif