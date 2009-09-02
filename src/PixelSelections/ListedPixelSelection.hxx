/*
 * PixelSelection.hxx
 *
 *  Created on: 13.08.2009
 *      Author: andreas
 */

#ifndef _LISTEDPIXELSELECTION_HXX_
#define _LISTEDPIXELSELECTION_HXX_

#include "ListedPixelSelection.h"

template<typename T>
ListedPixelSelection<T>::ListedPixelSelection(const std::string& name) :
	PixelSelection<T>::PixelSelection(name)
{

}

template<typename T>
void ListedPixelSelection<T>::execute()
{
	ParameteredObject::execute();
	cimg_library::CImg<T> pic = this->image();
	forRoiX(*(this->range()),x)
	{
		forRoiY(*(this->range()),y)
		{
			forRoiZ(*(this->range()),z)
			{
				forRoiT(*(this->range()),t)
				{
					Pixel<T> pixel;
					pixel.x = x;
					pixel.y = y;
					pixel.z = y;
					pixel.t = t;
					///@TODO könnte falsch sein
					pixel.red = pic.get_crop(pixel.x, pixel.y, pixel.z,
							pixel.t, 0);
					pixel.green = pic.get_crop(pixel.x, pixel.y, pixel.z,
							pixel.t, 1);
					pixel.blue = pic.get_crop(pixel.x, pixel.y, pixel.z,
							pixel.t, 2);
					pixelList.push_back(&pixel);
				}
			}
		}
	}
}

template<typename T>
std::vector<Pixel<T>*> ListedPixelSelection<T>::getListOfPixel() const
{
	return pixelList;
}
;

#endif
