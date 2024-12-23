#pragma once

#include "Ocean/Core/Types/Strings.hpp"

// std
#include <fstream>

// TODO: It may be beneficial to change from c++ to c-style 
// file interaction for better memory and performance.

class File {
public:
    File(cstring path);
    ~File();

    string GetFile() const;
    string GetToKey(cstring key, cstring startKey = nullptr);

private:
    std::ifstream m_File;

};
