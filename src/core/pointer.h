#ifndef POINTER_H
#define POINTER_H

#include <type_traits>

template <class T, class Cleanup>
class Pointer_Impl
{
public:
    //! For STL compatibility.
    using pointer = std::add_pointer_t<T>;
    //! For STL compatibility.
    using element_type = T;
    using reference = std::add_lvalue_reference_t<T>;
    using deleter = Cleanup;

    //! Sets the stored pointer to `nullptr` and returns its old value.
    pointer take() noexcept
    {
        auto ret = data();
        this->d = nullptr;
        return ret;
    }

    void reset()
    {
        auto oldD = take();
        if (oldD)
            deleter::cleanup(oldD);
    }

    //! Sets the stored pointer to `other` and destroys the old pointer.
    void reset(pointer other)
    {
        if (data() == other)
            return;
        auto oldD = data();
        this->d = other;
        if (oldD)
            deleter::cleanup(oldD);
    }

    //! Returns the stored interface pointer.
    pointer data() const noexcept           { return isNull() ? nullptr : this->d; }

    //! For STL compatibility. Calls \ref data.
    pointer get() const noexcept            { return data(); }

    //! Returns true if the stored pointer is valid.
    explicit operator bool() const noexcept { return !isNull(); }

    //! Returns true if the stored pointer is `nullptr`.
    bool operator!() const noexcept         { return isNull(); }

    //! Dereferences the stored pointer.
    reference operator*() const             { return *(data()); }

    //! Returns the stored pointer, allowing pointer-to-member semantics.
    pointer operator->() const noexcept     { return data(); }

    //! Returns if the stored pointer is null or not.
    bool isNull() const noexcept            { return (this->d == nullptr); }

    void swap(Pointer_Impl &other) noexcept { std::swap(this->d, other.d); }
    //! For STL compatibility. Equivalent to \ref take.
    pointer release() noexcept              { return take(); }

protected:
    pointer d;

    //! Default construction
    Pointer_Impl() noexcept : d(nullptr) {}
    Pointer_Impl(pointer p) noexcept : d(p) {}
    ~Pointer_Impl() {}
};

#endif // POINTER_H
