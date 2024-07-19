#ifndef __CYLINDER_LIGHT_HPP__
#define __CYLINDER_LIGHT_HPP__

#include "volumic_light.hpp"
#include "utils/random.hpp"

/*float phi = (_direction.x == 0.f) ? PI_2f : glm::atan(_direction.y / _direction.x) * 2.f; //[-PI/2;PI/2]
			float theta = glm::acos(-_direction.z)*2.f; // [PI;0]

			phi += (randomFloat()-0.5f) * _angle; // don't move ??
			theta += (randomFloat()-0.5f) * _angle;

			Vec3f dir = Vec3f(glm::sin(theta) * glm::cos(phi), glm::sin(theta) * glm::sin(phi), glm::cos(theta));*/

namespace RT_CPU
{

	class CylinderLight : public VolumicLight
	{
	  public:
		 CylinderLight( const Vec3f & p_position, const Vec3f& p_direction, const Vec3f& p_color, float p_power, float p_radius, float p_size, int p_nbShadowRay = 4)
			: VolumicLight( p_position, p_direction, p_color, p_power, PIf*p_radius*p_radius*p_size, p_nbShadowRay ), _radius(p_radius), _size(p_size) {}
		~CylinderLight() {}

		/*
		Interaction Cylinder::Sample(const Point2f &u) const {
			Float z = Lerp(u[0], zMin, zMax);
			Float phi = u[1] * phiMax;
			Point3f pObj = Point3f(radius * std::cos(phi), radius * std::sin(phi), 
								   z);
			Interaction it;
			it.n = Normalize((*ObjectToWorld)(Normal3f(pObj.x, pObj.y, 0)));
			if (reverseOrientation) it.n *= -1;
			<<Reproject pObj to cylinder surface and compute pObjError>> 
			it.p = (*ObjectToWorld)(pObj, pObjError, &it.pError);
			return it;
		}
		*/

		inline virtual LightSample sample(const Vec3f& p_point, int p_id) const override {
			float phi = randomFloat() * TWO_PIf;
			float theta = randomFloat() * TWO_PIf;
			float r = _radius * glm::max(1e-5f, glm::sqrt(randomFloat()));
			Vec3f frag = r * Vec3f(glm::sin(theta)*glm::cos(phi), glm::sin(theta)*glm::sin(phi), glm::cos(theta)) + _position;
			
			// todo sample stratified !

			Vec3f vec  = frag-p_point;
			float dist_sq = glm::dot(vec,vec);
			Vec3f dir  = glm::normalize(vec);

			float pdf = dist_sq/(glm::abs(glm::dot(glm::normalize(frag-_position),dir))*_volume);

			return LightSample(_color*_power, dir, glm::sqrt(dist_sq), pdf);
		}

		inline LightSample sample(const Vec3f& p_point) const override {
			
			
			float phi = randomFloat() * TWO_PIf;
			float theta = randomFloat() * TWO_PIf;
			float r = _radius * glm::max(1e-5f, glm::sqrt(randomFloat()));
			Vec3f frag = r * Vec3f(glm::sin(theta) * glm::cos(phi), glm::sin(theta) * glm::sin(phi), glm::cos(theta)) + _position;

			Vec3f vec = frag-p_point;
			float dist_sq = glm::dot(vec,vec);
			Vec3f dir = glm::normalize(vec);
		
			float pdf = dist_sq/(glm::abs(glm::dot(glm::normalize(frag-_position),dir))*_volume);

			return LightSample(_color*_power, dir, glm::sqrt(dist_sq), pdf);
		}

	  private:
		float _size;
		float _radius;
	};

}

#endif