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

/** \file CustomColorMaskWidget.cpp
 *  Implementation of class CustomColorMaskWidget.
 *  \author <a href="mailto:niels.buwen@web.de">Niels Buwen</a>
 *  \date 11.05.2012
 */
#include "CustomColorMaskWidget.hpp"

CustomColorMaskWidget::CustomColorMaskWidget(
		ParameteredObject* pp,
		Parameter<bool>& minimap,
		Parameter<double>& begin,
		Parameter<double>& end,
		int& maskType,
		QString title,
		QWidget* pw) :
	QDockWidget(pw),
	_active(false),
	_parent(pp),
	_display(0),
	_minimap(minimap),
	_begin(begin),
	_end(end),
	_maskType(maskType),
	_updatePending(false)
{
	this->setEnabled(true);

	//init components
	//text
	_title = new QLabel("adjust values: ");
	_minLab = new QLabel("min: ");
	_maxLab = new QLabel("max: ");

	//checkBox
	_mapBox = new QGroupBox("add minimap");
	_mapBox->setFlat(true);
	_mapBox->setCheckable(true);

	//line edit
	_minDisp = new QLineEdit("0");
	_maxDisp = new QLineEdit("0");
	_minDisp->setAlignment(Qt::AlignLeft);
	_maxDisp->setAlignment(Qt::AlignLeft);

	//slider
	_min = new QSlider(Qt::Horizontal);
	_max = new QSlider(Qt::Horizontal);
	_min->setMaximum(100);
	_max->setMaximum(100);

	//mask select combo box
	_maskSelect = new QComboBox();
	_maskSelect->addItem("Black-White");
	_maskSelect->addItem("White-Black");
	_maskSelect->addItem("Rainbow");

	//connecting
	connect(_min, SIGNAL(valueChanged(int)), this, SLOT(dispMin(int)));
	connect(_max, SIGNAL(valueChanged(int)), this, SLOT(dispMax(int)));
	connect(_minDisp, SIGNAL(returnPressed()), this, SLOT(minRet()));
	connect(_maxDisp, SIGNAL(returnPressed()), this, SLOT(maxRet()));
	connect(_maskSelect, SIGNAL(currentIndexChanged(int)), this, SLOT(changeMask(int)));
	connect(_mapBox, SIGNAL(clicked()), this, SLOT(click()));

	//central frame
	_central = new QFrame();
	_inner = new QFrame();

	//layout
	_layout = new QVBoxLayout();
	_cLayout = new QVBoxLayout();
	_slideOut = new QGridLayout();
	_minOut = new QHBoxLayout();
	_maxOut = new QHBoxLayout();

	_slideOut->addWidget(_minLab, 0, 0);
	_slideOut->addWidget(_minDisp, 0, 1);
	_slideOut->addWidget(_min, 1, 0, 1, 2);
	_slideOut->addWidget(_maxLab, 2, 0);
	_slideOut->addWidget(_maxDisp, 2, 1);
	_slideOut->addWidget(_max, 3, 0, 3, 2);

	_layout->addWidget(_title);
	_layout->addLayout(_slideOut);
	_layout->addWidget(_mapBox);
	_layout->addWidget(_maskSelect);


	_inner->setLayout(_layout);
	_inner->setLineWidth(1);
	_inner->setMidLineWidth(1);
	_inner->setFrameStyle(QFrame::Box | QFrame::Raised);
	
	_cLayout->addWidget(_inner);
	_cLayout->insertStretch(-1);
	
	_central->setLayout(_cLayout);

	this->setWindowTitle("CustomColorMask(" + title + ")");

	this->setWidget(_central);
}

CustomColorMaskWidget::~CustomColorMaskWidget()
{
	delete _title;
	delete _layout;
}

void CustomColorMaskWidget::dispMax(int i)
{
	_maxDisp->setText(QString::number((double)i*_step+_low));
	click();
}

void CustomColorMaskWidget::dispMin(int i)
{
	_minDisp->setText(QString::number((double)i*_step+_low));
	click();
}

void CustomColorMaskWidget::click()
{
	//begin, end <= slider Position
	double minPos = _min->value()*_step+_low;
	double maxPos = _max->value()*_step+_low;
	//begin, end <= line edit text
	if(_minDisp->text().toDouble() < minPos || _minManual)
		minPos = _minDisp->text().toDouble();
	if(_maxDisp->text().toDouble() > maxPos || _maxManual)
		maxPos = _maxDisp->text().toDouble();
	
	_begin() = minPos;
	_end() = maxPos;	
	_minimap() = _mapBox->isChecked();
	_parent->resetExecuted();
	_display->run();
}

void CustomColorMaskWidget::slideMin(QString s)
{
	_min->setValue((s.toDouble()-_low)/_step);
}

void CustomColorMaskWidget::slideMax(QString s)
{
	_max->setValue((s.toDouble()-_low)/_step);
}

void CustomColorMaskWidget::setDisplay(ParameteredObject* display)
{
	_display = display;
}

void CustomColorMaskWidget::setMinMax(double min, double max, bool map, bool custom)
{
	_low = min;
	_step = (max-min)/100;
	_minDisp->setText(QString::number(min));
	_maxDisp->setText(QString::number(max));
	_mapBox->setChecked(map);
	_min->setValue((min-_low)/_step);
	_max->setValue((max-_low)/_step);
	if(custom && _maskSelect->count() < 4)
		_maskSelect->addItem("Custom");
}

void CustomColorMaskWidget::changeMask(int mask)
{
	_maskType = mask;
	click();
}

void CustomColorMaskWidget::minRet()
{
	_minManual = true;
	click();
}

void CustomColorMaskWidget::maxRet()
{
	_maxManual = true;
	click();
}