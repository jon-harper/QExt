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
#ifndef QE_WINDOWS_IDLIST_IMPL_H
#define QE_WINDOWS_IDLIST_IMPL_H

#include <ShlObj_core.h>
#include <utility>
#include <cstdint>

namespace {

static const ITEMIDLIST static_null_id = {{0x0000, {0x00}}};

enum class abIDType : std::uint8_t {
    CplApplet   = 0x00, //virtual
    GUID        = 0x1F, //guid, virtual
    Drive       = 0x23, //drive
    Drive2      = 0x25, //drive     lnk/persistent
    Drive3      = 0x29, //drive
    ShellExt    = 0x2E, //guid, virtual
    Drive1      = 0x2F, //drive
    Folder1     = 0x30, //localfs   lnk/persistent
    FolderA     = 0x31, //localfs
    ValueA      = 0x32, //localfs
    ValueW      = 0x34, //localfs
    FolderW     = 0x35, //localfs
    Workgroup   = 0x41, //remote?, virtual
    Computer    = 0x42, //remote, virtual
    NetProvider = 0x46, //remote, virtual?
    Network     = 0x47, //virtual
    IESPECIAL1  = 0x61, //htmlhelp, possibly local, virtual
    Printer     = 0x70, //guid, virtual
    IESPECIAL2  = 0xb1, //localfs, virtual?
    Share       = 0xc3, //remote, filesystem
    Root        = 0xFE, //guessing (hoping?) these are unused
    Invalid     = 0xFF
};

template <class T>
using isCastablePtr = std::bool_constant
<
       std::is_same_v<T, PITEMID_CHILD>
    || std::is_same_v<T, PUITEMID_CHILD>
    || std::is_same_v<T, PIDLIST_ABSOLUTE>
    || std::is_same_v<T, PUIDLIST_RELATIVE>
    || std::is_same_v<T, PIDLIST_RELATIVE>
>;

template <class T>
using isCastableConstPtr = std::bool_constant
<
       std::is_same_v<T, PCIDLIST_ABSOLUTE>
    || std::is_same_v<T, PCUIDLIST_ABSOLUTE>
    || std::is_same_v<T, PCIDLIST_RELATIVE>
    || std::is_same_v<T, PCUIDLIST_RELATIVE>
    || std::is_same_v<T, PCITEMID_CHILD>
    || std::is_same_v<T, PCUITEMID_CHILD>
>;

template <class T>
using isCastablePtrPtr = isCastablePtr<std::remove_pointer_t<T>>;

template <class T>
const T * constPointerFromOffset(T * const &ptr, const USHORT distance)
{
    if (!ptr)
        return nullptr;
    auto temp = reinterpret_cast<const unsigned char *>(ptr);
    temp += distance;
    return reinterpret_cast<const T *>(temp);
}

template <class T>
T * pointerFromOffset(T *&ptr, const USHORT distance)
{
    if (!ptr)
        return nullptr;
    auto temp = reinterpret_cast<unsigned char *>(ptr);
    temp += distance;
    return reinterpret_cast<T *>(temp);
}

//results are undefined if left or right is nullptr
int compareId(const ITEMIDLIST *left, const ITEMIDLIST *right)
{
    //check for the terminator/root node
    if (left->mkid.cb == 0) {
        if (right->mkid.cb == 0)
            return 0; //terminator, equal
        return -1; //left is terminator
    } else if (right->mkid.cb == 0)
        return 1; //right is terminator

    //do a byte-by-byte comparison
    auto left_iter  = left->mkid.abID;
    auto right_iter = right->mkid.abID;
    auto left_end   = reinterpret_cast<const unsigned char *>(left + left->mkid.cb);
    auto right_end  = reinterpret_cast<const unsigned char *>(right + right->mkid.cb);
    while (left_iter < left_end && right_iter < right_end) {
        if (*left_iter < *right_iter)
            return -1;
        if (*left_iter > *right_iter)
            return 1;
        ++left_iter;
        ++right_iter;
    }
    //one of the iterators hit the end while being equal up to this point. so we just see which is longer.
    if (left->mkid.cb < right->mkid.cb)
        return -1;
    if (left->mkid.cb > right->mkid.cb)
        return 1;
    return 0;
}

//results are undefined if left or right is nullptr.
int compareIdList(const ITEMIDLIST *left, const ITEMIDLIST *right)
{
    //check for the root node
    if (!left->mkid.cb) {
        if (!right->mkid.cb)
            return 0; //root, equal
        return -1; //left is root
    } else if (!right->mkid.cb)
        return 1;

    auto left_pos = left;
    auto right_pos = right;
    while (left_pos->mkid.cb && right_pos->mkid.cb)
    {
        auto comp = compareId(left_pos, right_pos);
        if (comp != 0)
            return comp;

        //advance if equal so far
        left_pos = constPointerFromOffset(left_pos, left_pos->mkid.cb);
        right_pos = constPointerFromOffset(right_pos, right_pos->mkid.cb);
    }
    if (!left_pos->mkid.cb) {
        if (!right_pos->mkid.cb)
            return 0; //identical
        return -1; //left is shorter
    } else //left is longer
        return 1;
}

}// anonymous namespace
#endif // QE_WINDOWS_IDLIST_IMPL_H
