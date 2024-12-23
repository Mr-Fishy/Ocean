#include "File.hpp"

#include "Ocean/Core/Types/Bool.hpp"
#include "Ocean/Core/Types/Integers.hpp"

#include "Ocean/Core/Primitives/Assert.hpp"

// std
#include <cstring>
#include <sstream>

File::File(cstring path) : m_File(path) {
    OASSERTM(this->m_File.is_open(), "Failed to open file \"%s\"!", path);
}

File::~File() {
    this->m_File.close();
}

string File::GetFile() const {
    std::stringstream buffer;

    buffer << this->m_File.rdbuf();

    return buffer.str();
}



sizet IndexOrNpos(sizet value) {
    return value == std::string::npos ? 0 : value;
}



string File::GetToKey(cstring key, cstring startKey) {
    string result;
    string buffer;
    sizet index = 0;

    if (!startKey) {
        while (std::getline(this->m_File, buffer)) {
            if ((index = IndexOrNpos(buffer.find(key)))) {
                result.append(buffer.substr(0, index - 1));

                return result;
            }
            
            result.append(buffer);
        }
    }
    else {
        b8 started = false;

        while (std::getline(this->m_File, buffer)) {
            if ((index = IndexOrNpos(buffer.find(startKey)))) {
                result.append(buffer.substr(index + std::strlen(startKey)));

                index = 0;
                started = true;
                continue;
            }
            else if ((index = IndexOrNpos(buffer.find(key)))) {
                result.append(buffer.substr(0, index - 1));

                return result;
            }

            if (started)
                result.append(buffer);
        }
    }

    return result;
}
