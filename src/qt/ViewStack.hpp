/*  Copyright (C) 2010 Stephan Meister

    This file is part of Charon.

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
/** \file ViewStack.h
 *  Declaration of the ArgosDisplay ViewStack.
 *  \author Stephan Meister
 *  \date 29.11.2010
 */

#ifndef _ARGOSDISPLAY_VIEWSTACK_HPP_
#define _ARGOSDISPLAY_VIEWSTACK_HPP_

#include "charonwidgets.h"
//#include <charon-core/ParameteredObject.hxx>
#include <QWidget>
#include "vigraqt/vigraqimage.hxx"
#include <vigra/stdimage.hxx>
#include <vigra/multi_array.hxx>
#include <map>

#include <QMessageBox>

class QTabWidget ;
class QStatusBar ;

namespace ArgosDisplay {

	typedef vigra::MultiArrayView<5, int> VigraIntArray ;
	typedef vigra::MultiArrayView<5, float> VigraFloatArray ;
	typedef vigra::MultiArrayView<5, double> VigraDoubleArray ;

	class PixelInspector ;


	/// creates and manages all imageviewer instances
	class charonwidgets_DECLDIR ViewStack : public QWidget
	{
	Q_OBJECT ;
	
	public :
		
		/// default constructor
		ViewStack(QWidget* parent = 0) ;
		
		/// default destructor
		~ViewStack() ;


		/// add image to ViewStack for display
		template<typename T>
		void linkImage(const vigra::MultiArrayView<5, T>&, const std::string& type, const std::string& name = "") ;

	private:


		/// image stack
		QTabWidget* _tabWidget ;

		QStatusBar* _statusBar ;

		///pointers to pixel data for each possible template type
		std::vector<const VigraIntArray* const> _intImgMap ;
		std::vector<const VigraFloatArray* const> _floatImgMap ;
		std::vector<const VigraDoubleArray* const> _doubleImgMap ;

		/// create new FImageViewer and display float image
		void linkFloatImage(const vigra::FImage& img, const std::string& name) ;
		
		/// create new QImageViewer and display color image
		void linkRgbaImage(const vigra::QRGBImage& img, const std::string& name) ;


	private slots:

		/// handle mouse movement in ImageDisplays
		void processMouseMovement(int x, int y) ;

	signals:

		/// export status messages as signal
		void exportStatusMessage(QString message) ;
	} ; // class ViewStack

	template<typename T>
	void ViewStack::linkImage(const vigra::MultiArrayView<5, T>& mArray, const std::string& type, const std::string& name)
	{
		//cast the array from the template to the real type and save a pointer in a vector
	
		//otherwise ViewStack and several other Widget classes would need to be template classes themself, but Q_OBJECTS may not be templated.
		//The other solution for the widget to access the individual pixel values would be to make extra double-precission copies of every input image, which would be expensive
		//This section needs to be reworked when new base template types for Plugins would ever be introduced
		if(type == std::string("vigraarray5<int>"))
		{	_intImgMap.push_back(reinterpret_cast<const vigra::MultiArrayView<5, int>* >(&mArray))	;	}
		else if(type == std::string("vigraarray5<float>"))
		{	_floatImgMap.push_back(reinterpret_cast<const vigra::MultiArrayView<5, float>* >(&mArray))	;	}
		else if(type == std::string("vigraarray5<double>"))
		{	_doubleImgMap.push_back(reinterpret_cast<const vigra::MultiArrayView<5, double>* >(&mArray))	;	}
		else
		{	throw std::runtime_error("ViewStack::linkImage: unknown template type! only int, float and double are supported!") ;	}

		//intepret image as color image with range (0-255) if exactly 3 channels are present (make this switchable later)
		if(mArray.size(3) == 3)
		{	vigra::QRGBImage img(mArray.size(0), mArray.size(1)) ;
			for(int xx = 0 ; xx < mArray.size(0) ; ++xx)
				for(int yy = 0 ; yy < mArray.size(1) ; ++yy)
				{	img(xx,yy).red() = mArray(xx,yy,0,0,0) ;
					img(xx,yy).green() = mArray(xx,yy,0,1,0) ;
					img(xx,yy).blue() = mArray(xx,yy,0,2,0) ;
				}
			linkRgbaImage(img, name) ;
		}
		//otherwise interpret first slice of Array as float image
		else
		{	vigra::FImage img(mArray.size(0), mArray.size(1)) ;
			for(int xx = 0 ; xx < mArray.size(0) ; ++xx)
				for(int yy = 0 ; yy < mArray.size(1) ; ++yy)
				{	img(xx,yy) = mArray(xx,yy,0,0,0) ;	}
			linkFloatImage(img, name) ;
		}
	} //ViewStack::linkImage

} ; //namespace ArgosDisplay

#endif /* _ARGOSDISPLAY_VIEWSTACK_HPP_ */