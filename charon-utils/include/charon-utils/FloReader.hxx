/*
	This file is part of Charon.

	Charon is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License
        as published by the Free Software Foundation,
        either version 3 of the License, or (at your option) any later version.

	Charon is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Charon.  If not, see <http://www.gnu.org/licenses/>.
*/
/// @file FloReader.hxx
/// Implementation of the parameter class FloReader.
/// @author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">Michael Baron</a>
/// @date 24.10.2011

#ifndef _FLOREADER_HXX_
#define _FLOREADER_HXX_

#include "FloReader.h"

template <typename T>
FloReader<T>::FloReader(const std::string& name) :
		TemplatedParameteredObject<T>("floreader", name,
			"read motion from motion files (*.flo files) using CImg.<br>"
			"A reference implementation of this file format can be found at "
			"http://vision.middlebury.edu/flow/submit/ ."
		)
{
	ParameteredObject::_setTags("charon-utils;CImg;DiskIO") ;

	ParameteredObject::_addParameter (filename, "filename",
		"filename to read image from", "fileopen");
	ParameteredObject::_addParameter (invToZero, "invToZero",
		"set invalid flow values to zero", false);
	ParameteredObject::_addOutputSlot(out, "out",
		"image output", "CImgList<T>");
	ParameteredObject::_addOutputSlot(valid, "valid",
		"flow validity mask (1=valid,0=flow unknown)", "CImgList<T>");
}

// from http://vision.middlebury.edu/flow/code/flow-code/README.txt
template <typename T>
const float FloReader<T>::maxKnown = 1.e9;

template <typename T>
void FloReader<T>::execute() {
	try {
		const char *fn = filename().c_str();
		FILE *stream = fopen(fn, "rb");

		if (!stream) {
			ParameteredObject::raise("Failed to open file!");
		}

		int width, height;
		float tag;
		size_t read;

		cimg_library::CImgList<T>& o = out();
		valid().assign(1u);
		cimg_library::CImg<T>& v = valid()[0];

		read = fread(&tag,    sizeof(float), 1, stream);
		if (read != 1) {
			throw std::runtime_error("error reading flo tag");
		}
		read = fread(&width,  sizeof(int),   1, stream);
		if (read != 1) {
			throw std::runtime_error("error reading flo width");
		}
		read = fread(&height, sizeof(int),   1, stream);
		if (read != 1) {
			throw std::runtime_error("error reading flo height");
		}

		int nBands = 2;
		o.assign(nBands, width, height, 1, 1);
		v.assign(width,height);
		v.fill(T(1));
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < nBands*width; x++) {
				float fv;
				read = fread(&fv, sizeof(float), 1, stream);
				if (read != 1) {
					throw std::runtime_error("error reading flo values");
				}
				o.atNXY(x%nBands, x/nBands, y) = T(fv);
				if (std::abs((double)fv) > maxKnown) {
					v(x/nBands, y) = T(0);
					if (invToZero()) {
						cimglist_for(o,kk) {
							o.atNXY(kk, x/nBands, y) = T(0);
						}
					}
				}
			}
		}
		fclose(stream);
	}
	catch (const cimg_library::CImgException& err) {
		ParameteredObject::raise(
					std::string("Could not read file\n\t") + err.what());
	}

}

#endif /* _FLOREADER_HXX_ */

