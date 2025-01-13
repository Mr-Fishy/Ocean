#pragma once

// std
#include <memory>

/**
    * @brief A scope is a unique ptr. AKA it lives within the scope of usage without allowing lifetime outside of its usage.
    * 
    * @tparam T The object type.
    */
template <class T>
using Scope = std::unique_ptr<T>;

/**
    * @brief Create a Scope object.
    * 
    * @tparam T The object type.
    * @tparam Args The object's constructor argument types.
    * @param args The object's constructor args.
    * @return constexpr Scope<T>
    */
template<typename T, typename ... Args>
constexpr Scope<T> MakeScope(Args&& ... args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
}



/**
    * @brief A Ref is a shared ptr. AKA it lives as long as there is a reference to the object allowing extended lifetime by passing the ownership.
    * 
    * @tparam T The object type.
    */
template <class T>
using Ref = std::shared_ptr<T>;

/**
    * @brief Create a Ref object.
    * 
    * @tparam T The object type.
    * @tparam Args The object's constructor argument types.
    * @param args The object's constructor args.
    * @return constexpr Ref<T> 
    */
template<typename T, typename ... Args>
constexpr Ref<T> MakeRef(Args&& ... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}
