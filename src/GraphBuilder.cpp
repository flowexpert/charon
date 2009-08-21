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
/**	@file	GraphBuilder.cpp
 *	@brief	Implementation of class GraphBuilder
 *
 *	@date	06.11.2008
 *	@author	<a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#include "GraphBuilder.h"
#include "GraphModel.h"
#include <graphviz/graph.h>

#include "GraphBuilder.moc"

GraphBuilder::GraphBuilder(GraphModel* newModel, QObject* myParent) :
		QObject(myParent),
		_graph(0),
		_model(newModel),
		_rankLR(true) {

	// initialize graphviz
	aginit();
	_resetGraph();
	
	Q_ASSERT(_model);
	connect(_model, SIGNAL(graphChanged()),
		this, SLOT(updateGraph()));
}

GraphBuilder::~GraphBuilder() {
	// graphviz cleanup
	agclose(_graph);
}

void GraphBuilder::_resetGraph() {
	if(_graph)
		agclose(_graph);
	_graph = agopen(const_cast<char*> ("g"), AGDIGRAPH);
	
	// graph direction depends on _rankLR property
	if(_rankLR)
		agraphattr(_graph, const_cast<char*> ("rankdir"),
				const_cast<char*> ("LR"));
	else
		agraphattr(_graph, const_cast<char*> ("rankdir"),
				const_cast<char*> ("TB"));
	
	// default node and edge attributes
	agnodeattr(_graph, const_cast<char*> ("shape"),
			const_cast<char*> ("record"));
	agnodeattr(_graph, const_cast<char*> ("label"),
			const_cast<char*> (""));
	agnodeattr(_graph, const_cast<char*> ("fontname"),
			const_cast<char*> ("sans-serif"));
	agnodeattr(_graph, const_cast<char*> ("style"),
			const_cast<char*> ("filled"));
	agnodeattr(_graph, const_cast<char*> ("fillcolor"),
			const_cast<char*> ("white"));
	agnodeattr(_graph, const_cast<char*> ("color"),
			const_cast<char*> ("black"));
	agedgeattr(_graph, const_cast<char*> ("tailport"),
			const_cast<char*> ("center"));
	agedgeattr(_graph, const_cast<char*> ("headport"),
			const_cast<char*> ("center"));
	agedgeattr(_graph, const_cast<char*> ("label"),
			const_cast<char*> (""));
	agedgeattr(_graph, const_cast<char*> ("fontname"),
			const_cast<char*> ("sans-serif"));
	
	emit graphChanged();
}

void GraphBuilder::setDirection(bool lr) {
	if(_rankLR == lr)
		return;
	_rankLR = lr;
	
	// update graph direction depending on _rankLR property
	if(_rankLR)
		agraphattr(_graph, const_cast<char*> ("rankdir"),
				const_cast<char*> ("LR"));
	else
		agraphattr(_graph, const_cast<char*> ("rankdir"),
				const_cast<char*> ("TB"));
				
	emit graphChanged();
}

graph_t* GraphBuilder::graph() {
	return _graph;
}

const graph_t* GraphBuilder::graph() const {
	return _graph;
}

void GraphBuilder::setModel(GraphModel* model) {
	if(_model == model)
		return;

	// disconnect current connections (if _model still valid)
	if(_model)
		disconnect(_model, SIGNAL(graphChanged()),
			this, SLOT(updateGraph()));
	
	_model = model;
	Q_ASSERT(_model);
	
	// set up new connections
	connect(_model, SIGNAL(graphChanged()),
		this, SLOT(updateGraph()));

	// update
	emit graphChanged();
}
