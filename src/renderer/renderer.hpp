#ifndef __RENDERER_HPP__
#define __RENDERER_HPP__

#include "ray/ray.hpp"
#include "ray/hit_record.hpp"

#include "scene/scene.hpp"
#include "scene/meshes/mesh.hpp"
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
		Renderer(int p_nbPixelSamples, int p_nbBounceMax = 10) : _nbPixelSamples(p_nbPixelSamples), _nbBounceMax(p_nbBounceMax) {}
		~Renderer() {}

		void renderImage(const Scene& p_scene, Image& p_texture) {
			const int width = p_texture.getWidth();
			const int height = p_texture.getHeight();

			ProgressBar progressBar;
			progressBar.start(height, 50);

			// sampling camera aperture(u,v) => depth of field
			// sampling in time (the shutter) => motion blur

			/*
			 #ifdef MOVE_CAMERA
			float an = (-iMouse.x/iResolution.x-.5)*3.141592;
			vec3 ro = vec3(8.*sin(an), iMouse.y/iResolution.y*4.-2., 8.*cos(an)); // camera position
			#else
			vec3 ro = vec3(8.*sin(3.8), .3, 8.*cos(3.8)); // camera position
			#endif
			vec3 ta = vec3(0,.5,0); // target
			mat3 ca = setCamera(ro, ta); // camera matrix
			vec3 rd = ca * normalize(vec3(p,1.78)); // ray direction
    
			// depth of field
			vec3 n, mat; int type; vec2 v;
			float t = intersect(ro, normalize(ta - ro), n, mat, type, v);
			vec3 fp = ro + rd*t; // focus plane
			ro += ca*vec3(uniformVector().xy,0)*.12; // <- change this value for the aperture
			rd = normalize(fp - ro);
			*/

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
			Vec3f rayColor = VEC3F_ONE;
			Ray currentRay = p_ray;

			float ni=1.f, no=1.f;
			bool inside = false;

			for(int i=0; i<_nbBounceMax ;i++) {
				HitRecord hitRecord;
				if (p_scene.intersect(currentRay, p_near, p_far, hitRecord)) {
					no = ((inside) ? 1.f : hitRecord._mesh->getMaterial()->getIOR());
					
					for (Light* light : p_scene.getLights()) {
						Vec3f lightRadiosity = VEC3F_ZERO;
						for (int i = 0; i < light->getNbShadowRay();i++) {
							LightSample lightSample = light->sample(hitRecord._point, i);
							Ray shadowRay = Ray(hitRecord._point, lightSample._direction);
							shadowRay.offset((glm::dot(hitRecord._normal,lightSample._direction)<0.f) ? -hitRecord._normal : hitRecord._normal);

							//if (inside) break;
							//if (glm::dot(hitRecord._normal, shadowRay.getDirection()) > 0.f && glm::dot(hitRecord._normal, -currentRay.getDirection()) > 0.f && !p_scene.intersectAny(shadowRay, p_near, lightSample._distance-p_near))
							if (!p_scene.intersectAny(shadowRay, p_near, lightSample._distance-p_near)){
								/* --------------- MIS ---------------
								// https://pbr-book.org/3ed-2018/Monte_Carlo_Integration/Importance_Sampling
								float powerHeuristic(int nf, float fPdf, int ng, float gPdf){
									float f = float(nf) * fPdf, g = float(ng) * gPdf;
									return (f * f) / (f * f + g * g);
								}

								// float lightpdf = dist_sq/max(1e-5, abs(cosT) * area); // sure ?

								weight = powerHeuristic(1.f,lightSample._pdf, 1.f, bsdf.pdf)
								lighRadiosity += .... * weight / lightsample._pdf;*/
								float bsdfPdf = 1.f;
								Vec3f bsdfValue = hitRecord._mesh->getMaterial()->evaluateBSDF(-currentRay.getDirection(), hitRecord._normal, lightSample._direction, ni, no, bsdfPdf);
								float weight = lightSample._pdf*lightSample._pdf/(lightSample._pdf*lightSample._pdf+bsdfPdf*bsdfPdf);
								lightRadiosity += lightSample._radiance * weight * bsdfValue / glm::max(1e-5f, lightSample._pdf);
							}
						}
						finalColor += rayColor * lightRadiosity / (float)light->getNbShadowRay();
					}

					if(inside) rayColor *= hitRecord._mesh->getMaterial()->getAbsoptivity(hitRecord._distance);
					finalColor += hitRecord._mesh->getMaterial()->getEmissivity()*rayColor;
					currentRay = hitRecord._mesh->getMaterial()->sampleBSDF(currentRay,hitRecord,ni,no,rayColor);
					
					if (rayColor.x == 0.f && rayColor.y == 0.f && rayColor.z == 0.f) break;

					if (glm::dot(hitRecord._normal, currentRay.getDirection()) < 0.f) {
						ni = ((inside) ? 1.f : hitRecord._mesh->getMaterial()->getIOR());
						inside = !inside;
					}
				}
				else {
					finalColor += rayColor * Vec3f(0.001f); // envmap
					break;
				}
			}

			return finalColor;
		}

	private:
		int	_nbPixelSamples;
		int	_nbBounceMax;
	};
}

#endif