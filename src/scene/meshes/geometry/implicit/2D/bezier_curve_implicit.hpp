#ifndef __BEZIER_CURVE_IMPLICIT_HPP__
#define __BEZIER_CURVE_IMPLICIT_HPP__

#include "scene/meshes/geometry/implicit/mesh_implicit.hpp"

namespace RT_CPU
{
	class BezierCurveImplicit
	{
	public:
		BezierCurveImplicit(const Vec2f p_v1, const Vec2f p_control, const Vec3f p_v2) : _v1(p_v1), _control(p_control), _v2(p_v2) {}
		~BezierCurveImplicit() { }

	protected:
		float _evaluate(const Vec2f& p_point) const {
			Vec2f control = glm::mix(_control + Vec2f(1e-3f), _control, glm::abs(glm::sign(_control*2.f-_v1-_v2)));
    
            Vec2f a = control - _v1;
            Vec2f b = _v1 - 2.f*control + _v2;
            Vec2f c = a * 2.f;
            Vec2f d = _v1 - p_point;
    
            Vec3f k = Vec3f(3.f*glm::dot(a,b),2.f*glm::dot(a,a)+glm::dot(d,b),glm::dot(d,a)) / glm::dot(b,b);
    
            float p = k.y-k.x*k.x/3.f;
            float q = k.x * (2.f*k.x*k.x - 9.f*k.y)/27.f + k.z;
            float h = q*q + 4.f*p*p*p/27.f;
    
            if(h >= 0.f) { 
                float z = glm::sqrt(h);
                Vec2f x = (Vec2f(z, -z) - q) / 2.f;
                Vec2f uv = glm::sign(x)*glm::pow(glm::abs(x), Vec2f(1.f/3.f));
                float t = glm::clamp(uv.x+uv.y-k.x/3.f,0.f,1.f);
                return glm::length(_v1+(c+b*t)*t-p_point);
            }
    
            float v = glm::acos(-glm::sqrt(-27.f/(p*p*p))*q/2.f)/3.f;
            float m = glm::cos(v), n = glm::sin(v)*1.732050808f;
            Vec2f t = glm::clamp(Vec2f(m+m,-n-m)*glm::sqrt(-p/3.f)-k.x/3.f,0.f,1.f);
            return glm::min( glm::length(_v1+(c+b*t.x)*t.x-p_point),glm::length(_v1+(c+b*t.y)*t.y-p_point));
		}

	private:
		Vec2f _v1;
		Vec2f _control;
		Vec2f _v2;
	};
}

#endif