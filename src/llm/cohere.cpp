#include "cohere.h"
#include <string>
#include <curl/curl.h>
#include <json/json.h>
#include <cstdlib>
#include <sstream>
#include <iostream>
#include <pthread.h>

namespace llm {
    namespace {
        size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
            ((std::string*)userp)->append((char*)contents, size * nmemb);
            return size * nmemb;
        }
    }

    std::string query_cohere(const std::string& prompt) {
        const char* api_key = std::getenv("COHERE_API_KEY");
        if (!api_key) {
            return "[Error: COHERE_API_KEY environment variable not set]";
        }
        const std::string url = "https://api.cohere.ai/v1/chat";

        // Prepare JSON payload for Cohere chat
        Json::Value root;
        root["model"] = "command-a-03-2025";
        root["message"] = prompt;
        Json::StreamWriterBuilder writer;
        std::string payload = Json::writeString(writer, root);

        CURL* curl = curl_easy_init();
        std::string response_string;
        if (curl) {
            struct curl_slist* headers = nullptr;
            headers = curl_slist_append(headers, "Content-Type: application/json");
            std::string auth_header = std::string("Authorization: BEARER ") + api_key;
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

        Json::CharReaderBuilder reader;
        Json::Value json_response;
        std::string errs;
        std::istringstream s(response_string);
        if (!Json::parseFromStream(reader, s, &json_response, &errs)) {
            return "[Error parsing JSON response]";
        }
        try {
            return json_response["text"].asString();
        } catch (...) {
            return "[Error extracting Cohere response]";
        }
    }

    struct CohereThreadArgs {
        std::string prompt;
        std::string* result;
    };

    void* query_cohere_thread_func(void* arg) {
        CohereThreadArgs* args = static_cast<CohereThreadArgs*>(arg);
        *(args->result) = llm::query_cohere(args->prompt);
        return nullptr;
    }

    std::string query_cohere_async(const std::string& prompt) {
        pthread_t thread;
        std::string* result = new std::string;
        CohereThreadArgs args{prompt, result};
        pthread_create(&thread, nullptr, query_cohere_thread_func, &args);
        pthread_join(thread, nullptr);
        std::string ret = *result;
        delete result;
        return ret;
    }
}
