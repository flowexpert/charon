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
/** @file graphTemplateTest.cpp
 *  @brief Testing of graphviz libraries using some special classes set
 *  @date	2009-07-14
 *  @author	<a href="mailto:jmgottfried@web.de">Jens-Malte Gottfried</a>
 */

#include <QtTest>
#include "SelectorWidget.h"
#include "GraphModel.h"
#include "graphTemplateTest.h"
#include "graphTemplateTest.moc"

#ifndef TESTCLASSFILE
#error "Test class filename not defined!"
#endif

void TestViz::test() {
    qDebug() << "Using classes file \"" << TESTCLASSFILE << "\"";
    qDebug() << "If this test fails with a segfault in a graphviz library";
    qDebug() << "make sure, that you have installed a stable graphviz version";
    qDebug() << "e.g. Version 2.24 for VisualStudio.";

    try{
        GraphModel model("", 0, TESTCLASSFILE);
        SelectorWidget selector(0, TESTCLASSFILE);
        selector.setModel(&model);
    }
    catch (std::string msg) {
        qWarning() << "Caught exception of type std::string";
        qWarning() << "Message:";
        qFatal("%s", msg.c_str());
    }
}

QTEST_MAIN(TestViz);

