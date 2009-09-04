/*
 * Blockmatching.h
 *
 *  Created on: 12.08.2009
 *      Author: andreas
 */
/** @file Blockmatching.h
 *  @author <a href="mailto:Andreas.Runk@gmx.de">Andreas Runk</a>
 *  @date 12.08.2009
 */

#ifndef BLOCKMATCHING_H_
#define BLOCKMATCHING_H_

#include "ParameteredObject.hxx"
#include "Roi.h"
#include "CImg.h"
#include "Incrementor.h"
#include "SurfaceAnalysis.h"
#include "PixelSelection.h"
#include <vector>

template<typename T>
class BlockMatching: public ParameteredObject

{
protected:
	/// BrightnessModels
	bool constant; ///@param is true if Constant Brigthnes change has to be done
	bool diffusion; ///@param is true if Diffusion Brigthnes change has to be done
	bool exponential; ///@param is true if Exponential Brigthnes change has to be done
	bool movingIllumination; ///@param is true if MovingIllumination Brigthnes change has to be done
	/// MotionModels
	bool localAffines; ///@param is true if LocalAffines Motion change has to be done
	bool localConstant; ///@param is true if LocalConstant Motion change has to be done
	bool localPlanarity; ///@param is true if LocalPlanarity Motion change has to be done
	bool localRotation; ///@param is true if LocalRotation Motion change has to be done
	bool localStretch; ///@param is true if LocalStretch Motion change has to be done

public:
	/// standard constructor
	BlockMatching(const std::string& name);
	InputSlot<cimg_library::CImg<T> > sequence;
	InputSlot<Roi<int>*> range;
	InputSlot<std::vector<Pixel<T> >*> pixelList;
	InputSlot<Incrementor<T> *> newpos;
	InputSlot<std::vector<std::string>* > changes;
	/// standard execute from Parametered Object
	virtual void execute() =0;
	virtual void findFlow() =0;
	void setChanges ();

};

#endif /* BLOCKMATCHING_H_ */
