#ifndef __MATERIAL_HPP__
#define __MATERIAL_HPP__

#include "utils/random.hpp"
#include "utils/defines.hpp"

#include "ray/ray.hpp"
#include "ray/hit_record.hpp"
#include "scene/lights/light_sample.hpp"

#define pow2(a) (a)*(a)
#define pow5(a) (a)*(a)*(a)*(a)*(a)

namespace RT_CPU
{
    class Material
    {
    public:
        // --------------------------------------------- DESTRUCTOR / CONSTRUCTOR ----------------------------------------------
        Material(const Vec3f& p_baseColor, const Vec3f& p_emissiveColor, float p_emissiveStrength, float p_metalness, float p_roughness, float p_ior) :
            _baseColor(p_baseColor), _emissiveColor(p_emissiveColor), _emissiveStrength(p_emissiveStrength), _metalness(p_metalness), _roughness(glm::max(1e-6f,p_roughness)), _ior(p_ior) { }
        ~Material() {}

        inline const float getIOR() const { return _ior; }
        inline const Vec3f getEmissivity() const { return _emissiveColor*_emissiveStrength; }

        Vec3f evaluateBRDF(const Vec3f& V, const Vec3f& N, const Vec3f& H, const Vec3f& L, bool p_pdfWeighted){
            float cosNV = glm::max(1e-5f,glm::dot(N, V)); // max 1e-5f avoid artefacts
            float cosNL = glm::dot(N, L);
            float cosHN = glm::dot(H, N);
            float cosHL = glm::dot(H, L); float cosHV = glm::dot(H, V);
            
            // ------------ CONDUCTOR ------------
            float r = _roughness * _roughness;
            float r2 = r * r;

            Vec3f F = schlick(_baseColor, VEC3F_ONE, cosHL);
            float D = r2 / glm::max(1e-5f,(PIf * pow2(cosHN * cosHN * (r2 - 1.f) + 1.f)));
            float G1 = (2.f * cosNV) / glm::max(1e-5f, (cosNV + glm::sqrt((cosNV - cosNV * r2) * cosNV + r2)));
            float V2 = 0.5f / glm::max(1e-5f,(cosNL * glm::sqrt((cosNV - cosNV * r2) * cosNV + r2) + cosNV * glm::sqrt((cosNL - cosNL * r2) * cosNL + r2)));
            Vec3f conductor = F * D * V2;
            if(p_pdfWeighted) conductor /= glm::max(1e-5f,D*G1/glm::max(1e-5f,4.f*cosNV));

            // ------------ DIELECTRIC ------------
            float f90 = _roughness * (2.f * cosHL * cosHL + 0.5f);
            Vec3f dielectric = _baseColor * schlick(1.f, f90, cosNL)* schlick(1.f, f90, cosNV)* (1.f - _roughness * 0.51f / 1.51f) / PIf;
            if (p_pdfWeighted) dielectric /= cosNL*INV_PIf;

            return glm::mix(dielectric, conductor, _metalness) * cosNL;
        }

        Vec3f evaluateBTDF(const Vec3f& V, const Vec3f& N, const Vec3f& H, const Vec3f& L, float p_ni, float p_no ,bool p_pdfWeighted) {
            float cosNV = glm::max(1e-5f,glm::abs(glm::dot(N, V))); // max 1e-5f avoid artefacts
            float cosNL = glm::dot(N, L);
            float cosHN = glm::dot(H, N);
            float cosHL = glm::dot(H, L);
            float cosHV = glm::abs(glm::dot(H, V));
            
            float r = _roughness * _roughness;
            float r2 = r * r;
            
            if (p_pdfWeighted) {
                float tmpV = glm::sqrt(r2+(1.f-r2)*cosNV*cosNV);
                float tmpL = glm::sqrt(r2+(1.f-r2)*cosNL*cosNL);
                return _baseColor*2.f*cosNL / glm::max(1e-5f, (cosNL+tmpL)*(1.f+0.5f*(tmpL/cosNL-1.f)+0.5f*(tmpV/cosNV-1.f)));
            }

            //float g = glm::sqrt(glm::max(0.f,pow2(p_no/p_ni)-1.f+cosHV*cosHV)); // !0 wtf
            //float F = 0.5f*fresnel(g,cosHV)*(1.f+pow2((cosHL*(g+cosHL)-1.f)/(cosHL*(g-cosHL)+1.f)));
            //Vec3f transmit = _baseColor *4.f *(1.f-F)*D*V2 *p_no*p_no *cosHV*cosHL / glm::max(1e-5f,pow2(p_ni*cosHV + p_no*cosHL));
            //float pdf = glm::max(1e-5f, D * G1 / glm::max(1e-5f, 4.f * cosNV));
            return _baseColor * cosNL / PIf;
        }

        Ray evaluateBSDF(const Ray& p_ray, const HitRecord& p_hitRecord, float p_ni, float p_no, Vec3f& p_rayColor) {
           Vec3f H = sampleGGXVNDF(-p_ray.getDirection(),p_hitRecord._normal,_roughness,_roughness);

            float F = schlick(fresnel(p_ni, p_no), 1.f, glm::dot(p_hitRecord._normal,-p_ray.getDirection()));
            float reflectivity = 1.f-/*_transmitness**/(1.f-_metalness)*(1.f-F);
            
            if (randomFloat() <= reflectivity || glm::length(glm::refract(p_ray.getDirection(), H, p_ni/p_no))==0.f) {
                Ray reflectRay = Ray(p_hitRecord._point, glm::normalize(glm::reflect(p_ray.getDirection(), H)));
                reflectRay.offset(p_hitRecord._normal);

                if(glm::dot(reflectRay.getDirection(), p_hitRecord._normal) < 0.f) { 
                    p_rayColor = VEC3F_ZERO;
                } else {
                    p_rayColor *= evaluateBRDF(-p_ray.getDirection(), p_hitRecord._normal, H, reflectRay.getDirection(), true);
                }
                return reflectRay;
            } else {
                Ray refractRay = Ray(p_hitRecord._point, glm::normalize(glm::refract(p_ray.getDirection(), H, p_ni / p_no)));
                refractRay.offset(-p_hitRecord._normal);

                if (glm::dot(refractRay.getDirection(), -p_hitRecord._normal) < 0.f) {
                    p_rayColor = VEC3F_ZERO;
                } else {
                    p_rayColor *= evaluateBTDF(-p_ray.getDirection(), -p_hitRecord._normal, -H, refractRay.getDirection(), p_ni, p_no, true);
                }
                return refractRay;
            }
        }

    private:
        // fresnel for normal incidence cosTi = cosTo = 1
        float fresnel(float ni,float no) {
            return pow2((ni - no) / (ni + no));
        }

        float schlick(float f0, float f90, float cosT) {
            return f0 + (f90 - f0) * pow5(1.f-cosT);
        }

        Vec3f schlick(Vec3f f0, Vec3f f90, float cosT) {
            return f0 + (f90 - f0) * pow5(1.f-cosT);
        }

        Vec3f sampleGGXVNDF(Vec3f V, Vec3f N, float rx, float ry) {
            float s = (N.z>=0.) ? 1.f : -1.f;
            float a = -1.f/(s+N.z);
            float b = N.x*N.y*a;
            Vec3f T = Vec3f(1.f+s*N.x*N.x*a, s*b, -s*N.x);
            Vec3f B = Vec3f(b, s+N.y*N.y*a, -N.y);

            V = Vec3f(glm::dot(V, T), glm::dot(V, B), glm::dot(V, N));
            Vec3f Vh = glm::normalize(V * Vec3f(rx, ry, 1.f));

            float phi = TWO_PIf * randomFloat();
            float z = (1.f-randomFloat())*(1.f+Vh.z)-Vh.z;
            float sinTheta = glm::sqrt(glm::clamp(1.f - z*z, 0.f, 1.f));
            Vec3f Nh = glm::normalize(Vec3f(sinTheta*glm::cos(phi), sinTheta*glm::sin(phi), z) + Vh);
            Vec3f Ne = glm::normalize(Nh * Vec3f(rx, ry, 1.f));

            return Ne.x * T + Ne.y * B + Ne.z * N;
        }

    private:
        // ----------------------------------------------------- ATTRIBUTS -----------------------------------------------------
        Vec3f _baseColor = VEC3F_ONE;
        Vec3f _emissiveColor = VEC3F_ZERO;
        // float transmitness

        float _emissiveStrength = 0.f;
        float _metalness = 0.f;
        float _roughness = 1.f;
        float _ior = 1.5;
    };
}

#endif