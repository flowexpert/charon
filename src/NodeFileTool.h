/*
 * NodeFileTool.h
 *
 *  Created on: 14.02.2011
 *      Author: jonathan
 */

#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include<QList>
#include<QFile>
#include<QTextStream>

#include <iostream>
#include <vector>
#include "Node.h"
using namespace std;

#ifndef NODEFILETOOL_H_
#define NODEFILETOOL_H_

class NodeFileTool {
public:
	NodeFileTool();
	vector<Node> loadNodes(QString fname);
	virtual ~NodeFileTool();
};

#endif /* NODEFILETOOL_H_ */
