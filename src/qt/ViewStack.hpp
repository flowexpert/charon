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
#include "vigraqt/qimageviewer.hxx"
#include <vigra/stdimage.hxx>
#include <vigra/multi_array.hxx>
#include <charon-utils/ArgosDisplay.h>

#include <QMessageBox>

class QTabWidget ;
class QStatusBar ;

namespace ArgosDisplay {

	/// creates and manages all imageviewer instances
	class charonwidgets_DECLDIR ViewStack : public QWidget {
		Q_OBJECT
	
	public :
		/// default constructor
		ViewStack(QWidget* parent = 0) ;
		
		/// default destructor
		virtual ~ViewStack() ;

		/// removes all tab widgets
		void clear();

		/// get index of current tab widget
		int currentIndex() const ;

		/// set active tab widget (does check for valid index)
		void setCurrentIndex(int index) ;

		///register instance of PixelInspector whose image will be displayed on the next call to linkImage()
		void linkImage(AbstractPixelInspector*) ;

	protected:
		/// set active tab widget by keypress
		virtual void keyPressEvent(QKeyEvent * event ) ;

	private:

		/// create all QActions
		void _createActions() ;
		
		/// get reference to currently active viewer
		QImageViewer& _currentViewer() const ;

		/// image stack
		QTabWidget* _tabWidget ;

		/// switch the display of the current view between RGB and grayscale float
		QAction* _switchColorModeAct ;

		/// save current view as image file
		QAction* _saveCurrentViewAct ;

		/// move current view to center and reset zoom level
		QAction* _centerAndResetZoomAct ;

		/// objects to access pixel data
		std::vector<AbstractPixelInspector*> _inspectors ;

		/// was update signal issued to event queue ?
		bool _updatePending ;

		/// tab widget index which will be set on next update
		int _index ;

	private slots:
		/// handle mouse movement in ImageDisplays
		void _processMouseMovement(int x, int y) ;

		/// create tab widgets for each instance in _inspectors
		void _linkImages() ;

		///switch the display of the current view between RGB and grayscale float
		void _switchColorMode() ;

		/// save current view as image file
		void _saveCurrentView() ;

		/// move current view to center and reset zoom level
		void _centerAndResetZoom() ;

		void _currentChanged(int index) ;

	signals:
		/// export status messages as signal
		void exportStatusMessage(QString message) ;

		/// export dimensions of current image as string
		void exportDimensionsMessage(QString message) ;
	
		/// issued when linkImages() should be called
		void imageLinked() ;
	} ; // class ViewStack


} // namespace ArgosDisplay

#endif /* _ARGOSDISPLAY_VIEWSTACK_HPP_ */
