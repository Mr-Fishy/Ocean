#pragma once

#include "Ocean/Core/Primitives/Memory.hpp"

template <class Type>
class UniquePtr {
public:
    inline UniquePtr() : p_Pointer(nullptr) { }
    inline UniquePtr(Type* pointer) : p_Pointer(pointer) { }
    template <typename ... Args>
    inline UniquePtr(Args&& ... args) : p_Pointer(oallocat(Type, 1, Ocean::MemoryService::Instance().SystemAllocator())(std::forward(args)...)) { }
    UniquePtr(const UniquePtr<Type>& other) = delete;
    inline ~UniquePtr() {
        ofree(p_Pointer, Ocean::MemoryService::Instance().SystemAllocator());
    }

    UniquePtr<Type>& operator = (const UniquePtr<Type>& other) = delete;

    template <class Other>
    inline operator UniquePtr<Other>() const {
        return static_cast<UniquePtr<Other>>(this);
    }

    inline Type* operator -> () { return this->p_Pointer; }
    inline const Type* operator -> () const { return this->p_Pointer; }

private:
    Type* p_Pointer;

};  // UniquePtr

template <class T, typename ... Args>
inline constexpr UniquePtr<T> MakeUniquePtr(Args&& ... args) {
    #ifdef OC_DEBUG

        oprint(CONSOLE_TEXT_GREEN("Making Unique Pointer."));

    #endif

    return UniquePtr<T>(std::forward<Args>(args)...);
}
