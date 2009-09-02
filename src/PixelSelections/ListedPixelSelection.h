/*
 * PixelSelektion.h
 *
 *  Created on: 13.08.2009
 *      Author: andreas
 */

#ifndef PIXELSELEKTION_H_
#define PIXELSELEKTION_H_

#include "Roi.h"
//#include "BlockMatching.h"
#include "PixelSelection.hxx"
#include "CImg.h"
#include <vector>

template<typename T>
class ListedPixelSelection : public PixelSelection<T> {

public:

	ListedPixelSelection(const std::string& name);
	void execute();
	std::vector<Pixel<T>*> getListOfPixel() const ;

private:
	std::vector<Pixel<T>*> pixelList;
};

#endif /* PIXELSELEKTION_H_ */
