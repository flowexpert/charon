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
/** @file   HierarchyGraphBuilder.cpp
 *  @brief  Implementation of class HierarchyGraphBuilder
 *
 *  @date   19.11.2008
 *  @author	<a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#include "HierarchyGraphBuilder.h"
#include "GraphModel.h"
#include "ParameterFile.hxx"
#include "MetaData.h"
#include <QStringList>
#include <graphviz/graph.h>
#include <map>

#include "HierarchyGraphBuilder.moc"

HierarchyGraphBuilder::HierarchyGraphBuilder(GraphModel* newModel,
    QObject* myParent) :
        GraphBuilder(newModel, myParent) {
}

HierarchyGraphBuilder::~HierarchyGraphBuilder() {
}

void HierarchyGraphBuilder::updateGraph() {
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
        label << className << "|" << node;
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
        agset(nodemap[node], const_cast<char*> ("label"),
                const_cast<char*>(label.str().c_str()));
    }

    // now traverse nodes and add edges
    std::map<std::string, node_t*>::iterator nodeIter;
    for (nodeIter = nodemap.begin(); nodeIter != nodemap.end();
            nodeIter++) {
        std::string curNode = nodeIter->first;

        // check for input nodes
        std::string className = _model->getClass(curNode);
        const std::vector<std::string> inputs =
            _model->metaInfo()->getInputs(className);
        std::vector<std::string>::const_iterator inIter;

        for (inIter = inputs.begin(); inIter != inputs.end(); inIter++) {
            // skip unset input nodes
            if (!_model->parameterFile().isSet(curNode + "." + *inIter))
                continue;

            std::vector<std::string> sources =
                    _model->parameterFile().getList<std::string>(
                            curNode + "." + *inIter);
            std::vector<std::string>::const_iterator source;

            for(source = sources.begin(); source != sources.end(); source++) {
                _model->connected(source->c_str(),
                    (curNode + "." + *inIter).c_str());

                std::string sourceObj = ParameterFile::objName(*source);
                std::string sourceSlot = ParameterFile::parName(*source);

                std::string slotType = _model->getType(*source);

                // add edge to graph
                edge_t* edge = agedge(_graph, nodemap[sourceObj],
                                      nodemap[curNode]);
                agset(edge, const_cast<char*>("tailport"),
                    const_cast<char*>(sourceSlot.c_str()));
                agset(edge, const_cast<char*>("headport"),
                    const_cast<char*>(inIter->c_str()));
                agset(edge, const_cast<char*>("label"),
                    const_cast<char*>(slotType.c_str()));
            }
        }
    }

    // colorize selected item
    if(_model->prefixValid() && _model->prefix().size()) {
        node_t* selected = nodemap[_model->prefix().toAscii().constData()];
        agset(selected, const_cast<char*>("fillcolor"),
                const_cast<char*>("#FFAAAA"));
        agset(selected, const_cast<char*>("color"),
                const_cast<char*>("#FF0000"));
    }

    // update viewers
    emit graphChanged();
}
