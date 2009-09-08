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
	getListOfPixel();
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
					pixel.setX(x);
					pixel.setY(y);
					pixel.setZ(z);
					pixel.setT(t);
					///@TODO könnte falsch sein
					pixel.setRed(this->image().get_crop(pixel.getX(),
							pixel.getY(), pixel.getZ(), pixel.getT(), 0));
					pixel.setGreen(this->image().get_crop(pixel.getX(),
							pixel.getY(), pixel.getZ(), pixel.getT(), 1));
					pixel.setBlue(this->image().get_crop(pixel.getX(),
							pixel.getY(), pixel.getZ(), pixel.getT(), 2));
					this->pixelList.push_back(&pixel);
				}
			}
		}
	}
}
;

#endif
