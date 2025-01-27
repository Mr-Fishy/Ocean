#pragma once

#include <exception>
#include <string>

namespace Ocean{
    enum EXCEPTIONS{
        //Logic errors
        INVALID_ARGUMENT =0,
        //
        DOMAIN_ERROR =1,
        //Length 
        LENGTH_ERROR =2,
        //Out of range
        OUT_OF_RANGE =3,
        //This error specifically is to be used by threads.
        FUTURE_ERROR =4,

        //runtime errors
        RANGE_ERROR =5,
        //Overflow error
        OVERFLOW_ERROR =6,
        //Underflow error
        UNDERFLOW_ERROR =7,
        //Regex errors
        REGEX_ERROR =8,
        //General system failure
        SYSTEM_ERROR =9,
        //Input output failure
        IOS_FAILURE =10,
        //Filesystem failure
        FILESYSTEM_ERROR =11,
        //unsure if this ever made it in c++, here just in case
        TX_EXCEPTION =12,
        //should be thrown when typeid is nullptr of polymorphic type
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
  
    };

    class OExcept : std::exception{
        //Accepts string and cstyle string
        explicit OExcept(const char* message) noexcept: msg(message){};
        explicit OExcept(const std::string& message) noexcept: msg(message){};

        //Just returns the message.
        virtual char const* what() const noexcept {return msg.c_str();};
        
        protected:
        const std::string msg;

        
    };

}