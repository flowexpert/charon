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
/** \file MainWindow.h
 *  Declaration of the ArgosDisplay's MainWindow.
 *  \author Stephan Meister
 *  \date 26.11.2010
 */


#ifndef _ARGOSDISPLAY_MAINWINDOW_HPP_
#define _ARGOSDISPLAY_MAINWINDOW_HPP_

#include <QMainWindow>
#include "charonwidgets.h"

namespace ArgosDisplay
{
	class ViewStack ;
	
	/// ArgosDisplay MainWindow
	class charonwidgets_DECLDIR MainWindow : public QMainWindow
	{
		Q_OBJECT ;

	public:
		
		/// standard constructor
		MainWindow() ;

		/// standard destructor
		~MainWindow() ;

		/// link images to the Windows central ViewStack widget
		ViewStack& viewStack() ;

		void addDockWidget(QWidget* widget) ;

	private:

		
		/// central Widget for image view management
		ViewStack* _viewStack ;

		std::vector<QWidget* > _widgets ;

		bool _updatePending ;

	private slots:
		void _addDockWidgets() ;

	signals:
		void widgetAdded() ;


	} ; /* class MainWindow */

} ; /* namespace ArgosDisplay */

#endif /* _ARGOSDISPLAY_MAINWINDOW_HPP_ */