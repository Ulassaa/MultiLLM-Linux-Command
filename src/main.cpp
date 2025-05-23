// Entry point for the MultiLLM-Linux-Command tool
#include "llm/gemini.h"
#include "llm/chatgpt.h"
#include "llm/deepseek.h"
#include "llm/cohere.h"
#include "llm/mistral.h"
#include <iostream>
#include <cstring>

int main(int argc, char* argv[]) {
    // --help flag
    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "--help") == 0) {
            std::cout << "MultiLLM-Linux-Command CLI\n";
            std::cout << "Usage: multillm [--model MODEL]\n";
            std::cout << "\n";
            std::cout << "Options:\n";
            std::cout << "  --help           Show this help message and exit\n";
            std::cout << "  --model MODEL    Select LLM model (gemini, chatgpt, deepseek, cohere, mistral)\n";
            std::cout << "\n";
            std::cout << "Available models:\n";
            std::cout << "  gemini    (default, free tier)\n";
            std::cout << "  chatgpt   (OpenAI, paid/free trial)\n";
            std::cout << "  deepseek  (OpenAI-compatible, paid)\n";
            std::cout << "  cohere    (free trial available)\n";
            std::cout << "  mistral   (OpenAI-compatible, free tier)\n";
            std::cout << "\n";
            std::cout << "Example:\n";
            std::cout << "  multillm --model gemini\n";
            return 0;
        }
    }

    std::cout << "MultiLLM-Linux-Command CLI" << std::endl;
    std::string prompt;
    std::string model_flag;
    // Parse --model flag if present
    for (int i = 1; i < argc - 1; ++i) {
        if (std::strcmp(argv[i], "--model") == 0 && i + 1 < argc) {
            model_flag = argv[i + 1];
            break;
        }
    }
    if (model_flag.empty()) {
        model_flag = "gemini";
    }
    std::cout << "Enter your prompt: ";
    std::getline(std::cin, prompt);
    std::string response;
    if (model_flag == "chatgpt") {
        response = llm::query_chatgpt(prompt);
        std::cout << "ChatGPT: " << response << std::endl;
    } else if (model_flag == "deepseek") {
        response = llm::query_deepseek(prompt);
        std::cout << "DeepSeek: " << response << std::endl;
    } else if (model_flag == "cohere") {
        response = llm::query_cohere(prompt);
        std::cout << "Cohere: " << response << std::endl;
    } else if (model_flag == "mistral") {
        response = llm::query_mistral(prompt);
        std::cout << "Mistral: " << response << std::endl;
    } else {
        response = llm::query_gemini(prompt);
        std::cout << "Gemini: " << response << std::endl;
    }
    // Debug: print raw response if empty or error
    if (response.empty() || response[0] == '[') {
        std::cout << "(Debug) The response may be empty or an error. Check your API key, quota, or network.\n";
    }
    return 0;
}
