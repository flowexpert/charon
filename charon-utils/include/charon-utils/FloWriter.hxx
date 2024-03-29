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
/// @file FloWriter.hxx
/// Implementation of the parameter class FloWriter.
/// @author <a href="mailto:michael.baron@iwr.uni-heidelberg.de">Michael Baron</a>
/// @date 23.11.2011

#ifndef _FLOWRITER_HXX_
#define _FLOWRITER_HXX_

#include "FloWriter.h"

#define TAG_STRING "PIEH"

template <typename T>
FloWriter<T>::FloWriter(const std::string& name) :
		TemplatedParameteredObject<T>("flowriter", name,
			"write motion to motion (*.flo) file using CImg. "
			"See http://vision.middlebury.edu/flow/submit/ for "
			"reference implementations of this format. "
		)
{
	ParameteredObject::_setTags("charon-utils;CImg;DiskIO") ;

	this->_addParameter (filename, "filename",
		"filename to write image to", "fileopen");
	this->_addInputSlot(in, "in",
		"image input", "CImgList<T>");
}

template <typename T>
void FloWriter<T>::execute() {
	try {
		const char *fn = filename().c_str();
		FILE *stream = fopen(fn, "wb");
		int width, height, nBands;

		const cimg_library::CImgList<T>& i = in();

		width = in()[0].width();
		height = in()[0].height();
		nBands = 2;

		// write the header
		fprintf( stream, TAG_STRING );
		if (   (int)fwrite(&width,  sizeof(int),   1, stream) != 1
			|| (int)fwrite(&height, sizeof(int),   1, stream) != 1)
				ParameteredObject::raise("problem writing header");

		// write the rows
		float fv = 0;
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < nBands*width; x++) {
				fv = (float)(i.atNXYZC(x%nBands, x/nBands, y, 0, 0));
				if ((int)fwrite(&fv, sizeof(float), 1, stream) != 1)
					ParameteredObject::raise("problem writing data");
			}
		}

		fclose(stream);
	}
	catch (const cimg_library::CImgException& err) {
		ParameteredObject::raise(
			std::string("Could not read file: ") + err.what());
	}
}

#endif /* _FLOWRITER_HXX_ */

