// Mistral integration
#include "mistral.h"
#include <string>
#include <curl/curl.h>
#include <json/json.h>
#include <cstdlib>
#include <sstream>
#include <iostream>

namespace llm {
    namespace {
        size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
            ((std::string*)userp)->append((char*)contents, size * nmemb);
            return size * nmemb;
        }
    }

    std::string query_mistral(const std::string& prompt) {
        const char* api_key = std::getenv("MISTRAL_API_KEY");
        if (!api_key) {
            return "[Error: MISTRAL_API_KEY environment variable not set]";
        }
        const std::string url = "https://api.mistral.ai/v1/chat/completions";

        // Prepare JSON payload for Mistral chat (OpenAI-compatible)
        Json::Value root;
        root["model"] = "mistral-tiny";
        Json::Value user_msg;
        user_msg["role"] = "user";
        user_msg["content"] = prompt;
        root["messages"].append(user_msg);
        Json::StreamWriterBuilder writer;
        std::string payload = Json::writeString(writer, root);

        CURL* curl = curl_easy_init();
        std::string response_string;
        if (curl) {
            struct curl_slist* headers = nullptr;
            headers = curl_slist_append(headers, "Content-Type: application/json");
            std::string auth_header = std::string("Authorization: Bearer ") + api_key;
            headers = curl_slist_append(headers, auth_header.c_str());
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);
            CURLcode res = curl_easy_perform(curl);
            curl_slist_free_all(headers);
            curl_easy_cleanup(curl);
            if (res != CURLE_OK) {
                return std::string("[CURL error: ") + curl_easy_strerror(res) + "]";
            }
        } else {
            return "[Error initializing CURL]";
        }

        // Debug: print raw response
        // std::cout << "[DEBUG] Raw Mistral response: " << response_string << std::endl;

        // Parse JSON response
        Json::CharReaderBuilder reader;
        Json::Value json_response;
        std::string errs;
        std::istringstream s(response_string);
        if (!Json::parseFromStream(reader, s, &json_response, &errs)) {
            return "[Error parsing JSON response]";
        }
        // Extract the model's reply
        try {
            return json_response["choices"][0]["message"]["content"].asString();
        } catch (...) {
            return "[Error extracting Mistral response]";
        }
    }
}
