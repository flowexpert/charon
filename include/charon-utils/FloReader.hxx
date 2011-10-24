/*
	This file is part of Charon.

	Charon is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License
        as published by the Free Software Foundation,
        either version 3 of the License, or (at your option) any later version.

	Charon is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
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
			"read motion from motion file using cimg")
{
	this->_addParameter (filename, "filename",
		"filename to read image from", "fileopen");
	this->_addOutputSlot(out, "out",
		"image output", "CImgList<T>");
}

template <typename T>
void FloReader<T>::execute() {
	PARAMETEREDOBJECT_AVOID_REEXECUTION;
	ParameteredObject::execute();
	try
	{
		const char *fn = filename().c_str();
		FILE *stream = fopen(fn, "rb");
		int width, height;
		float tag;

		fread(&tag,    sizeof(float), 1, stream);
		fread(&width,  sizeof(int),   1, stream);
		fread(&height, sizeof(int),   1, stream);

		int nBands = 2;

		out() = cimg_library::CImgList<T>( nBands, width, height, 1, 1 );

		for (int y = 0; y < height; y++)
		for (int x = 0; x < nBands*width; x++)
		{
			float fv;
			fread(&fv, sizeof(float), 1, stream);
//			out().atNXYZC(x/width, x%width, y, 0, 0) = T(fv);
			out().atNXYZC(x%nBands, x/nBands, y, 0, 0) = T(fv);
		}

		fclose(stream);		
	}
	catch(const cimg_library::CImgException& err)
	{
		throw std::runtime_error(
			this->getClassName() + " instance \"" +
			this->getName() + "\" Could not read file\n\t" + err.what());
	}

}

#endif /* _FLOREADER_HXX_ */

