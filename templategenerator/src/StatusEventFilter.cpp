#include "StatusEventFilter.h"
#include <QStatusTipEvent>
#include <QMessageBox>

StatusEventFilter::StatusEventFilter(QObject* pp) :
	QObject(pp)
{
}

bool StatusEventFilter::eventFilter(QObject* obj, QEvent* ev) {
	if(ev->type() == QEvent::StatusTip) {
		QStatusTipEvent* sev = static_cast<QStatusTipEvent*>(ev);
		emit statusMessage(sev->tip());
		return true;
	}
	else {
		return QObject::eventFilter(obj,ev);
	}
}
