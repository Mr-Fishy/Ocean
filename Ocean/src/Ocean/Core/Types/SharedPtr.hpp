#pragma once

#include "Ocean/Core/Types/Bool.hpp"
#include "Ocean/Core/Types/Integers.hpp"

#include "Ocean/Core/Primitives/Log.hpp"
#include "Ocean/Core/Primitives/Macros.hpp"
#include "Ocean/Core/Primitives/Memory.hpp"

// https://en.cppreference.com/w/cpp/language/operators

namespace internal {

    struct Reference {
        inline Reference() : refCount(0) { }
        Reference(const Reference& other) = default;
        inline ~Reference() {
            oprint("Destroying Shared Ptr!");
        }

        Reference& operator = (const Reference& other) = delete;

        u16 refCount;

        inline b8 isDestructable() noexcept {
            return this->refCount == 0;
        }
        inline b8 isActive() noexcept {
            return !this->isDestructable();
        }

        inline Reference& operator ++ () { ++this->refCount; return *this; }
        inline Reference operator ++ (i32) { Reference old = *this; operator++(); return old; }

        inline Reference& operator -- () { --this->refCount; return *this; }
        inline Reference operator -- (i32) { Reference old = *this; operator--(); return old; }

    };  // Reference

}   // internal

template <class Type>
class SharedPtr {
private:
    struct TypeFlag {
        Type* flag;

    };  // TypeFlag

    template <typename ... _Args>
    inline SharedPtr(OC_UNUSED TypeFlag flag,_Args&& ... args) : p_Ref(oallocat(internal::Reference, 1, Ocean::MemoryService::Instance().SystemAllocator())), p_Pointer(oallocat(Type, 1, Ocean::MemoryService::Instance().SystemAllocator())) {
        new(this->p_Ref) internal::Reference();

        new(this->p_Pointer) Type(std::forward<_Args>(args)...);
    }

    template <class T, typename ... Args>
    friend constexpr SharedPtr<T> MakeSharedPtr(Args&& ... args);

public:
    inline SharedPtr() : p_Ref(nullptr), p_Pointer(nullptr) { }
    inline SharedPtr(Type* pointer) : p_Ref(oallocat(internal::Reference, 1, Ocean::MemoryService::Instance().SystemAllocator())), p_Pointer(pointer) {
        new(this->p_Ref) internal::Reference();
    }
    inline SharedPtr(internal::Reference* ref, Type* pointer) : p_Ref(ref), p_Pointer(pointer) {
        this->p_Ref++;
    }
    inline SharedPtr(const SharedPtr& other) : p_Ref(other.p_Ref), p_Pointer(other.p_Pointer) {
        this->p_Ref++;
    }
    inline ~SharedPtr() {
        this->p_Ref--;

        if (this->p_Ref->isDestructable()) {
            ofree(this->p_Ref, Ocean::MemoryService::Instance().SystemAllocator());
            ofree(this->p_Pointer, Ocean::MemoryService::Instance().SystemAllocator());
        }
    }

    inline Type* Get() const { return this->p_Pointer; }
    inline Type* Get() { return this->p_Pointer; }
    inline b8 Set(Type* pointer) { if (!p_Ref->refCount) { this->p_Pointer = pointer; return true; } return false; }

    inline void Reset(Type* newPointer, b8 force = false) {
        if (!force && this->p_Ref->isActive())
            oprint(CONSOLE_TEXT_YELLOW("NOTICE: ReferencePointer attempted to reset when references still exist!"));

        this->p_Pointer = newPointer;
        if (this->p_Ref->refCount == 0)
            this->p_Ref++;
    }

    inline SharedPtr<Type>& operator = (const SharedPtr<Type> other) {
        if (this == &other || this->p_Ref == other.p_Ref)
            return *this;

        this->p_Ref--;
        if (this->p_Ref->isDestructable())
            ofree(this->p_Ref, Ocean::MemoryService::Instance().SystemAllocator());

        this->p_Ref = other.p_Ref;
        this->p_Ref++;

        return *this;
    }

    template <class Other>
    inline operator SharedPtr<Other>() const {
        return SharedPtr<Other>(this->p_Ref, static_cast<Other*>(this->p_Pointer));
    }

    inline Type* operator -> () { return this->p_Pointer; }
    inline const Type* operator -> () const { return this->p_Pointer; }

private:
    internal::Reference* p_Ref;

    Type* p_Pointer;

};  // SharedPtr



template <class T, typename ... Args>
constexpr SharedPtr<T> MakeSharedPtr(Args&& ... args) {
    #ifdef OC_DEBUG

        oprint(CONSOLE_TEXT_GREEN("Making Shared Pointer."));

    #endif

    return SharedPtr<T>({ nullptr }, std::forward<Args>(args)...);
}
