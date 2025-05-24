#ifndef MISTRAL_H
#define MISTRAL_H

#include <string>

namespace llm {
    std::string query_mistral(const std::string& prompt);
    std::string query_mistral_async(const std::string& prompt);
}

#endif
