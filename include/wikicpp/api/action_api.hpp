#pragma once

#include <string>
#include <map>
#include <sstream>
#include <curl/curl.h>
#include "wikicpp/client.hpp"
#include "wikicpp/utils/json.hpp"
#include "wikicpp/exceptions.hpp"

namespace wikicpp {
namespace api {

class ActionAPI {
public:
    ActionAPI(const std::string& language = "en")
        : base_url_("https://" + language + ".wikipedia.org/w/api.php") {}

    void set_param(const std::string& key, const std::string& value) {
        params_[key] = value;
    }

    void set_params(const std::map<std::string, std::string>& params) {
        for (const auto& [key, value] : params) {
            params_[key] = value;
        }
    }

    void clear_params() {
        params_.clear();
    }

    std::string build_query() const {
        std::ostringstream oss;
        bool first = true;
        for (const auto& [key, value] : params_) {
            if (!first) {
                oss << "&";
            }
            first = false;
            char* escaped = curl_easy_escape(nullptr, value.c_str(), value.size());
            oss << key << "=" << escaped;
            curl_free(escaped);
        }
        return oss.str();
    }

    json send() {
        std::string url = base_url_ + "?" + build_query();

        HttpClient client;
        client.set_user_agent(user_agent_);
        client.set_timeout(timeout_seconds_);

        std::string response_str = client.get(url);
        json response = json::parse(response_str);

        if (response.contains("error")) {
            const auto& err = response["error"];
            std::string code = get_string(err, "code");
            std::string info = get_string(err, "info");
            throw APIError("MediaWiki API error [" + code + "]: " + info);
        }

        return response;
    }

    void set_user_agent(const std::string& ua) {
        user_agent_ = ua;
    }

    void set_timeout(long sec) {
        timeout_seconds_ = sec;
    }

    void set_format_json() {
        set_param("format", "json");
    }

    void set_action_query() {
        set_param("action", "query");
    }

private:
    std::string base_url_;
    std::map<std::string, std::string> params_;
    std::string user_agent_ = "wikicpp/1.0 (github.com/username/wikicpp; contact@email.com)";
    long timeout_seconds_ = 30;
};

}
}
