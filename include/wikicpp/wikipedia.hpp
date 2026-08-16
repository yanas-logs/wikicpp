#pragma once

#include <string>
#include <vector>
#include "wikicpp/api/action_api.hpp"
#include "wikicpp/utils/json.hpp"

namespace wikicpp {

struct SearchResult {
    std::string title;
    std::string snippet;
    int pageid = 0;
    int size = 0;
    int word_count = 0;
    std::string timestamp;
};

class Wikipedia {
public:
    explicit Wikipedia(const std::string& language = "en")
        : language_(language) {}

    void set_user_agent(const std::string& ua) {
        user_agent_ = ua;
    }

    std::vector<SearchResult> search(const std::string& query, int limit = 10) {
        if (limit < 1) {
            limit = 1;
        }
        if (limit > 50) {
            limit = 50;
        }

        api::ActionAPI action(language_);
        action.set_user_agent(user_agent_);
        action.set_format_json();
        action.set_action_query();

        std::map<std::string, std::string> params = {
            {"list", "search"},
            {"srsearch", query},
            {"srlimit", std::to_string(limit)},
            {"srprop", "snippet|size|wordcount|timestamp"}
        };
        action.set_params(params);

        json response = action.send();

        std::vector<SearchResult> results;
        if (response.contains("query") && response["query"].contains("search")) {
            const auto& search_array = response["query"]["search"];
            for (const auto& item : search_array) {
                SearchResult res;
                res.title = get_string(item, "title");
                res.snippet = get_string(item, "snippet");
                res.pageid = item.value("pageid", 0);
                res.size = item.value("size", 0);
                res.word_count = item.value("wordcount", 0);
                res.timestamp = get_string(item, "timestamp");
                results.push_back(res);
            }
        }

        return results;
    }

private:
    std::string language_;
    std::string user_agent_ = "";
};

}
