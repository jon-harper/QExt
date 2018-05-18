/*******************************************************************************
*  QExt: Extensions to Qt                                                      *
*  Copyright (C) 2016  Jonathan Harper                                         *
*                                                                              *
*  This program is free software: you can redistribute it and/or modify        *
*  it under the terms of the GNU General Public License as published by *
*  the Free Software Foundation, either version 3 of the License, or           *
*  (at your option) any later version.                                         *
*                                                                              *
*  This program is distributed in the hope that it will be useful,             *
*  but WITHOUT ANY WARRANTY; without even the implied warranty of              *
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               *
*  GNU General Public License for more details.                         *
*                                                                              *
*  You should have received a copy of the GNU General Public License    *
*  along with this program.  If not, see <http://www.gnu.org/licenses/>.       *
*******************************************************************************/

#ifndef QE_EXAMPLE_COLORBUTTON_P_H
#define QE_EXAMPLE_COLORBUTTON_P_H

#include <QIcon>
#include <qecore/dptr.h>
#include "colorbutton.h"

class QeColorButtonPrivate : public qe::PrivateBase
{
    QE_DECLARE_PUBLIC(QeColorButton)
public:
    QeColorButtonPrivate(QePublicBase *qq) : qe::PrivateBase(qq) {}
    ~QeColorButtonPrivate() {}
    void init();

    virtual void updateIcon();
    virtual void updateText();

    QColor color = Qt::white;
    QColor frameColor = Qt::black;
    bool drawFrame = true;
    QString format = QStringLiteral("%r, %g, %b");
};

#endif //QE_EXAMPLE_COLORBUTTON_P_H
