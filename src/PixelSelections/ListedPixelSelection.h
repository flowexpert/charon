/*  This file is part of Charon.

 Charon is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Charon is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with Charon.  If not, see <http://www.gnu.org/licenses/>.
 */
/**
 *  @file ListedPixelSelection.h
 *  @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
 *  @date 13.08.2009
 */

#ifndef PIXELSELEKTION_H_
#define PIXELSELEKTION_H_

#include "Roi.h"
#include "PixelSelection.hxx"
#include "CImg.h"
#include <vector>

template<typename T>
class ListedPixelSelection: public PixelSelection<T>
{

public:

	ListedPixelSelection(const std::string& name);
	void execute();
	void getListOfPixel();

private:
	std::vector<Pixel<T>*> pixelList;
};

#endif /* PIXELSELEKTION_H_ */
