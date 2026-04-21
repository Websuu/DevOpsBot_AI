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
#include <QCloseEvent> // DODANE: Do obsługi zamykania okna
#include <QCheckBox>

class WorkerThread;

/**
 * @brief Główna klasa okna aplikacji DevOpsBot AI.
 */
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

protected:
    /**
     * @brief Obsługuje zdarzenia klawiatury (F12).
     */
    void keyPressEvent(QKeyEvent* event) override;

    /**
     * @brief Obsługuje zdarzenie zamykania okna.
     * Tutaj wywołamy zapisywanie ustawień (QSettings).
     */
    void closeEvent(QCloseEvent* event) override; // DODANE

private slots:
    void onExecuteClicked();
    void onCommandGenerated(const QString& command);
    void onCommandExecuted(const QString& output);
    void onErrorOccurred(const QString& errorMsg);

private:
    QLineEdit* inputField;
    QTextEdit* contextField;  // To pole (Protokół Systemowy) będzie zapamiętywane
    QLineEdit* responseField;
    QTextEdit* outputField;
    QPushButton* executeBtn;
    QCheckBox* dryRunCheck;

    void setupUI();

    // Opcjonalnie: funkcje pomocnicze dla czytelności kodu
    void loadSettings(); // DODANE
    void saveSettings(); // DODANE
};

/**
 * @brief Klasa wątku roboczego (Ollama API / Terminal).
 */
class WorkerThread : public QThread {
    Q_OBJECT
public:
    WorkerThread(const QString& context, const QString& prompt, bool dryRun, QObject* parent = nullptr);
    void run() override;

signals:
    void commandGenerated(const QString& command);
    void commandExecuted(const QString& output);
    void errorOccurred(const QString& errorMsg);

private:
    QString m_context;
    QString m_prompt;
    bool m_dryRun;
};

#endif // MAINWINDOW_H
