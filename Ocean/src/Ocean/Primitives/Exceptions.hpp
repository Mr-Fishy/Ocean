#pragma once

#include "Ocean/Types/Strings.hpp"

// std
#include <exception>

namespace Ocean{

    /**
     * @brief An enum of different error types that can occur.
     */
    enum Error {
        /** @brief Logic errors */
        INVALID_ARGUMENT =0,
        /** @brief  */
        DOMAIN_ERROR =1,
        /** @brief Length */
        LENGTH_ERROR =2,
        /** @brief Out of range */
        OUT_OF_RANGE =3,
        /** @brief This error specifically is to be used by threads. */
        FUTURE_ERROR =4,

        /** @brief runtime errors */
        RANGE_ERROR =5,
        /** @brief Overflow error */
        OVERFLOW_ERROR =6,
        /** @brief Underflow error */
        UNDERFLOW_ERROR =7,
        /** @brief Regex errors */
        REGEX_ERROR =8,
        /** @brief General system failure */
        SYSTEM_ERROR =9,
        /** @brief Input output failure */
        IOS_FAILURE =10,
        /** @brief Filesystem failure */
        FILESYSTEM_ERROR =11,
        /** @brief unsure if this ever made it in c++, here just in case */
        TX_EXCEPTION =12,
        /** @brief should be thrown when typeid is nullptr of polymorphic type */
        BAD_TYPEID =13,
        BAD_CAST =14,
        BAD_ANY_CAST =15,
        BAD_OPTIONAL_ACCESS =16,
        BAD_EXCPECTED_ACCESS =17,
        BAD_WEAK_PTR =18,
        BAD_FUNCTION_CALL =19,
        BAD_ALLOC =20,
        BAD_ARRAY_NEW_LENGTH =21,
        BAD_EXCEPTION =22,
        BAD_VARIANT_ACCESS =23

    };  // Error

    /**
     * @brief A exception class that describes an error when thrown.
     */
    class Exception : std::exception{
    public:
        /**
         * @brief The information container of an Exception.
         */
        struct ExceptionInfo {
            const Error error; /** @brief The type of @ref Error that the exception is. */
            const cstring msg; /** @brief The message that describes the error. */

        };  // ExceptionInfo

    public:
        /**
         * @brief Construct a new Exception object.
         * 
         * @param error The error type of the exception.
         * @param message 
         */
        explicit Exception(const Error error, cstring message) noexcept : m_Info({ error, message }) { }
        explicit Exception(const Error error, const string& message) noexcept : m_Info({ error, message.c_str() }) { }

        /**
         * @brief Gets the message of the error.
         * 
         * @return cstring 
         */
        virtual cstring what() const noexcept override { return this->m_Info.msg; }

        /**
         * @brief Gets the error type.
         * 
         * @return Error 
         */
        Error error() const noexcept { return this->m_Info.error; }

    protected:
        ExceptionInfo m_Info; /** @brief The information of the exception. */

    };  // Exception

}   // Ocean
