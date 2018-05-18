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

#include <QtWidgets/QDialog>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QColorDialog>
#include "colorbutton.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QDialog dlg;
    dlg.setWindowTitle(QStrLit("Color Button Test"));

    QHBoxLayout layout(&dlg);

    QeColorButton colorButton(Qt::red);
    QObject::connect(&colorButton, &QeColorButton::clicked, [&colorButton]()
    {
        QColorDialog dlg(colorButton.color());
        dlg.exec();
        colorButton.setColor(dlg.currentColor());
    }
    );
    layout.addWidget(&colorButton);

    QPushButton closeButton(QStrLit("&Close"));
    QObject::connect(&closeButton, &QPushButton::clicked, &dlg, &QDialog::close);
    layout.addWidget(&closeButton);

    dlg.show();

    return app.exec();
}
