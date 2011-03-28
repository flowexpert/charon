#include "WizardPageSlots.h"
#include "ui_WizardPageSlots.h"

WizardPageSlots::WizardPageSlots(QWidget* p) :
		QWizardPage(p),
		_ui(new Ui::WizardPageSlots) {
	_ui->setupUi(this);

	_inputSlots.skipFlags = false;
	_outputSlots.skipFlags = true;

	_ui->tableInputSlots->setModel(&_inputSlots);
	_ui->tableOutputSlots->setModel(&_outputSlots);

	connect(&_inputSlots, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
			this, SIGNAL(completeChanged()));
	connect(&_inputSlots, SIGNAL(rowsInserted(QModelIndex,int,int)),
			this, SIGNAL(completeChanged()));
	connect(&_inputSlots, SIGNAL(rowsRemoved(QModelIndex,int,int)),
			this, SIGNAL(completeChanged()));
	connect(&_outputSlots, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
			this, SIGNAL(completeChanged()));
	connect(&_outputSlots, SIGNAL(rowsInserted(QModelIndex,int,int)),
			this, SIGNAL(completeChanged()));
	connect(&_outputSlots, SIGNAL(rowsRemoved(QModelIndex,int,int)),
			this, SIGNAL(completeChanged()));
}

WizardPageSlots::~WizardPageSlots() {
	delete _ui;
}

bool WizardPageSlots::isComplete() const {
	return QWizardPage::isComplete()
			&& (_inputSlots.names.indexOf("") < 0)
			&& (_inputSlots.docs.indexOf("") < 0)
			&& (_outputSlots.names.indexOf("") < 0)
			&& (_outputSlots.docs.indexOf("") < 0);
}

void WizardPageSlots::_addInputSlot(
		QString name,QString doc,QString type, bool optional,bool multi) {
	int nr = _inputSlots.rowCount();
	_inputSlots.insertRow(nr);
	name.isEmpty() || _inputSlots.setData(_inputSlots.index(nr,0),name);
	doc.isEmpty() || _inputSlots.setData(_inputSlots.index(nr,1),doc);
	type.isEmpty() || _inputSlots.setData(_inputSlots.index(nr,2),type);
	_inputSlots.setData(_inputSlots.index(nr,3),
			optional ? Qt::Checked : Qt::Unchecked, Qt::CheckStateRole);
	_inputSlots.setData(_inputSlots.index(nr,4),
			multi ? Qt::Checked : Qt::Unchecked, Qt::CheckStateRole);
}

void WizardPageSlots::_addOutputSlot(
		QString name,QString doc,QString type) {
	int nr = _outputSlots.rowCount();
	_outputSlots.insertRow(nr);
	name.isEmpty() || _outputSlots.setData(_outputSlots.index(nr,0),name);
	doc.isEmpty() || _outputSlots.setData(_outputSlots.index(nr,1),doc);
	type.isEmpty() || _outputSlots.setData(_outputSlots.index(nr,2),type);
}

void WizardPageSlots::_delInputSlot() {
	QModelIndex cur = _ui->tableInputSlots->currentIndex();
	_inputSlots.removeRow(
			cur.isValid() ? cur.row() : (_inputSlots.rowCount()-1));
}

void WizardPageSlots::_delOutputSlot() {
	QModelIndex cur = _ui->tableOutputSlots->currentIndex();
	_outputSlots.removeRow(
			cur.isValid() ? cur.row() : (_outputSlots.rowCount()-1));
}

// ==========================  slot model  ==========================

int WizardPageSlots::SlotModel::rowCount(const QModelIndex&) const {
	return names.size();
}

int WizardPageSlots::SlotModel::columnCount(const QModelIndex&) const {
	return skipFlags ? 3 : 5;
}

QVariant WizardPageSlots::SlotModel::data(
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
			return optional[idx.row()] ? tr("optional") : tr("required");
		case 4:
			return multi[idx.row()] ? tr("multi") : tr("single");
		}
	}
	else if (role == Qt::CheckStateRole) {
		switch(idx.column()) {
		case 3:
			return optional[idx.row()] ? Qt::Checked : Qt::Unchecked;
		case 4:
			return multi[idx.row()] ? Qt::Checked : Qt::Unchecked;
		}
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

QVariant WizardPageSlots::SlotModel::headerData(
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
			return tr("Optional");
		case 4:
			return tr("Multi");
		}
	}
	return QVariant();
}

Qt::ItemFlags WizardPageSlots::SlotModel::flags(
		const QModelIndex& idx) const {
	Qt::ItemFlags ret = QAbstractTableModel::flags(idx);
	if (idx.column() < 3) {
		return ret | Qt::ItemIsEditable;
	}
	else if (idx.column() >= 3) {
		return ret | Qt::ItemIsUserCheckable;
	}
	return ret;
}

bool WizardPageSlots::SlotModel::setData(
		const QModelIndex& idx, const QVariant& value,
		int role) {
	if (role == Qt::EditRole) {
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
		default:
			return false;
		}
		emit dataChanged(idx,idx);
		return true;
	}
	if (role == Qt::CheckStateRole) {
		switch (idx.column()) {
		case 3:
			optional[idx.row()] = value.toBool();
			break;
		case 4:
			multi[idx.row()] = value.toBool();
			break;
		default:
			return false;
		}
		emit dataChanged(idx,idx);
		return true;
	}
	return QAbstractTableModel::setData(idx,value,role);
}

bool WizardPageSlots::SlotModel::insertRows(
		int row, int count, const QModelIndex& pp) {
	if (row == rowCount()) {
		beginInsertRows(pp,row,row+count-1);
		endInsertRows();
		for(int ii=0; ii < count; ii++) {
			names.append("");
			docs.append("");
			types.append("");
			optional.append(false);
			multi.append(false);
		}
	}
	return QAbstractTableModel::insertRows(row,count,pp);
}

bool WizardPageSlots::SlotModel::removeRows(
		int row, int count, const QModelIndex& pp) {
	if (count > 0 || row+count <= rowCount()) {
		beginRemoveRows(pp,row,row+count-1);
		for (int ii=0; ii<count; ii++) {
			names.removeAt(row+ii);
			docs.removeAt(row+ii);
			types.removeAt(row+ii);
			optional.removeAt(row+ii);
			multi.removeAt(row+ii);
		}
		endRemoveRows();
		return true;
	}
	return QAbstractTableModel::removeRows(row,count,pp);
}

