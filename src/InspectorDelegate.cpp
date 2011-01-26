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
#include "QDirEdit.h"

InspectorDelegate::InspectorDelegate(QObject* p) :
		QStyledItemDelegate(p) {
}

QWidget* InspectorDelegate::createEditor(QWidget* p,
		const QStyleOptionViewItem& opt, const QModelIndex& ind) const {

	// detect parameter type
	const ParameterFileModel* model =
			qobject_cast<const ParameterFileModel*>(ind.model());
	if (model && model->onlyParams()) {
		std::string param = model->data(
				ind.sibling(ind.row(),0)).toString().toAscii().constData();
		if (!model->prefix().isEmpty())
			param = std::string(model->prefix().toAscii().constData())
					+ "." + param;
		QString type = model->metaInfo()->getType(
				param, model->getClass(param)).c_str();
		type.toLower();

		if (type=="openfile" || type=="fileopen") {
			QDirEdit* editor = new QDirEdit(param.c_str(), p);
			editor->acceptFiles(true, false);
			connect(
					editor, SIGNAL(dialogOpen(bool)),
					this, SLOT(_setFileDialogFlag(bool)));
			return editor;
		}
		if (type=="writefile" || type=="filewrite" || type=="filename") {
			QDirEdit* editor = new QDirEdit(param.c_str(), p);
			editor->acceptFiles(true, true);
			connect(
					editor, SIGNAL(dialogOpen(bool)),
					this, SLOT(_setFileDialogFlag(bool)));
			return editor;
		}
		if (type == "path") {
			QDirEdit* editor = new QDirEdit(param.c_str(), p);
			connect(
					editor, SIGNAL(dialogOpen(bool)),
					this, SLOT(_setFileDialogFlag(bool)));
			return editor;
		}
		if (type.contains(QRegExp("^\\s*\\{\\s*\\w.*\\}\\s*$"))) {
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
			QDoubleSpinBox* editor = qobject_cast<QDoubleSpinBox*>(
					QStyledItemDelegate::createEditor(p,opt,ind));
			Q_ASSERT(editor);
			// using the provided standart editor, min/max values
			// have already been set to reasonable values,
			// decimals have to be corrected only
			editor->setDecimals(6) ;
			return editor;
		}
	}
	return QStyledItemDelegate::createEditor(p,opt,ind);
}

void InspectorDelegate::setModelData (QWidget* editor,
	QAbstractItemModel* model, const QModelIndex & index ) const {
	
	if(editor && (editor->objectName() == "selectBox")) {
		QComboBox* box = qobject_cast<QComboBox*>(editor);
		model->setData(index,box->currentText());
		return;
	}
	QStyledItemDelegate::setModelData(editor, model, index);
}

void InspectorDelegate::_setFileDialogFlag(bool f) {
	_fileDialogFlag = f;
}

bool InspectorDelegate::eventFilter(QObject* object, QEvent* ev)
{
	if(ev->type() == QEvent::FocusOut && _fileDialogFlag)
		return false;
	return QStyledItemDelegate::eventFilter(object, ev);
}

#include "InspectorDelegate.moc"
