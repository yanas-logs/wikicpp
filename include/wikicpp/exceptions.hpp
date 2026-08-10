#pragma once

#include <stdexcept>
#include <string>

namespace wikicpp {

class WikiError : public std::runtime_error {
public:
    explicit WikiError(const std::string& msg) : std::runtime_error(msg) {}
};

class NetworkError : public WikiError {
public:
    explicit NetworkError(const std::string& msg) : WikiError(msg) {}
};

class APIError : public WikiError {
public:
    explicit APIError(const std::string& msg) : WikiError(msg) {}
};

class PageNotFoundError : public APIError {
public:
    explicit PageNotFoundError(const std::string& msg) : APIError(msg) {}
};

class RateLimitError : public APIError {
public:
    explicit RateLimitError(const std::string& msg) : APIError(msg) {}
};

}
