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
	T x; ///@param x				x position
	T y; ///@param y				y position
	T z; ///@param z				z position
	T t; ///@param t				Time value
	T red; ///@param red 			red intensity at x,y,z position
	T green; ///@param green		green intensity at x,y,z position
	T blue; ///@param blue 			blue intensity at x,y,z position

public:
	void setX(const T& newX);
	const T& getX() const;
	void setY(const T& newY);
	const T& getY() const;
	void setZ(const T& newZ);
	const T& getZ() const;
	void setT(const T& newT);
	const T& getT() const;
	void setRed(const T& newRed);
	const T& getRed() const;
	void setBlue(const T& newBlue);
	const T& getBlue() const;
	void setGreen(const T& newGreen);
	const T& getGreen() const;

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
	InputSlot<cimg_library::CImg<T>&> image;
	OutputSlot<std::vector<Pixel<T> > *> pixelList;
	/// the execute method starts the execute method form ParameteredObject
	virtual void execute()=0;
	/// creates the List of Pixel
	virtual void getListOfPixel() =0;

};

#endif /* ABSRACTPIXELSELECTION_H_ */
