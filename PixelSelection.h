/*
 * AbsractPixelSelection.h
 *
 *  Created on: 17.08.2009
 *      Author: andreas
 */

#ifndef ABSRACTPIXELSELECTION_H_
#define ABSRACTPIXELSELECTION_H_

#include "ParameteredObject.hxx"
#include "Roi.h"
#include "BlockMatching.h"
#include "CImg.h"
#include <vector>

template<typename T>
class Pixel
{
public:
	///is able to save pixel data
	float x;		///@param x				x position
	float y;		///@param y				y position
	float z; 		///@param z				z position
	float t;		///@param t				Time value
	T red; 			///@param red 			red intensity at x,y,z position
	T green; 		///@param green			green intensity at x,y,z position
	T blue; 		///@param blue 			blue intensity at x,y,z position
};

/// Pixel Selection
/// This class saves all Pixel we are interested in in a list of pixel
template<typename T>
class PixelSelection: public ParameteredObject
{

public:

	PixelSelection(const std::string& name);
	InputSlot<Roi<int>*> range;
	InputSlot<cimg_library::CImg<T>& > image;
	virtual void execute()=0;
	virtual std::vector<Pixel<T>*> getListOfPixel() const =0;

};


#endif /* ABSRACTPIXELSELECTION_H_ */
