#include "ParamSlotModel.h"
#include <QBrush>

int ParamSlotModel::rowCount(const QModelIndex&) const {
	return names.size();
}

int ParamSlotModel::columnCount(const QModelIndex&) const {
	return 7;
}

QVariant ParamSlotModel::data(
		const QModelIndex& idx, int role) const {
	QBrush redBG(QColor("#FAA"));

	switch (role) {
	case Qt::EditRole:
		// special case: auto guess
		// all other cases: identical to Display role
		if(idx.column() == 2)
			return types[idx.row()];
	case Qt::DisplayRole:
		switch (idx.column()) {
		case 0:
			return names[idx.row()];
		case 1:
			return docs[idx.row()];
		case 2:
			return types[idx.row()].isEmpty() ?
					tr("auto") : types[idx.row()];
		case 3:
			return defaults[idx.row()];
		case 4:
			return optional[idx.row()] ? tr("optional") : tr("required");
		case 5:
			return multi[idx.row()] ? tr("multi") : tr("single");
		case 6:
			return list[idx.row()] ?
					tr("parameter list") : tr("simple parameter");
		}
		break;

	case Qt::CheckStateRole:
		switch(idx.column()) {
		case 4:
			return optional[idx.row()] ? Qt::Checked : Qt::Unchecked;
		case 5:
			return multi[idx.row()] ? Qt::Checked : Qt::Unchecked;
		case 6:
			return list[idx.row()] ? Qt::Checked : Qt::Unchecked;
		}
		break;

	case Qt::BackgroundRole:
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
		break;

	case Qt::ForegroundRole:
		if( idx.column() == 2 && types[idx.row()] == "")
			return QBrush(QColor(Qt::gray));
		break;
	}
	return QVariant();
}

QVariant ParamSlotModel::headerData(
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
			return tr("Optional");
		case 5:
			return tr("MultiSlot");
		case 6:
			return tr("ParameterList");
		}
	}
	return QVariant();
}

Qt::ItemFlags ParamSlotModel::flags(
		const QModelIndex& idx) const {
	Qt::ItemFlags ret = QAbstractTableModel::flags(idx);
	if (idx.column() < 4) {
		return ret | Qt::ItemIsEditable;
	}
	else if (idx.column() >= 4) {
		return ret | Qt::ItemIsUserCheckable;
	}
	return ret;
}

bool ParamSlotModel::setData(
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
		case 3:
			defaults[idx.row()] = nv;
			break;
		default:
			return false;
		}
		emit dataChanged(idx,idx);
		return true;
	}
	if (role == Qt::CheckStateRole) {
		switch (idx.column()) {
		case 4:
			optional[idx.row()] = value.toBool();
			break;
		case 5:
			multi[idx.row()] = value.toBool();
			break;
		case 6:
			list[idx.row()] = value.toBool();
			break;
		default:
			return false;
		}
		emit dataChanged(idx,idx);
		return true;
	}
	return QAbstractTableModel::setData(idx,value,role);
}

bool ParamSlotModel::insertRows(
		int row, int count, const QModelIndex& pp) {
	if (row == rowCount()) {
		beginInsertRows(pp,row,row+count-1);
		endInsertRows();
		for(int ii=0; ii < count; ii++) {
			names.append("");
			docs.append("");
			types.append("");
			defaults.append("");
			optional.append(false);
			multi.append(false);
			list.append(false);
		}
	}
	return QAbstractTableModel::insertRows(row,count,pp);
}

bool ParamSlotModel::removeRows(
		int row, int count, const QModelIndex& pp) {
	if (count > 0 || row+count <= rowCount()) {
		beginRemoveRows(pp,row,row+count-1);
		for (int ii=0; ii<count; ii++) {
			names.removeAt(row+ii);
			docs.removeAt(row+ii);
			types.removeAt(row+ii);
			defaults.removeAt(row+ii);
			optional.removeAt(row+ii);
			multi.removeAt(row+ii);
			list.removeAt(row+ii);
		}
		endRemoveRows();
		return true;
	}
	return QAbstractTableModel::removeRows(row,count,pp);
}

QStringList ParamSlotModel::toStringList(const QList<bool>& l) {
	QStringList res;
	for (int ii=0; ii< l.size(); ii++) {
		res.append(QVariant(l[ii]).toString());
	}
	return res;
}

