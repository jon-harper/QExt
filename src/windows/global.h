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
 \headerfile global.h <qewindows/global.h>
 \brief Defines global macros for the QeWindows module.
*/

#ifndef QE_WINDOWS_GLOBAL_H
#define QE_WINDOWS_GLOBAL_H

#include <qecore/global.h>

#if defined(QE_BUILD_WINDOWS)
#  define QE_WINDOWS_EXPORT __declspec(dllexport)
#else
#  define QE_WINDOWS_EXPORT __declspec(dllimport)
#endif

#endif //QE_WINDOWS_GLOBAL_H
