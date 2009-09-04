/*
 * ListedPixelSelection.hxx
 *
 *  Created on: 13.08.2009
 *      Author: andreas
 */
/** @file ListedPixelSelection.hxx
 *  @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
 *  @date 13.08.2009
 */

#ifndef _LISTEDPIXELSELECTION_HXX_
#define _LISTEDPIXELSELECTION_HXX_

#include "ListedPixelSelection.h"
#include "PixelSelection.hxx"

template<typename T>
ListedPixelSelection<T>::ListedPixelSelection(const std::string& name) :
	PixelSelection<T>::PixelSelection(name)
{

}

template<typename T>
void ListedPixelSelection<T>::execute()
{
	ParameteredObject::execute();
}

template<typename T>
void ListedPixelSelection<T>::getListOfPixel()
{
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
					pixel.red = this->image().get_crop(pixel.x, pixel.y,
							pixel.z, pixel.t, 0);
					pixel.green = this->image().get_crop(pixel.x, pixel.y,
							pixel.z, pixel.t, 1);
					pixel.blue = this->image().get_crop(pixel.x, pixel.y,
							pixel.z, pixel.t, 2);
					this->pixelList.push_back(&pixel);
				}
			}
		}
	}
}
;

#endif
