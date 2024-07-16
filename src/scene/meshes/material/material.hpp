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
        Material(const Vec3f& p_baseColor, const Vec3f& p_emissiveColor, float p_emissiveStrength, float p_metalness, float p_roughness, float p_transmitness, float p_ior) :
            _baseColor(p_baseColor), _emissiveColor(p_emissiveColor), _emissiveStrength(p_emissiveStrength), _metalness(p_metalness), _roughness(p_roughness), _transmitness(p_transmitness), _ior(p_ior) { }
        ~Material() {}

        inline const float getIOR() const { return _ior; }
        inline const Vec3f getEmissivity() const { return _emissiveColor*_emissiveStrength; }

        /*
        // https://pbr-book.org/3ed-2018/Monte_Carlo_Integration/Importance_Sampling
        float powerHeuristic(int nf, float fPdf, int ng, float gPdf){
            float f = float(nf) * fPdf, g = float(ng) * gPdf;
            return (f * f) / (f * f + g * g);
        }

        light pdf => dist_sq(point,pos) / (aire*cos(N,L))

        vec3 direct    = vec3( 0. );
        LightSample lightSample = sampleLight(pposition);

        vec3  wiLocal  = normalize( multiply(transform, lightSample.wi) );
        float cosTheta = wiLocal.z;
        if( lightSample.pdf > 0. && cosTheta > 0. ) {
            if( trace(pposition, lightSample.wi).t < 0. ) {
               vec3 brdf = evalBSDF(hit, woLocal, wiLocal) * cosTheta;

               float scatteringPdf = getPDFBSDF( hit, woLocal, wiLocal );
               float weight		= powerHeuristic( 1, lightSample.pdf, 1, scatteringPdf );

               direct += brdf * lightSample.intensity * weight / max(1e-4, lightSample.pdf);
            }
        }
        */

        /*float getDiffusePDF(const Vec3f& V, const Vec3f& N, const Vec3f& H, const Vec3f& L) {
            float cosNL = glm::max(0.f, glm::dot(N, L));

            return cosNL * INV_PIf;
        }
        
        float getSpecularReflectPDF(const Vec3f& V, const Vec3f& N, const Vec3f& H, const Vec3f& L) {
            float roughness = glm::clamp(_roughness, 0.01f, 0.99f);
            float r = roughness * roughness;
            float r2 = r * r;
            
            float cosNV = glm::max(1e-5f, glm::abs(glm::dot(N, V)));
            float cosNL = glm::max(0.f, glm::dot(N, L));
            float cosNH = glm::max(0.f, glm::dot(N, H));

            float XL = glm::sqrt(r2 + (1.f - r2) * cosNL * cosNL);
            float G1L = 2.f * cosNL / glm::max(1e-5f, (cosNL + XL));
            float D = r2 / glm::max(1e-5f,(PIf*pow2((cosNH*cosNH)*(r2-1.f)+1.f)));
            
            return D * G1L / glm::max(1e-5f,4.f*cosNV);
        }

        float getSpecularRefractPDF() { return 1.f; }*/

        Vec3f evaluateBRDF(const Vec3f& V, const Vec3f& N, const Vec3f& L, float p_ni, float p_no) {
            float roughness = glm::clamp(_roughness, 0.01f, 0.99f);
            float r = roughness * roughness;
            float r2 = r * r;
            
            Vec3f H = glm::normalize(V+L);

            float cosNV = glm::max(1e-5f,glm::dot(N, V));
            float cosNL = glm::max(0.f,glm::dot(N, L));
            float cosNH = glm::clamp(glm::dot(N, H), 0.f, 1.f-1e-10f);
            float cosHL = glm::max(0.f,glm::dot(H, L));
            float cosHV = glm::max(0.f,glm::dot(H, V));

            // ------------ SPECULAR ------------
            Vec3f f0 = glm::mix(VEC3F_ONE, _baseColor, _metalness);
            Vec3f F = schlick(f0, VEC3F_ONE, cosHV);
            float D = r2/glm::max(1e-10f,(PIf*pow2((cosNH*cosNH)*(r2-1.f)+1.f)));
            float V2 = 0.5f/glm::max(1e-5f,(cosNL*glm::sqrt(r2+(1.f-r2)*cosNV*cosNV) + cosNV*glm::sqrt(r2+(1.f-r2)*cosNL*cosNL)));
            //float V2 = 0.5f/glm::max(1e-5f,glm::mix(2.f*cosNL*cosNV,cosNL+cosNV,r));
            Vec3f specular = F * D * V2;
            
            // ------------ DIFFUSE ------------
            float Rr = r * 2.f * cosHL * cosHL + 0.5f;
            float Fl = pow5(1.f - cosNL);
            float Fv = pow5(1.f - cosNV);
            Vec3f diffuse = _baseColor * ((1.f - 0.5f * Fl) * (1.f - 0.5f * Fv) + Rr * (Fl + Fv + Fl * Fv * (Rr - 1.f))) * INV_PIf;
            
            float DielF = schlick(fresnel(p_ni, p_no), 1.f, cosHV);

            float diffuseRate = (1.f - _metalness) * (1.f - _transmitness);
            float specularReflectionRate = DielF;
            
            return (diffuse*diffuseRate+ specular*specularReflectionRate) / (diffuseRate+specularReflectionRate) * cosNL;
        }

        /*Vec3f evaluateBTDF(const Vec3f& V, const Vec3f& N, const Vec3f& H, const Vec3f& L, float p_ni, float p_no, bool p_pdfWeighted) {
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
        }*/

        Ray evaluateWeightedBSDF(const Ray& p_ray, const HitRecord& p_hitRecord, float p_ni, float p_no, Vec3f& p_rayColor) {
            float roughness = glm::clamp(_roughness, 0.01f, 0.99f);
            float r = roughness * roughness;
            float r2 = r * r;

            Vec3f V = -p_ray.getDirection();
            Vec3f N = p_hitRecord._normal;
            Vec3f H = sampleGGXVNDF(V,N,r,r);

            float DielF = schlick(fresnel(p_ni, p_no), 1.f, glm::max(0.f, glm::dot(H, V)));
            float cosNV = glm::max(1e-5f, glm::abs(glm::dot(N, V)));

            float diffuseRate = (1.f-_metalness) * (1.f-_transmitness);
            float specularReflectionRate = DielF;
            float specularRefractionRate = _transmitness * (1.f - _metalness) * (1.f - DielF);
            float GlobalRate = randomFloat();
                        
            // --- specular refraction ---
            if (GlobalRate <= specularRefractionRate) {
                Vec3f refractRayDir = glm::refract(p_ray.getDirection(), H, p_ni / p_no);
                if (!(refractRayDir.x == 0.f && refractRayDir.y == 0.f && refractRayDir.z == 0.f)) {
                    Ray refractRay = Ray(p_hitRecord._point, glm::normalize(refractRayDir));
                    refractRay.offset(-p_hitRecord._normal);

                    float cosNL = glm::max(0.f, glm::dot(-N, refractRay.getDirection()));

                    float XL = glm::sqrt(r2 + (1.f - r2) * cosNL * cosNL);
                    float XV = glm::sqrt(r2 + (1.f - r2) * cosNV * cosNV);
                    float G1L = 2.f * cosNL / glm::max(1e-5f, (cosNL + XL));
                    float G2 = 2.f * cosNL * cosNV / glm::max(1e-5f, (cosNV * XL + cosNL * XV));

                    p_rayColor *= glm::sqrt(_baseColor) * (1.f - DielF) * G2 * pow2(p_ni / p_no) / glm::max(1e-5f, G1L);
                    
                    return refractRay;
                }
            }

            // --- specular reflection ---
            if (GlobalRate <= specularReflectionRate / (diffuseRate + specularReflectionRate)) {
                Ray reflectRay = Ray(p_hitRecord._point, glm::normalize(glm::reflect(p_ray.getDirection(), H)));
                reflectRay.offset(p_hitRecord._normal);

                float cosNL = glm::max(0.f, glm::dot(N, reflectRay.getDirection()));
                float cosHV = glm::max(0.f, glm::dot(H, V));

                Vec3f f0 = glm::mix(VEC3F_ONE, _baseColor, _metalness);
                Vec3f F = schlick(f0, VEC3F_ONE, cosHV);
                float XL = glm::sqrt(r2 + (1.f - r2) * cosNL * cosNL);
                float XV = glm::sqrt(r2 + (1.f - r2) * cosNV * cosNV);
                float G1V = 2.f * cosNV / glm::max(1e-5f, (cosNV + XV));
                float G2 = 2.f * cosNL * cosNV / glm::max(1e-5f, (cosNV * XL + cosNL * XV));

                p_rayColor *= F * G2 / glm::max(1e-5f, G1V);
                
                return reflectRay;
            }

            // --- diffuse ---
            Ray diffuseRay = Ray(p_hitRecord._point, sampleHemisphere(N));
            diffuseRay.offset(p_hitRecord._normal);

            float cosNL = glm::max(0.f, glm::dot(N, diffuseRay.getDirection()));
            float cosHL = glm::max(0.f, glm::dot(glm::normalize(diffuseRay.getDirection() + V), diffuseRay.getDirection()));

            float Rr = r * 2.f * cosHL * cosHL + 0.5f;
            float Fl = pow5(1.f - cosNL);
            float Fv = pow5(1.f - cosNV);

            p_rayColor *= _baseColor * ((1.f - 0.5f * Fl) * (1.f - 0.5f * Fv) + Rr * (Fl + Fv + Fl * Fv * (Rr - 1.f)));

            return diffuseRay;
        }

    private:
        // fresnel for normal incidence cosTi = cosTo = 1
        float fresnel(float ni, float no)                const { return pow2((ni - no) / (ni + no)); }
        float schlick(float f0, float f90, float cosT)  const { return f0 + (f90 - f0) * pow5(1.f - cosT); }
        Vec3f schlick(Vec3f f0, Vec3f f90, float cosT)  const { return f0 + (f90 - f0) * pow5(1.f - cosT); }

        Vec3f sampleGGXVNDF(Vec3f V, Vec3f N, float rx, float ry) const {
            float s = (N.z >= 0.) ? 1.f : -1.f;
            float a = -1.f / (s + N.z);
            float b = N.x * N.y * a;
            Vec3f T = Vec3f(1.f + s * N.x * N.x * a, s * b, -s * N.x);
            Vec3f B = Vec3f(b, s + N.y * N.y * a, -N.y);

            V = Vec3f(glm::dot(V, T), glm::dot(V, B), glm::dot(V, N));
            Vec3f Vh = glm::normalize(V * Vec3f(rx, ry, 1.f));

            float phi = TWO_PIf * randomFloat();
            float z = (1.f - randomFloat()) * (1.f + Vh.z) - Vh.z;
            float sinTheta = glm::sqrt(glm::clamp(1.f - z * z, 0.f, 1.f));
            Vec3f Nh = glm::normalize(Vec3f(sinTheta * glm::cos(phi), sinTheta * glm::sin(phi), z) + Vh);
            Vec3f Ne = glm::normalize(Nh * Vec3f(rx, ry, 1.f));

            return glm::normalize(Ne.x * T + Ne.y * B + Ne.z * N);
        }

        Vec3f sampleHemisphere(Vec3f N) const {
            float a = TWO_PIf * randomFloat();
            float b = 2.f * randomFloat() - 1.f;
            return glm::normalize(N + Vec3f(glm::sqrt(1.f - b * b) * Vec2f(glm::cos(a), glm::sin(a)), b));
        }

    private:
        // ----------------------------------------------------- ATTRIBUTS -----------------------------------------------------
        Vec3f _baseColor = VEC3F_ONE;
        Vec3f _emissiveColor = VEC3F_ZERO;
        
        float _emissiveStrength = 0.f;
        float _metalness = 0.f;
        float _roughness = 1.f;
        float _transmitness = 0.f;
        float _ior = 1.f;
    };
}

#endif