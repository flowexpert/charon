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
/**	@file	HierarchyGraphBuilder.h
 *	@brief	Declaration of class HierarchyGraphBuilder
 *
 *	@date	19.11.2008
 *	@author	<a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#ifndef HIERARCHYGRAPHBUILDER_H_
#define HIERARCHYGRAPHBUILDER_H_

#include "GraphBuilder.h"

/// Create a graph from a graph model.
class HierarchyGraphBuilder : public GraphBuilder {
	Q_OBJECT
	
public:
	/// default constructor
	/// @param model	model to use as graph source
	/// @param parent	parent object
	HierarchyGraphBuilder(GraphModel* model, QObject* parent = 0);
	
	/// default destructor
	virtual ~HierarchyGraphBuilder();
	
public slots:
	void updateGraph();
};

#endif /*HIERARCHYGRAPHBUILDER_H_*/
