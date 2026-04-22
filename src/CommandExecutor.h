#ifndef COMMANDEXECUTOR_H
#define COMMANDEXECUTOR_H

/**
 * @file CommandExecutor.h
 * @brief Moduł odpowiedzialny za bezpieczne uruchamianie poleceń systemowych.
 * @author Mateusz Kołodziejczak, Maciej Kwiatkowski
 */

#include <string>
#include <stdexcept>

/**
 * @class CommandExecutionException
 * @brief Wyjątek rzucany w przypadku krytycznego błędu podczas próby uruchomienia komendy.
 * * Używany najczęściej, gdy system operacyjny nie może utworzyć potoku (pipe) 
 * lub brakuje uprawnień do wykonania procesu.
 */
class CommandExecutionException : public std::runtime_error {
public:
    /**
     * @brief Konstruktor wyjątku.
     * @param msg Treść komunikatu o błędzie.
     */
    explicit CommandExecutionException(const std::string& msg) : std::runtime_error(msg) {}
};

/**
 * @class CommandExecutor
 * @brief Statyczna klasa pomocnicza do interakcji z powłoką systemu operacyjnego.
 * * Klasa zapewnia mechanizm przechwytywania standardowego wyjścia (stdout) 
 * wykonanych komend, co pozwala na analizę wyników przez bota.
 */
class CommandExecutor {
public:
    /**
     * @brief Wykonuje komendę systemową i zwraca jej wynik.
     * * Funkcja wykorzystuje mechanizm `popen` do otwarcia potoku z procesem komendy.
     * Odczytuje dane aż do zakończenia strumienia.
     * * @param command Pełna treść komendy do wykonania (np. "dir", "ipconfig").
     * @return std::string Zwrócony przez system tekst (standard output).
     * @throws CommandExecutionException Jeśli otwarcie potoku przez `popen` nie powiedzie się.
     */
    static std::string execute(const std::string& command);
};

#endif // COMMANDEXECUTOR_H
