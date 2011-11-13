/*  Copyright (C) 2009 Jens-Malte Gottfried

	This file is part of Tuchulcha.

	Tuchulcha is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Tuchulcha is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with Tuchulcha.  If not, see <http://www.gnu.org/licenses/>.
*/
/** \file   AdvancedInspector.h
 *  \brief  Declaration of class AdvancedInspector
 *  \date   17.10.2008
 *  \author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */
#ifndef ADVANCEDINSPECTOR_H_
#define ADVANCEDINSPECTOR_H_

#include "ObjectInspector.h"

class QPushButton;
class QCheckBox;


///	Enhances standart ObjectInspector with edit options.
///	Add/remove parameters, enable/disable usage of metadata.
class AdvancedInspector : public ObjectInspector {
	Q_OBJECT
	
public:
	/**	Default constructor, setting parent widget.
	 *	@param parent	parent widget
	 *	@param model	model to use in ObjectInspector
	 */
	AdvancedInspector(QWidget* parent = 0, ParameterFileModel* model = 0);

	/// default destructor
	virtual ~AdvancedInspector();

public slots:
	/// Enable/Disable structural editing
	/// @param on		set editing
	virtual void setEdit(bool on);
	
	/// Change used model
	///	@param model	new model to use
	virtual void setModel(ParameterFileModel* model);
	
private:
	/// setup widget layout
	void init();

	QFrame*				_advancedFrame;	///< frame containing edit buttons
	QCheckBox*			_useMetadata;	///< hide non-configurable parameters
	QCheckBox*			_onlyparams;	///< hide non-configurable parameters
	QPushButton* 		_addButton;		///< button to add lines
	QPushButton* 		_delButton;		///< button to delete lines
	QPushButton* 		_clearButton;	///< button to clear list
};

#endif /*ADVANCEDINSPECTOR_H_*/
