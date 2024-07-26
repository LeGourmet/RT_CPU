#include "scene.hpp"

#include "fastgltf/core.hpp"
#include "fastgltf/types.hpp"
#include <glm/gtc/type_ptr.hpp>

#include "utils/defines.hpp"

#include "scene/meshes/geometry/analytic/plane.hpp"
#include "scene/meshes/geometry/analytic/sphere.hpp"
#include "scene/meshes/geometry/analytic/triangle_mesh.hpp"

#include "scene/meshes/geometry/implicit/3D/plane_implicit.hpp"
#include "scene/meshes/geometry/implicit/3D/sphere_implicit.hpp"
#include "scene/meshes/geometry/implicit/3D/cube_implicit.hpp"
#include "scene/meshes/geometry/implicit/3D/torus_implicit.hpp"
#include "scene/meshes/geometry/implicit/3D/cylinder_implicit.hpp"

#include "scene/meshes/geometry/implicit/fractal/mandelbulb_fractal.hpp"
#include "scene/meshes/geometry/implicit/fractal/julia_fractal.hpp"
#include "scene/meshes/geometry/implicit/fractal/menger_fractal.hpp"
#include "scene/meshes/geometry/implicit/fractal/sierpinski_fractal.hpp"
#include "scene/meshes/geometry/implicit/fractal/apollonius_fractal.hpp"

#include "scene/cameras/camera_perspective.hpp"
#include "scene/cameras/camera_orthographic.hpp"
#include "scene/cameras/camera_panoramic.hpp"
#include "scene/cameras/camera_fisheye.hpp"

#include "scene/lights/basic/ponctual_light.hpp"
#include "scene/lights/basic/directional_light.hpp"
#include "scene/lights/surfacic/quad_light.hpp"
#include "scene/lights/surfacic/disk_light.hpp"
#include "scene/lights/volumic/sphere_light.hpp"

#include <iostream>

namespace RT_CPU
{
	Scene::Scene(const Image& p_image, unsigned int p_id) {
		float aspectRatio = float(p_image.getWidth()) / float(p_image.getHeight());
		
		// material test
		// fractal test	
		// infinit fractal test
		// implicit test
		// triangle mesh test
		// micro mesh test
		// lights test
		// camera test

		if(p_id==0){ // material test
			_camera = new CameraPerspective(Vec3f(0.f, -8.f, 16.f), Vec3f(0.f, -1.f, 2.f), PIf / 3.f, 1.f, aspectRatio);
			//_lights.push_back(new QuadLight(Vec3f(0.f, 0.f, 50.f), glm::normalize(Vec3f(0.f, 0.f, -1.f)), VEC3F_ONE, 100.f, Vec3f(10.f,0.f,0.f), Vec3f(0.f,10.f,0.f)));
			_lights.push_back(new PonctualLight(Vec3f(0.f, 0.f, 50.f), glm::normalize(Vec3f(0.f, 0.f, -1.f)), VEC3F_ONE, 10000.f, -PIf, -PIf));
			
			_meshes.push_back(new Plane(Vec3f(0.f, 0.f, 0.f), Vec3f(0., 0., 1.f)));
			_materials.push_back(new Material(Vec4f(0.5f, 0.5f, 0.5f, 1.f), 0.f, VEC3F_ZERO, 0.f, 0.f, 1.f, 0.f, VEC3F_ZERO, 0.f, 1.f));

			//_meshes.push_back(new Sphere(Vec3f(0.f, 0.f, 100.f), 10.f));
			//_materials.push_back(new Material(VEC4F_ONE, 0.f, VEC3F_ONE, 100.f, 0.f, 1.f, 0.f, VEC3F_ZERO, 0.f, 1.f));

			for(int i=0; i<9 ;i++) {
				// Sheen
				//_materials.push_back(new Material(Vec3f(randomFloat(), randomFloat(), randomFloat()), VEC3F_ZERO, 0.f, 0.f, i*0.125f, 0.f, VEC3F_ZERO, 0.f, 1.5f));
				//_meshes.push_back(new Sphere(Vec3f(-10.f + i * 2.5f, -10.f, 2.f), 1.f));
				
				// Dielectric
				_materials.push_back(new Material(Vec4f(randomFloat(), randomFloat(), randomFloat(), 1.f), 0.f, VEC3F_ZERO, 0.f, 0.f, i*0.125f, 0.f, VEC3F_ZERO, 0.f, 1.5f));
				_meshes.push_back(new Sphere(Vec3f(-10.f + i * 2.5f, -7.5f, 2.f), 1.f));
				
				// Metalic
				_materials.push_back(new Material(Vec4f(randomFloat(), randomFloat(), randomFloat(), 1.f), 0.f, VEC3F_ZERO, 0.f, 1.f, i*0.125f, 0.f, VEC3F_ZERO, 0.f, 1.5f));
				_meshes.push_back(new Sphere(Vec3f(-10.f + i * 2.5f, -5.f, 2.f), 1.f));

				// Metalic Anisotropie
				//_materials.push_back(new Material(Vec3f(randomFloat(), randomFloat(), randomFloat()), VEC3F_ZERO, 0.f, 1.f, i*0.125f, 0.f, VEC3F_ZERO, 0.f, 1.5f));
				//_meshes.push_back(new Sphere(Vec3f(-10.f + i * 2.5f, -2.5f, 2.f), 1.f));

				// Clear Coat
				//_materials.push_back(new Material(Vec3f(randomFloat(), randomFloat(), randomFloat()), VEC3F_ZERO, 0.f, 1.f, 0.6f, 0.f, VEC3F_ZERO, 0.f, 1.5f));
				//_meshes.push_back(new Sphere(Vec3f(-10.f + i * 2.5f, 0.f, 2.f), 1.f));
				
				// Alpha
				_materials.push_back(new Material(Vec4f(randomFloat(), randomFloat(), randomFloat(), i*0.125f), 1.f, VEC3F_ZERO, 0.f, 0.5f, 0.2f, 0.f, VEC3F_ZERO, 0.f, 1.5f));
				_meshes.push_back(new Sphere(Vec3f(-10.f + i * 2.5f, 2.5f, 2.f), 1.f));

				// Transparent
				_materials.push_back(new Material(Vec4f(randomFloat(), randomFloat(), randomFloat(), 1.f), 0.f, VEC3F_ZERO, 0.f, 0.f, i*0.125f, 1.f, VEC3F_ZERO, 0.f, 1.5f));
				_meshes.push_back(new Sphere(Vec3f(-10.f + i * 2.5f, 5.f, 2.f), 1.f));

				// Absorbance
				_materials.push_back(new Material(Vec4f(1.f,1.f,1.f,1.f), 0.f, VEC3F_ZERO, 0.f, 0.f, 0.f, 1.f, Vec3f(0.9f, 0.3f, 0.3f), i * 1.f, 1.5f));
				_meshes.push_back(new Sphere(Vec3f(-10.f + i * 2.5f, 7.5f, 2.f), 1.f));

				// Emissive
				_materials.push_back(new Material(Vec4f(1.f, 1.f, 1.f, 1.f), 0.f, Vec3f(randomFloat(), randomFloat(), randomFloat()), i*2.5f, 0.f, 1.f, 0.f, VEC3F_ZERO, 0.f, 1.5f));
				_meshes.push_back(new Sphere(Vec3f(-10.f + i * 2.5f, 10.f, 2.f), 1.f));
			}

			for(int i=0; i<_meshes.size() ;i++) _meshes[i]->setMaterial(_materials[i]);

		}else if (p_id == 1) { // fractal test
			_camera = new CameraPerspective(Vec3f(0.f, -4.f, 3.f), Vec3f(0.f, 0.f, 1.f), PIf / 3.f, 1.f, aspectRatio);

			_materials.push_back(new Material(Vec4f(1.f, 1.f, 1.f, 1.f), 0.f, VEC3F_ONE , 5.f, 0.f, 1.f, 0.f, VEC3F_ZERO, 0.f, 1.f));
			_materials.push_back(new Material(Vec4f(1.f, 1.f, 1.f, 1.f), 0.f, VEC3F_ZERO, 0.f, 1.f, 1.f, 0.f, VEC3F_ZERO, 0.f, 1.f));
			_materials.push_back(new Material(Vec4f(1.f, 0.f, 0.f, 1.f), 0.f, VEC3F_ZERO, 0.f, 1.f, 1.f, 0.f, VEC3F_ZERO, 0.f, 1.f));
			_materials.push_back(new Material(Vec4f(0.f, 1.f, 0.f, 1.f), 0.f, VEC3F_ZERO, 0.f, 1.f, 1.f, 0.f, VEC3F_ZERO, 0.f, 1.f));
			_materials.push_back(new Material(Vec4f(0.8f, 0.235f, 0.f, 1.f), 0.f, VEC3F_ZERO, 0.f, 1.f, 0.5f, 0.f, VEC3F_ZERO, 0.f, 1.f));
			_materials.push_back(new Material(Vec4f(0.f, 0.235f, 0.8f, 1.f), 0.f, VEC3F_ZERO, 0.f, 1.f, 0.5f, 0.f, VEC3F_ZERO, 0.f, 1.f));
			
			_meshes.push_back(new Plane(Vec3f(0.f, 0.f, 10.f), Vec3f(0.f, 0.f, -1.f)));
			_meshes.push_back(new Plane(Vec3f(0.f, 0.f, 0.f), Vec3f(0.f, 0.f, 1.f)));
			_meshes.push_back(new Plane(Vec3f(-5.f, 0.f, 0.f), Vec3f(1.f, 0.f, 0.f)));
			_meshes.push_back(new Plane(Vec3f(5.f, 0.f, 0.f), Vec3f(-1.f, 0.f, 0.f)));
			_meshes.push_back(new Plane(Vec3f(0.f, 5.f, 0.f), Vec3f(0.f, -1.f, 0.f)));
			_meshes.push_back(new Plane(Vec3f(0.f, -5.f, 0.f), Vec3f(0.f, 1.f, 0.f)));
			_meshes.push_back(new MandelbulbImplicit(Vec3f(-2.f, 0.f, 1.5f), 1.f, 8.f));
			_meshes.push_back(new JuliaImplicit(Vec3f(2.f, 0.f, 1.5f), 1.f, Vec4f(0.f, 0.1f, 1.f, -0.1f)));

			_meshes[0]->setMaterial(_materials[0]);
			_meshes[1]->setMaterial(_materials[1]);
			_meshes[2]->setMaterial(_materials[2]);
			_meshes[3]->setMaterial(_materials[3]);
			_meshes[4]->setMaterial(_materials[1]);
			_meshes[5]->setMaterial(_materials[1]);
			_meshes[6]->setMaterial(_materials[4]);
			_meshes[7]->setMaterial(_materials[5]);

		} else if (p_id == 2) { // infinite fractal test
			_camera = new CameraPerspective(Vec3f(-3.f, 1.f, 5.f), Vec3f(-3.f, 0.f, 5.f), PIf / 3.f, 1.f, aspectRatio);

			_materials.push_back(new Material(Vec4f(1.f, 1.f, 1.f, 1.f), 0.f, VEC3F_ONE, 20.f, 0.f, 1.f, 0.f, VEC3F_ZERO, 0.f, 1.f));
			_materials.push_back(new Material(Vec4f(0.35f, 0.2f, 0.05f, 1.f), 0.f, VEC3F_ZERO, 0.f, 0.75f, 0.5f, 0.f, VEC3F_ZERO, 0.f, 1.f));

			_meshes.push_back(new Plane(Vec3f(0.f, 0.f, 10.f), Vec3f(0.f,0.f,-1.f)));
			_meshes.push_back(new ApolloniusImplicit(VEC3F_ZERO,1.f));

			for(int i=0; i<_meshes.size() ;i++) _meshes[i]->setMaterial(_materials[i]);

		} else if(p_id == 3) { // triangle mesh test
			_camera = new CameraPerspective(Vec3f(-3.47f, -1.35f, 1.7f), Vec3f(0.f, 0.f, 0.f), PIf / 3.f, 1.f, aspectRatio);
			_lights.push_back(new QuadLight(Vec3f(0.f, 0.f, 2.8f), glm::normalize(Vec3f(0.f, 0.f, -1.f)), VEC3F_ONE, 2.5f, Vec3f(4.f, 0.f, 0.f), Vec3f(0.f, 4.f, 0.f)));
			//_lights.push_back(new PonctualLight(Vec3f(0.f, 0.f, 2.8f), glm::normalize(Vec3f(0.f, 0.f, -1.f)), VEC3F_ONE, 30.f, -PIf, -PIf));
			_materials.push_back(new Material(Vec4f(0.5f, 0.5f, 0.5f, 1.f), 0.f, VEC3F_ZERO, 0.f, 0.f, 1.f, 0.f, VEC3F_ZERO, 0.f, 1.f));
			_loadGltf("assets/conference_dragon_and_bunny.glb");
		} else if (p_id == 4) {
			//_camera = new CameraPerspective(Vec3f(0.f, -2.f, 2.f), Vec3f(0.f, 0.f, 1.f), PIf / 3.f, 1.f, aspectRatio);

			_camera = new CameraPerspective(Vec3f(0.f, -9.f, 9.f), Vec3f(0.f, -2.f, 2.f), PIf / 3.f, 1.f, aspectRatio);
			//_camera = new CameraOrthographic(Vec3f(0.f, -8.f, 8.f), Vec3f(0.f, -2.f, 2.f), 15.f*aspectRatio,15.f);
			//_camera = new CameraPanoramic(Vec3f(0.f, -8.f, 8.f), Vec3f(0.f, -2.f, 2.f), 1.f);
			//_camera = new CameraFisheye(Vec3f(0.f, -8.f, 8.f), Vec3f(0.f, -2.f, 2.f), 1.f);

			_meshes.push_back(new Plane(Vec3f(0.f, 0.f, 0.f), Vec3f(0., 0., 1.f)));
			_materials.push_back(new Material(Vec4f(0.5f, 0.5f, 0.5f, 1.f), 0.f, VEC3F_ZERO, 0.f, 0.f, 1.f, 0.f, VEC3F_ZERO, 0.f, 1.f));

			//_lights.push_back(new DirectionalLight(Vec3f(0.f, 0.f, 0.f), glm::normalize(Vec3f(0.f,0.f,-1.f)), VEC3F_ONE, 3.f, 0.f));
			//_lights.push_back(new DirectionalLight(Vec3f(0.f, 0.f, 0.f), glm::normalize(Vec3f(0.f,0.f,-1.f)), VEC3F_ONE, 3.f, PIf/10.f));
			//_lights.push_back(new PonctualLight(Vec3f(0.f, 0.f, 10.f), glm::normalize(Vec3f(0.f, 0.f, -1.f)), VEC3F_ONE, 300.f, -PIf, -PIf));
			//_lights.push_back(new PonctualLight(Vec3f(0.f, 0.f, 10.f), glm::normalize(Vec3f(0.f, 0.f, -1.f)), VEC3F_ONE, 300.f, PIf/5.f, PIf/3.f));
			//_lights.push_back(new QuadLight(Vec3f(0.f, 0.f, 10.f), glm::normalize(Vec3f(0.f, 0.f, -1.f)), VEC3F_ONE, 20.f, Vec2f(4.f,0.f), Vec2f(0.f,4.f)));
			//_lights.push_back(new DiskLight(Vec3f(0.f, 0.f, 10.f), glm::normalize(Vec3f(0.f, 0.f, -1.f)), VEC3F_ONE, 25.f, 2.f));
			//_lights.push_back(new SphereLight(Vec3f(0.f, 0.f, 10.f), glm::normalize(Vec3f(0.f, 0.f, -1.f)), VEC3F_ONE, 3.f, 4.f, -PIf, -PIf));
			_lights.push_back(new SphereLight(Vec3f(0.f, 0.f, 10.f), glm::normalize(Vec3f(0.f, 0.f, -1.f)), VEC3F_ONE, 3.f, 4.f, PIf/5.f, PIf/3.f));

			for (int i = 0; i < 5; i++)
				for (int j = 0; j < 5; j++) {
					_materials.push_back(new Material(Vec4f(randomFloat(), randomFloat(), randomFloat(), 1.f), 0.f, VEC3F_ZERO, 0.f, 1.f, 0.1f, 0.f, VEC3F_ZERO, 0.f, 1.5f));
					_meshes.push_back(new Sphere(Vec3f(-5.f + i * 2.5f, -5.f + j * 2.5f, 2.f), 1.f));
				}

			//_materials.push_back(new Material(Vec3f(0.5f)+0.5f*Vec3f(randomFloat(), randomFloat(), randomFloat()), VEC3F_ZERO, 0.f, 0.f, 1.f, 0.f, VEC3F_ZERO, 0.f, 1.5f));
			//_meshes.push_back(new SierpinskiImplicit(Vec3f(0.f,0.f,1.f),1.f));
			//_meshes.push_back(new JuliaImplicit(Vec3f(0.f,0.f,1.f), 1.f, Vec4f(0.f, 0.1f, 1.f, -0.1f))); // probleme
			//_meshes.push_back(new MandelbulbImplicit(Vec3f(0.f,0.f,1.f), 1.f, 3.f));
			//_meshes.push_back(new MengerImplicit(Vec3f(0.f,0.f,1.f), 1.f));
			//_meshes.push_back(new ApolloniusImplicit(Vec3f(0.f,0.f,0.f), 1.f));

			for(int i=0; i<_meshes.size() ;i++) _meshes[i]->setMaterial(_materials[i]);
						
		}

		_bvh.build(&_meshes);
	}

	Scene::~Scene(){
		delete _camera;
		for(unsigned int i=0; i<_lights.size() ;i++) delete _lights[i];
		for(unsigned int i=0; i<_meshes.size() ;i++) delete _meshes[i];
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

		fastgltf::Parser _parser = fastgltf::Parser(
			fastgltf::Extensions::KHR_lights_punctual |
			fastgltf::Extensions::KHR_materials_ior |
			//fastgltf::Extensions::KHR_materials_iridescence |
			fastgltf::Extensions::KHR_materials_volume |
			fastgltf::Extensions::KHR_materials_transmission |
			//fastgltf::Extensions::KHR_materials_clearcoat |
			fastgltf::Extensions::KHR_materials_emissive_strength
			//fastgltf::Extensions::KHR_materials_sheen |
			//fastgltf::Extensions::KHR_materials_anisotropy
			//fastgltf::Extensions::KHR_materials_dispersion
			); 
		// + EXT_lights_ies
		// + EXT_lights_image_based
		// + KHR_materials_subsuface

		// preserved chanel ?

		auto data = fastgltf::GltfDataBuffer::FromPath(p_path);
		if (data.error() != fastgltf::Error::None) throw std::runtime_error("Fail to load file: file incorect.");

		fastgltf::Expected<fastgltf::Asset> asset = _parser.loadGltfBinary(data.get(), p_path.parent_path(), fastgltf::Options::DecomposeNodeMatrices | fastgltf::Options::DontRequireValidAssetMember);
		if (asset.error() != fastgltf::Error::None) throw std::runtime_error("Fail to load file: gltf incorect.");

		// ------------- MATERIALS
		unsigned int startIdMaterials = (unsigned int)_materials.size();
		_materials.reserve(startIdMaterials + asset->materials.size());
		for (fastgltf::Material& m : asset->materials)
			_materials.push_back(new Material(
				glm::make_vec4(m.pbrData.baseColorFactor.data()),
				(m.alphaMode == fastgltf::AlphaMode::Opaque ? 0.f : (m.alphaMode == fastgltf::AlphaMode::Blend ? 1.f : (float)m.alphaCutoff)),
				glm::make_vec3(m.emissiveFactor.data()),
				(float)m.emissiveStrength, 
				(float)m.pbrData.metallicFactor,
				(float)m.pbrData.roughnessFactor,
				((m.transmission.get() == nullptr) ? 0.f : (float)m.transmission.get()->transmissionFactor),
				((m.volume.get() == nullptr) ? VEC3F_ONE : glm::make_vec3(m.volume.get()->attenuationColor.data())),
				((m.volume.get() == nullptr) ? FLT_MAX : (float)m.volume.get()->attenuationDistance),
				(float)m.ior
			));

		std::cout << "materials loaded: " << _materials.size() - startIdMaterials << std::endl;

		// ------------- MESHES
		unsigned int startIdMeshes = (unsigned int)_meshes.size();
		_meshes.reserve(startIdMeshes + asset->meshes.size());
		for (fastgltf::Mesh& m : asset->meshes) {
			for (fastgltf::Primitive& p : m.primitives) {
				if (!p.indicesAccessor.has_value()) throw std::runtime_error("Fail to load file: primitive indices must be define.");
				if (p.type != fastgltf::PrimitiveType::Triangles) throw std::runtime_error("Fail to load file: primitive must be define by triangles.");

				fastgltf::Attribute* itPosition = p.findAttribute("POSITION");
				if (itPosition == p.attributes.end()) throw std::runtime_error("Fail to load file: primitive must contain position buffer.");
				fastgltf::Accessor& a_position = asset->accessors[itPosition->accessorIndex];

				fastgltf::Attribute* itNormal = p.findAttribute("NORMAL");
				if (itNormal == p.attributes.end()) throw std::runtime_error("Fail to load file: primitive must contain normal buffer.");
				fastgltf::Accessor& a_normal = asset->accessors[itNormal->accessorIndex];

				if (a_position.count != a_normal.count) throw std::runtime_error("Fail to load file: primitive vertices must have the same number of position and normal.");
				
				fastgltf::Accessor& a_indices = asset->accessors[p.indicesAccessor.value()];
				if (!a_indices.bufferViewIndex.has_value()) throw std::runtime_error("Fail to load file: primitive indices must be define.");
				fastgltf::BufferView& bv_indices = asset->bufferViews[a_indices.bufferViewIndex.value()];

				std::vector<unsigned int> indices;
				if (!std::holds_alternative<fastgltf::sources::Array>(asset->buffers[bv_indices.bufferIndex].data)) throw std::runtime_error("Primitive indices type should be vector!");
				if (a_indices.componentType == fastgltf::ComponentType::UnsignedInt) {
					const unsigned int* data = reinterpret_cast<const unsigned int*>(std::get<3>(asset->buffers[bv_indices.bufferIndex].data).bytes.data() + a_indices.byteOffset + bv_indices.byteOffset);
					indices = std::vector<unsigned int>(data, &data[a_indices.count]);
				}
				else if (a_indices.componentType == fastgltf::ComponentType::UnsignedShort) {
					indices.reserve((unsigned int)a_indices.count);
					const unsigned short* data = reinterpret_cast<const unsigned short*>(std::get<3>(asset->buffers[bv_indices.bufferIndex].data).bytes.data() + a_indices.byteOffset + bv_indices.byteOffset);
					for (unsigned int i = 0; i < (unsigned int)a_indices.count; i++) indices.push_back(data[i]);
				}
				else if (a_indices.componentType == fastgltf::ComponentType::UnsignedByte) {
					indices.reserve((unsigned int)a_indices.count);
					const unsigned char* data = reinterpret_cast<const unsigned char*>(std::get<3>(asset->buffers[bv_indices.bufferIndex].data).bytes.data() + a_indices.byteOffset + bv_indices.byteOffset);
					for (unsigned int i = 0; i < (unsigned int)a_indices.count; i++) indices.push_back(data[i]);
				}

				if (!a_position.bufferViewIndex.has_value()) throw std::runtime_error("Fail to load file: primitive positions must be define.");
				fastgltf::BufferView& bv_position = asset->bufferViews[a_position.bufferViewIndex.value()];
				if (!std::holds_alternative<fastgltf::sources::Array>(asset->buffers[bv_position.bufferIndex].data)) throw std::runtime_error("Fail to load file: primitive positions type should be vector!");
				const float* positionsBuffer = reinterpret_cast<const float*>(std::get<3>(asset->buffers[bv_position.bufferIndex].data).bytes.data() + a_position.byteOffset + bv_position.byteOffset);

				if (!a_normal.bufferViewIndex.has_value()) throw std::runtime_error("Fail to load file: primitive normals must be define.");
				fastgltf::BufferView& bv_normal = asset->bufferViews[a_normal.bufferViewIndex.value()];
				if (!std::holds_alternative<fastgltf::sources::Array>(asset->buffers[bv_normal.bufferIndex].data)) throw std::runtime_error("Fail to load file: primitive normals type should be vector!");
				const float* normalsBuffer = reinterpret_cast<const float*>(std::get<3>(asset->buffers[bv_normal.bufferIndex].data).bytes.data() + a_normal.byteOffset + bv_normal.byteOffset);

				std::vector<Vec3f> positions; positions.reserve(a_position.count);
				std::vector<Vec3f> normals; positions.reserve(a_normal.count);
				for (unsigned int i = 0; i < a_position.count;i++) {
					Vec3f pos = glm::make_vec3(&positionsBuffer[i * 3]);
					Vec3f norm = glm::normalize(glm::make_vec3(&normalsBuffer[i * 3]));
					positions.push_back(Vec3f(pos.x, -pos.z, pos.y));
					normals.push_back(Vec3f(norm.x, -norm.z, norm.y));
				}

				_meshes.push_back(new TriangleMesh(positions,normals,indices));
				_meshes[_meshes.size()-1]->setMaterial(_materials[((p.materialIndex.has_value()) ? startIdMaterials + p.materialIndex.value() : 0)]);
			}
		}
		std::cout << "meshes loaded: " << _meshes.size() - startIdMeshes << std::endl;

		std::cout << "Finished to load " << p_path << std::endl;
	}
}