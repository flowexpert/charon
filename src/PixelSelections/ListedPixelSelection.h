/*
 * ListedPixelSelektion.h
 *
 *  Created on: 13.08.2009
 *      Author: andreas
 */
/** @file ListedPixelSelection.h
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
