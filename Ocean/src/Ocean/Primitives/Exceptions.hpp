#pragma once

#include "Ocean/Types/Strings.hpp"

// std
#include <exception>

namespace Ocean{

    /**
     * @brief An enum of different error types that can occur.
     */
    typedef enum Error {
        /** @brief Logic errors */
        INVALID_ARGUMENT = 0,
        /** @brief  */
        DOMAIN_ERROR,
        /** @brief Length */
        LENGTH_ERROR,
        /** @brief Out of range */
        OUT_OF_RANGE,
        /** @brief This error specifically is to be used by threads. */
        FUTURE_ERROR,

        /** @brief runtime errors */
        RANGE_ERROR,
        /** @brief Overflow error */
        OVERFLOW_ERROR,
        /** @brief Underflow error */
        UNDERFLOW_ERROR,
        /** @brief Regex errors */
        REGEX_ERROR,
        /** @brief General system failure */
        SYSTEM_ERROR,
        /** @brief Input output failure */
        IOS_FAILURE,
        /** @brief Filesystem failure */
        FILESYSTEM_ERROR,
        /** @brief unsure if this ever made it in c++, here just in case */
        TX_EXCEPTION,
        /** @brief should be thrown when typeid is nullptr of polymorphic type */
        BAD_TYPEID,
        BAD_CAST,
        BAD_ANY_CAST,
        BAD_OPTIONAL_ACCESS,
        BAD_EXCPECTED_ACCESS,
        BAD_WEAK_PTR,
        BAD_FUNCTION_CALL,
        BAD_ALLOC,
        BAD_ARRAY_NEW_LENGTH,
        BAD_EXCEPTION,
        BAD_VARIANT_ACCESS,

        // Ocean Specific Errors

        /** @brief Occurs when you reach code that you weren't supposed to reach. */
        YOU_FUCKED_UP,

        // Window / Platform Errors

        /** @brief Occurs when the platform cannot be properly determined. */
        BAD_PLATFORM,
        /** @brief Occurs when the window handle is not valid. */
        BAD_WINDOW_HANDLE,

    } Error;

    /**
     * @brief A exception class that describes an error when thrown.
     */
    class Exception : std::exception{
    public:
        /**
         * @brief The information container of an Exception.
         */
        struct ExceptionInfo {
            /** @brief The type of @ref Error that the exception is. */
            const Error error;
            /** @brief The message that describes the error. */
            const cstring msg;

        };  // ExceptionInfo

    public:
        /**
         * @brief Construct a new Exception object.
         * 
         * @param error The error type of the exception.
         * @param message 
         */
        explicit Exception(const Error error, cstring message) noexcept : m_Info({ error, message }) { }
        // explicit Exception(const Error error, const string& message) noexcept : m_Info({ error, message.c_str() }) { }

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
        /** @brief The information of the exception. */
        ExceptionInfo m_Info;

    };  // Exception

}   // Ocean
