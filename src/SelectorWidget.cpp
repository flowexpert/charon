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
/** @file   SelectorWidget.cpp
 *  @brief  Implementation of class SelectorWidget
 *
 *  @date   06.11.2008
 *  @author <a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#include <QtGui>

#include "SelectorWidget.h"
#include "TemplateGraphView.h"
#include "GraphModel.h"
#include "MetaData.h"
#include "FileManager.h"

#include "SelectorWidget.moc"

SelectorWidget::SelectorWidget(QWidget* myParent, QString classesFile) :
        QWidget(myParent),
        _editmodel(0),
        _model(0),
        _matchingOnly(false){

	if (classesFile.isEmpty()) {
		// meta file manager
		const FileManager& fileManager = FileManager::instance();
		classesFile = fileManager.classesFile();
	}

	// empty GraphModel
	if(_model) {
		delete _model;
	}
	_model = new GraphModel("", this, classesFile);
	_model->setUseMetaInfo(false);

    // set up GUI
    QGridLayout* gLayout = new QGridLayout(this);
    _view = new TemplateGraphView(0, classesFile);
    _view->setModel(_model);
    gLayout->addWidget(_view, 1, 0, 1, -1);

    QCheckBox* matchingBox = new QCheckBox(
        tr("show only &connectable items"));
    matchingBox->setChecked(false);
    matchingBox->setToolTip(tr("Only classes that have slots\n"
        "matching one of the selected\nitem's slots, will be shown."));
    QLabel* searchLabel = new QLabel(
        tr("&search:"));
    QLineEdit* search = new QLineEdit();
    QString searchTip = tr("Search for items containing this string.\n"
        "If only one item is left and you press return,\nthis item will "
        "be added to the active flowchart.");
    search->setToolTip(searchTip);
    searchLabel->setToolTip(searchTip);
    searchLabel->setBuddy(search);
    QPushButton* showAll = new QPushButton(tr("show &all"));
    showAll->setToolTip(tr("reset applied filters\n"
        "(search and only matching)"));
    gLayout->addWidget(matchingBox, 0, 0);
    gLayout->setColumnStretch(1, 2);
    gLayout->addWidget(searchLabel, 0, 2);
    gLayout->addWidget(search,      0, 3);
    gLayout->setColumnStretch(3, 3);
    gLayout->addWidget(showAll,     0, 4);

    // connect signals and slots
    connect(_view, SIGNAL(addNode(QString)),
        this, SLOT(_reqAddNode(QString)));
    connect(_view, SIGNAL(showDocPage(QString)),
        this, SIGNAL(showDocPage(QString)));
    connect(_view, SIGNAL(showClassDoc(QString)),
        this, SIGNAL(showClassDoc(QString)));
    connect(matchingBox, SIGNAL(clicked(bool)),
        this, SLOT(_setMatchingOnly(bool)));
    connect(search, SIGNAL(textEdited(QString)),
        this, SLOT(_setSearch(QString)));
    connect(search, SIGNAL(returnPressed()),
        this, SLOT(_addShown()));
    connect(this, SIGNAL(matchingOnlyChanged(bool)),
        matchingBox, SLOT(setChecked(bool)));
    connect(this, SIGNAL(searchChanged(QString)),
        search, SLOT(setText(QString)));
    connect(showAll, SIGNAL(clicked()),
        this, SLOT(_resetFilters()));

    // add items
    show();
    _populate();
}

SelectorWidget::~SelectorWidget() {
}

void SelectorWidget::_populate() {
    // clear content
    _model->clear(false);

    const MetaData& metaInfo = *(_model->metaInfo());

    const std::vector<std::string>& classes = metaInfo.getClasses();
    std::list<std::string> selected;
    selected.assign(classes.begin(), classes.end());

    if(!_search.isEmpty()) {
        std::list<std::string> temp;
        std::string searchstring = _search.toAscii().constData();
        std::list<std::string>::const_iterator classI;

        for(classI = selected.begin(); classI != selected.end(); classI++)
            if(classI->find(searchstring) != std::string::npos)
                temp.push_back(*classI);

        selected = temp;
    }

    if(_matchingOnly && _editmodel && _editmodel->prefix().size()) {
        // filter classes: only classes with matchin slots to the selected
        // object are shown.
        const std::string& curClass = _editmodel->getClass(
            _editmodel->prefix().toAscii().constData());

        if(!curClass.size()) {
            // no item selected -> no items connectable
            _model->reDraw();
            return;
        }

        std::list<std::string> temp;

        std::vector<std::string>::const_iterator slot;
        std::list<std::string>::const_iterator classI;

        // input types
        const std::vector<std::string>& inputs = metaInfo.getInputs(curClass);
        std::set<std::string> inTypes;
        for (slot = inputs.begin(); slot != inputs.end(); slot++) {
            const std::string& type = metaInfo.getType(*slot, curClass);
            inTypes.insert(type);
        }

        // output types
        const std::vector<std::string>& outputs = metaInfo.getOutputs(curClass);
        std::set<std::string> outTypes;
        for (slot = outputs.begin(); slot != outputs.end(); slot++) {
            const std::string& type = metaInfo.getType(*slot, curClass);
            outTypes.insert(type);
        }

        // iterate over avaiable classes and select those who could be
        // connected to the selected item
        for(classI = selected.begin(); classI != selected.end(); classI++) {
            bool valid = false;

            // check for matching input slots
            const std::vector<std::string>& curInputs =
                metaInfo.getInputs(*classI);
            for(slot = curInputs.begin(); slot != curInputs.end(); slot++) {
                if (outTypes.find(metaInfo.getType(*slot, *classI))
                        == outTypes.end())
                    continue;
                valid = true;
                break;
            }
            if (valid) {
                temp.push_back(*classI);
                continue;
            }

            // check for matching output slots
            const std::vector<std::string>& curOutputs =
                metaInfo.getOutputs(*classI);
            for(slot = curOutputs.begin(); slot != curOutputs.end(); slot++) {
                if (inTypes.find(metaInfo.getType(*slot, *classI))
                        == inTypes.end())
                    continue;
                valid = true;
                break;
            }
            if (valid)
                temp.push_back(*classI);
        }
        selected = temp;
    }

    _model->clear(false);
    _model->insertRows(0, selected.size());

    std::list<std::string>::const_iterator selIter;
    int i = 0;
    for(selIter = selected.begin(); selIter != selected.end(); selIter++) {
        _model->setData(_model->index(i, 0), QString("dummy%1.type").arg(i+1));
        _model->setData(_model->index(i, 1), selIter->c_str());
        i++;
    }
    _model->reDraw();
}

void SelectorWidget::setModel(ParameterFileModel* model) {
    if (qobject_cast<GraphModel*>(_editmodel))
        disconnect(_editmodel, SIGNAL(prefixChanged(QString)),
            this, SLOT(_updateMatching()));

    _editmodel = qobject_cast<GraphModel*>(model);

    if (_editmodel)
        connect(_editmodel, SIGNAL(prefixChanged(QString)),
            this, SLOT(_updateMatching()));

    _updateMatching();
}

void SelectorWidget::_updateMatching() {
    if (_matchingOnly)
        _populate();
}

void SelectorWidget::_reqAddNode(const QString& className) const {
    if(_editmodel)
        _editmodel->addNode(className);
}

void SelectorWidget::_setMatchingOnly(bool value) {
    if(_matchingOnly == value)
        return;
    _matchingOnly = value;
    emit matchingOnlyChanged(value);
    _populate();
}

void SelectorWidget::_setSearch(const QString& str) {
    if(str == _search)
        return;

    _search = str;
    emit searchChanged(str);
    _populate();
}

void SelectorWidget::_resetFilters() {
    _setMatchingOnly(false);
    _setSearch("");
}

void SelectorWidget::_addShown() {
    if(_model->rowCount() != 1)
        return;
    _reqAddNode(_model->data(_model->index(0, 1)).toString());
}

void SelectorWidget::update() {
	_model->loadMetaFile(FileManager::instance().classesFile());
	_populate();
}
