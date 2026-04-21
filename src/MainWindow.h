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
#include <QCheckBox> // Dodane dla opcji Dry Run

class WorkerThread;

/**
 * @brief Główna klasa okna aplikacji DevOpsBot AI.
 * Obsługuje interfejs graficzny, skróty klawiszowe (F12) oraz logikę bezpieczeństwa.
 */
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

protected:
    /**
     * @brief Obsługuje zdarzenia klawiatury.
     * F12: Przełącza widoczność terminala diagnostycznego.
     */
    void keyPressEvent(QKeyEvent* event) override;

private slots:
    /** @brief Inicjuje proces generowania i (opcjonalnie) egzekucji komendy. */
    void onExecuteClicked();

    /** @brief Wywoływane, gdy AI zwróci gotową komendę. */
    void onCommandGenerated(const QString& command);

    /** @brief Wywoływane po zakończeniu operacji w terminalu. */
    void onCommandExecuted(const QString& output);

    /** @brief Wywoływane w przypadku błędów sieciowych lub systemowych. */
    void onErrorOccurred(const QString& errorMsg);

private:
    QLineEdit* inputField;    // Pole polecenia użytkownika
    QTextEdit* contextField;  // Instrukcje systemowe dla AI
    QLineEdit* responseField; // Wynikowa komenda wygenerowana przez AI
    QTextEdit* outputField;   // Wyjście z terminala (STDOUT)
    QPushButton* executeBtn;  // Przycisk startowy
    QCheckBox* dryRunCheck;   // Checkbox trybu bezpiecznego ("Tylko generuj")

    /** @brief Buduje strukturę UI i nakłada style CSS. */
    void setupUI();
};

/**
 * @brief Klasa wątku roboczego. Odpowiada za ciężkie operacje (HTTP do Ollama, procesy systemowe).
 * Zastosowanie wątku zapobiega "zamrażaniu" okna aplikacji.
 */
class WorkerThread : public QThread {
    Q_OBJECT
public:
    /**
     * @brief Konstruktor wątku.
     * @param context Kontekst systemowy.
     * @param prompt Zapytanie użytkownika.
     * @param dryRun Jeśli true, komenda nie zostanie wykonana w systemie.
     */
    WorkerThread(const QString& context, const QString& prompt, bool dryRun, QObject* parent = nullptr);

    /** @brief Implementacja logiki wątku. */
    void run() override;

signals:
    void commandGenerated(const QString& command);
    void commandExecuted(const QString& output);
    void errorOccurred(const QString& errorMsg);

private:
    QString m_context;
    QString m_prompt;
    bool m_dryRun; // Flaga trybu bezpiecznego
};

#endif // MAINWINDOW_H