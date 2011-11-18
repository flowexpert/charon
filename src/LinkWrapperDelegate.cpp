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

#include "LinkWrapperDelegate.h"
#include "CombinedLinkEditor.h"
#include "ParameterFileModel.h"
#include "MetaData.h"
#include "QParameterFile.h"
#include <QLineEdit>
#include <QStringList>

LinkWrapperDelegate::LinkWrapperDelegate(QObject* p) :
		InspectorDelegate(p) {
}

QWidget* LinkWrapperDelegate::createEditor(QWidget* p,
		const QStyleOptionViewItem& opt, const QModelIndex& ind) const {

	QStringList paramList;
	
	const ParameterFileModel* model =
			qobject_cast<const ParameterFileModel*>(ind.model());

	if(model) {
		QStringList keyList = model->parameterFile().getKeyList();

		if(model->useMetaInfo() && model->onlyParams()) {
			QString param = model->data(ind.sibling(ind.row(),0)).toString();
			if (!model->prefix().isEmpty())
				param = model->prefix() + "." + param;
			QString type = model->getType(param);

			foreach(QString cur, keyList) {
				if(QString::compare(type, model->getType(cur),Qt::CaseInsensitive) == 0) {
					paramList << cur;
				}
			}
		}
		else {
			paramList = keyList;
		}
	}

	return new CombinedLinkEditor(InspectorDelegate::createEditor(p, opt, ind), paramList, p);
}

void LinkWrapperDelegate::setModelData (QWidget* editor,
	QAbstractItemModel* model, const QModelIndex & index ) const {
	//if(link mode)
	//model->setData(index,box->currentText());
	//else
	CombinedLinkEditor* ceditor = qobject_cast<CombinedLinkEditor*>(editor);
	Q_ASSERT(ceditor);
	InspectorDelegate::setModelData(ceditor->getCustomEdit(), model, index);
}

void LinkWrapperDelegate::setEditorData (QWidget* editor, const QModelIndex & index ) const {
	CombinedLinkEditor* ceditor = qobject_cast<CombinedLinkEditor*>(editor);
	Q_ASSERT(ceditor);
	QLineEdit *test = qobject_cast<QLineEdit*>(ceditor->getCustomEdit());
	if(test) {
		test->setText(index.model()->data(index,Qt::EditRole).toString());
		//test->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	}
	else {
		InspectorDelegate::setEditorData(ceditor->getCustomEdit(), index);
	}
}

#include "LinkWrapperDelegate.moc"
