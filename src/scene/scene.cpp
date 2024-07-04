#include "scene.hpp"

#include "tinygltf/tiny_gltf.h"
#include <glm/gtc/type_ptr.hpp>

#include "utils/defines.hpp"

#include "scene/objects/geometry/analytic/plane.hpp"
#include "scene/objects/geometry/analytic/sphere.hpp"
#include "scene/objects/geometry/analytic/triangle_mesh.hpp"

#include "scene/objects/geometry/implicit/plane_implicit.hpp"
#include "scene/objects/geometry/implicit/sphere_implicit.hpp"
#include "scene/objects/geometry/implicit/cube_implicit.hpp"
#include "scene/objects/geometry/implicit/torus_implicit.hpp"
#include "scene/objects/geometry/implicit/cylinder_implicit.hpp"

#include "scene/objects/geometry/implicit/mandelbulb_implicit.hpp"
#include "scene/objects/geometry/implicit/julia_implicit.hpp"
#include "scene/objects/geometry/implicit/menger_implicit.hpp"
#include "scene/objects/geometry/implicit/sierpinski_implicit.hpp"
#include "scene/objects/geometry/implicit/appolonius_implicit.hpp"

#include "scene/cameras/camera_perspective.hpp"
#include "scene/cameras/camera_orthographic.hpp"
#include "scene/cameras/camera_panoramic.hpp"
#include "scene/cameras/camera_fisheye.hpp"

#include "scene/lights/basic/ponctual_light.hpp"
#include "scene/lights/basic/directional_light.hpp"
#include "scene/lights/surfacic/quad_light.hpp"
#include "scene/lights/volumic/sphere_light.hpp"

#include <iostream>

namespace RT_CPU
{
	Scene::Scene(const Image& p_image, unsigned int p_id) {
		float aspectRatio = float(p_image.getWidth()) / float(p_image.getHeight());
		
		if(p_id==0){
			_camera = new CameraPerspective(Vec3f(0.f, -9.f, 9.f), Vec3f(0.f, -2.f, 2.f), PIf / 3.f, 1.f, aspectRatio);
			
			_objects.push_back(new Plane(Vec3f(0.f, 0.f, 0.f), Vec3f(0., 0., 1.f)));
			_objects.push_back(new Sphere(Vec3f(0.f, 0.f, 100.f), 30.f));
			
			_materials.push_back(new Material(Vec3f(0.5f, 0.5f, 0.5f), VEC3F_ZERO, 0.f, 0.f, 1.f, 0.f, 1.f));
			_materials.push_back(new Material(Vec3f(1.f, 1.f, 1.f), VEC3F_ONE, 10.f, 0.f, 1.f, 0.f, 1.f));
			
			for(int i=0; i<5 ;i++)
				for(int j=0; j<5 ;j++){
					_materials.push_back(new Material(Vec3f(randomFloat(), randomFloat(), randomFloat()), VEC3F_ZERO, 0.f, 0.f, 0.f, 0.f, 1.5f));
					_objects.push_back(new Sphere(Vec3f(-5.f + i * 2.5f, -5.f + j * 2.5f, 2.f), 1.f));
				}

			for(int i=0; i<_objects.size() ;i++)
				_objects[i]->setMaterial(_materials[i]);

		}else if(p_id==1){
			_camera = new CameraPerspective(Vec3f(0.f, -3.f, 2.5f), Vec3f(0.f, 0.f, 1.f), PIf / 3.f, 1.f, aspectRatio);

			_materials.push_back(new Material(Vec3f(1.f, 1.f, 1.f)    , VEC3F_ONE , 5.f, 1.f, 0.8f, 0.f, 1.f));
			_materials.push_back(new Material(Vec3f(1.f, 1.f, 1.f)    , VEC3F_ZERO, 0.f, 1.f, 0.8f, 0.f, 1.f));
			_materials.push_back(new Material(Vec3f(1.f, 0.f, 0.f)    , VEC3F_ZERO, 0.f, 1.f, 0.8f, 0.f, 1.f));
			_materials.push_back(new Material(Vec3f(0.f, 1.f, 0.f)    , VEC3F_ZERO, 0.f, 1.f, 0.8f, 0.f, 1.f));
			_materials.push_back(new Material(Vec3f(0.8f, 0.235f, 0.f), VEC3F_ZERO, 0.f, 0.f, 0.f, 0.f, 1.f));
			_materials.push_back(new Material(Vec3f(0.f, 0.235f, 0.8f), VEC3F_ZERO, 0.f, 0.f, 0.f, 0.f, 1.f));
			
			_objects.push_back(new Sphere(Vec3f(0.f, 0.f, 10.f), 3.f));
			_objects.push_back(new Plane(Vec3f(0.f, 0.f, 0.f), Vec3f(0.f, 0.f, 1.f)));
			_objects.push_back(new Plane(Vec3f(0.f, 0.f, 10.f), Vec3f(0.f, 0.f, -1.f)));
			_objects.push_back(new Plane(Vec3f(-5.f, 0.f, 0.f), Vec3f(1.f, 0.f, 0.f)));
			_objects.push_back(new Plane(Vec3f(5.f, 0.f, 0.f), Vec3f(-1.f, 0.f, 0.f)));
			_objects.push_back(new Plane(Vec3f(0.f, 5.f, 0.f), Vec3f(0.f, -1.f, 0.f)));
			_objects.push_back(new Plane(Vec3f(0.f, -5.f, 0.f), Vec3f(0.f, 1.f, 0.f)));
			_objects.push_back(new MandelbulbImplicit(Vec3f(-2.f, 0.f, 1.5f), 8.f));
			_objects.push_back(new JuliaImplicit(Vec3f(2.f, 0.f, 1.5f), Vec4f(0.f, 0.1f, 1.f, -0.1f)));

			_objects[0]->setMaterial(_materials[0]);
			_objects[1]->setMaterial(_materials[1]);
			_objects[2]->setMaterial(_materials[1]);
			_objects[3]->setMaterial(_materials[2]);
			_objects[4]->setMaterial(_materials[3]);
			_objects[5]->setMaterial(_materials[1]);
			_objects[6]->setMaterial(_materials[1]);
			_objects[7]->setMaterial(_materials[4]);
			_objects[8]->setMaterial(_materials[5]);
		} else if (p_id == 2) {
			_camera = new CameraPerspective(Vec3f(-3.f, 0.f, 5.f), Vec3f(-3.f, -1.f, 5.f), PIf / 3.f, 1.f, aspectRatio);

			_materials.push_back(new Material(Vec3f(1.f, 1.f, 1.f), VEC3F_ONE, 20.f, 0.f, 1.f, 0.f, 1.f));
			_materials.push_back(new Material(Vec3f(0.53f, 0.3f, 0.075f), VEC3F_ZERO, 0.f, 0.9f, 0.5f, 0.f, 1.f));

			_objects.push_back(new Plane(Vec3f(0.f, 0.f, 10.f), Vec3f(0.f,0.f,-1.f)));
			_objects.push_back(new AppoloniusImplicit(Vec3f(0.f, 0.f, 0.f)));

			for (int i = 0; i < _objects.size();i++)
				_objects[i]->setMaterial(_materials[i]);
		} else if (p_id == 3) {
			_camera = new CameraPerspective(Vec3f(-3.47f, -1.35f, 1.7f), Vec3f(0.f, 0.f, 0.f), PIf / 3.f, 1.f, aspectRatio);
			_lights.push_back(new QuadLight(Vec3f(0.f, 0.f, 2.8f), glm::normalize(Vec3f(0.f, 0.f, -1.f)), VEC3F_ONE, 2.5f, Vec3f(4.f,0.f,0.f), Vec3f(0.f,4.f,0.f)));
			_materials.push_back(new Material(Vec3f(0.5f, 0.5f, 0.5f), VEC3F_ZERO, 0.f, 0.f, 1.f, 0.f, 1.f));
			_loadGltf("assets/conference.glb");
		}else if(p_id == 4) {
			_camera = new CameraPerspective(Vec3f(-3.47f, -1.35f, 1.7f), Vec3f(0.f, 0.f, 0.f), PIf / 3.f, 1.f, aspectRatio);
			_materials.push_back(new Material(Vec3f(0.5f, 0.5f, 0.5f), VEC3F_ZERO, 0.f, 0.f, 1.f, 0.f, 1.f));
			_loadGltf("assets/conference_dragon_and_bunny.glb");
		}

		else if (p_id == 5) {
			_camera = new CameraPerspective(Vec3f(0.f, -15.f, 15.f), Vec3f(0.f, -2.f, 2.f), PIf/3.f, 1.f, aspectRatio);
			//_camera = new CameraOrthographic(Vec3f(0.f, -8.f, 8.f), Vec3f(0.f, -2.f, 2.f), 15.f*aspectRatio,15.f);
			//_camera = new CameraPanoramic(Vec3f(0.f, -8.f, 8.f), Vec3f(0.f, -2.f, 2.f), 1.f);
			//_camera = new CameraFisheye(Vec3f(0.f, -8.f, 8.f), Vec3f(0.f, -2.f, 2.f), 1.f);

			_objects.push_back(new Plane(Vec3f(0.f, 0.f, 0.f), Vec3f(0., 0., 1.f)));
			//_lights.push_back(new DirectionalLight(Vec3f(0.f, 0.f, 0.f), glm::normalize(Vec3f(0.f, -1.f, -1.f)), VEC3F_ONE, 3.f));
			//_lights.push_back(new PonctualLight(Vec3f(0.f, 0.f, 10.f), glm::normalize(Vec3f(0.f, 0.f, -1.f)), VEC3F_ONE, 300.f, -PIf, -PIf));
			//_lights.push_back(new PonctualLight(Vec3f(0.f, 0.f, 10.f), glm::normalize(Vec3f(0.f, 0.f, -1.f)), VEC3F_ONE, 300.f, PIf/5.f, PIf/3.f));
			//_lights.push_back(new QuadLight(Vec3f(0.f, 0.f, 10.f), glm::normalize(Vec3f(0.f, 0.f, -1.f)), VEC3F_ONE, 10.f, Vec3f(4.f,0.f,0.f), Vec3f(0.f,4.f,0.f)));
			_lights.push_back(new SphereLight(Vec3f(0.f, 0.f, 10.f), glm::normalize(Vec3f(0.f, 0.f, -1.f)), VEC3F_ONE, 10.f, 2.5f));
			_materials.push_back(new Material(Vec3f(0.5f, 0.5f, 0.5f), VEC3F_ZERO, 0.f, 0.f, 1.f, 0.f, 1.f));

			for (int i = 0; i < 5; i++)
				for (int j = 0; j < 5; j++) {
					_materials.push_back(new Material(Vec3f(randomFloat(), randomFloat(), randomFloat()), VEC3F_ZERO, 0.f, 1.f, 0.5f, 0.f, 1.5f));
					_objects.push_back(new Sphere(Vec3f(-5.f + i * 2.5f, -5.f + j * 2.5f, 2.f), 1.f));
				}

			for (int i = 0; i < _objects.size(); i++)
				_objects[i]->setMaterial(_materials[i]);

		}

		_bvh.build(&_objects);
	}

	Scene::~Scene(){
		delete _camera;
		for(unsigned int i=0; i<_lights.size() ;i++) delete _lights[i];
		for(unsigned int i=0; i<_objects.size() ;i++) delete _objects[i];
		for(unsigned int i=0; i<_materials.size() ;i++) delete _materials[i];
	}

	bool Scene::intersect(const Ray& p_ray, const float p_tMin, const float p_tMax, HitRecord& p_hitRecord) const {
		return _bvh.intersect(p_ray, p_tMin, p_tMax, p_hitRecord);
	}

	bool Scene::intersectAny(const Ray& p_ray, const float p_tMin, const float p_tMax) const {
		return _bvh.intersectAny(p_ray, p_tMin, p_tMax);
	}

	void Scene::_loadGltf(const std::filesystem::path& p_path) {
		std::cout << "Start loading " << p_path << std::endl;

        tinygltf::TinyGLTF loader;
        tinygltf::Model model;

        loader.SetPreserveImageChannels(true);

        if (p_path.extension() == ".gltf") {
            if (!loader.LoadASCIIFromFile(&model, nullptr, nullptr, p_path.string())) throw std::runtime_error("Fail to load file: " + p_path.string());
        } else {
            if (!loader.LoadBinaryFromFile(&model, nullptr, nullptr, p_path.string())) throw std::runtime_error("Fail to load file: " + p_path.string());
        }

		// ------------- MATERIALS
		unsigned int startIdMaterials = (unsigned int)_materials.size();
		_materials.reserve(startIdMaterials + model.materials.size());
		for (tinygltf::Material m : model.materials)
			_materials.push_back(new Material(
				Vec3f(glm::make_vec4(m.pbrMetallicRoughness.baseColorFactor.data())),
				glm::make_vec3(m.emissiveFactor.data()),
				m.extensions.find("KHR_materials_emissive_strength") != m.extensions.end() && m.extensions.at("KHR_materials_emissive_strength").Has("emissiveStrength") ? (float)m.extensions.at("KHR_materials_emissive_strength").Get("emissiveStrength").GetNumberAsDouble() : 1.f,
				(float)m.pbrMetallicRoughness.metallicFactor,
				(float)m.pbrMetallicRoughness.roughnessFactor,
				m.extensions.find("KHR_materials_transmission") != m.extensions.end() && m.extensions.at("KHR_materials_transmission").Has("transmissionFactor") ? (float)m.extensions.at("KHR_materials_transmission").Get("transmissionFactor").GetNumberAsDouble() : 0.f,
				m.extensions.find("KHR_materials_ior") != m.extensions.end() && m.extensions.at("KHR_materials_ior").Has("ior") ? (float)m.extensions.at("KHR_materials_ior").Get("ior").GetNumberAsDouble() : 1.f
			));

		std::cout << "materials loaded: " << _materials.size() - startIdMaterials << std::endl;

		// ------------- MESHES
		unsigned int startIdObjects = (unsigned int)_objects.size();
		_objects.reserve(startIdObjects + model.meshes.size());
		for (tinygltf::Mesh m : model.meshes) {
			for (tinygltf::Primitive p : m.primitives) {
				if (p.indices == -1) throw std::runtime_error("Fail to load file: primitive indices must be define.");

				tinygltf::Accessor a_position = model.accessors[p.attributes.at("POSITION")];
				tinygltf::Accessor a_normal = model.accessors[p.attributes.at("NORMAL")];

				if ((a_position.count != a_normal.count)) throw std::runtime_error("Fail to load file: primitive vertices must have the same number of position, normal and texcoord0.");

				tinygltf::Accessor a_indices = model.accessors[p.indices];
				tinygltf::BufferView bv_indices = model.bufferViews[a_indices.bufferView];

				std::vector<Vec2u> indices;
				indices.reserve((unsigned int)a_indices.count);
				if (a_indices.componentType == TINYGLTF_PARAMETER_TYPE_UNSIGNED_INT) {
					const unsigned int* data = reinterpret_cast<const unsigned int*>(&model.buffers[bv_indices.buffer].data[a_indices.byteOffset + bv_indices.byteOffset]);
					for(unsigned int i=0; i<(unsigned int)a_indices.count ;i++) indices.push_back(Vec2u(data[i],data[i]));
				} else if(a_indices.componentType == TINYGLTF_PARAMETER_TYPE_UNSIGNED_SHORT){
					const unsigned short* data = reinterpret_cast<const unsigned short*>(&model.buffers[bv_indices.buffer].data[a_indices.byteOffset + bv_indices.byteOffset]);
					for(unsigned int i=0; i<(unsigned int)a_indices.count ;i++) indices.push_back(Vec2u(data[i],data[i]));
				} else if(a_indices.componentType == TINYGLTF_PARAMETER_TYPE_UNSIGNED_BYTE){
					const unsigned char* data = reinterpret_cast<const unsigned char*>(&model.buffers[bv_indices.buffer].data[a_indices.byteOffset + bv_indices.byteOffset]);
					for(unsigned int i=0; i<(unsigned int)a_indices.count ;i++) indices.push_back(Vec2u(data[i],data[i]));
				}

				std::vector<Vec3f> positions;
				positions.reserve(a_position.count);
				tinygltf::BufferView bv_position = model.bufferViews[a_position.bufferView];
				const float* positionsBuffer = reinterpret_cast<const float*>(&model.buffers[bv_position.buffer].data[a_position.byteOffset + bv_position.byteOffset]);

				std::vector<Vec3f> normals;
				positions.reserve(a_normal.count);
				tinygltf::BufferView bv_normal = model.bufferViews[a_normal.bufferView];
				const float* normalsBuffer = reinterpret_cast<const float*>(&model.buffers[bv_normal.buffer].data[a_normal.byteOffset + bv_normal.byteOffset]);

				for (unsigned int i = 0; i < a_position.count;i++) {
					positions.push_back(glm::make_vec3(&positionsBuffer[i * 3]));
					normals.push_back(glm::normalize(glm::make_vec3(&normalsBuffer[i * 3])));
				}

				_objects.push_back(new TriangleMesh(positions,normals,indices));
				_objects[_objects.size()-1]->setMaterial(_materials[(p.material == -1) ? 0 : startIdMaterials + p.material]);
			}
		}
		std::cout << "objects loaded: " << _objects.size() - startIdObjects << std::endl;

		std::cout << "Finished to load " << p_path << std::endl;
	}
}