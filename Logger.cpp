#include "Logger.h"
#include <iostream>
#include <chrono>
#include <ctime>

Logger::Logger(const std::string& filename) {
    file.open(filename, std::ios::app); // bestand openen in append-modus
    if (!file.is_open()) {
        std::cerr << "Kan bestand niet openen: " << filename << std::endl;
    }
}

Logger::~Logger() {
    if (file.is_open()) {
        file.close();
    }
}

void Logger::log(const std::string& message) {
    if (!file.is_open()) return;

    // timestamp toevoegen
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);

    // Timestamp ophalen en newline verwijderen
    std::string timestamp = std::ctime(&time);
    if (!timestamp.empty() && timestamp.back() == '\n') {
        timestamp.pop_back();
    }

    file << "[" << timestamp << "] " << message << std::endl;
}