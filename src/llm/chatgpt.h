#ifndef CHATGPT_H
#define CHATGPT_H

#include <string>

namespace llm {
    std::string query_chatgpt(const std::string& prompt);
    std::string query_chatgpt_async(const std::string& prompt);
}

#endif
