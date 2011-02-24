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
/** \file StatisticsDisplayWidget.cpp
 *  Declaration of class StatisticsDisplayWidget.
 *  \author <a href="mailto:stephan.meister@iwr.uni-heidelberg.de">Stephan Meister</a>
 *  \date 05.01.2011
 */

#include "StatisticsDisplayWidget.hpp"
#include <QtGui>


namespace StatisticsDisplay {

StatisticsDisplayWidget::StatisticsDisplayWidget(const std::string& title,QWidget* parent) : 
	QDockWidget(parent),
	_tabWidget(0),
	_updatePending(false),
	_title(QString::fromStdString(title))
{
	//create tab widget with one tab for each input image
	this->setObjectName(QString("Statistics")) ;
	_tabWidget = new QTabWidget(this) ;
		_tabWidget->setUsesScrollButtons(true) ;
		_tabWidget->setTabPosition(QTabWidget::East) ;
		_tabWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum) ;

	this->setWidget(_tabWidget) ;
	this->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea) ;
	
	this->setWindowTitle(QString("StatisticsDisplay [") + _title + QString("]") ) ;
	this->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable) ;

	connect(this, SIGNAL(statsUpdated()), this, SLOT(_updateStats()), Qt::QueuedConnection) ;
}

void StatisticsDisplayWidget::setTitle(const std::string& title)
{
	_title = QString::fromStdString(title) ;
	if(!_updatePending)
	{
		_updatePending = true ;
		emit statsUpdated() ;
	}
}

StatisticsDisplayWidget::~StatisticsDisplayWidget()
{
	;
}


void StatisticsDisplayWidget::clearContent()
{
	while(_tabWidget->count() > 0)
	{
		delete _tabWidget->widget(0) ;
	}
}

void StatisticsDisplayWidget::_updateStats()
{
	if(!_updatePending)
		return ;

	this->setWindowTitle(QString("StatisticsDisplay [") + _title + QString("]") ) ;

	//rember the current index and set it again if possible once the new Stats have been entered
	int index = _tabWidget->currentIndex() ;
	
	clearContent() ;
	
	//create a tab for every Statistics object
	std::vector<Statistics>::const_iterator it1 = _stats.begin() ;
	for(; it1 != _stats.end() ; it1++)
	{
		QWidget* tab = new QWidget ;
		QVBoxLayout* vLayout = new QVBoxLayout ;
		vLayout->setContentsMargins(1,1,1,1) ;
		QGridLayout* layout = new QGridLayout ;
		layout->setContentsMargins(1,1,1,1) ;

		//create a caption-label and a value-LineEdit for every pair in a Statistics Object
		const Statistics& s = (*it1) ;
		std::map<std::string, double>::const_iterator it = s.stats.begin() ;
		int row = 0 ;
		for(; it != s.stats.end() ; it++)
		{
			QLabel* label = new QLabel(QString::fromStdString(it->first), tab) ;
			//use a line edit for the values to make select-and-copy possible
			QLineEdit* line = new QLineEdit(QString("%2").arg(it->second), tab) ;
			line->setReadOnly(true) ;
			line->setFrame(false) ;
			layout->addWidget(label, row, 0) ;
			layout->addWidget(line, row++, 1) ;
		}
		//set the layout so it does not spread over the whole available size
		vLayout->addLayout(layout) ;
		vLayout->addStretch() ;
		tab->setLayout(vLayout) ;
		_tabWidget->addTab(tab, QString::fromStdString(s.origin)) ;
	}
	if(index < _tabWidget->count())
	{	_tabWidget->setCurrentIndex(index) ;	}

	_updatePending = false ;
}

void StatisticsDisplayWidget::updateStats(const std::vector<Statistics>& stats)
{
	_stats = stats ;
	if(!_updatePending)
	{
		_updatePending = true ;
		emit statsUpdated() ;
	}
}

} //namespace StatisticsDisplay