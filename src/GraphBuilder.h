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
/**	@file	GraphBuilder.h
 *	@brief	Declaration of class GraphBuilder
 *
 *	@date	06.11.2008
 *	@author	<a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#ifndef GRAPHBUILDER_H_
#define GRAPHBUILDER_H_

#include <QObject>
#include <QPointer>
#include <graphviz/types.h>

class GraphModel;

/// Abstract class for graph generation
class GraphBuilder : public QObject {
	Q_OBJECT

public:
	/// default constructor
	/// @param model		model to use as graph source
	/// @param parent		parent object
	GraphBuilder(GraphModel* model, QObject* parent = 0);

	/// default destructor
	virtual ~GraphBuilder();
	
	///	Get FlowGraph corresponding to ParameterFile content.
	///	@return 			graphviz graph
	virtual graph_t* graph();

	/// const version
	virtual const graph_t* graph() const;
	
public slots:
	/// update graph
	virtual void updateGraph() = 0;

	/// set model
	/// @param model	new model to set
	void setModel(GraphModel* model);

protected:
	/// Set Graph direction (LR or TB).
	/// @param lr		true: LR, false: TB
	void setDirection(bool lr);

	/// reset graph to defaults
	virtual void _resetGraph();

	/// graph pointer
	graph_t* _graph;

	/// used graph model
	QPointer<GraphModel> _model;
	
private:
	/// graph rank direction from left to right
	bool _rankLR;
	
signals:
	/// redisplay graph
	void graphChanged();
};

#endif /*GRAPHBUILDER_H_*/
