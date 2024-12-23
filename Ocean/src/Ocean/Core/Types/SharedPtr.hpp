#pragma once

#include "Ocean/Core/Types/Bool.hpp"
#include "Ocean/Core/Types/Integers.hpp"

#include "Ocean/Core/Primitives/Macros.hpp"
#include "Ocean/Core/Primitives/Memory.hpp"

// https://en.cppreference.com/w/cpp/language/operators

template <class Type>
class SharedPtr {
private:
    struct Reference {
        inline Reference() : pointer(nullptr), refCount(0) { }
        inline Reference(Type* pointer) : pointer(pointer), refCount(1) { }
        template <typename ... Args>
        inline Reference(Args&& ... args) {
            oallocat(Type, 1, Ocean::MemoryService::Instance().SystemAllocator())(std::forward(args)...);
        }
        inline ~Reference() {
            ofree(pointer, Ocean::MemoryService::Instance().SystemAllocator());
        }

        Type* pointer;
        u16 refCount;

        inline Reference& operator ++ () { ++this->refCount; return *this; }
        inline Reference operator ++ (i32) { Reference old = *this; operator++(); return old; }

        inline Reference& operator -- () { --this->refCount; return *this; }
        inline Reference operator -- (i32) { Reference old = *this; operator--(); return old; }

    };  // Reference

public:
    inline SharedPtr() : p_Ref(nullptr) { }
    inline SharedPtr(Type* pointer) : p_Ref(oallocat(Reference, 1, Ocean::MemoryService::Instance().SystemAllocator())(pointer)) { }
    template <typename ... Args>
    inline SharedPtr(Args&& ... args) : p_Ref(oallocat(Reference, 1, Ocean::MemoryService::Instance().SystemAllocator())(std::forward(args)...)) { }
    inline SharedPtr(const SharedPtr& other) {
        this->p_Ref = other.p_Ref;
        this->p_Ref++;
    }
    inline ~SharedPtr() {
        this->p_Ref--;

        if (this->p_Ref->refCount == 0)
            ofree(this->p_Ref, Ocean::MemoryService::Instance().SystemAllocator());
    }

    inline Type* Get() const { return this->p_Pointer; }
    inline Type* Get() { return this->p_Pointer; }
    inline b8 Set(Type* pointer) { if (!p_Ref->count) { this->p_Pointer = pointer; return true; } return false; }

    inline void Reset(Type* newPointer, b8 force = false) {
        if (!force && this->p_Ref->refCount != 0)
            oprint(CONSOLE_TEXT_YELLOW("NOTICE: ReferencePointer attempted to reset when references still exist!"));

        this->p_Ref->pointer = newPointer;
        if (this->p_Ref->refCount == 0)
            this->p_Ref++;
    }

    inline SharedPtr<Type>& operator = (const SharedPtr<Type> other) {
        if (this == &other || this->p_Ref == other.p_Ref)
            return *this;

        this->p_Ref--;
        if (this->p_Ref->refCount == 0)
            ofree(this->p_Ref, Ocean::MemoryService::Instance().SystemAllocator());

        this->p_Ref = other.p_Ref;
        this->p_Ref++;

        return *this;
    }

    template <class Other>
    inline operator SharedPtr<Other>() const {
        return static_cast<SharedPtr<Other>>(this);
    }

    inline Type* operator -> () { return this->p_Ref->pointer; }
    inline const Type* operator -> () const { return this->p_Ref->pointer; }

private:
    Reference* p_Ref;

};  // SharedPtr

template <class T, typename ... Args>
inline constexpr SharedPtr<T> MakeSharedPtr(Args&& ... args) {
    #ifdef OC_DEBUG

        oprint(CONSOLE_TEXT_GREEN("Making Shared Pointer."));

    #endif

    return SharedPtr<T>(std::forward<Args>(args)...);
}
