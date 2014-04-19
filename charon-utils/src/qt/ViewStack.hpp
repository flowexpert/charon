/*  Copyright (C) 2010 Stephan Meister

    This file is part of Charon.

    Charon is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Charon is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
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
#include <QMap>

class QStatusBar ;
class QActionGroup ;
class QSignalMapper ;
class QTableWidget ;

namespace ArgosDisplay {

	/// Helper Widget to allow drap&drop op Tabs between multiple TabBars
	class DropTabBar : public QTabBar {
		Q_OBJECT

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

	/// Helper Widget which allows moving of tabbed Widgets between TabWidgets
	class DropTabWidget : public QTabWidget {
		Q_OBJECT

	public:
		DropTabWidget(QWidget* parent = 0) ; 
		
		virtual void dropEvent(QDropEvent* event) ;

		virtual void dragEnterEvent(QDragEnterEvent* event) ;

		virtual void enterEvent(QEvent* event) ;

	private slots:
		void moveTabs(DropTabBar* source, int sourceIndex, DropTabBar* dest, int destIndex) ;


	signals:
		void mouseEntered(DropTabWidget* src) ;

	} ;

	
	/// creates and manages all imageviewer instances
	class charonwidgets_DECLDIR ViewStack : public QWidget {
		Q_OBJECT
	
	public :
		enum ViewMode {Table = 0, Grey, Rgb3, Rgb4} ;

		struct View {
			View(AbstractPixelInspector* i = 0) ;

			AbstractPixelInspector* inspector ;
			ViewMode viewMode ;
			QWidget* widget ;
		} ;


		/// default constructor
		ViewStack(QWidget* parent = 0) ;
		
		/// default destructor
		virtual ~ViewStack() ;

		/// removes all tab widgets
		void clear();

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

		/// pointer to TabWidget the mouse is currently hovering or the first if none
		DropTabWidget* _currentTabWidget() const ;

		/// image stacks
		QVector<DropTabWidget*> _tabWidgets ;

		/// switches between view modes
		QActionGroup* _switchViewModeActs ;

		/// switches between layouts
		QActionGroup* _layoutActions ;

		/// save current view as image file
		QAction* _saveCurrentViewAct ;

		/// move current view to center and reset zoom level
		QAction* _centerAndResetZoomAct ;

		/// switch greyscale view to log mode and vice versa
		QAction* _switchLogModeAct ;

		/// set center pixel and zoom level of all views to the same as the current view
		QAction* _alignAndZoomAct ;

		/// objects to access pixel data
		QMap<QString, View> _views ;

		/// was update signal issued to event queue ?
		bool _updatePending ;

		/// tab widget index which will be set on next update
		int _index ;

		/// saved zoom level
		int _zoomLevel;

		DropTabWidget* __currentTabWidget ;

	private slots:
		
		///select on which TabWidget Actions will be performed
		void _changeCurrentTabWidget(DropTabWidget* widget) ;

		///select current layout by name
		void _switchToNamedLayout(const QString& layout) ;
		
		/// handle mouse movement in ImageDisplays
		void _processMouseMovement(int x, int y, Qt::MouseButtons buttons) ;

		void _mouseReleased() ;

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

		/// perform pixel exact alignment for current tabwidget
		void _alignAndZoom() ;

		/// send string to be displayed in the status bar
		void _emitDimensionMessage() ;

		/// create a QTableWidget with pixel values of inspector as entries or update an existing one
		QWidget* _createImageTableView(AbstractPixelInspector* inspector, QWidget* widget= 0) ;

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
