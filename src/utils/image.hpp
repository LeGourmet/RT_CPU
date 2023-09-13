#ifndef __IMAGE_HPP__
#define __IMAGE_HPP__

#include "utils/defines.hpp"

#include <vector>

namespace RT_CPU
{
	class Image {
	public:
		// --------------------------------------------- DESTRUCTOR / CONSTRUCTOR ----------------------------------------------
		Image(int p_width, int p_height) : _width(p_width), _height(p_height), _pixels(_width*_height*_nbChannels,0) { _pixels.shrink_to_fit(); }
		~Image() {}

		// ----------------------------------------------------- GETTERS -------------------------------------------------------
		inline const int getHeight()			const { return _height; }
		inline const int getWidth()				const { return _width; }
		inline const int getNbChannels()		const { return _nbChannels; }
		inline const unsigned char* getPixels() const { return _pixels.data(); }

		void setPixel(const int p_i, const int p_j, const Vec3f& p_color)
		{
			const int pixelId = (p_i + p_j * _width) * _nbChannels;
			for(int i=0; i<_nbChannels ;i++) _pixels[pixelId+i] = static_cast<unsigned char>(p_color[i] * 255);
		}

	private:
		// ----------------------------------------------------- ATTRIBUTS -----------------------------------------------------
		int _width;
		int _height;
		int _nbChannels = 3;
		std::vector<unsigned char> _pixels;
	};
}

#endif