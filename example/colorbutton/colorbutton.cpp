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

#include "colorbutton_p.h"
#include <QtGui/QPainter>
#include <QtWidgets/QColorDialog>

//! \brief Initializes the object.
//! \note This method is necessary because it is called from the public classes's constructor,
//! \em after all of the public classes initializer calls are complete. Any
//! calls that involve the q-ptr during construction must be deferred to this method.
void QeColorButtonPrivate::init()
{
    QE_QPTR;
    q->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    QObject::connect(q, &QAbstractButton::clicked, q, &QeColorButton::showColorDialog);
    updateIcon();
    updateText();
}

//! Updates the icon after a change in the color
void QeColorButtonPrivate::updateIcon()
{
    QE_QPTR;
    QPixmap pixmap(q->iconSize());
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);

    QRect rect = pixmap.rect().adjusted(0,0,-1,-1);
    if (drawFrame) {
        painter.setPen(QPen(q->palette().shadow(), 1));
        painter.drawRect(rect);
        rect.adjust(2, 2, -1, -1);
    }

    painter.fillRect(rect, color);
    painter.end();
    q->setIcon(QIcon(pixmap));
}

void QeColorButtonPrivate::updateText()
{
    QString txt = format;
    txt.replace(QStrLit("%r"), QString::number(color.red()));
    txt.replace(QStrLit("%R"), QString::number(color.redF(), 'g', 2));
    txt.replace(QStrLit("%g"), QString::number(color.green()));
    txt.replace(QStrLit("%G"), QString::number(color.greenF(), 'g', 2));
    txt.replace(QStrLit("%b"), QString::number(color.blue()));
    txt.replace(QStrLit("%B"), QString::number(color.blueF(), 'g', 2));
    txt.replace(QStrLit("%h"), QString::number(color.hue()));
    txt.replace(QStrLit("%H"), QString::number(color.hueF(), 'g', 2));
    txt.replace(QStrLit("%s"), QString::number(color.saturation()));
    txt.replace(QStrLit("%S"), QString::number(color.saturationF(), 'g', 2));
    txt.replace(QStrLit("%v"), QString::number(color.value()));
    txt.replace(QStrLit("%V"), QString::number(color.valueF(), 'g', 2));
    txt.replace(QStrLit("%l"), QString::number(color.lightness()));
    txt.replace(QStrLit("%L"), QString::number(color.lightnessF(), 'g', 2));

    QE_QPTR;
    q->setText(txt);
}

//! Constructs a new QeColorButton with the color \a col, text format, \a txt and \a parent.
QeColorButton::QeColorButton(const QColor &color, QString textFormat, QWidget *parent)
    : QeColorButton(*new QeColorButtonPrivate(this), parent)
{
    setTextFormat(textFormat);
    setColor(color);
}

//! Constructs a new QeColorButton with the given color and parent.
QeColorButton::QeColorButton(const QColor &color, QWidget *parent)
    : QeColorButton(*new QeColorButtonPrivate(this), parent)
{
    setColor(color);
}

//! The default constructor, with \arg parent as the parent widget.
QeColorButton::QeColorButton(QWidget *parent)
    : QeColorButton(*new QeColorButtonPrivate(this), parent)
{

}

QeColorButton::QeColorButton(QeColorButtonPrivate &dd, QWidget *parent)
    : QToolButton(parent), qe::PublicBase(dd)
{
    QE_DPTR;
    d->init();
}

//! The destructor.
QeColorButton::~QeColorButton()
{
}

//! \brief Returns the current color.
QColor QeColorButton::color() const
{
    QE_CD;
    return d->color;
}

//! \brief Returns whether or not a frame is drawn around the color swatch.
bool QeColorButton::isFrameVisible() const
{
    QE_CD;
    return d->drawFrame;
}

//! \brief Returns the current `textFormat`.
QString QeColorButton::textFormat() const
{
    QE_CONST_DPTR;
    return d->format;
}

//! \brief Setter for the `textFormat` property.
void QeColorButton::setTextFormat(QString format)
{
    QE_D;
    if (d->format == format)
        return;
    d->format = format;
    d->updateText();
}

//! \brief Setter for the `color` property.
void QeColorButton::setColor(const QColor &c)
{
    QE_DPTR;
    if (c != d->color)
        d->color = c;
    d->updateIcon();
    d->updateText();
}

/*!
   \brief Sets whether or not a frame is drawn around the color swatch.
 */
void QeColorButton::setFrameVisible(bool b)
{
    QE_DPTR;
    if (b != d->drawFrame)
        d->drawFrame = b;
    d->updateIcon();
}



//! Shows a color selection dialog and changes the current `color` property, if appropriate.
//! This will emit the `colorChanged(QColor)` signal.
void QeColorButton::showColorDialog()
{
    QColorDialog dlg(color());
    dlg.exec();

    auto newColor = dlg.currentColor();
    if (newColor != color()) {
        setColor(newColor);
        emit colorChanged(newColor);
    }
}

/*!
  \class QeColorButton
  \brief The QeColorButton class displays a user-selected color.

  This class is specifically meant for use in settings dialogs and toolbars
  for setting and displaying a color selection (e.g., window background
  color or text color).
*/
