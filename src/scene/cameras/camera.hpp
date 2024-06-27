#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include "utils/defines.hpp"
#include "ray/ray.hpp"

namespace RT_CPU
{
	class Camera
	{
	public:
		// ----------------------------------------------- DESTRUCTOR / CONSTRUCTOR ----------------------------------------------
		Camera(const Vec3f& p_position, const Vec3f& p_lookat) : _position(p_position) {
			_w = glm::normalize(p_lookat - p_position);
			_u = glm::normalize(glm::cross(_w, Vec3f(0.f,0.f,1.f)));
			_v = glm::normalize(glm::cross(_w, _u));
		}
		virtual ~Camera() = default;

		// ------------------------------------------------------ GETTERS --------------------------------------------------------
		inline const Vec3f& getPosition() { return _position; }

		// ----------------------------------------------------- FONCTIONS -------------------------------------------------------
		virtual inline Ray generateRay(const float p_sx, const float p_sy) const = 0;

	protected:
		// ----------------------------------------------------- ATTRIBUTS -------------------------------------------------------
		Vec3f _u = VEC3F_X;
		Vec3f _v = VEC3F_Z;
		Vec3f _w = VEC3F_Y;

		Vec3f _position = VEC3F_ZERO;
	};

}

#endif