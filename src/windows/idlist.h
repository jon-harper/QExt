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
#ifndef QE_WINDOWS_SHELL_IDLIST_H
#define QE_WINDOWS_SHELL_IDLIST_H

#include <iterator>
#include <ShlObj_core.h>
#include <qewindows/global.h>
#include <qewindows/unaligned.h>

namespace qe {
namespace windows {
namespace shell {

#include "idlist_impl.h"

//! \brief An enum for type data computed from the final abID[0] element in an `ITEMIDLIST` array.
//! \related IdList
enum class InferredType {
    Local           = 0x01, //!< The id is part of the local system.
    Remote          = 0x02, //!< The id is remove and may be slow to access.
    Filesystem      = 0x04, //!< The id is part of a filesystem (i.e. not virtual).
    Virtual         = 0x08, //!< The id is *not* part of a filesystem.
    Drive           = 0x10 | Filesystem,    //!< The node represents a mount point, possibly a remote disk.
    Folder          = 0x20 | Filesystem,    //!< The id is a folder on a filesystem (possibly remote).
    File            = 0x40 | Filesystem,    //!< The id is a file on a filesystem (possibly remote).

    LocalFile       = Local | File,         //!< The id is a local file on the filesystem.
    LocalFolder     = Local | Folder,       //!< The id is a local folder on the filesystem.
    LocalDrive      = Local | Drive,        //!< The id is a locally mounted disk.
    LocalFilesystem = Local | Filesystem,   //!< The id is part of the filesystem, but the type is unknown.
    LocalVirtual    = Local | Virtual,      //!< The id is a local virtual item (e.g. This PC, the Control Panel...)

    RemoteFile      = Remote | File,        //!< The id is a remote or offline file on the filesystem.
    RemoteFolder    = Remote | Folder,      //!< The id is a remote or offline folder on the filesystem.
    RemoteDrive     = Remote | Drive,       //!< The id is a mounted network drive.
    RemoteFilesystem= Remote | Filesystem,  //!< The id is part of the filesystem, but the type is unknown.
    RemoteVirtual   = Remote | Virtual,     //!< The id is virtual and enumerating it may be slow (its children are remote).

    Unknown         = 0xF0, //!< The type of the id cannot be inferred.
    Invalid         = 0xFF  //!< The id is invalid.
};

//! This class holds an ITEMIDLIST * pointer, which is the base type of all Windows Shell id list
//! types.
//!
//! IdList is a cross between a smart pointer and a container. It provides `const` access to the
//! underlying data via IdList::const_iterator, and direct access via the castTo and castAddress
//! functions. The `ITEMIDLIST` pointer is copied using `ILCLone` when an IdList is copied, so each
//! instance is unique if the same pointer is not assigned to two IdLists.
class IdList
{
public:
    using element_type = ITEMIDLIST *;
    using pointer = ITEMIDLIST **;

    //! Default constructs an IdList initialized with `nullptr`.
    IdList() noexcept : m_id(nullptr) {}
    //! Constructs an IdList by taking ownership of id.
    IdList(ITEMIDLIST *id) noexcept : m_id(id) {}
    inline IdList(const IdList &other);
    //! Move constructs from other.
    IdList(IdList &&other) noexcept : m_id(nullptr) { swap(other); }
    //! Destroys the IdList by calling `ILFree`.
    ~IdList() noexcept                              { reset(); }

    //! Constructs an IdList by copying other.
    IdList & operator=(const IdList &other) noexcept{ IdList tmp(other); swap(tmp); return *this;  }
    //! Move constructs from other.
    IdList & operator=(IdList &&other) noexcept     { swap(other); return *this; }

    //! Enables `if(foo)` syntax.
    explicit operator bool() const noexcept         { return m_id; }
    //bool operator!() const noexcept                 { return !m_id; }
    inline bool isRoot() const noexcept;

    void swap(IdList &other) noexcept               { std::swap(m_id, other.m_id); }
    inline void reset(ITEMIDLIST *id = nullptr);

    //! Returns the stored pointer.
    ITEMIDLIST * data() const noexcept              { return m_id; }
    //! Returns a pointer to the stored pointer.
    ITEMIDLIST ** address() noexcept                { return &m_id; }
    inline const ITEMIDLIST *lastId() const noexcept;

    inline std::uint8_t rawType() const noexcept;
    inline InferredType inferredType() const noexcept;
    inline unsigned int elementCount() const noexcept;
    inline unsigned int byteCount() const noexcept;
    inline IdList parent() const;

    //! This function casts the stored pointer to a given `ITEMIDLIST` typedef.
    //!
    //! The following types are compatible with castTo:
    //! * P[C][U]ITEMID_CHILD
    //! * P[C][U]IDLIST_ABSOLUTE
    //! * P[C][U]IDLIST_RELATIVE>
    template <class T, class = std::enable_if<isCastablePtr<T>::value>>
    T castTo() noexcept                             { return reinterpret_cast<T>(m_id); }

    template <class T, class = std::enable_if<isCastableConstPtr<T>::value>>
    T castTo() const noexcept                       { return reinterpret_cast<T>(m_id); }

    //! This function returns a non-const pointer to the stored pointer (ITEMIDLIST **) that is
    //! cast to type T. This can be one of the following:
    //! * `PUITEMIDLIST_ABSOLUTE *`
    //! * `PITEMIDLIST_ABSOLUTE *`
    //! * `PUITEMIDLIST_RELATIVE *`
    //! * `PITEMIDLIST_RELATIVE *`
    //!
    //! castAddress is used to gain direct access to the stored pointer to set its value via
    //! shell functions.
    template <class T, class = std::enable_if<isCastablePtrPtr<T>::value>>
    T castAddress() noexcept                        { return reinterpret_cast<T>(&m_id); }

public:
    struct const_iterator
    {
        using difference_type = USHORT;
        using value_type = const ITEMIDLIST;
        using pointer = const ITEMIDLIST *;
        using reference = const ITEMIDLIST &;
        using iterator_category = std::forward_iterator_tag;

        explicit const_iterator() noexcept : p(nullptr) {}
        explicit const_iterator(IdList &id) noexcept : p(id.data()) {}
        explicit const_iterator(const ITEMIDLIST * id) noexcept : p(id) {}

        explicit operator bool() const noexcept     { return p; }
        operator const ITEMIDLIST *() const noexcept{ return p; }
        reference operator*() const                 { return *p; }
        pointer operator->() const noexcept         { return p; }
        pointer operator&() const noexcept          { return p; }
        const_iterator &operator++()                { return next(); }
        bool operator==(const const_iterator &other) const noexcept { return compareId(p, other.p) == 0; }
        bool operator!=(const const_iterator &other) const noexcept { return !(*this == other); }

        bool isNull() const noexcept                { return !p || !p->mkid.cb; }
        PCUITEMID_CHILD asChildId() const noexcept  { return aligned_cast<PCUITEMID_CHILD>(p); }

        inline bool hasNext() const noexcept;
        inline const_iterator &next();

    private:
        const ITEMIDLIST *p;
    };

    const_iterator begin() const noexcept           { return cbegin(); }
    const_iterator cbegin() const noexcept          { return const_iterator(m_id); }
    const_iterator end() const noexcept             { return cend(); }
    const_iterator cend() const noexcept            { return const_iterator(&static_null_id); }

private:
    ITEMIDLIST *m_id;
};

//! Equality operator for IdList.
//! \relates IdList
inline bool operator==(const IdList &lhs, const IdList &rhs)
{
    return compareIdList(lhs.data(), rhs.data()) == 0;
}

//! Inequality operator for IdList.
//! \relates IdList
inline bool operator!=(const IdList &lhs, const IdList &rhs)
{
    return !(lhs == rhs);
}

////////
// Implementation below

//! Checks if it is safe for the iterator to advance.
bool IdList::const_iterator::hasNext() const noexcept
{
    if (isNull())
        return false;
    const_iterator nxt = *this;
    ++nxt;
    if (!nxt || nxt->mkid.cb == 0) //double null termination
        return false;
    return true;
}

//! Advances the iterator. Synonymous with operator++.
IdList::const_iterator &IdList::const_iterator::next()
{
    if (!p || p->mkid.cb == 0)
        throw std::out_of_range("iterator out of range");
    p = constPointerFromOffset(p, p->mkid.cb);
    return *this;
}

//! Constructs an IdList by copying from other.
IdList::IdList(const IdList &other)
    : m_id(nullptr)
{
    auto tmp = ILCloneFull(aligned_cast<PCUIDLIST_ABSOLUTE>(other.m_id));
    m_id = aligned_cast<ITEMIDLIST *>(tmp);
}

//! Returns true if the stored pointer is a single null-initialized element.
bool IdList::isRoot() const noexcept
{
    if (!m_id)
        return false;
    if (cbegin() == cend())
        return true;
    return false;
}

//! Frees the currently stored PIDL and resets on id.
void IdList::reset(ITEMIDLIST *id)
{
    if (m_id)
        ILFree(aligned_cast<PIDLIST_RELATIVE>(m_id));
    m_id = id;
}

//! Returns a const pointer to the last id in the array.
//! \note Returns nullptr if the iterator is invalid or the (only) element if this is the root id.
const ITEMIDLIST *IdList::lastId() const noexcept
{
    auto iter = cbegin();
    if (iter.isNull())
        return iter;
    while (iter.hasNext())
        ++iter;
    return iter;
}

//! Gets the value of `mkid.abID[0]` of the final element in the array.
//!
//! \note This function performs a rudimentary sanity checks for the root/desktop and invalid
//! `mkid.cb` values. It returns `0xFF` for an invalid id list and `0xFE` for the root node.
uint8_t IdList::rawType() const noexcept
{
    //make sure it's not nullptr or the desktop id.
    if (!m_id)
        return static_cast<uint8_t>(abIDType::Invalid);
    if(m_id->mkid.cb == 0) //
        return static_cast<uint8_t>(abIDType::Root);
    if (m_id->mkid.cb < 3) //we've already checked for the desktop; anything else is invalid
        return static_cast<uint8_t>(abIDType::Invalid);
    unsigned char ret = 0; //caches the next-to-last value
    auto iter = cbegin();
    while (iter.hasNext()) {
        ++iter;
        if (iter->mkid.cb < 3) //check for invalid child ids
            return static_cast<uint8_t>(abIDType::Invalid);
        ret = iter->mkid.abID[0];
    }
    return ret;
}

//! Returns the type of the final `ITEMIDLIST`, if known or abIDType::Unknown otherwise.
//! \see InferredType
InferredType IdList::inferredType() const noexcept
{
    switch(static_cast<abIDType>(rawType())) {
    case abIDType::CplApplet: //local, virtual
    case abIDType::GUID: //guid, virtual
        return InferredType::LocalVirtual;
    case abIDType::Drive: //drive
    case abIDType::Drive1: //drive
    case abIDType::Drive2: //drive     lnk/persistent
    case abIDType::Drive3: //drive
        return InferredType::Drive;
    case abIDType::Folder1: //localfs   lnk/persistent
    case abIDType::FolderA: //localfs
    case abIDType::FolderW: //localfs
        return InferredType::Folder;
    case abIDType::ValueA: //localfs
    case abIDType::ValueW: //localfs
        return InferredType::File;
    case abIDType::ShellExt:    //guid, virtual, possibly remote/offline
    case abIDType::Printer:     //guid, virtual, possibly remote
        return InferredType::Virtual;
    case abIDType::Workgroup: //remote?, virtual
    case abIDType::Computer: //remote, virtual
    case abIDType::NetProvider: //remote, virtual?
    case abIDType::Network: //remote?, virtual
        return InferredType::RemoteVirtual;
    case abIDType::IESPECIAL1: //htmlhelp, possibly local, virtual
        return InferredType::Virtual;
    case abIDType::IESPECIAL2: //localfs, virtual?
        return InferredType::Local;
    case abIDType::Share: //remote, filesystem
        return InferredType::RemoteFilesystem;
    case abIDType::Root:
        return InferredType::LocalVirtual;
    case abIDType::Invalid:
        return InferredType::Invalid;
    default:
        return InferredType::Unknown;
    }
}

//! Returns the number of elements in the `ITEMIDLIST` array.
unsigned int IdList::elementCount() const noexcept
{
    unsigned int ret = 0;
    for (const auto &iter : *this) {
        (void)iter;
        ++ret;
    }
    return ret;
}

//! Returns the total byte count of the `ITEMIDLIST` array.
//! This is functionally equivalent to `ILGetSize` except that it is noexcept.
unsigned int IdList::byteCount() const noexcept
{
    unsigned int ret = 0;
    for(const auto &iter : *this) {
        ret += iter.mkid.cb;
    }
    return ret + 2; //add the null terminating bytes
}

//! Gets a pidl's parent id. This calls `ILRemoveLastID`.
//! This function returns a default-constructed id list if the called id list instance is invalid or
//! is the root node (desktop).
IdList IdList::parent() const
{
    if (!m_id || isRoot())
        return {};
    PIDLIST_ABSOLUTE ret = ILCloneFull(castTo<PCUIDLIST_ABSOLUTE>());
    if (!ILRemoveLastID(aligned_cast<PUIDLIST_RELATIVE>(ret)))
        return {};
    return {aligned_cast<ITEMIDLIST *>(ret)};
}

} // namespace shell
} // namespace windows
} // namespace qe

#ifndef QEXT_NO_CLUTTER
using QeShellIdList = qe::windows::shell::IdList;
#endif

#endif // QE_WINDOWS_SHELL_IDLIST_H
