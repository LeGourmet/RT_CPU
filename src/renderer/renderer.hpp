#ifndef __RENDERER_HPP__
#define __RENDERER_HPP__

#include "ray/ray.hpp"
#include "ray/hit_record.hpp"

#include "scene/scene.hpp"
#include "scene/objects/object.hpp"
#include "scene/lights/light.hpp"
#include "scene/lights/light_sample.hpp"

#include "utils/image.hpp"
#include "utils/random.hpp"
#include "utils/progress_bar.hpp"

namespace RT_CPU
{
	class Renderer
	{
	public:
		Renderer(int p_nbPixelSamples = 1) : _nbPixelSamples(p_nbPixelSamples) {}
		~Renderer() {}

		void renderImage(const Scene& p_scene, Image& p_texture) {
			const int width = p_texture.getWidth();
			const int height = p_texture.getHeight();

			ProgressBar progressBar;
			progressBar.start(height, 50);

			// sampling camera aperture(u,v) => depth of field
			// sampling in time (the shutter) => motion blur

			#pragma omp parallel for schedule( dynamic )
			for (int j = 0; j < height;j++) {
				for (int i = 0; i < width;i++) {

					Vec3f sumColor = VEC3F_ZERO;
					for (int a = 0; a < _nbPixelSamples;a++) {
						for (int b = 0; b < _nbPixelSamples;b++) {
							Ray ray = p_scene.getCamera().generateRay(
								(i/(float)width)  + (a+randomFloat()) / (float)(width*_nbPixelSamples),
								(j/(float)height) + (b+randomFloat()) / (float)(height*_nbPixelSamples));

							sumColor += _trace(p_scene, ray, 1e-3f, 1000.f);
						}
					}
					sumColor /= _nbPixelSamples * _nbPixelSamples;
					sumColor = glm::clamp((sumColor * (2.51f * sumColor + 0.03f)) / (sumColor * (2.43f * sumColor + 0.59f) + 0.14f), 0.f, 1.f);
					sumColor = glm::pow(sumColor, Vec3f(1.f / 2.2f));
					p_texture.setPixel(i, j, sumColor);
				}
				progressBar.next();
			}

			progressBar.stop();
		}

	private:
		Vec3f _trace(const Scene& p_scene, const Ray& p_ray, const float p_near, const float p_far) {
			Vec3f finalColor = VEC3F_ZERO;

			#ifdef RENDERING_DIRECT_LIGHTING
			HitRecord hitRecord;
			if (p_scene.intersect(p_ray, p_near, p_far, hitRecord)) {
				for (Light* light : p_scene.getLights()) {
					Vec3f lightRadiosity = VEC3F_ZERO;
					for (int i = 0; i < light->getNbShadowRay();i++) {
						LightSample lightSample = light->sample(hitRecord._point,i);
						Ray shadowRay = Ray(hitRecord._point, lightSample._direction);
						shadowRay.offset(hitRecord._normal);

						if(glm::dot(hitRecord._normal,shadowRay.getDirection())>0.f && glm::dot(hitRecord._normal,-p_ray.getDirection())>0.f && !p_scene.intersectAny(shadowRay,1e-2f,lightSample._distance))
							lightRadiosity += lightSample._radiance * hitRecord._object->getMaterial()->evaluateBRDF(-p_ray.getDirection(), hitRecord._normal, lightSample._direction);
					}
					finalColor += lightRadiosity / (float)light->getNbShadowRay();
				}
			}
			#else
			Vec3f rayColor = VEC3F_ONE;
			Ray currentRay = p_ray;

			float ni=1.f, no=1.f;
			bool inside = false;

			for(int i=0; i<_nbBounceMax ;i++) {
				HitRecord hitRecord;
				if (p_scene.intersect(currentRay, p_near, p_far, hitRecord)) {
					no = ((inside) ? 1.f : hitRecord._object->getMaterial()->getIOR());
					
					/*Vec3f directLighting = VEC3F_ZERO;
					for (Light* light : p_scene.getLights()) {
						LightSample lightSample = light->sample(hitRecord._point,0);
						Ray shadowRay = Ray(hitRecord._point, lightSample._direction);
						shadowRay.offset(hitRecord._normal);

						if (glm::dot(hitRecord._normal, shadowRay.getDirection()) > 0.f && glm::dot(hitRecord._normal, -p_ray.getDirection()) > 0.f && !p_scene.intersectAny(shadowRay, 1e-2, lightSample._distance))
							directLighting += lightSample._radiance * hitRecord._object->getMaterial()->evaluateBRDF(-p_ray.getDirection(), hitRecord._normal, lightSample._direction) / glm::max(1e-5f, lightSample._pdf);
					}

					finalColor += directLighting;*/

					finalColor += hitRecord._object->getMaterial()->getEmissivity()*rayColor;
					currentRay = hitRecord._object->getMaterial()->evaluateWeightedBSDF(currentRay,hitRecord,ni,no,rayColor);
					
					if (rayColor.x == 0.f && rayColor.y == 0.f && rayColor.z == 0.f) break;

					if (glm::dot(hitRecord._normal, currentRay.getDirection()) < 0.f) {
						ni = ((inside) ? 1.f : hitRecord._object->getMaterial()->getIOR());
						inside = !inside;
					}
				}
				else {
					finalColor += rayColor * Vec3f(0.001f); // envmap
					break;
				}
			}
			#endif

			return finalColor;
		}

	private:
		int	_nbPixelSamples;
		int	_nbBounceMax = 10;
	};
}

#endif