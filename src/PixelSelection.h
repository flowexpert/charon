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
 *  @file PixelSelection.h
 *  @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
 *  @date 17.08.2009
 */

#ifndef ABSRACTPIXELSELECTION_H_
#define ABSRACTPIXELSELECTION_H_

#include "ParameteredObject.hxx"
#include "Roi.h"
#include "BlockMatching.h"
#include "CImg.h"
#include <vector>

/// Pixel is a data structure to save one pixel in it
template<typename T>
class Pixel
{
protected:
	///is able to save pixel data
	T x; ///@param x		x position
	T y; ///@param y		y position
	T z; ///@param z		z position
	T t; ///@param t		Time value
	std::vector<T> intensity; ///@param intensity 		intensity at x,y,z position

public:
	void setX(const T& newX);
	const T& getX() const;
	void setY(const T& newY);
	const T& getY() const;
	void setZ(const T& newZ);
	const T& getZ() const;
	void setT(const T& newT);
	const T& getT() const;
	void pushBackIntensity(const T& newI);
	void insertIntensity(const int pos, const T& updatedI);
	const std::vector<T>& getIntensity() const;
};

/// Pixel Selection
/// This class saves all Pixel we are interested in, in a list of pixel
template<typename T>
class PixelSelection: public ParameteredObject
{

public:
	/// standard constructor
	PixelSelection(const std::string& name);
	InputSlot<Roi<int>*> range;
	InputSlot<cimg_library::CImgList<T> &> sequence;
	OutputSlot<std::vector<Pixel<T> > *> pixelList;
	/// the execute method starts the execute method form ParameteredObject
	virtual void execute()=0;
	/// creates the List of Pixel
	virtual void getListOfPixel() =0;

};

#endif /* ABSRACTPIXELSELECTION_H_ */
