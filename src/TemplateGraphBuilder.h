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
/**	@file	TemplateGraphBuilder.h
 *	@brief	Declaration of class TemplateGraphBuilder
 *
 *	@date	21.11.2008
 *	@author	<a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#ifndef TEMPLATEGRAPHBUILDER_H_
#define TEMPLATEGRAPHBUILDER_H_

#include "GraphBuilder.h"

/// Create a graph from a graph model.
class TemplateGraphBuilder : public GraphBuilder {
	Q_OBJECT
	
public:
	/// default constructor
	/// @param model	model to use as graph source
	/// @param parent	parent object
	TemplateGraphBuilder(GraphModel* model, QObject* parent = 0);
	
	/// default destructor
	virtual ~TemplateGraphBuilder();
	
public slots:
	void updateGraph();
    void setModel(GraphModel* model);
};

#endif /*TEMPLATEGRAPHBUILDER_H_*/
