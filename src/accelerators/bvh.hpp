#ifndef __BVH_HPP__
#define __BVH_HPP__

#include "aabb.hpp"
#include "ray/hit_record.hpp"
#include "scene/meshes/mesh.hpp"

#include <vector>

namespace RT_CPU
{
	struct Bucket {
		int _nb = 0;
		AABB _aabb;
	};

	struct BVHNode
	{
		BVHNode() {}
		~BVHNode() { delete _left; delete _right; }
		bool isLeaf() const { return (_left == nullptr && _right == nullptr); }

		AABB			_aabb;
		BVHNode*		_left			= nullptr;
		BVHNode*		_right			= nullptr;
		unsigned int	_firstMeshId	= 0;
		unsigned int	_lastMeshId	= 0;
	};

	class BVH
	{
	public:
		BVH() {}
		~BVH() { delete _root; }

		inline AABB getRootAABB() { return _root->_aabb; }

		void build(std::vector<Mesh*>* p_meshes);
		bool intersect(const Ray& p_ray, const float p_tMin, const float p_tMax, HitRecord& p_hitRecord) const;
		bool intersectAny(const Ray& p_ray, const float p_tMin, const float p_tMax) const;

	private:
		void _buildRec(BVHNode* p_node, unsigned int p_firstMeshId, unsigned int p_lastMeshId, unsigned int p_depth);
		bool _intersectRec(const BVHNode* p_node, const Ray& p_ray, float p_tMin, float p_tMax, HitRecord& p_hitRecord) const;
		bool _intersectAnyRec(const BVHNode* p_node, const Ray& p_ray, float p_tMin, float	p_tMax) const;

	private:
		BVHNode* _root = nullptr;
		std::vector<Mesh*>* _meshes = nullptr;

		static const unsigned int _maxMeshesPerLeaf = 8;
		static const unsigned int _maxDepth = 32;
	};
}

#endif