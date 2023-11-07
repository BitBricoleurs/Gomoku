//
// Created by alexandre on 07/11/23.
//

#pragma once

#include <exception>
#include <string>


class Error : public std::exception {
    public:
        explicit Error(const std::string& message);

        const char* what() const noexcept override;

    private:
        std::string message_;
};
