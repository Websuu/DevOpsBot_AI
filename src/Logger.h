#ifndef LOGGER_H
#define LOGGER_H

/**
 * @file Logger.h
 * @brief Prosty i bezpieczny wątkowo system logowania zdarzeń.
 * @author Mateusz Kołodziejczak, Maciej Kwiatkowski
 */

#include <string>
#include <iostream>
#include <mutex>

/**
 * @class Logger
 * @brief Statyczna klasa pomocnicza do zapisu zdarzeń i błędów aplikacji.
 * * Klasa zapewnia synchronizację za pomocą mutexa, co gwarantuje, że logi 
 * z różnych wątków (np. UI i WorkerThread) nie zostaną wymieszane w konsoli.
 */
class Logger {
public:
    /**
     * @brief Zapisuje wiadomość informacyjną do standardowego wyjścia (stdout).
     * @param msg Treść wiadomości do zalogowania.
     */
    static void info(const std::string& msg);

    /**
     * @brief Zapisuje komunikat o błędzie do standardowego wyjścia błędów (stderr).
     * @param msg Treść błędu do zalogowania.
     */
    static void error(const std::string& msg);

private:
    /**
     * @brief Mutex synchronizujący dostęp do strumieni wyjściowych.
     * Zapobiega przeplataniu się komunikatów przy jednoczesnym dostępie z wielu wątków.
     */
    static std::mutex logMutex;
};

#endif // LOGGER_H
