#include "SideWidget.h"
SideWidget::SideWidget(QWidget* pp) :
		QListWidget(pp) {
	setObjectName("sideWidget");
	setEnabled(false);
	setFrameStyle(QFrame::NoFrame);
}

void SideWidget::updateProgress(int id) {
	setCurrentRow(id);
}
