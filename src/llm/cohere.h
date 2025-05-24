#ifndef COHERE_H
#define COHERE_H

#include <string>

namespace llm {
    std::string query_cohere(const std::string& prompt);
    std::string query_cohere_async(const std::string& prompt);
}

#endif
