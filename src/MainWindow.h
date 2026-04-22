#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QThread>
#include <QKeyEvent>
#include <QCloseEvent>
#include <QCheckBox>

class WorkerThread;

/**
 * @brief Główna klasa okna aplikacji DevOpsBot AI.
 * Zarządza interfejsem użytkownika i komunikacją z wątkiem roboczym.
 */
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    /**
     * @brief Konstruktor klasy MainWindow.
     * @param parent Wskaźnik na obiekt nadrzędny (opcjonalny).
     */
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

protected:
    /**
     * @brief Obsługuje zdarzenia klawiatury (np. skrót F12 dla wygody).
     * @param event Wskaźnik na zdarzenie klawiatury.
     */
    void keyPressEvent(QKeyEvent* event) override;

    /**
     * @brief Obsługuje zdarzenie zamykania okna.
     * Zapisuje stan sesji i ustawienia aplikacji przed wyjściem.
     * @param event Wskaźnik na zdarzenie zamknięcia.
     */
    void closeEvent(QCloseEvent* event) override;

private slots:
    /** @brief Slot wywoływany po naciśnięciu przycisku wykonania. */
    void onExecuteClicked();
    
    /** @brief Przetwarza komendę wygenerowaną przez model AI. @param command Treść komendy. */
    void onCommandGenerated(const QString& command);
    
    /** @brief Wyświetla wynik wykonania komendy w terminalu. @param output Wynik z terminala. */
    void onCommandExecuted(const QString& output);
    
    /** @brief Wyświetla błędy powstałe podczas pracy aplikacji. @param errorMsg Treść błędu. */
    void onErrorOccurred(const QString& errorMsg);

private:
    QLineEdit* inputField;    ///< Pole do wpisywania poleceń dla AI.
    QTextEdit* contextField;  ///< Pole Protokół Systemowy (zapamiętywane między sesjami).
    QLineEdit* responseField; ///< Pole wyświetlające wygenerowaną komendę gotową do uruchomienia.
    QTextEdit* outputField;   ///< Pole wyświetlające surowy wynik z terminala systemowego.
    QPushButton* executeBtn;  ///< Główny przycisk uruchamiający proces.
    QCheckBox* dryRunCheck;   ///< Flaga: czy tylko generować komendę bez jej uruchamiania.

    /** @brief Konfiguruje układ elementów graficznych w oknie. */
    void setupUI();

    /** @brief Wczytuje ustawienia użytkownika z rejestru/pliku (QSettings). */
    void loadSettings();
    
    /** @brief Zapisuje aktualny stan aplikacji do pamięci trwałej (QSettings). */
    void saveSettings();
};

/**
 * @brief Klasa wątku roboczego (Ollama API / Terminal).
 * Pozwala na wykonywanie ciężkich zadań bez blokowania interfejsu graficznego.
 */
class WorkerThread : public QThread {
    Q_OBJECT
public:
    /**
     * @brief Inicjalizuje wątek roboczy.
     * @param context Kontekst systemowy dla AI.
     * @param prompt Zapytanie użytkownika.
     * @param dryRun Czy uruchamiać komendę w systemie.
     * @param parent Wskaźnik na obiekt nadrzędny.
     */
    WorkerThread(const QString& context, const QString& prompt, bool dryRun, QObject* parent = nullptr);
    
    /** @brief Główna metoda wątku wykonująca komunikację z API i terminalem. */
    void run() override;

signals:
    /** @brief Sygnał wysyłany po pomyślnym wygenerowaniu komendy przez AI. */
    void commandGenerated(const QString& command);
    
    /** @brief Sygnał wysyłany po otrzymaniu odpowiedzi z terminala. */
    void commandExecuted(const QString& output);
    
    /** @brief Sygnał wysyłany w przypadku wystąpienia jakiegokolwiek błędu. */
    void errorOccurred(const QString& errorMsg);

private:
    QString m_context; ///< Przechowywany kontekst systemowy.
    QString m_prompt;  ///< Przechowywane zapytanie użytkownika.
    bool m_dryRun;     ///< Przechowywana flaga trybu testowego.
};

#endif // MAINWINDOW_H
