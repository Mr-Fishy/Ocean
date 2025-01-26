#pragma once

#include <exception>
#include <string>

namespace Ocean{
    enum EXCEPTIONS{
        
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