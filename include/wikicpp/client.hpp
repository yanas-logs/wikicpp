#pragma once

#include <curl/curl.h>
#include <string>
#include <memory>
#include "wikicpp/exceptions.hpp"

namespace wikicpp {

static size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t total_size = size * nmemb;
    std::string* response = static_cast<std::string*>(userp);
    response->append(static_cast<char*>(contents), total_size);
    return total_size;
}

class HttpClient {
public:
    HttpClient() {
        curl_global_init(CURL_GLOBAL_DEFAULT);
        handle_ = curl_easy_init();
        if (!handle_) {
            throw std::runtime_error("Failed to initialize libcurl");
        }
    }

    ~HttpClient() {
        if (handle_) {
            curl_easy_cleanup(handle_);
        }
        curl_global_cleanup();
    }

    HttpClient(const HttpClient&) = delete;
    HttpClient& operator=(const HttpClient&) = delete;

    void set_user_agent(const std::string& ua) {
        user_agent_ = ua;
    }

    void set_timeout(long timeout_sec) {
        timeout_ = timeout_sec;
    }

    std::string get(const std::string& url) {
        if (!handle_) {
            throw NetworkError("CURL handle is not initialized");
        }

        std::string response;
        curl_easy_setopt(handle_, CURLOPT_URL, url.c_str());
        curl_easy_setopt(handle_, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(handle_, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(handle_, CURLOPT_TIMEOUT, timeout_);
        curl_easy_setopt(handle_, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(handle_, CURLOPT_SSL_VERIFYPEER, 1L);

        if (!user_agent_.empty()) {
            curl_easy_setopt(handle_, CURLOPT_USERAGENT, user_agent_.c_str());
        }

        CURLcode res = curl_easy_perform(handle_);
        if (res != CURLE_OK) {
            throw NetworkError("CURL error: " + std::string(curl_easy_strerror(res)));
        }

        long http_code = 0;
        curl_easy_getinfo(handle_, CURLINFO_RESPONSE_CODE, &http_code);
        if (http_code != 200) {
            throw NetworkError("HTTP error code: " + std::to_string(http_code));
        }

        return response;
    }

private:
    CURL* handle_ = nullptr;
    std::string user_agent_ = "wikicpp/1.0 (github.com/username/wikicpp; contact@email.com)";
    long timeout_ = 30;
};

}
