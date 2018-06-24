/*  QExt: Extensions to Qt
 *  Copyright (C) 2016  Jonathan Harper
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef QE_TEST_SHELLNODEINFO_H
#define QE_TEST_SHELLNODEINFO_H

#include <qewindows/global.h>
#include <qewindows/shellnodeinfo.h>
#include <QDebug>

using namespace qe::windows;

struct test_shellnodeinfo
{
    static void run()
    {
        test_construction();
    }

    static void test_construction()
    {
        auto val1 = ShellNodeInfo("C:");
        qDebug() << "Parsing name: " << val1.parsingName();
        qDebug() << "Display name: " << val1.displayName();

        auto val2 = ShellNodeInfo("C:\\Windows");
        qDebug() << "Parsing name: " << val2.parsingName();
        qDebug() << "Display name: " << val2.displayName();

        auto id = val1.idList();
        auto item = val1.itemPointer();
    }
};


#endif // QE_TEST_SHELLNODEINFO_H
