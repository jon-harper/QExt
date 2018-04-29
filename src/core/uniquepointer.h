#ifndef UNIQUEPOINTER_H
#define UNIQUEPOINTER_H

#include "pointer.h"
#include <QScopedPointerDeleter>

template <class T, class Cleanup = QScopedPointerDeleter<T>>
class QeUniquePointer : public Pointer_Impl<T, Cleanup>
{
public:
   //! Default constructor.
    inline QeUniquePointer(T *p = nullptr) : Pointer_Impl(p) {}
    inline QeUniquePointer(QeUniquePointer && rhs)  : Pointer_Impl(rhs.take()) { }

    //! Move assignment operator.
    inline QeUniquePointer &operator=(QeUniquePointer &&rhs)
    {
        if (*this != rhs)
            reset(rhs.take());
        return *this;
    }

    //! Disables copying.
    QeUniquePointer(const QeUniquePointer &) = delete;
    //! Disables copy assignment.
    QeUniquePointer &operator=(const QeUniquePointer &) = delete;

    //! The destructor checks if the stored pointer is valid, and destroys it if necessary.
    inline ~QeUniquePointer()
    {
        reset();
    }

};

//! Uses forwarding to construct an instance of UniquePointer.
template <class T, class... Args>
QeUniquePointer<T> qeMakeUnique(Args &&... args)
{
    return new T(std::forward<Args>(args)...);
}

/*!
  Returns true if \a lhs and \a rhs manage the same pointer.
  \relates UniquePointer
 */
template <class T, class Cleanup>
inline bool operator==(const QeUniquePointer<T, Cleanup> &lhs, const QeUniquePointer<T, Cleanup> &rhs)
{
    return lhs.data() == rhs.data();
}

/*!
  Returns true if \a lhs and \a rhs do *not* manage the same pointer.
  \relates UniquePointer
 */
template <class T, class Cleanup>
inline bool operator!=(const QeUniquePointer<T, Cleanup> &lhs, const QeUniquePointer<T, Cleanup> &rhs)
{
    return !(lhs == rhs);
}

namespace std {
/*!
  Specialization of `std::swap` for UniquePointer.
  \relates UniquePointer
 */
    template <class T, class Cleanup>
    inline void swap(QeUniquePointer<T, Cleanup> &p1, QeUniquePointer<T, Cleanup> &p2)
    {
        p1.swap(p2);
    }
}

#endif // UNIQUEPOINTER_H
