#include "WizardPageParameters.h"
#include "ui_WizardPageParameters.h"

WizardPageParameters::WizardPageParameters(QWidget* p) :
			QWizardPage(p),
			_ui(new Ui::WizardPageParameters) {
	_ui->setupUi(this);
	_ui->tableParameters->setModel(&_model);

	connect(&_model, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
			this, SIGNAL(completeChanged()));
	connect(&_model, SIGNAL(rowsInserted(QModelIndex,int,int)),
			this, SIGNAL(completeChanged()));
	connect(&_model, SIGNAL(rowsRemoved(QModelIndex,int,int)),
			this, SIGNAL(completeChanged()));
}

WizardPageParameters::~WizardPageParameters() {
	delete _ui;
}

void WizardPageParameters::_addParameter(
		QString name,QString doc,QString type, QString defVal, bool list) {
	int nr = _model.rowCount();
	if(_model.insertRow(nr)) {
		name.isEmpty() || _model.setData(_model.index(nr,0),name);
		doc.isEmpty() || _model.setData(_model.index(nr,1),doc);
		type.isEmpty() || _model.setData(_model.index(nr,2),type);
		defVal.isEmpty() || _model.setData(_model.index(nr,3),defVal);
		_model.setData(_model.index(nr,4),
				list ? Qt::Checked : Qt::Unchecked, Qt::CheckStateRole);
	}
}

void WizardPageParameters::_delParameter() {
	QModelIndex cur = _ui->tableParameters->currentIndex();
	_model.removeRow(cur.isValid() ? cur.row() : (_model.rowCount()-1));
}

bool WizardPageParameters::isComplete() const {
	return QWizardPage::isComplete()
			&& (_model.names.indexOf("") < 0)
			&& (_model.docs.indexOf("") < 0);
}

// ==========================  param model  ==========================

int WizardPageParameters::ParamModel::rowCount(const QModelIndex&) const {
	return names.size();
}

int WizardPageParameters::ParamModel::columnCount(const QModelIndex&) const {
	return 5;
}

QVariant WizardPageParameters::ParamModel::data(
		const QModelIndex& idx, int role) const {
	if (role == Qt::DisplayRole || role == Qt::EditRole) {
		switch(idx.column()) {
		case 0:
			return names[idx.row()];
		case 1:
			return docs[idx.row()];
		case 2:
			return types[idx.row()];
		case 3:
			return defaults[idx.row()];
		case 4:
			return list[idx.row()] ?
					tr("parameter list") : tr("simple parameter");
		}
	}
	else if (role == Qt::CheckStateRole && idx.column() == 4) {
		return list[idx.row()] ? Qt::Checked : Qt::Unchecked;
	}
	else if (role == Qt::BackgroundRole) {
		QBrush redBG(QColor("#FAA"));
		switch(idx.column()) {
		case 0:
			if (names[idx.row()].isEmpty())
				return redBG;
			break;
		case 1:
			if (docs[idx.row()].isEmpty())
				return redBG;
			break;
		}
	}
	return QVariant();
}

QVariant WizardPageParameters::ParamModel::headerData(
		int section, Qt::Orientation ori, int role) const {
	if (ori == Qt::Horizontal && role == Qt::DisplayRole) {
		switch(section) {
		case 0:
			return tr("Name");
		case 1:
			return tr("Documentation");
		case 2:
			return tr("Type");
		case 3:
			return tr("Default");
		case 4:
			return tr("List");
		}
	}
	return QVariant();
}

Qt::ItemFlags WizardPageParameters::ParamModel::flags(
		const QModelIndex& idx) const {
	Qt::ItemFlags ret = QAbstractTableModel::flags(idx);
	if (idx.column() < 4) {
		return ret | Qt::ItemIsEditable;
	}
	else if (idx.column() == 4) {
		return ret | Qt::ItemIsUserCheckable;
	}
	return ret;
}

bool WizardPageParameters::ParamModel::setData(
		const QModelIndex& idx, const QVariant& value,
		int role) {
	if (role == Qt::EditRole && idx.column() < 4) {
		QString nv = value.toString();
		switch(idx.column()) {
		case 0:
			if (names.indexOf(nv) >= 0)
				return false;
			names[idx.row()] = nv;
			break;
		case 1:
			docs[idx.row()] = nv;
			break;
		case 2:
			types[idx.row()] = nv;
			break;
		case 3:
			defaults[idx.row()] = nv;
			break;
		}
		emit dataChanged(idx,idx);
		return true;
	}
	if (role == Qt::CheckStateRole && idx.column() == 4) {
		list[idx.row()] = value.toBool();
		emit dataChanged(idx,idx);
		return true;
	}
	return QAbstractTableModel::setData(idx,value,role);
}

bool WizardPageParameters::ParamModel::insertRows(
		int row, int count, const QModelIndex& pp) {
	if (row == rowCount()) {
		beginInsertRows(pp,row,row+count-1);
		endInsertRows();
		for(int ii=0; ii < count; ii++) {
			names.append("");
			docs.append("");
			types.append("");
			defaults.append("");
			list.append(false);
		}
	}
	return QAbstractTableModel::insertRows(row,count,pp);
}

bool WizardPageParameters::ParamModel::removeRows(
		int row, int count, const QModelIndex& pp) {
	if (count > 0 || row+count <= rowCount()) {
		beginRemoveRows(pp,row,row+count-1);
		for (int ii=0; ii<count; ii++) {
			names.removeAt(row+ii);
			docs.removeAt(row+ii);
			types.removeAt(row+ii);
			defaults.removeAt(row+ii);
			list.removeAt(row+ii);
		}
		endRemoveRows();
		return true;
	}
	return QAbstractTableModel::removeRows(row,count,pp);
}
