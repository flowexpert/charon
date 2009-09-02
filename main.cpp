/*
 * main.cpp
 *
 *  Created on: 12.08.2009
 *      Author: andreas
 */

#include "BlockMatching.hxx"
#include "ListedPixelSelection.hxx"
#include "IncrementorConstant.hxx"
#include "IncrementorDiffusion.hxx"
#include "IncrementorExponential.hxx"
#include "IncrementorMovingIllumination.hxx"
//#include "Crop.hxx"

int main()
{
	ListedPixelSelection <int> 			ka("kaisnbu", 0);
	IncrementorConstant <int>			kb("ahbfsed", 0);
	IncrementorDiffusion <int>			kc("sakbuwe", 0);
	IncrementorExponential <int>		kd("bjkwerz", 0);
	IncrementorMovingIllumination <int>	ke("ahggdad", 0);
	//Crop <int> ka("fkb", 0);

	return 0;

}
