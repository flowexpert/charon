/*	Copyright (C) 2009 Jens-Malte Gottfried

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
/**	@file ModelToolBar.h
 *	@brief Declaration of class ModelToolBar
 *
 *	@date	28.11.2008
 *	@author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#ifndef MODELTOOLBAR_H_
#define MODELTOOLBAR_H_

#include <QToolBar>

class ParameterFileModel;
class GraphModel;

/// Toolbar specialized for graph model handling.
/// Adds by default buttons for node deletion, renaming and disconnection.
/// Further buttons can be added like to a usual QToolBar.
class ModelToolBar : public QToolBar {
	Q_OBJECT

public:
	/// default constructor
	/// @param title	toolbar title
	/// @param parent	parent widget
	ModelToolBar(const QString& title, QWidget* parent = 0);
	
	/// default destructor
	virtual ~ModelToolBar();
	
	/// Add model specific buttons onto toolbar.
	/// This is a separate function to be able to add other buttons
	/// \e before the model buttons.
	void addModelActions();
	
public slots:
	/// set graph model to edit
	/// @param model	new model to use for node addition
	void setModel(ParameterFileModel* model);

private slots:
	/// delete selected node
	void _delete();
	
	/// rename selected node
	void _rename();
	
	/// disconnect selected node
	void _disconnect();
	
private:
	/// used graph model
	GraphModel* _model;
};

#endif /*MODELTOOLBAR_H_*/
