/**
 *  @file PixelSelection.hxx
 *  @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
 *  @date 17.08.2009
 */

#ifndef _PIXELSELECTION_HXX_
#define _PIXELSELECTION_HXX_

#include "PixelSelection.h"

template<typename T>
PixelSelection<T>::PixelSelection(const std::string& name) :
	ParameteredObject("blockmatching", name,
			"save range of interest in a list of pixels")
{
	_addInputSlot(range, "range", "Range of Interest", "roi");
	_addInputSlot(image, "image", "Image to work with", "CImg");
	_addOutputSlot(pixelList, "pixelList", "List of Pixel out of Roi",
			"std::vector<Pixel<T>>*");
}

template<typename T>
void Pixel<T>::setX(const T& newX)
{
	this->x = newX;
}

template<typename T>
const T& Pixel<T>::getX() const
{
	return this->x;
}

template<typename T>
void Pixel<T>::setY(const T& newY)
{
	this->y = newY;
}

template<typename T>
const T& Pixel<T>::getY() const
{
	return this->y;
}

template<typename T>
void Pixel<T>::setZ(const T& newZ)
{
	this->z = newZ;
}

template<typename T>
const T& Pixel<T>::getZ() const
{
	return this->z;
}

template<typename T>
void Pixel<T>::setT(const T& newT)
{
	this->t = newT;
}

template<typename T>
const T& Pixel<T>::getT() const
{
	return this->t;
}

template<typename T>
void Pixel<T>::setRed(const T& newRed)
{
	this->red = newRed;
}

template<typename T>
const T& Pixel<T>::getRed() const
{
	return this->red;
}

template<typename T>
void Pixel<T>::setBlue(const T& newBlue)
{
	this->blue = newBlue;
}

template<typename T>
const T& Pixel<T>::getBlue() const
{
	return this->blue;
}

template<typename T>
void Pixel<T>::setGreen(const T& newGreen)
{
	this->green = newGreen;
}

template<typename T>
const T& Pixel<T>::getGreen() const
{
	return this->green;
}

#endif
