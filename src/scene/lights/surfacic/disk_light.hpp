#ifndef __DISK_LIGHT_HPP__
#define __DISK_LIGHT_HPP__

#include "surfacic_light.hpp"
#include "utils/random.hpp"

namespace RT_CPU
{

	class DiskLight : public SurfacicLight {
	public:
		DiskLight(const Vec3f& p_position, const Vec3f& p_direction, const Vec3f& p_color, float p_power, float p_radius, int p_nbShadowRay = 4)
			: SurfacicLight(p_position, p_direction, p_color, p_power, PIf*p_radius*p_radius, p_nbShadowRay), _radius(p_radius) {}
		~DiskLight() {}

		/*
		Interaction Disk::Sample(const Point2f &u) const {
			Point2f pd = ConcentricSampleDisk(u);
			Point3f pObj(pd.x * radius, pd.y * radius, height);
			Interaction it;
			it.n = Normalize((*ObjectToWorld)(Normal3f(0, 0, 1)));
			if (reverseOrientation) it.n *= -1;
			it.p = (*ObjectToWorld)(pObj, Vector3f(0, 0, 0), &it.pError);
			return it;
		}
		*/

		inline virtual LightSample sample(const Vec3f& p_point, const int p_i) const override {
			// todo stratified sampling
			
			float theta = randomFloat() * TWO_PIf;
			float r = _radius * glm::max(1e-5f, glm::sqrt(randomFloat()));
			
			Vec2f frag2D = Vec2f(r*glm::cos(theta), r*glm::sin(theta));
			Vec3f frag = VEC3F_ZERO;

			// todo rotate with direction

			Vec3f vec = frag - p_point;
			float dist_sq = glm::dot(vec, vec);
			Vec3f dir = glm::normalize(vec);

			float pdf = dist_sq/(glm::abs(glm::dot(_direction,dir))*_area);

			return LightSample(_color*_power, dir, glm::sqrt(dist_sq), pdf);
		}

		inline LightSample sample(const Vec3f& p_point) const override {
			float theta = randomFloat()* TWO_PIf;
			float r = _radius * glm::max(1e-5f, glm::sqrt(randomFloat()));
			
			// todo compute frag
			Vec3f frag2D = Vec3f(r*glm::cos(theta), r*glm::sin(theta), 0.f);
			Vec3f frag = VEC3F_ZERO;

			// todo rotate with direction

			Vec3f vec = frag - p_point;
			float dist_sq = glm::dot(vec, vec);
			Vec3f dir = glm::normalize(vec);

			float pdf = dist_sq/(glm::abs(glm::dot(_direction,dir))*_area);

			return LightSample(_color*_power, dir, glm::sqrt(dist_sq), pdf);
		}

	private:
		float _radius;
	};

}

#endif