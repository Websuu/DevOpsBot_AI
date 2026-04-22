#ifndef OLLAMACLIENT_H
#define OLLAMACLIENT_H

/**
 * @file OllamaClient.h
 * @brief Klient API do komunikacji z lokalnym serwerem Ollama.
 * @author Mateusz Kołodziejczak, Maciej Kwiatkowski
 */

#include <string>
#include <stdexcept>

/**
 * @class OllamaConnectionException
 * @brief Wyjątek rzucany w przypadku problemów z połączeniem lub odpowiedzią API.
 * * Występuje, gdy serwer Ollama jest wyłączony, adres URL jest błędny 
 * lub otrzymany JSON jest uszkodzony.
 */
class OllamaConnectionException : public std::runtime_error {
public:
    /**
     * @brief Konstruktor wyjątku.
     * @param msg Treść komunikatu o błędzie połączenia.
     */
    explicit OllamaConnectionException(const std::string& msg) : std::runtime_error(msg) {}
};

/**
 * @class OllamaClient
 * @brief Klasa obsługująca zapytania REST do modelu językowego (LLM).
 * * Wykorzystuje bibliotekę LibCURL do wysyłania zapytań POST oraz nlohmann/json 
 * do przetwarzania odpowiedzi z modelu AI.
 */
class OllamaClient {
public:
    /**
     * @brief Konstruktor klienta API.
     * @param baseUrl Adres bazowy API (domyślnie: http://localhost:11434).
     */
    explicit OllamaClient(const std::string& baseUrl = "http://localhost:11434");

    /**
     * @brief Generuje komendę systemową na podstawie kontekstu i zapytania.
     * * Metoda wysyła synchroniczne zapytanie do endpointu /api/generate.
     * @param systemContext Instrukcje systemowe definiujące rolę AI (system prompt).
     * @param userPrompt Zapytanie użytkownika w języku naturalnym.
     * @return std::string Wyekstrahowana z JSON-a czysta komenda systemowa.
     * @throws OllamaConnectionException W przypadku błędu sieciowego lub błędu parsowania JSON.
     */
    std::string generateCommand(const std::string& systemContext, const std::string& userPrompt);

private:
    std::string m_baseUrl; ///< Przechowywany adres bazowy serwera.
    
    /**
     * @brief Statyczna funkcja zwrotna (callback) dla LibCURL.
     * * Służy do odbierania strumienia danych z serwera i dopisywania go do bufora string.
     * @param contents Wskaźnik na otrzymane dane.
     * @param size Rozmiar pojedynczego elementu danych.
     * @param nmemb Liczba otrzymanych elementów.
     * @param userp Wskaźnik na obiekt (string), do którego dopisywane są dane.
     * @return size_t Liczba faktycznie przetworzonych bajtów.
     */
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);
};

#endif // OLLAMACLIENT_H
