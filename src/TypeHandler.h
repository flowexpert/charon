/*
 * TypeHandler.h
 *
 *  Created on: 18.02.2011
 *      Author: jonathan
 */

#ifndef TYPEHANDLER_H_
#define TYPEHANDLER_H_

#include "ParameterType.h"
#include <QMap>

class TypeHandler {
public:
	static void addType(ParameterType *ptype);
	static bool hasType(QString parameterTypeName);
	static ParameterType *getType(QString typeName);
	static void inizialize();
private:
	static QMap<QString,ParameterType*> types;
};

#endif /* TYPEHANDLER_H_ */
