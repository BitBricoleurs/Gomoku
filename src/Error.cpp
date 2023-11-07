//
// Created by alexandre on 07/11/23.
//

#include "Error.hpp"


Error::Error(const std::string& message) : message_(message) {}

const char* Error::what() const noexcept {
    return message_.c_str();
}
