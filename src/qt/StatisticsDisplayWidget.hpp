/*  Copyright (C) 2011 Stephan Meister

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
/** \file StatisticsDisplayWidget.hpp
 *  Declaration of class StatisticsDisplayWidget.
 *  \author <a href="mailto:stephan.meister@iwr.uni-heidelberg.de">Stephan Meister</a>
 *  \date 05.01.2011
 */

#ifndef STATSDISPLAYWIDGET_HPP_
#define STATSDISPLAYWIDGET_HPP_

#include <QDockWidget>
#include <QVector>
#include <QString>
#include "charonwidgets.h"
#include <charon-utils/StatisticsDisplay.h>
#include <string>

class QLabel ;
class QLineEdit ;
class QTabWidget ;

namespace StatisticsDisplay {

/// Widget for Displaying Image Statistics
class charonwidgets_DECLDIR StatisticsDisplayWidget : public QDockWidget
{
	Q_OBJECT ;
public:
	
	/// default constructor
	StatisticsDisplayWidget(const std::string& title = "", QWidget* parent = 0) ;
	
	/// default destructor
	virtual ~StatisticsDisplayWidget() ;
	
	/// create new tabs and populate with values
	void updateStats(const std::vector<Statistics>& stats) ;

	void setTitle(const std::string& title) ;

private:
	
	/// reset current layout and remove all tabs
	void clearContent() ;

	/// central widget
	QTabWidget* _tabWidget ;

	QString _title ;

	bool _updatePending ;

	std::vector<Statistics> _stats ;

private slots:

	void _updateStats() ;

signals:

	void statsUpdated() ;

} ; /* class StatisticsDisplayWidget */

} ; /* namespace StatisticsDisplay */

#endif /* STATSDISPLAYWIDGET_HPP_ */