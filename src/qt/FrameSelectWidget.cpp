/*  Copyright (C) 2011 Jens-Malte Gottfried

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
/** \file FrameSelectWidget.cpp
 *  Implementation of class FrameSelectWidget.
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 *  \date 05.01.2011
 */

#include "FrameSelectWidget.hpp"
#include "ui_FrameSelectWidget.h"

FrameSelectWidget::FrameSelectWidget(
		ParameteredObject* pp,
		Parameter<bool>& cV,
		Parameter<uint>& z,
		Parameter<uint>& t,
		Parameter<uint>& v,
		QWidget* pw) :
	QDockWidget(pw),
	_active(false),
	_ui(new Ui::FrameSelectWidget),
	_parent(pp),
	_display(0),
	_cropV(cV),
	_z(z),
	_t(t),
	_v(v),
	_dz(0),
	_dt(0),
	_dv(0),
	_updatePending(false)
{
	this->setEnabled(false) ;
	_ui->setupUi(this);
	QObject::connect(this, SIGNAL(updatePending()), this, SLOT(_updateWidget()), Qt::QueuedConnection) ;
}

FrameSelectWidget::~FrameSelectWidget()
{
	delete _ui;
}

void FrameSelectWidget::setDisplay(ParameteredObject* d) {
	_display = d;
}

void FrameSelectWidget::updateWidget() {
	if(!_updatePending)
	{
		_updatePending = true ;
		emit updatePending() ;
	}
}

void FrameSelectWidget::setShape(uint dz, uint dt, uint dv) {

	_dz = dz ; _dt = dt ; _dv = dv ;

}

void FrameSelectWidget::setTitle(const std::string& title) {
	_title = QString::fromStdString(title) ;
}

void FrameSelectWidget::setCropV(bool val) {
	if(!_active)
		return ;
	bool& cV = _cropV();
	if(cV == val)
		return;
	cV = val;
	_updatePlugin();
}

void FrameSelectWidget::setDim2(int val) {
	if(!_active)
		return ;
	if((int)_z==val)
		return;
	_ui->slider2->setValue(val);
	_z() = val;
	_updatePlugin();
}

void FrameSelectWidget::setDim3(int val) {
	if(!_active)
		return ;
	if((int)_t==val)
		return;
	_ui->slider3->setValue(val);
	_t() = val;
	_updatePlugin();
}

void FrameSelectWidget::setDim4(int val) {
	if(!_active)
		return ;
	if((int)_v==val)
		return;
	_ui->slider4->setValue(val);
	_v() = val;
	_updatePlugin();
}

void FrameSelectWidget::_updatePlugin() {
	if(_active && _display && _display->executed()) {
		_parent->resetExecuted();
		_display->run();
	}
}

void FrameSelectWidget::_updateWidget() {
	if(!_updatePending)
		return ;
	int lz = _z() ;
	int lt = _t() ;
	int lv = _v() ;
	if(_dz <= 0 || _dt <= 0 || _dv <= 0 || lz < 0 || lv < 0 || lt< 0)
	{	this->setEnabled(false) ;
		return ;
	}
	this->setEnabled(true) ;
	_active = true ;
	_ui->spinBox2->setSuffix(QString(" / %1").arg(_dz -1));
	_ui->spinBox3->setSuffix(QString(" / %1").arg(_dt -1));
	_ui->spinBox4->setSuffix(QString(" / %1").arg(_dv - 1));

	// max val is dz/t/v - 1 since values are starting at zero
	_ui->spinBox2->setMaximum(_dz-1);
	_ui->spinBox3->setMaximum(_dt-1);
	_ui->spinBox4->setMaximum(_dv-1);
	_ui->slider2->setMaximum(_dz-1);
	_ui->slider3->setMaximum(_dt-1);
	_ui->slider4->setMaximum(_dv-1);

	_ui->slider2->setValue(lz);
	_ui->slider3->setValue(lt);
	_ui->slider4->setValue(lv);
	bool& cV = _cropV();
	if (_dv != 3) {
		cV = true;
		_ui->checkCropV->setEnabled(false);
	}
	_ui->checkCropV->setChecked(cV);

	_updatePending = false ;
}
