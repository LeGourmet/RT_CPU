#include "stb/stb_image_write.h"

#include "scene/scene.hpp"

#include "utils/chrono.hpp"
#include "utils/image.hpp"

#include <iostream>
#include <string>

using namespace RT_CPU;

#define RENDERING_DIRECT_LIGHTING
#include "renderer/renderer.hpp"

int main(int argc, char** argv)
{
	try
	{
		const int imgWidth = 1920;
		const int imgHeight = 1080;

		Image img = Image(imgWidth, imgHeight);

		Scene scene = Scene(img, 3);
		Renderer renderer = Renderer(16);

		// Launch rendering.
		std::cout << "Rendering..." << std::endl;
		std::cout << "- Image size: " << imgWidth << "x" << imgHeight << std::endl;
		
		Chrono chrono;
		chrono.start();
		renderer.renderImage(scene, img);
		chrono.stop();

		std::cout << "-> Done in " << chrono.elapsedTime() << "s" << std::endl;

		// Save rendered image.
		stbi_write_png("results/image.png", img.getWidth(), img.getHeight(), img.getNbChannels(), img.getPixels(), 0);

		return EXIT_SUCCESS;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Exception caught:" << std::endl << e.what() << std::endl;
		return EXIT_FAILURE;
	}
}