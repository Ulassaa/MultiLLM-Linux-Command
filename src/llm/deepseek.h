#ifndef DEEPSEEK_H
#define DEEPSEEK_H

#include <string>

namespace llm {
    std::string query_deepseek(const std::string& prompt);
    std::string query_deepseek_async(const std::string& prompt);
}

#endif
