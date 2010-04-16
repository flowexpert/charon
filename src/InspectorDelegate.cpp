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
		std::string type = model->metaInfo()->getType(param, model->getClass(param));
		if (type == "filename") {
			QDirEdit* editor =
					new QDirEdit(param.c_str(), p);
			editor->acceptFiles();
			connect(
					editor, SIGNAL(dialogOpen(bool)),
					this, SLOT(_setFileDialogFlag(bool)));
			return editor;
		}
		if (type == "path") {
			QDirEdit* editor =
					new QDirEdit(param.c_str(), p);
			connect(
					editor, SIGNAL(dialogOpen(bool)),
					this, SLOT(_setFileDialogFlag(bool)));
			return editor;
		}
		if (param.substr(param.find(".")+1) == "templatetype") {
			QComboBox* editor = new QComboBox(p);
			editor->setObjectName("templatetypeBox");
			editor->addItem("int");
			editor->addItem("float");
			editor->addItem("double");
			QString cur = ind.model()->data(ind).toString();
			if (cur == "int")
				editor->setCurrentIndex(0);
			if (cur == "float")
				editor->setCurrentIndex(1);
			if (cur == "double")
				editor->setCurrentIndex(2);
			return editor;
		}
	}
	return QStyledItemDelegate::createEditor(p,opt,ind);
}

void InspectorDelegate::setModelData (QWidget* editor,
		QAbstractItemModel* model, const QModelIndex & index ) const {
	QComboBox* box = qobject_cast<QComboBox*>(editor);
	if(editor && (editor->objectName() == "templatetypeBox")) {
		model->setData(index,box->currentText());
		return;
	}
	QStyledItemDelegate::setModelData(editor, model, index);
}

void InspectorDelegate::_setFileDialogFlag(bool f) {
	_fileDialogFlag = f;
}

bool InspectorDelegate::editorEvent(QEvent* e, QAbstractItemModel* model,
       const QStyleOptionViewItem& option, const QModelIndex& index) {
	Qt::ItemFlags flags = model->flags(index);
	if (!(flags & Qt::ItemIsUserCheckable) ||
			!(option.state & QStyle::State_Enabled) ||
			!(flags & Qt::ItemIsEnabled))
		return false;

	QVariant value = index.data(Qt::CheckStateRole);
	if (!value.isValid())
		return false;

	if ((e->type() == QEvent::MouseButtonRelease)
			|| (e->type() == QEvent::MouseButtonDblClick)) {
		// eat the double click events inside the check rect
		if (e->type() == QEvent::MouseButtonDblClick)
			return true;
	}
	else if (e->type() == QEvent::KeyPress) {
		if(static_cast<QKeyEvent*>(e)->key() != Qt::Key_Space &&
			static_cast<QKeyEvent*>(e)->key() != Qt::Key_Select)
				return false;
	}
	else
		return false;

	Qt::CheckState state = (static_cast<Qt::CheckState>(value.toInt()) ==
		Qt::Checked ? Qt::Unchecked : Qt::Checked);
	return model->setData(index, state, Qt::CheckStateRole);
}

bool InspectorDelegate::eventFilter(QObject* object, QEvent* ev)
{
	if(ev->type() == QEvent::FocusOut && _fileDialogFlag)
		return false;
	return QStyledItemDelegate::eventFilter(object, ev);
}

#include "InspectorDelegate.moc"
