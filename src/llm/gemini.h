#ifndef GEMINI_H
#define GEMINI_H

#include <string>

namespace llm {
    std::string query_gemini(const std::string& prompt);
    std::string query_gemini_async(const std::string& prompt);
}

#endif
