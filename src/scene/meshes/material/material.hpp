#ifndef __MATERIAL_HPP__
#define __MATERIAL_HPP__

#include "utils/random.hpp"
#include "utils/defines.hpp"

#include "ray/ray.hpp"
#include "ray/hit_record.hpp"
#include "scene/lights/light_sample.hpp"

#define pow2(a) (a)*(a)
#define pow5(a) (a)*(a)*(a)*(a)*(a)

#include <iostream>

namespace RT_CPU
{
    class Material
    {
    public:
        // --------------------------------------------- DESTRUCTOR / CONSTRUCTOR ----------------------------------------------
        Material(const Vec4f& p_albedo, float p_alphaCutoff, const Vec3f& p_emissiveColor, float p_emissiveStrength, float p_metalness, float p_roughness, float p_transmitness, float p_anisotropy, const Vec3f& p_absorptionColor, float p_absorptionDensity, float p_ior) :
            _albedo(p_albedo), _alphaCutoff(p_alphaCutoff), _emissiveColor(p_emissiveColor), _emissiveStrength(p_emissiveStrength), _metalness(p_metalness), _roughness(p_roughness), _transmitness(p_transmitness), _anisotropy(p_anisotropy), _absorptionColor(p_absorptionColor), _absorptionDensity(p_absorptionDensity), _ior(p_ior) { }
        ~Material() {}

        inline const float getIOR() const { return _ior; }
        inline const Vec3f getEmissivity() const { return _emissiveColor*_emissiveStrength; }   
        inline const Vec3f getAbsoptivity(float p_d) const { return glm::pow(_absorptionColor,Vec3f(_absorptionDensity)/glm::max(1e-5f,p_d)); }      
        //const Vec3f getAbsoptivity(float p_d) const { return glm::exp(-(1.f-_absorptionColor)/glm::max(1e-5f,_absorptionDensity) * p_d);}

        Vec3f evaluateBSDF(const Vec3f& V, const Vec3f& N, const Vec3f& T, const Vec3f& B, const Vec3f& L, float p_ni, float p_no, float& p_pdf) {
            Vec3f baseColor = Vec3f(_albedo);
            float alpha = _albedo.a;
            
            float roughness = glm::clamp(_roughness, 0.04f, 0.999f);
            float r = roughness * roughness; //float r2 = r * r;
            float rt = glm::max(0.0016f, r*(1.f+_anisotropy));
            float rb = glm::max(0.0016f, r*(1.f-_anisotropy));
            
            Vec3f H = (glm::dot(N, L)<0.f) ? -(p_no * V + p_ni * L) : V + L;
            if(glm::dot(H,H)==0.f) H = V;
            H = glm::normalize(H);
            if(glm::dot(H,V)<0.f) H = -H;
            
            float cosNV = glm::max(1e-5f, glm::abs(glm::dot(N, V)));
            float cosNH = glm::clamp(glm::dot(N, H), 0.f, 1.f-1e-10f);
            float cosHV = glm::max(0.f, glm::dot(H, V));
            float DielF = schlick(fresnel(p_ni, p_no), 1.f, cosHV);
            
            float cosTL = glm::dot(T,L);
            float cosTV = glm::dot(T,V);
            float cosBL = glm::dot(B,L);
            float cosBV = glm::dot(B,V);
            float cosBH = glm::dot(B,H);
            float cosTH = glm::dot(T,H);

            float diffuseRate = (1.f - _metalness) * (1.f - _transmitness);
            float specularReflectionRate = DielF;
            float specularRefractionRate = _transmitness * (1.f - _metalness) * (1.f - DielF);
            float totalRate = diffuseRate + specularReflectionRate + specularRefractionRate;

            if(glm::dot(N,L)<0.f){
                float cosNL = glm::max(0.f, glm::dot(-N, L));
                float cosHL = glm::max(0.f, glm::dot(-H, L));     

                // ------------ SPECULAR REFRACTION ------------
                float D = 1.f/glm::max(1e-10f,(PIf*rt*rb*pow2(pow2(cosTH/rt)+pow2(cosBH/rb)+cosNH*cosNH))); //r2/glm::max(1e-10f, (PIf*pow2((cosNH*cosNH)*(r2-1.f)+1.f)));
                float XL = glm::sqrt(rt*rt*cosTL*cosTL + rb*rb*cosBL*cosBL + cosNL*cosNL); //glm::sqrt(r2 + (1.f-r2) * cosNL * cosNL);
                float XV = glm::sqrt(rt*rt*cosTV*cosTV + rb*rb*cosBV*cosBV + cosNV*cosNV); //glm::sqrt(r2 + (1.f-r2) * cosNV * cosNV);
                float G1L = 2.f * cosNL / glm::max(1e-5f, (cosNL + XL));
                float V2 = 2.f/glm::max(1e-5f, cosNL*XV + cosNV*XL);

                Vec3f specularRefraction = glm::sqrt(baseColor) * (1.f-DielF) * D * V2 * cosHL * cosHV / pow2(cosHV + (p_no/p_ni)*cosHL);
                float pdfSpecularRefraction = D * G1L * cosHV * cosHL * pow2(p_no/p_ni) / glm::max(1e-5f,(cosNV*pow2(cosHV+(p_no/p_ni)*cosHL)));

                // (_alphaCutoff!=0.f && !(_alphaCutoff==1.f && randomFloat()<alpha) && _alphaCutoff>=alpha)
                p_pdf = (pdfSpecularRefraction*specularRefractionRate)/totalRate;

                return alpha * (specularRefraction*specularRefractionRate) / totalRate * cosNL;
            }
            
            float cosNL = glm::max(0.f, glm::dot(N, L));
            float cosHL = glm::max(0.f, glm::dot(H, L));

            // ------------ SPECULAR REFLECTION ------------
            Vec3f f0 = glm::mix(VEC3F_ONE, baseColor, _metalness);
            Vec3f F = schlick(f0, VEC3F_ONE, cosHV);
            float D = 1.f/glm::max(1e-10f,(PIf*rt*rb*pow2(pow2(cosTH/rt)+pow2(cosBH/rb)+cosNH*cosNH))); //r2/glm::max(1e-10f, (PIf*pow2((cosNH*cosNH)*(r2-1.f)+1.f)));
            float XL = glm::sqrt(rt*rt*cosTL*cosTL + rb*rb*cosBL*cosBL + cosNL*cosNL); //glm::sqrt(r2 + (1.f-r2) * cosNL * cosNL);
            float XV = glm::sqrt(rt*rt*cosTV*cosTV + rb*rb*cosBV*cosBV + cosNV*cosNV); //glm::sqrt(r2 + (1.f-r2) * cosNV * cosNV);
            float G1V = 2.f * cosNV / glm::max(1e-5f, (cosNV + XV));
            float V2 = 0.5f/glm::max(1e-5f, cosNL*XV + cosNV*XL);
            
            Vec3f specularReflection = F * D * V2;
            float pdfSpecularReflection = D * G1V / (4.f*cosNV);

            // ------------ DIFFUSE ------------
            float Rr = 2.f*r * cosHL*cosHL + 0.5f;
            float Fl = pow5(1.f-cosNL);
            float Fv = pow5(1.f-cosNV);

            Vec3f diffuse = baseColor * ((1.f-0.5f*Fl) * (1.f-0.5f*Fv) + Rr*(Fl+Fv+Fl*Fv*(Rr-1.f))) * INV_PIf;
            float pdfDiffuse = cosNL * INV_PIf;

            // (_alphaCutoff!=0.f && !(_alphaCutoff==1.f && randomFloat()<alpha) && _alphaCutoff>=alpha)
            p_pdf = (pdfDiffuse*diffuseRate + pdfSpecularReflection*specularReflectionRate) / totalRate;

            return alpha * (diffuse*diffuseRate + specularReflection*specularReflectionRate) / totalRate * cosNL;
        }

        Ray sampleBSDF(const Ray& p_ray, const HitRecord& p_hitRecord, float p_ni, float p_no, Vec3f& p_rayColor) {           
            Vec3f baseColor = Vec3f(_albedo);
            float alpha = _albedo.a;

            float roughness = glm::clamp(_roughness, 0.04f, 0.999f);
            float r = roughness * roughness; //float r2 = r * r;
            float rt = glm::max(0.0016f, r*(1.f+_anisotropy));
            float rb = glm::max(0.0016f, r*(1.f-_anisotropy));
            
            Vec3f V = -p_ray.getDirection();
            Vec3f N = p_hitRecord._normal;
            Vec3f T = p_hitRecord._tangent;
            Vec3f B = p_hitRecord._bitangent;
            Vec3f H = sampleGGXVNDF(V,N,r,r);

            float cosNV = glm::max(1e-5f, glm::abs(glm::dot(N, V)));
            float cosHV = glm::max(0.f, glm::dot(H, V));
            float DielF = schlick(fresnel(p_ni, p_no), 1.f, cosHV);
            
            float cosTV = glm::dot(T, V);
            float cosBV = glm::dot(B, V);

            float diffuseRate = (1.f-_metalness) * (1.f-_transmitness);
            float specularReflectionRate = DielF;
            float specularRefractionRate = _transmitness * (1.f-_metalness) * (1.f-DielF);
            float GlobalRate = randomFloat();
                        
            // --- alpha ---
            if (_alphaCutoff!=0.f && !(_alphaCutoff==1.f && randomFloat()<alpha) && _alphaCutoff>=alpha) {
                Ray alphaRay = Ray(p_hitRecord._point, p_ray.getDirection());
                alphaRay.offset(-p_hitRecord._normal);
                return alphaRay;
            }

            // --- specular refraction ---
            if (GlobalRate <= specularRefractionRate) {
                Vec3f refractRayDir = glm::refract(p_ray.getDirection(), H, p_ni / p_no);
                if (!(refractRayDir.x == 0.f && refractRayDir.y == 0.f && refractRayDir.z == 0.f)) {
                    Ray refractRay = Ray(p_hitRecord._point, glm::normalize(refractRayDir));
                    refractRay.offset(-p_hitRecord._normal);

                    float cosNL = glm::max(0.f, glm::dot(-N, refractRay.getDirection()));
                    float cosTL = glm::dot(T, refractRay.getDirection());
                    float cosBL = glm::dot(B, refractRay.getDirection());
                    
                    float XL = glm::sqrt(rt*rt*cosTL*cosTL + rb*rb*cosBL*cosBL + cosNL*cosNL); //glm::sqrt(r2 + (1.f-r2) * cosNL * cosNL);
                    float XV = glm::sqrt(rt*rt*cosTV*cosTV + rb*rb*cosBV*cosBV + cosNV*cosNV); //glm::sqrt(r2 + (1.f-r2) * cosNV * cosNV);
                    p_rayColor *= glm::sqrt(baseColor) * (1.f-DielF) * pow2(p_ni/p_no) * cosNV * (cosNL+XL) / (cosNV*XL + cosNL*XV); // sqrt(col) * (1-DielF) * pow2(p_ni/p_no) * G2 / G1L

                    return refractRay;
                }
            }

            // --- specular reflection ---
            if (GlobalRate <= specularReflectionRate / (diffuseRate + specularReflectionRate)) {
                Ray reflectRay = Ray(p_hitRecord._point, glm::normalize(glm::reflect(p_ray.getDirection(), H)));
                reflectRay.offset(p_hitRecord._normal);

                float cosNL = glm::max(0.f, glm::dot(N, reflectRay.getDirection()));
                float cosTL = glm::dot(T, reflectRay.getDirection());
                float cosBL = glm::dot(B, reflectRay.getDirection());

                Vec3f f0 = glm::mix(VEC3F_ONE, baseColor, _metalness);
                Vec3f F = schlick(f0, VEC3F_ONE, cosHV);
                float XL = glm::sqrt(rt*rt*cosTL*cosTL + rb*rb*cosBL*cosBL + cosNL*cosNL); //glm::sqrt(r2 + (1.f-r2) * cosNL * cosNL);
                float XV = glm::sqrt(rt*rt*cosTV*cosTV + rb*rb*cosBV*cosBV + cosNV*cosNV); //glm::sqrt(r2 + (1.f-r2) * cosNV * cosNV);
                p_rayColor *= F * cosNL * (cosNV+XV) / (cosNV*XL + cosNL*XV); // F * G2 / G1V
                
                return reflectRay; 
            }

            // --- diffuse ---
            Ray diffuseRay = Ray(p_hitRecord._point, sampleHemisphere(N));
            diffuseRay.offset(p_hitRecord._normal);

            float cosNL = glm::max(0.f, glm::dot(N, diffuseRay.getDirection()));
            float cosHL = glm::max(0.f, glm::dot(glm::normalize(diffuseRay.getDirection() + V), diffuseRay.getDirection()));

            float Rr = 2.f*r * cosHL*cosHL + 0.5f;
            float Fl = pow5(1.f-cosNL);
            float Fv = pow5(1.f-cosNV);

            p_rayColor *= baseColor * ((1.f-0.5f*Fl) * (1.f-0.5f*Fv) + Rr*(Fl+Fv + Fl*Fv*(Rr-1.f)));

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
        Vec4f _albedo                   = VEC4F_ONE;    // [0;1] => rgb + a
        float _alphaCutoff              = 0.f;          // [0;1] => 0:Opaque / 1:Blend / ]0;1[:Mask
        
        Vec3f _emissiveColor            = VEC3F_ZERO;   // [0;1] => rgb
        float _emissiveStrength         = 0.f;          // [0;+inf[         (sould be lumen ?)

        float _metalness                = 0.f;          // [0;1]
        float _roughness                = 1.f;          // [0;1]
        float _transmitness             = 0.f;          // [0;1]
        
        // https://github.com/KhronosGroup/glTF/blob/main/extensions/2.0/Khronos/KHR_materials_clearcoat/README.md
        //float _coatness               = 0.f;
        //float _coatRoughness          = 1.f;
        //float _coatIor                = 1.5f;
        //Vec3f _coatTint               = VEC3F_ONE;
        
        // https://github.com/KhronosGroup/glTF/blob/main/extensions/2.0/Khronos/KHR_materials_sheen/README.md
        //float _sheenness              = 0.f; // _sheeness = 0 if _sheenColor = VEC3F_ZERO and 1.f else
        //float _sheenRoughness         = 1.f;
        //Vec3f _sheenColor             = VEC3F_ONE;

        // https://github.com/KhronosGroup/glTF/blob/main/extensions/2.0/Khronos/KHR_materials_anisotropy/README.md
        float _anisotropy               = 0.f;          // [-1;1]

        Vec3f _absorptionColor          = VEC3F_ONE;    // [0;1] => rgb
        float _absorptionDensity        = 0.f;          // [0;+inf[ => atDistance

        float _ior                      = 1.f;          // ]0;+inf[         (should be decompose in 3 value ? n,k,i) (metamateriaux peuvent avoir des indices negatifs !)

        /*
        Texture* _albedoMap             = nullptr;
        Texture* _metalnessRoughnessMap = nullptr;  // sould merge MetalnessRoughnessTransmitnessMap
        Texture* _transmitnessMap       = nullptr;  // should be merge with MetalRoughnessMap
        Texture* _normalMap             = nullptr;  // +normal map scale
        Texture* _occlusionMap          = nullptr;  // should be not used
        Texture* _emissiveMap           = nullptr;
        Texture* _anisotropyMap         = nullptr;  // (red and green => direction t and b and blue the anisotropyStrength to multiply by anisotropy)
        Texture* _coatMap               = nullptr;  // clearcoat = clearcoatFactor * clearcoatTexture.r
        Texture* _coatRougnessMap       = nullptr;  // clearcoatRoughness = clearcoatRoughnessFactor * clearcoatRoughnessTexture.g
        Texture* _coatNormalMap         = nullptr; 
        Texture* _sheenColorMap         = nullptr;  // sheenColor = sheenColorFactor * sampleLinear(sheenColorTexture).rgb
        Texture* _sheenRougnessMap      = nullptr;  // sheenRoughness = sheenRoughnessFactor * sample(sheenRoughnessTexture).a
        */
    };
}

#endif