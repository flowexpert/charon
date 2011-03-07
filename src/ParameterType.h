/*
 * ParameterType.h
 *
 *  Created on: 18.02.2011
 *      Author: jonathan
 */

#ifndef PARAMETERTYPE_H_
#define PARAMETERTYPE_H_
#include <QString>
#include <QColor>

class ParameterType {
public:
	ParameterType(QString typeName,QColor color=Qt::gray);
	QString getTypeName();
        QString getTypeNameUnTemplated();
	QColor getColor();
	void setColor(QColor col);
	void setTempName(QString name);
	void setTemplated(bool tmp);
	bool isTemplated();
	void setTypeName(QString name);
	virtual ~ParameterType();
private:
	QString typeName;
	QColor color;
	bool templated;
	QString tempName;
};

#endif /* PARAMETERTYPE_H_ */
