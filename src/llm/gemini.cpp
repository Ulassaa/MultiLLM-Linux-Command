#include "gemini.h"
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

    std::string query_gemini(const std::string& prompt) {
        const char* api_key = std::getenv("GEMINI_API_KEY");
        if (!api_key) {
            return "[Error: GEMINI_API_KEY environment variable not set]";
        }
        const std::string url = "https://generativelanguage.googleapis.com/v1beta/models/gemini-2.0-flash:generateContent?key=" + std::string(api_key);

        // Prepare JSON payload
        Json::Value root;
        root["contents"][0]["parts"][0]["text"] = prompt;
        Json::StreamWriterBuilder writer;
        std::string payload = Json::writeString(writer, root);

        CURL* curl = curl_easy_init();
        std::string response_string;
        std::string header_string;
        if (curl) {
            struct curl_slist* headers = nullptr;
            headers = curl_slist_append(headers, "Content-Type: application/json");
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
            return json_response["candidates"][0]["content"]["parts"][0]["text"].asString();
        } catch (...) {
            return "[Error extracting Gemini response]";
        }
    }

    struct GeminiThreadArgs {
        std::string prompt;
        std::string* result;
    };

    void* query_gemini_thread_func(void* arg) {
        GeminiThreadArgs* args = static_cast<GeminiThreadArgs*>(arg);
        *(args->result) = llm::query_gemini(args->prompt);
        return nullptr;
    }

    std::string query_gemini_async(const std::string& prompt) {
        pthread_t thread;
        std::string* result = new std::string;
        GeminiThreadArgs args{prompt, result};
        pthread_create(&thread, nullptr, query_gemini_thread_func, &args);
        pthread_join(thread, nullptr);
        std::string ret = *result;
        delete result;
        return ret;
    }
}
