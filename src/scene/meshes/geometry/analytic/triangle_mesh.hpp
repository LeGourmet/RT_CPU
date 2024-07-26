#ifndef __TRIANGLE_MESH_HPP__
#define __TRIANGLE_MESH_HPP__

#include "triangle.hpp"
#include "accelerators/bvh.hpp"
#include "scene/meshes/mesh.hpp"

#include <vector>

namespace RT_CPU
{
	class TriangleMesh : public Mesh
	{
	public:
		TriangleMesh(std::vector<Vec3f>& p_vertices, std::vector<Vec3f> &p_normals, std::vector<unsigned int>& p_indices) : Mesh() {
			_vertices = p_vertices;
			_normals = p_normals;
			
			for(unsigned int i=0; i<p_indices.size() ;i+=3)
				_triangles.push_back(new Triangle(
					_vertices[p_indices[i]], _vertices[p_indices[i+1]], _vertices[p_indices[i+2]],
					_normals [p_indices[i]], _normals [p_indices[i+1]], _normals [p_indices[i+2]]
				));

			_bvh.build(&_triangles);
			_aabb.extend(_bvh.getRootAABB());
		}

		~TriangleMesh() {
			for(unsigned int i=0; i<_triangles.size() ;i++) delete _triangles[i];
		}

		inline bool intersect(const Ray& p_ray, const float p_tMin, const float p_tMax, HitRecord& p_hitRecord) const override { 
			bool hit = _bvh.intersect(p_ray, p_tMin, p_tMax, p_hitRecord); 
			if(hit) p_hitRecord._mesh = this;
			return hit;
		}

		inline bool intersectAny(const Ray& p_ray, const float p_tMin, const float p_tMax) const override { 
			return _bvh.intersectAny(p_ray, p_tMin, p_tMax); 
		}

	private:
		BVH					_bvh;
		std::vector<Vec3f>	_vertices;
		std::vector<Vec3f>	_normals;
		std::vector<Mesh*>	_triangles;
	};
}

#endif