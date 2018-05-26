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

/*!
 \headerfile global.h <qecore/global.h>
 \brief Defines global macros.
*/

#ifndef QE_CORE_GLOBAL_H
#define QE_CORE_GLOBAL_H


#if defined(QE_BUILD_CORE)
#  define QE_CORE_EXPORT __declspec(dllexport)
#else
#  define QE_CORE_EXPORT __declspec(dllimport)
#endif

#if defined(QE_BUILD_WIDGETS)
#  define QE_WIDGETS_EXPORT __declspec(dllexport)
#else
#  define QE_WIDGETS_EXPORT __declspec(dllimport)
#endif

/*!
    \brief Defines a class as static.

    Static classes may not have non-static member functions and cannot be instantiated.
    A static class is often useful as a drop-in replacement for namespaces with
    QMetaObject support. Use in conjunction with the Q_GADGET macro to use Q_ENUM, Q_FLAG, etc.
  */
#define QE_STATIC_CLASS(Classname) \
        Classname () = delete; \
        Classname (const Classname &) = delete; \
        Classname ( Classname && ) = delete; \

//! \brief Shorthand for QStringLiteral.
#define QStrLit QStringLiteral

#endif //QE_CORE_GLOBAL_H
