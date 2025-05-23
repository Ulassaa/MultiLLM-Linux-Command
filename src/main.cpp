// Entry point for the MultiLLM-Linux-Command tool
#include "llm/gemini.h"
#include <iostream>

int main(int argc, char* argv[]) {
    // ...parse arguments, select model, etc...
    std::cout << "MultiLLM-Linux-Command CLI" << std::endl;
    std::string prompt;
    std::cout << "Enter your prompt: ";
    std::getline(std::cin, prompt);
    std::string response = llm::query_gemini(prompt);
    std::cout << "Gemini: " << response << std::endl;
    return 0;
}
