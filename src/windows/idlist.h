#ifndef QE_WINDOWS_SHELL_IDLIST_H
#define QE_WINDOWS_SHELL_IDLIST_H

#include <iterator>
#include <ShlObj_core.h>
#include <qewindows/unaligned.h>

namespace qe {
namespace windows {
namespace shell {

#include "idlist_impl.h"

class IdList
{
public:
    using element_type = ITEMIDLIST *;
    using pointer = ITEMIDLIST **;

    enum class InferredType {
        Local = 0x1,
        Remote = 0x2,
        Filesystem = 0x4,
        Drive = 0x8 | Filesystem,
        Virtual = 0x10,

        LocalFilesystem = Local | Filesystem,
        LocalDrive = Drive | Local,
        LocalVirtual = Local | Virtual,

        RemoteDrive = Remote | Drive,
        RemoteVirtual = Remote | Virtual,
        RemoteFilesystem = Remote | Filesystem,

        Unknown = 0xF0,
        Invalid = 0xFF
    };

    IdList() noexcept : m_id(nullptr) {}
    IdList(ITEMIDLIST *id) noexcept : m_id(id) {}
    inline IdList(const IdList &other);
    IdList(IdList &&other) noexcept : m_id(nullptr) { swap(other); }
    ~IdList() noexcept                              { if (m_id) ILFree(aligned_cast<PIDLIST_RELATIVE>(m_id)); }

    IdList & operator=(const IdList &other) noexcept{ IdList tmp(other); swap(tmp); return *this;  }
    IdList & operator=(IdList &&other) noexcept     { swap(other); return *this; }

    explicit operator bool() const noexcept         { return m_id; }
    bool operator!() const noexcept                 { return !m_id; }

    void swap(IdList &other) noexcept               { std::swap(m_id, other.m_id); }

    ITEMIDLIST * data() const noexcept              { return m_id; }
    ITEMIDLIST ** address() noexcept                { return &m_id; }

    std::uint8_t rawType() const noexcept;
    InferredType inferredType() const noexcept;
    unsigned int elementCount() const noexcept;
    unsigned int byteCount() const noexcept;

    template <class T, class = std::enable_if<isCastablePtr<T>::value>>
    T castTo() noexcept                             { return reinterpret_cast<T>(m_id); }

    template <class T, class = std::enable_if<isCastableConstPtr<T>::value>>
    T castTo() const noexcept                       { return reinterpret_cast<T>(m_id); }

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
        reference operator*() const                 { return *p; }
        pointer operator->() const noexcept         { return p; }
        pointer operator&() const noexcept          { return p; }
        const_iterator &operator++()                { return next(); }
        bool operator==(const const_iterator &other) const noexcept { return compareId(p, other.p) == 0; }
        bool operator!=(const const_iterator &other) const noexcept { return !(*this == other); }

        bool isNull() const noexcept                { return p || p->mkid.cb; }
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

bool IdList::const_iterator::hasNext() const noexcept
{
    if (isNull())
        return false;
    auto nxt = constPointerFromOffset(p, p->mkid.cb);
    if (!nxt || nxt->mkid.cb == 0) //double null termination
        return false;
    return true;
}

IdList::const_iterator &IdList::const_iterator::next()
{
    if (!p || p->mkid.cb == 0)
        throw std::out_of_range("iterator out of range");
    p = constPointerFromOffset(p, p->mkid.cb);
    return *this;
}

IdList::IdList(const IdList &other)
    : m_id(aligned_cast<ITEMIDLIST *>(ILCloneFull(aligned_cast<PCUIDLIST_ABSOLUTE>(other.m_id))))
{
}

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

IdList::InferredType IdList::inferredType() const noexcept
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
    case abIDType::ValueA: //localfs
    case abIDType::ValueW: //localfs
    case abIDType::FolderW: //localfs
        return InferredType::LocalFilesystem;
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

unsigned int IdList::elementCount() const noexcept
{
    unsigned int ret = 0;
    for (const auto &iter : *this) {
        (void)iter;
        ++ret;
    }
    return ret;
}

unsigned int IdList::byteCount() const noexcept
{
    unsigned int ret = 0;
    for(const auto &iter : *this) {
        ret += iter.mkid.cb;
    }
    return ret;
}

} // namespace shell
} // namespace windows
} // namespace qe

#endif // QE_WINDOWS_SHELL_IDLIST_H
