#ifndef __SCENE_HPP__
#define __SCENE_HPP__

#include "utils/image.hpp"

#include "lights/light.hpp"
#include "cameras/camera.hpp"
#include "meshes/material/material.hpp"
#include "meshes/mesh.hpp"
#include "ray/hit_record.hpp"

#include "accelerators/bvh.hpp"

#include <vector>
#include <filesystem>

namespace RT_CPU
{
	class Scene
	{
	public:
		Scene(const Image& p_image, unsigned int p_id);
		~Scene();

		inline const Camera& getCamera() const { return *_camera; }
		inline const std::vector<Light*>& getLights() const { return _lights; }
		inline const std::vector<Mesh*>& getMeshes() const { return _meshes; }

		bool intersect(const Ray& p_ray, const float p_tMin, const float p_tMax, HitRecord& p_hitRecord) const;
		bool intersectAny(const Ray& p_ray, const float p_tMin, const float p_tMax) const;
	
	private:
		void _loadGltf(const std::filesystem::path& p_path);

	private:
		BVH _bvh;

		Camera*					_camera;
		std::vector<Light*>		_lights;
		std::vector<Mesh*>		_meshes;
		std::vector<Material*>	_materials;
	};
}

#endif