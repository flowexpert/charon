/*  Copyright (C) 2012 Niels Buwen

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
/** \file CustomColorMaskWidget.hpp
 *  Declaration of class CustomColorMaskWidget.
 *  \author <a href="mailto:niels.buwen@web.de">Niels Buwen</a>
 *  \date 11.05.2012
 */

#ifndef CustomMASKWIDGET_H
#define CustomMASKWIDGET_H

#include <charon-core/ParameteredObject.h>
#include <QDockWidget>
#include <QString>
#include <QtGui>
#include "charonwidgets.h"
//#include <CustomColorMask.h>

//namespace Ui {
//	class CustomColorMaskWidget;
//}

class charonwidgets_DECLDIR CustomColorMaskWidget : public QDockWidget
{
	Q_OBJECT
private:
	bool _running ;
	ParameteredObject* _parent;
	ParameteredObject* _display;
	Parameter<bool>& _minimap;
	Parameter<double>& _begin;
	Parameter<double>& _end;
	Parameter<std::string>& _maskType;

	void _updatePlugin();

	double _currentMin ;
	double _currentMax ;

	double _step ;

	//Widget Components
	QLabel* _title;
	QLabel*	_minLab;
	QLabel* _maxLab;
	
	QGroupBox* _mapBox;

	QLineEdit* _minDisp;
	QLineEdit* _maxDisp;

	QSlider* _minSlider;
	QSlider* _maxSlider;

	QComboBox* _maskSelect;

	QFrame* _central;
	QFrame* _inner;

	QVBoxLayout* _layout;
	QVBoxLayout* _cLayout;
	QHBoxLayout* _minOut;
	QHBoxLayout* _maxOut;
	QGridLayout* _slideOut;

public:
	
	CustomColorMaskWidget(
			ParameteredObject*,
			Parameter<bool>&,
			Parameter<double>&,
			Parameter<double>&,
			Parameter<std::string>&,
			const std::string&,
			QWidget* parentWidget = 0);
	~CustomColorMaskWidget();

	void setDisplay(ParameteredObject* display);
	void setMinMax(double, double);

private slots:
	void dispMin();
	void dispMax();
	void changeMask(int);
	void click();

};

#endif