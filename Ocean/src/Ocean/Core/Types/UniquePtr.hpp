#pragma once

#include "Ocean/Core/Types/Bool.hpp"

#include "Ocean/Core/Primitives/Assert.hpp"
#include "Ocean/Core/Primitives/Memory.hpp"

namespace internal {

    template <class Type>
    struct TypeFlag {
        Type* flag;

    };  // TypeFlag

    template <class Type>
    struct UniqueData {
        inline b8 isDestructable() const {
            return this->destructable;
        }

        inline b8 isActive() const {
            return this->pointer != nullptr;
        }

        Type* pointer;
        b8 destructable;

    };  // UniqueData

}

template <class Type>
class UniquePtr {
private:
    template <typename ... _Args>
    inline UniquePtr(OC_UNUSED internal::TypeFlag<Type> flag, _Args&& ... args) : p_Data(oallocat(internal::UniqueData<Type>, 1, oSystemAllocator)) {
        this->p_Data->pointer = oallocat(Type, 1, oSystemAllocator);

        new(this->p_Data->pointer) Type(std::forward<_Args>(args)...);
    }

    template <class T, typename ... Args>
    friend constexpr UniquePtr<T> MakeUniquePtr(Args&& ... args);

    inline UniquePtr(const internal::UniqueData<Type>& data) : p_Data(oallocat(internal::UniqueData<Type>, 1, oSystemAllocator)) {
        this->p_Data->pointer = data.pointer;
        this->p_Data->destructable = data.destructable;
    }

public:
    inline UniquePtr() : p_Data(nullptr) { }
    inline UniquePtr(Type* pointer) : p_Data(oallocat(internal::UniqueData<Type>, 1, oSystemAllocator)) {
        this->p_Data->pointer = pointer;
        this->p_Data->destructable = true;
    }
    UniquePtr(const UniquePtr<Type>& other) = delete;
    inline ~UniquePtr() {
        if (this->p_Data->isActive() && this->p_Data->isDestructable())
            ofree(this->p_Data->pointer, oSystemAllocator);

        if (this->p_Data->isDestructable())
            ofree(this->p_Data, oSystemAllocator);
    }

    inline UniquePtr<Type>& operator = (const UniquePtr<Type>& other) {
        this->p_Data = other.p_Data;
        this->p_Data->destructable = false;

        return *this;
    }

    inline void SetPtr(Type* pointer) {
        OASSERTM(!this->p_Data->isActive(), "Unique Pointer being set when a previous pointer still exists!");

        this->p_Data->pointer = pointer;
    }

    template <class Other>
    inline operator UniquePtr<Other>() const {
        return UniquePtr<Other>({ static_cast<Other*>(this->p_Data->pointer), false });
    }

    inline Type* operator -> () { return this->p_Data->pointer; }
    inline const Type* operator -> () const { return this->p_Data->pointer; }

private:
    internal::UniqueData<Type>* p_Data;

};  // UniquePtr

template <class T, typename ... Args>
inline constexpr UniquePtr<T> MakeUniquePtr(Args&& ... args) {
    #ifdef OC_DEBUG

        oprint(CONSOLE_TEXT_GREEN("\tMaking Unique Pointer.\n"));

    #endif

    return UniquePtr<T>(internal::TypeFlag<T>(), std::forward<Args>(args)...);
}
