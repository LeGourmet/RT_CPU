#include "bvh.hpp"

#include "utils/chrono.hpp"

#include <algorithm>
#include <iostream>

#define NB_BUCKETS_BVH 16

namespace RT_CPU
{
	void BVH::build(std::vector<Mesh*>* p_meshes) {
		std::cout << "Building BVH..." << std::endl;

		if (p_meshes == nullptr || p_meshes->empty()) throw std::exception("BVH::build() error: no mesh provided");
		delete _root; _meshes = p_meshes;

		Chrono chr;
		chr.start();
		_buildRec(_root = new BVHNode(), 0, (unsigned int)p_meshes->size(), 0);
		chr.stop();

		std::cout << "[DONE]: " << chr.elapsedTime() << "s" << std::endl;
	}

	bool BVH::intersect(const Ray& p_ray, const float p_tMin, const float p_tMax, HitRecord& p_hitRecord) const { return _intersectRec(_root, p_ray, p_tMin, glm::min(p_tMax, p_hitRecord._distance), p_hitRecord); }

	bool BVH::intersectAny(const Ray& p_ray, const float p_tMin, const float p_tMax) const { return _intersectAnyRec(_root, p_ray, p_tMin, p_tMax); }

	void BVH::_buildRec(BVHNode* p_node, unsigned int p_firstMeshId, unsigned int p_lastMeshId, unsigned int p_depth)
	{
		for (unsigned int i = p_firstMeshId; i<p_lastMeshId;i++)
			p_node->_aabb.extend((*_meshes)[i]->getAABB());

		p_node->_firstMeshId = p_firstMeshId;
		p_node->_lastMeshId = p_lastMeshId;
		unsigned int nbTriangles = p_lastMeshId - p_firstMeshId;

		if (_maxDepth > p_depth && _maxMeshesPerLeaf < nbTriangles) {
			float invSA_P = 1.f / (p_node->_aabb.surface() * nbTriangles);
			float min_cost = FLT_MAX;
			int	  min_id, min_axis;
			for (int axis = 0; axis < 3;axis++) {
				// hashage spacial O(N) contre trie O(nlog(n)) ==> pbr-book
				Bucket buckets[NB_BUCKETS_BVH];
				for(unsigned int i=p_firstMeshId; i<p_lastMeshId ;i++) {
					int x = ((int)(((float)NB_BUCKETS_BVH) *
						((*_meshes)[i]->getAABB().centroid()[axis] - p_node->_aabb.getMin()[axis]) /
						(p_node->_aabb.getMax()[axis] - p_node->_aabb.getMin()[axis])));
					if (x >= NB_BUCKETS_BVH) x = NB_BUCKETS_BVH - 1;
					else if (x < 0) x = 0;
					buckets[x]._nb++;
					buckets[x]._aabb.extend((*_meshes)[i]->getAABB());
				}

				for (int i = 1; i < NB_BUCKETS_BVH - 1; i++) {
					Bucket left, right;

					for (int j = 0; j < i; j++) {
						left._nb += buckets[j]._nb;
						left._aabb.extend(buckets[j]._aabb);
					}

					for (int j = i; j < NB_BUCKETS_BVH; j++) {
						right._nb += buckets[j]._nb;
						right._aabb.extend(buckets[j]._aabb);
					}

					float SAH = (left._nb * left._aabb.surface() + right._nb * right._aabb.surface()) * invSA_P;
					if (min_cost > SAH) {
						min_id = i;
						min_cost = SAH;
						min_axis = axis;
					}
				}
			}

			if (min_cost >= 1.f) return; // stop if it's not worth to split

			Vec3f min = p_node->_aabb.getMin();
			Vec3f max = p_node->_aabb.getMax();
			std::vector<Mesh*>::iterator mid =
				std::partition(_meshes->begin() + p_firstMeshId, _meshes->begin() + p_lastMeshId,
					[min_axis, min_id, min, max](Mesh* a) {
						int x = ((int)(((float)NB_BUCKETS_BVH)
							* (a->getAABB().centroid()[min_axis] - min[min_axis])
							/ (max[min_axis] - min[min_axis])));
						if (x >= NB_BUCKETS_BVH) x = NB_BUCKETS_BVH - 1;
						else if (x < 0) x = 0;
						return (x < min_id);
					});

			unsigned int cutId = (unsigned int)std::distance(_meshes->begin(), mid);

			_buildRec(p_node->_left = new BVHNode(), p_firstMeshId, cutId, p_depth + 1);
			_buildRec(p_node->_right = new BVHNode(), cutId, p_lastMeshId, p_depth + 1);
		}
	}

	bool BVH::_intersectRec(const BVHNode* p_node, const Ray& p_ray, float p_tMin, float p_tMax, HitRecord& p_hitRecord) const {
		if (p_node==nullptr || !p_node->_aabb.intersect(p_ray, p_tMin, p_tMax)) return false;

		if (p_node->isLeaf()) {
			bool hit = false;

			for (unsigned int i=p_node->_firstMeshId; i<p_node->_lastMeshId ;i++)
				if((*_meshes)[i]->intersect(p_ray, p_tMin, glm::min(p_tMax,p_hitRecord._distance), p_hitRecord))
					hit=true;

			return hit;
		}

		bool hitLeft = _intersectRec(p_node->_left, p_ray, p_tMin, glm::min(p_tMax, p_hitRecord._distance), p_hitRecord);
		bool hitRight = _intersectRec(p_node->_right, p_ray, p_tMin, glm::min(p_tMax,p_hitRecord._distance), p_hitRecord);
		return hitLeft || hitRight;
	}

	bool BVH::_intersectAnyRec(const BVHNode* p_node, const Ray& p_ray, float p_tMin, float p_tMax) const {
		if (p_node==nullptr || !p_node->_aabb.intersect(p_ray, p_tMin, p_tMax)) return false;

		if (p_node->isLeaf()) {
			for (unsigned int i=p_node->_firstMeshId; i<p_node->_lastMeshId ;i++)
				if((*_meshes)[i]->intersectAny(p_ray, p_tMin, p_tMax))
					return true;

			return false;
		}

		return _intersectAnyRec(p_node->_left, p_ray, p_tMin, p_tMax) || _intersectAnyRec(p_node->_right, p_ray, p_tMin, p_tMax);
	}
}