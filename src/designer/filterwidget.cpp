/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Designer of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "filterwidget_p.h"

#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLineEdit>
#include <QtGui/QFocusEvent>
#include <QtGui/QPalette>
#include <QtGui/QCursor>
#include <QtGui/QToolButton>
#include <QtGui/QPainter>
#include <QtGui/QStyle>
#include <QtGui/QStyleOption>

#include <QtCore/QDebug>
#include <QtCore/QPropertyAnimation>

enum { debugFilter = 0 };

QT_BEGIN_NAMESPACE

namespace qdesigner_internal {

HintLineEdit::HintLineEdit(QWidget *pp) :
    QLineEdit(pp),
    m_defaultFocusPolicy(focusPolicy()),
    m_refuseFocus(false)
{
}

IconButton::IconButton(QWidget *pp)
    : QToolButton(pp)
{
    setCursor(Qt::ArrowCursor);
}

void IconButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    // Note isDown should really use the active state but in most styles
    // this has no proper feedback
    QIcon::Mode state = QIcon::Disabled;
    if (isEnabled())
        state = isDown() ? QIcon::Selected : QIcon::Normal;
    QPixmap iconpixmap = icon().pixmap(QSize(ICONBUTTON_SIZE, ICONBUTTON_SIZE),
                                       state, QIcon::Off);
    QRect pixmapRect = QRect(0, 0, iconpixmap.width(), iconpixmap.height());
    pixmapRect.moveCenter(rect().center());
    painter.setOpacity(m_fader);
    painter.drawPixmap(pixmapRect, iconpixmap);
}

void IconButton::animateShow(bool visible)
{
    if (visible) {
        QPropertyAnimation *animation = new QPropertyAnimation(this, "fader");
        animation->setDuration(160);
        animation->setEndValue(1.0);
        animation->start(QAbstractAnimation::DeleteWhenStopped);
    } else {
        QPropertyAnimation *animation = new QPropertyAnimation(this, "fader");
        animation->setDuration(160);
        animation->setEndValue(0.0);
        animation->start(QAbstractAnimation::DeleteWhenStopped);
    }
}

bool HintLineEdit::refuseFocus() const
{
    return m_refuseFocus;
}

void HintLineEdit::setRefuseFocus(bool v)
{
    if (v == m_refuseFocus)
        return;
    m_refuseFocus = v;
    setFocusPolicy(m_refuseFocus ? Qt::NoFocus : m_defaultFocusPolicy);
}

void HintLineEdit::mousePressEvent(QMouseEvent *e)
{
    if (debugFilter)
        qDebug() << Q_FUNC_INFO;
    // Explicitly focus on click.
    if (m_refuseFocus && !hasFocus())
        setFocus(Qt::OtherFocusReason);
    QLineEdit::mousePressEvent(e);
}

void HintLineEdit::focusInEvent(QFocusEvent *e)
{
    if (debugFilter)
        qDebug() << Q_FUNC_INFO;
    if (m_refuseFocus) {
        // Refuse the focus if the mouse it outside. In addition to the mouse
        // press logic, this prevents a re-focussing which occurs once
        // we actually had focus
        const Qt::FocusReason reason = e->reason();
        if (reason == Qt::ActiveWindowFocusReason || reason == Qt::PopupFocusReason) {
            const QPoint mousePos = mapFromGlobal(QCursor::pos());
            const bool refuse = !geometry().contains(mousePos);
            if (debugFilter)
                qDebug() << Q_FUNC_INFO << refuse ;
            if (refuse) {
                e->ignore();
                return;
            }
        }
    }

    QLineEdit::focusInEvent(e);
}

// ------------------- FilterWidget
FilterWidget::FilterWidget(QWidget *pp, LayoutMode lm)  :
    QWidget(pp),
    m_editor(new HintLineEdit(this)),
    m_button(new IconButton(m_editor)),
    m_buttonwidth(0)
{
    m_editor->setPlaceholderText(tr("Filter"));

    // Let the style determine minimum height for our widget
    QSize wsize(ICONBUTTON_SIZE + 6, ICONBUTTON_SIZE + 2);

    // Note KDE does not reserve space for the highlight color
    if (style()->inherits("OxygenStyle")) {
        wsize = wsize.expandedTo(QSize(24, 0));
    }

    // Make room for clear icon
    QMargins margins = m_editor->textMargins();
    if (layoutDirection() == Qt::LeftToRight)
        margins.setRight(wsize.width());
    else
        margins.setLeft(wsize.width());

    m_editor->setTextMargins(margins);

    QHBoxLayout *l = new QHBoxLayout(this);
    l->setMargin(0);
    l->setSpacing(0);
    if (lm == LayoutAlignRight)
        l->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));

    l->addWidget(m_editor);

    // KDE has custom icons for this. Notice that icon namings are counter intuitive
    // If these icons are not avaiable we use the freedesktop standard name before
    // falling back to a bundled resource
    QIcon icon = QIcon::fromTheme(layoutDirection() == Qt::LeftToRight ?
                     QLatin1String("edit-clear-locationbar-rtl") :
                     QLatin1String("edit-clear-locationbar-ltr"),
                     QIcon::fromTheme("edit-clear", QIcon(":/icons/edit-clear.png")));

    m_button->setIcon(icon);
    m_button->setToolTip(tr("Clear text"));
    connect(m_button, SIGNAL(clicked()), this, SLOT(reset()));
    connect(m_editor, SIGNAL(textChanged(QString)), this, SLOT(checkButton(QString)));
    connect(m_editor, SIGNAL(textEdited(QString)), this, SIGNAL(filterChanged(QString)));
}

QString FilterWidget::text() const
{
    return m_editor->text();
}

void FilterWidget::checkButton(const QString &txt)
{
    if (m_oldText.isEmpty() || txt.isEmpty())
        m_button->animateShow(!m_editor->text().isEmpty());
    m_oldText = txt;
}

void FilterWidget::reset()
{
    if (debugFilter)
        qDebug() << Q_FUNC_INFO;

    if (!m_editor->text().isEmpty()) {
        // Editor has lost focus once this is pressed
        m_editor->clear();
        emit filterChanged(QString());
    }
}

void FilterWidget::resizeEvent(QResizeEvent *)
{
    QRect contentRect = m_editor->rect();
    if (layoutDirection() == Qt::LeftToRight) {
        const int iconoffset = m_editor->textMargins().right() + 4;
        m_button->setGeometry(contentRect.adjusted(m_editor->width() - iconoffset, 0, 0, 0));
    } else {
        const int iconoffset = m_editor->textMargins().left() + 4;
        m_button->setGeometry(contentRect.adjusted(0, 0, -m_editor->width() + iconoffset, 0));
    }
}

bool FilterWidget::refuseFocus() const
{
    return m_editor->refuseFocus();
}

void FilterWidget::setRefuseFocus(bool v)
{
    m_editor->setRefuseFocus(v);
}
} // namespace qdesigner_internal

QT_END_NAMESPACE
