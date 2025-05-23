// Entry point for the MultiLLM-Linux-Command tool
#include "llm/gemini.h"
#include "llm/chatgpt.h"
#include "llm/deepseek.h"
#include "llm/cohere.h"
#include "llm/mistral.h"
#include <iostream>

int main(int argc, char* argv[]) {
    // ...parse arguments, select model, etc...
    std::cout << "MultiLLM-Linux-Command CLI" << std::endl;
    std::string prompt;
    std::cout << "Enter your prompt: ";
    std::getline(std::cin, prompt);
    std::string response;
    std::cout << "Select model ([g]emini/[c]hatgpt/[d]eepseek/[h]cohere/[m]istral): ";
    char model_choice;
    std::cin >> model_choice;
    std::cin.ignore(); // clear newline
    if (model_choice == 'c' || model_choice == 'C') {
        response = llm::query_chatgpt(prompt);
        std::cout << "ChatGPT: " << response << std::endl;
    } else if (model_choice == 'd' || model_choice == 'D') {
        response = llm::query_deepseek(prompt);
        std::cout << "DeepSeek: " << response << std::endl;
    } else if (model_choice == 'h' || model_choice == 'H') {
        response = llm::query_cohere(prompt);
        std::cout << "Cohere: " << response << std::endl;
    } else if (model_choice == 'm' || model_choice == 'M') {
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
