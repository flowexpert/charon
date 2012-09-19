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
		Parameter<std::string>& maskType,
		const std::string& title,
		QWidget* pw) :
	QDockWidget(pw),
	_running(false),
	_parent(pp),
	_display(0),
	_minimap(minimap),
	_begin(begin),
	_end(end),
	_maskType(maskType),
	_currentMin(0.0),
	_currentMax(0.0),
	_step(0.0)
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
	_mapBox->setChecked(_minimap());

	//line edit
	_minDisp = new QLineEdit("0");
	_maxDisp = new QLineEdit("0");
	_minDisp->setAlignment(Qt::AlignLeft);
	_maxDisp->setAlignment(Qt::AlignLeft);
	QDoubleValidator* validator = new QDoubleValidator(_begin(),_begin(),2,0) ;
	validator->setNotation(QDoubleValidator::ScientificNotation) ;
	_minDisp->setValidator(validator) ;
	_minDisp->setText(QString("%1").arg(_begin(),0,'E',2)) ;
	validator = new QDoubleValidator(_end,_end(),2,0) ;
	validator->setNotation(QDoubleValidator::ScientificNotation) ;
	_maxDisp->setValidator(validator) ;
	_maxDisp->setText(QString("%1").arg(_end(),0,'E',2)) ;

	//slider
	_minSlider = new QSlider(Qt::Horizontal);
	_maxSlider = new QSlider(Qt::Horizontal);
	_minSlider->setMaximum(100); _minSlider->setMinimum(0) ;
	_maxSlider->setMaximum(100); _maxSlider->setMinimum(0) ;
	_minSlider->setTracking(true) ;
	_maxSlider->setTracking(true) ;

	//mask select combo box
	_maskSelect = new QComboBox();
	_maskSelect->addItem("BlackWhite");
	_maskSelect->addItem("WhiteBlack");
	_maskSelect->addItem("Rainbow");
	_maskSelect->addItem("Jet");
	_maskSelect->addItem("Custom");
	if(maskType() == "BlackWhite")
		_maskSelect->setCurrentIndex(0) ;
	else if(maskType() == "WhiteBlack")
		_maskSelect->setCurrentIndex(1) ;
	else if(maskType() == "Rainbow")
		_maskSelect->setCurrentIndex(2) ;
	else if(maskType() == "Jet")
		_maskSelect->setCurrentIndex(3) ;
	else if(maskType() == "Custom")
		_maskSelect->setCurrentIndex(4) ;

	//connecting
	connect(_minSlider, SIGNAL(sliderReleased()), this, SLOT(dispMin()));
	connect(_maxSlider, SIGNAL(sliderReleased()), this, SLOT(dispMax()));
	connect(_minDisp, SIGNAL(textEdited(const QString&)), this, SLOT(click()));
	connect(_maxDisp, SIGNAL(textEdited(const QString&)), this, SLOT(click()));
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
	_slideOut->addWidget(_minSlider, 1, 0, 1, 2);
	_slideOut->addWidget(_maxLab, 2, 0);
	_slideOut->addWidget(_maxDisp, 2, 1);
	_slideOut->addWidget(_maxSlider, 3, 0, 3, 2);

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

	this->setWindowTitle("CustomColorMask(" + QString::fromStdString(title) + ")");

	this->setWidget(_central);
}

CustomColorMaskWidget::~CustomColorMaskWidget()
{
}

void CustomColorMaskWidget::dispMax()
{
	if(_running)
		return ;
	double val = double(_maxSlider->value()) * _step + _currentMin ;
	_maxDisp->setText(QString("%1").arg(val,0,'E',2));
	click();
}

void CustomColorMaskWidget::dispMin()
{
	if(_running)
		return ;
	double val = double(_minSlider->value()) * _step + _currentMin ;
	_minDisp->setText(QString("%1").arg(val,0,'E',2));
	click();
}

void CustomColorMaskWidget::click()
{
	//uninitialized
	if(_running || (_currentMin == _currentMax))
		return ;
	_running = true ;

	_begin() = _minDisp->text().toDouble() ;
	_end() = _maxDisp->text().toDouble() ;
	
	_minSlider->setValue((_begin() - _currentMin) / _step);
	_maxSlider->setValue((_end() - _currentMin) / _step);

	_minimap() = _mapBox->isChecked();
	if(_parent)
		_parent->resetExecuted();
	if(_display)
		_display->run();
	_running = false ;
}

void CustomColorMaskWidget::setDisplay(ParameteredObject* display)
{
	_display = display;
}

void CustomColorMaskWidget::setMinMax(double min, double max)
{
	_currentMin = min ;
	const_cast<QDoubleValidator*>(qobject_cast<const QDoubleValidator*>(_minDisp->validator()))->setRange(min,max,2) ;
	_currentMax = max ;
	const_cast<QDoubleValidator*>(qobject_cast<const QDoubleValidator*>(_maxDisp->validator()))->setRange(min,max,2) ;
	_step = (_currentMax - _currentMin) / 100 ;
	_minSlider->setValue((_begin() - _currentMin) / _step);
	_maxSlider->setValue((_end() - _currentMin) / _step);
}

void CustomColorMaskWidget::changeMask(int mask)
{
	_maskType() = _maskSelect->currentText().toStdString();
	click();
}
