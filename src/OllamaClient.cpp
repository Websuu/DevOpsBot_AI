#include "OllamaClient.h"
#include "Logger.h"
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

OllamaClient::OllamaClient(const std::string& baseUrl) : m_baseUrl(baseUrl) {}

size_t OllamaClient::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string OllamaClient::generateCommand(const std::string& systemContext, const std::string& userPrompt) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        throw OllamaConnectionException("Failed to initialize libcurl.");
    }

    std::string endpoint = m_baseUrl + "/api/generate";
    std::string readBuffer;

    // Build the request using nlohmann::json
    // Uses the "mistral" or "llama3" model locally. Stream is disabled.
    json requestJson = {
        {"model", "mistral"}, 
        {"system", systemContext},
        {"prompt", userPrompt},
        {"stream", false}
    };

    std::string requestData = requestJson.dump();

    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, endpoint.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, requestData.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L); // 30 seconds max timeout

    Logger::info("Sending POST request to Ollama endpoint: " + endpoint);
    CURLcode res = curl_easy_perform(curl);
    
    // Cleanup curl handles
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    // Exception handling for network errors
    if (res != CURLE_OK) {
        throw OllamaConnectionException(std::string("curl_easy_perform() failed: ") + curl_easy_strerror(res));
    }

    try {
        json responseJson = json::parse(readBuffer);
        if (responseJson.contains("response")) {
            std::string command = responseJson["response"];
            
            // Clean up text format / trim accidental newline
            if (!command.empty() && command.back() == '\n') {
                command.pop_back();
            }
            return command;
        } else {
            throw OllamaConnectionException("JSON response does not contain 'response' field.");
        }
    } catch (const json::parse_error& e) {
        throw OllamaConnectionException(std::string("Failed to parse JSON response: ") + e.what() + "\nRaw Response:\n" + readBuffer);
    }
}
