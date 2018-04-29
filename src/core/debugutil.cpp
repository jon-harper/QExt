#include "debugutil.h"

#include <QMetaType>
#include <QBrush>
#include <QFont>
#include <QUuid>
#include <QRegion>
#include <QRect>

QString QeStringTools::fromVariant(const QVariant &var)
{
    QString ret;
    switch(var.type()) {
    case QMetaType::QString:
    case QMetaType::Bool:
    case QMetaType::QByteArray:
    case QMetaType::QChar:
    case QMetaType::QDate:
    case QMetaType::QDateTime:
    case QMetaType::QStringList:
    case QMetaType::QTime:
        ret = var.toString();
        break;
    case QMetaType::Double:
    case QMetaType::Int:
    case QMetaType::UInt:
    case QMetaType::LongLong:
    case QMetaType::ULongLong:
        ret = var.toString();
        break;
    case QMetaType::Float:
        ret = QString::number(var.value<float>());
        break;
    case QMetaType::Short:
        ret = QString::number(var.value<short>());
        break;
    case QMetaType::UShort:
        ret = QString::number(var.value<ushort>());
        break;
    case QMetaType::SChar:
        ret = QString::number(var.value<signed char>());
        break;
//    case QMetaType::QPoint:
//    {
//        QPoint p = var.value<QPoint>();
//        ret = QString("(%1, %2)").arg(p.x()).arg(p.y());
//        break;
//    }
    case QMetaType::QSize:
    {
        QSize sz = var.value<QSize>();
        ret = QString("%1 x %2").arg(sz.width()).arg(sz.height());
        break;
    }
    case QMetaType::QRect:
    {
        QRect r = var.value<QRect>();
        ret = QString("[(%1, %2), %3 x %4]").arg(
                    r.left()).arg(r.top()).arg(r.width()).arg(r.height());
        break;
    }
    case QMetaType::QFont:
    {
        QFont f = var.value<QFont>();
        ret = QString("[%1, %2]").arg(f.family()).arg(f.pointSize());
        break;
    }
    case QMetaType::QUuid:
        ret = var.value<QUuid>().toString();
    default:
        break;
    }
    return ret;
}
