/*  Copyright (C) 2009 Jens-Malte Gottfried

    This file is part of Tuchulcha.

    Tuchulcha is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Tuchulcha is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Tuchulcha.  If not, see <http://www.gnu.org/licenses/>.
*/
/** \file InspectorDelegate.cpp
 *  Implementation of class InspectorDelegate
 *  \author Jens-Malte Gottfried <jmgottfried@web.de>
 *  \date 30.09.2009
 */

//  The filename editor widgets are based on CMake code:
//
//  Copyright (c) 2002 Kitware, Inc., Insight Consortium.  All rights reserved.
//
//  See CMakeCopyright.txt or http://www.cmake.org/HTML/Copyright.html for details.
//
//  This software is distributed WITHOUT ANY WARRANTY; without even
//  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
//  PURPOSE.  See the above copyright notices for more information.

#include "InspectorDelegate.h"
#include "ParameterFileModel.h"
#include "MetaData.h"
#include <QDirModel>
#include <QFileInfo>
#include <QFileDialog>
#include <QToolButton>
#include <QComboBox>
#include <QResizeEvent>
#include <QDoubleSpinBox>
#include <QSettings>
#include <QDir>
#include "QDirEdit.h"

InspectorDelegate::InspectorDelegate(QObject* p) :
	QStyledItemDelegate(p), _fileDialogFlag(false) {
	connect(this,
		SIGNAL(closeEditor(QWidget*,QAbstractItemDelegate::EndEditHint)),
		SLOT(_handleCloseEditor(QWidget*,QAbstractItemDelegate::EndEditHint)));
}

QWidget* InspectorDelegate::createEditor(QWidget* p,
		const QStyleOptionViewItem& opt, const QModelIndex& ind) const {

	const ParameterFileModel* model =
			qobject_cast<const ParameterFileModel*>(ind.model());
	if (model && QFileInfo(model->fileName()).exists()) {
		*const_cast<QString*>(&_workingDir) = QDir::currentPath();
		QDir::setCurrent(QFileInfo(model->fileName()).path());
	}
	else {
		qDebug("InspectorDelegate::setModelData: no parameter file model");
	}

	// detect parameter type
	if (model && model->onlyParams()) {
		QString param = model->data(ind.sibling(ind.row(),0)).toString();
		if (!model->prefix().isEmpty())
			param = model->prefix() + "." + param;
		// do not cast to lowercase since
		// e.g. combo box entries would get messed up
		QString type = model->getType(param);

		if (type.compare("OpenFile",Qt::CaseInsensitive)==0 ||
				type.compare("FileOpen",Qt::CaseInsensitive)==0) {
			QDirEdit* editor = new QDirEdit(param, p);
			editor->acceptFiles(true, false);
			connect(
					editor, SIGNAL(dialogOpen(bool)),
					this, SLOT(_setFileDialogFlag(bool)));
			return editor;
		}
		if (type.compare("WriteFile",Qt::CaseInsensitive)==0 ||
				type.compare("FileWrite",Qt::CaseInsensitive)==0 ||
				type.compare("FileName",Qt::CaseInsensitive)==0) {
			QDirEdit* editor = new QDirEdit(param, p);
			editor->acceptFiles(true, true);
			connect(
					editor, SIGNAL(dialogOpen(bool)),
					this, SLOT(_setFileDialogFlag(bool)));
			return editor;
		}
		if (type.compare("Path",Qt::CaseInsensitive)==0) {
			QDirEdit* editor = new QDirEdit(param, p);
			connect(
					editor, SIGNAL(dialogOpen(bool)),
					this, SLOT(_setFileDialogFlag(bool)));
			return editor;
		}
		if (type.contains(QRegExp("^\\{\\s*\\w.*\\}\\s*$"))) {
			QComboBox* editor = new QComboBox(p);
			editor->setObjectName("selectBox");
			QStringList options = type.trimmed().mid(
					1,type.length()-2).trimmed().split(
							";",QString::SkipEmptyParts);
			editor->addItems(options);
			QString cur = ind.model()->data(ind).toString();
			int curInd = options.indexOf(cur);
			if (curInd >= 0)
				editor->setCurrentIndex(curInd);
			return editor;
		}
		// fix decimals for double editors
		// (also handles parameters of type T)
		if (ind.model()->data(ind).type() == QVariant::Double) {
			QLineEdit* editor = new QLineEdit(p) ;
			QDoubleValidator* validator = new QDoubleValidator(editor) ;
				validator->setNotation(QDoubleValidator::ScientificNotation) ;
			editor->setValidator(validator) ;
			editor->setObjectName("doubleLineEdit") ;
			editor->setText(QString("%1").arg(ind.model()->data(ind).toDouble(),0,'G',16)) ;	
			Q_ASSERT(editor);
			return editor;
		}
	}
	return QStyledItemDelegate::createEditor(p,opt,ind);
}

void InspectorDelegate::setModelData (QWidget* editor,
	QAbstractItemModel* model, const QModelIndex & index ) const {
	QDirEdit* dirEdit = 0;
	if(editor && (editor->objectName() == "selectBox")) {
		QComboBox* box = qobject_cast<QComboBox*>(editor);
		model->setData(index,box->currentText());
		return;
	}
	else if(editor && (editor->objectName() == "doubleLineEdit")) {
		QLineEdit* line = qobject_cast<QLineEdit*>(editor);
		model->setData(index,line->text().toDouble());
		return;
	}
	else if ( (dirEdit = qobject_cast<QDirEdit*>(editor)) ) {
		QSettings s;
		ParameterFileModel* pmodel = qobject_cast<ParameterFileModel*>(model);
		if (pmodel) {
			if (s.value("relativePaths",true).toBool()) {
				QStringList eData = dirEdit->text().split(";"), rData;
				QDir wDir = QFileInfo(pmodel->fileName()).dir();
				foreach(QString cur, eData) {
					rData << wDir.relativeFilePath(cur);
				}
				model->setData(index,rData.join(";"));
				return;
			}
		}
		else {
			qDebug("InspectorDelegate::setModelData: no parameter file model");
		}
	}
	QStyledItemDelegate::setModelData(editor, model, index);
}

void InspectorDelegate::_setFileDialogFlag(bool f) {
	_fileDialogFlag = f;
}

bool InspectorDelegate::eventFilter(QObject* object, QEvent* ev) {
	if(ev->type() == QEvent::FocusOut && _fileDialogFlag)
		return false;
	return QStyledItemDelegate::eventFilter(object, ev);
}

void InspectorDelegate::_handleCloseEditor(QWidget*, QAbstractItemDelegate::EndEditHint) {
	if (!_workingDir.isEmpty()) {
		QDir::setCurrent(_workingDir);
		_workingDir = QString();
	}
}
