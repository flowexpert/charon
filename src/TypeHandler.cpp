/*
 * TypeHandler.cpp
 *
 *  Created on: 18.02.2011
 *      Author: jonathan
 */

#include "TypeHandler.h"
#include <math.h>
#include <iostream>
#include <qt4/QtCore/qregexp.h>
using namespace std;
QMap<QString, ParameterType*> TypeHandler::types;

void TypeHandler::addType(ParameterType *ptype) {
    QString type = ptype->getTypeName();
    QRegExp exp("*<?*>*");
    exp.setPatternSyntax(QRegExp::Wildcard);
    if (exp.exactMatch(ptype->getTypeName())) {
        QString tname = ptype->getTypeName();
        type = ptype->getTypeName().remove(QRegExp("<?*>*", Qt::CaseSensitive, QRegExp::Wildcard));
        QString tmpname = ptype->getTypeName().remove(QRegExp("*<", Qt::CaseSensitive, QRegExp::Wildcard));
        tmpname = tmpname.remove(QRegExp(">*", Qt::CaseSensitive, QRegExp::Wildcard));
        /*cout << "has template: " << tname.toStdString() << endl;
        cout << "w.o.:" << type.toStdString() << endl;
        cout << "type:" << tmpname.toStdString() << endl;*/
        ptype->setTempName(tmpname);
        ptype->setTemplated(true);
        ptype->setTypeName(type);
    }
    ptype->setColor(QColor::fromRgbF(fabs(sin(types.size()*0.1f)), fabs(cos(types.size()*0.5f)), 0.5));
    types.insert(type, ptype);
}

bool TypeHandler::hasType(QString parameterTypeName) {
    QRegExp exp("*<?*>*");
    exp.setPatternSyntax(QRegExp::Wildcard);
    if (exp.exactMatch(parameterTypeName)) {
        parameterTypeName = parameterTypeName.remove(QRegExp("<?*>*", Qt::CaseSensitive, QRegExp::Wildcard));
    }
    ParameterType *pt = types.value(parameterTypeName);
    if (pt != 0) {
        return true;
    }
    return false;
}

ParameterType *TypeHandler::getType(QString typeName) {
    QRegExp exp("*<?*>*");
    exp.setPatternSyntax(QRegExp::Wildcard);
    if (exp.exactMatch(typeName)) {
        typeName = typeName.remove(QRegExp("<?*>*", Qt::CaseSensitive, QRegExp::Wildcard));
    }
    ParameterType *pt = types.value(typeName);
    if (pt != 0) {
        return pt;
    } else {
        cout << "error: requested unknown type:" << typeName.toStdString() << endl;
        return 0;
    }
}

void TypeHandler::inizialize() {
    types = QMap<QString, ParameterType*>();
}
