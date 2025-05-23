// Entry point for the MultiLLM-Linux-Command tool
#include "llm/gemini.h"
#include "llm/chatgpt.h"
#include <iostream>

int main(int argc, char* argv[]) {
    // ...parse arguments, select model, etc...
    std::cout << "MultiLLM-Linux-Command CLI" << std::endl;
    std::string prompt;
    std::cout << "Enter your prompt: ";
    std::getline(std::cin, prompt);
    std::string response;
    std::cout << "Select model ([g]emini/[c]hatgpt): ";
    char model_choice;
    std::cin >> model_choice;
    std::cin.ignore(); // clear newline
    if (model_choice == 'c' || model_choice == 'C') {
        response = llm::query_chatgpt(prompt);
        std::cout << "ChatGPT: " << response << std::endl;
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
