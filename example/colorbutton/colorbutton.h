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

#ifndef QE_EXAMPLE_COLORBUTTON_H
#define QE_EXAMPLE_COLORBUTTON_H

#include <QToolButton>
#include <QColor>

#include <qecore/dptr.h>

class QeColorButtonPrivate;
class QeColorButton : public QToolButton, public qe::PublicBase
{
    Q_OBJECT

    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(bool isFrameVisible READ isFrameVisible WRITE setFrameVisible)
    Q_PROPERTY(QString textFormat READ textFormat WRITE setTextFormat)

public:
    QeColorButton(QColor color, const QString &textFormat, QWidget *parent = nullptr);
    QeColorButton(QColor color, QWidget *parent = nullptr);
    explicit QeColorButton(QWidget *parent = nullptr);
    ~QeColorButton();

    QColor color() const;
    bool isFrameVisible() const;
    QString textFormat() const;
    void setTextFormat(QString format);

public Q_SLOTS:
    void setColor(const QColor &c);
    void setFrameVisible(bool b);

Q_SIGNALS:
    void colorChanged(QColor);

protected:
    QeColorButton(QeColorButtonPrivate &dd, QWidget *parent = nullptr);
private:
    Q_DISABLE_COPY(QeColorButton)
    QE_DECLARE_PRIVATE(QeColorButton)
};

#endif // QE_EXAMPLE_COLORBUTTON_H
