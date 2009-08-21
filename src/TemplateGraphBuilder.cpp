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
/**	@file	TemplateGraphBuilder.cpp
 *	@brief	Implementation of class TemplateGraphBuilder
 *
 *	@date	21.11.2008
 *	@author	<a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#include "TemplateGraphBuilder.h"
#include "GraphModel.h"
#include "ParameterFile.hxx"
#include "MetaData.h"
#include <QStringList>
#include <graphviz/graph.h>
#include <map>

#include "TemplateGraphBuilder.moc"

TemplateGraphBuilder::TemplateGraphBuilder(GraphModel* newModel,
	QObject* myParent) :
		GraphBuilder(newModel, myParent) {
	// show templates side by side horizontally
	setDirection(false);
}

TemplateGraphBuilder::~TemplateGraphBuilder() {
}

void TemplateGraphBuilder::updateGraph() {
	// clear current content
	_resetGraph();
	
	std::map<std::string,node_t*> nodemap;

	// detect objects
	std::vector<std::string> keys = _model->parameterFile().getKeyList();
	for (uint i = 0; i < keys.size(); i++) {
		ulong pos = keys[i].find(".");
		if ((pos == std::string::npos) || (pos == 0))
			continue;
		std::string node = keys[i].substr(0, pos);
		if (nodemap.find(node) != nodemap.end())
			continue; // node already detected
		
		if(!_model->nodeValid(node.c_str()))
			throw tr("Node %1 invalid!").arg(node.c_str());

		nodemap[node] = agnode(_graph, const_cast<char*> (node.c_str()));
		std::ostringstream label;
		std::string className = _model->getClass(node);
		
		// here we need an aditional { / } pair due to horizontal direction
		// the node name is also not needed to display
		label << "{" << className << "|" /* << node */;
		std::vector<std::string> inputs  =
			_model->metaInfo()->getInputs (className);
		std::vector<std::string> outputs =
			_model->metaInfo()->getOutputs(className);

		// add input/output slots
		uint  inputAnz =  inputs.size();
		uint outputAnz = outputs.size();
		uint       max = qMax(inputAnz, outputAnz);
		for(uint j=0; j < max; j++) {
			label << "|{";
			if (j < inputAnz)
				label << "<" <<  inputs[j] << ">" <<  inputs[j];
			label << "|";
			if (j < outputAnz)
				label << "<" << outputs[j] << ">" << outputs[j];
			label << "}";
		}
		label << "}";
		agset(nodemap[node], const_cast<char*> ("label"),
				const_cast<char*>(label.str().c_str()));
	}

	// here, there are no edges

	// update viewers
	emit graphChanged();
}

void TemplateGraphBuilder::setModel(GraphModel* model) {
    GraphBuilder::setModel(model);
    updateGraph();
}