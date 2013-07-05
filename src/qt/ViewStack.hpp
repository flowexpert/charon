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
#include <QWidget>
#include "vigraqt/vigraqimage.hxx"
#include "vigraqt/qimageviewer.hxx"
#include <vigra/stdimage.hxx>
#include <vigra/multi_array.hxx>
#include <charon-utils/ArgosDisplay.h>

#include <QMessageBox>
#include <QVector>
#include <QActionGroup>
#include <QTabWidget>
#include <QPoint>
#include <QTabBar>

class QStatusBar ;
class QActionGroup ;
class QSignalMapper ;

namespace ArgosDisplay {


	struct View {
		View(AbstractPixelInspector* i = 0, RGBChannels mode = NONE) ;

		RGBChannels channelMode ;
		AbstractPixelInspector* inspector ;
	} ;

	class DropTabBar : public QTabBar {
		Q_OBJECT ;

	public:	
		DropTabBar(QWidget* parent = 0) ;

		QPoint _dragStartPos ;

	protected:

		virtual void dropEvent(QDropEvent* event) ;

		virtual void mousePressEvent(QMouseEvent* event) ;

		virtual void mouseMoveEvent(QMouseEvent* event) ;

		virtual void dragEnterEvent(QDragEnterEvent* event) ;

	private:

	signals:
		void tabMoved(DropTabBar* source, int sourceIndex, DropTabBar* dest, int destIndex) ;
	} ;

	class DropTabWidget : public QTabWidget {
		Q_OBJECT ;

	public:
		DropTabWidget(QWidget* parent = 0) ; 
		
		virtual void dropEvent(QDropEvent* event) ;

		virtual void dragEnterEvent(QDragEnterEvent* event) ;

	private slots:
		void moveTabs(DropTabBar* source, int sourceIndex, DropTabBar* dest, int destIndex) ;

	signals:

	} ;

	
	/// creates and manages all imageviewer instances
	class charonwidgets_DECLDIR ViewStack : public QWidget {
		Q_OBJECT
	
	public :
		enum ViewMode {Table = 0, Grey, Rgb3, Rgb4} ;
		
		/// default constructor
		ViewStack(QWidget* parent = 0) ;
		
		/// default destructor
		virtual ~ViewStack() ;

		/// removes all tab widgets
		void clear();

		/// get index of current tab widget
		int currentIndex() const;

		/// set active tab widget (does check for valid index)
		void setCurrentIndex(int index);

		/// get active zoom level
		int getZoomLevel() const;
		/// set active zoom level
		void setZoomLevel(int level /** [in] new default zoom level*/);

		/// register instance of PixelInspector whose image
		/// will be displayed on the next call to linkImage()
		void linkImage(AbstractPixelInspector*) ;

		/// get current viewer
		QWidget *getCurrentViewer();

		const QList<QAction*> layoutActions() const ;

	public slots:

		void switchLayout(int rows, int columns) ;

	protected:
		/// set active tab widget by keypress
		virtual void keyPressEvent(QKeyEvent * event ) ;

	private:
		/// create all QActions
		void _createActions() ;
		
		/// get reference to currently active viewer
		QImageViewer& _currentViewer() const ;

		/// image stack
		QVector<DropTabWidget*> _tabWidgets ;

		QActionGroup* _switchViewModeActs ;
		QSignalMapper* _switchViewModeMapper ;

		QActionGroup _layoutActions ;

		/// switch the display of the current view between RGB and grayscale float
		QAction* _switchColorModeAct ;

		/// save current view as image file
		QAction* _saveCurrentViewAct ;

		/// move current view to center and reset zoom level
		QAction* _centerAndResetZoomAct ;

		QAction* _switchLogModeAct ;

		/// set center pixel and zoom level of all views to the same as the current view
		QAction* _alignAndZoomAct ;

		/// objects to access pixel data
		std::vector<View> _views ;

		/// was update signal issued to event queue ?
		bool _updatePending ;

		/// tab widget index which will be set on next update
		int _index ;

		/// saved zoom level
		int _zoomLevel;

	private slots:
		
		void _switchToNamedLayout(const QString& layout) ;
		
		/// handle mouse movement in ImageDisplays
		void _processMouseMovement(int x, int y) ;

		/// create tab widgets for each instance in _inspectors
		void _linkImages() ;

		///switch the display of the current view between RGB and grayscale float
		void _switchColorMode(int mode) ;

		/// 
		void _switchLogMode() ;

		/// save current view as image file
		void _saveCurrentView() ;

		/// move current view to center and reset zoom level
		void _centerAndResetZoom() ;

		void _alignAndZoom() ;

		void _emitDimensionMessage() ;

		QWidget* _createImageTableView(AbstractPixelInspector* inspector) ;

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
