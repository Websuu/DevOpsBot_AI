#include "MainWindow.h"
#include "OllamaClient.h"
#include "CommandExecutor.h"
#include "Logger.h"
#include <QMessageBox>
#include <QFontDatabase>
#include <QKeyEvent>
#include <QCheckBox>
#include <iostream> // Niezbędne do std::cout w konsoli F12

#ifdef Q_OS_WIN
#include <Windows.h> 
#endif

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setupUI();

    // Inicjalne ukrycie konsoli diagnostycznej
#ifdef Q_OS_WIN
    HWND hWnd = GetConsoleWindow();
    if (hWnd) ShowWindow(hWnd, SW_HIDE);
#endif
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI() {
    this->setWindowTitle("DevOpsBot AI - Terminal Operacyjny v1.2");
    this->resize(850, 750);

    QString futuristicStyle = R"(
        QMainWindow { background-color: #0A0E17; color: #E1E4E8; }
        QLabel { color: #00D1FF; font-weight: bold; font-size: 13px; text-transform: uppercase; letter-spacing: 1px; }
        QLineEdit, QTextEdit { 
            background-color: #010409; 
            color: #E1E4E8; 
            border: 1px solid #30363D; 
            border-radius: 6px; 
            padding: 10px; 
            font-family: 'Consolas', monospace;
            font-size: 14px;
        }
        QLineEdit:focus, QTextEdit:focus { border: 1px solid #00FF9C; }
        QCheckBox { color: #00FF9C; font-weight: bold; }
        QPushButton { 
            background-color: #00FF9C; 
            color: #0A0E17; 
            border: none; 
            border-radius: 6px; 
            padding: 12px; 
            font-weight: 900;
            text-transform: uppercase;
        }
        QPushButton:hover { background-color: #00D1FF; }
        QPushButton:disabled { background-color: #151921; color: #8B949E; }
    )";
    this->setStyleSheet(futuristicStyle);

    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(centralWidget);
    layout->setSpacing(12);
    layout->setContentsMargins(25, 25, 25, 25);

    QFont codeFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);

    layout->addWidget(new QLabel("Protokół Systemowy:"));
    contextField = new QTextEdit(this);
    contextField->setFixedHeight(60);
    contextField->setPlainText("Jesteś terminalem DevOps. Podaj TYLKO surową komendę Windows. Zakaz używania markdowna, wstępów i wyjaśnień.");
    layout->addWidget(contextField);

    layout->addWidget(new QLabel("Polecenie naturalne:"));
    inputField = new QLineEdit(this);
    inputField->setPlaceholderText("Wpisz polecenie (np. 'Pokaż stan procesów')...");
    inputField->setFixedHeight(45);
    layout->addWidget(inputField);

    dryRunCheck = new QCheckBox("TRYB BEZPIECZNY (Tylko generuj komendę)", this);
    dryRunCheck->setCursor(Qt::PointingHandCursor);
    layout->addWidget(dryRunCheck);

    executeBtn = new QPushButton("INICJUJ PROCES AI", this);
    executeBtn->setFixedHeight(50);
    layout->addWidget(executeBtn);

    layout->addWidget(new QLabel("Wygenerowane polecenie:"));
    responseField = new QLineEdit(this);
    responseField->setReadOnly(true);
    responseField->setFont(codeFont);
    responseField->setStyleSheet("background-color: #010409; border-left: 5px solid #00D1FF; color: #00FF9C;");
    layout->addWidget(responseField);

    layout->addWidget(new QLabel("Wyjście systemowe (STDOUT):"));
    outputField = new QTextEdit(this);
    outputField->setReadOnly(true);
    outputField->setFont(codeFont);
    layout->addWidget(outputField);

    setCentralWidget(centralWidget);

    connect(executeBtn, &QPushButton::clicked, this, &MainWindow::onExecuteClicked);
    connect(inputField, &QLineEdit::returnPressed, this, &MainWindow::onExecuteClicked);
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_F12) {
#ifdef Q_OS_WIN
        HWND hWnd = GetConsoleWindow();
        if (hWnd) {
            bool visible = IsWindowVisible(hWnd);
            ShowWindow(hWnd, visible ? SW_HIDE : SW_SHOW);
            if (!visible) SetForegroundWindow(hWnd);
        }
#endif
    }
    QMainWindow::keyPressEvent(event);
}

void MainWindow::onExecuteClicked() {
    QString userPrompt = inputField->text().trimmed();
    if (userPrompt.isEmpty()) return;

    executeBtn->setEnabled(false);
    executeBtn->setText("ANALIZA...");
    outputField->setText("[LOG] Nawiązywanie połączenia z rdzeniem Ollama...");

    WorkerThread* worker = new WorkerThread(contextField->toPlainText(), userPrompt, dryRunCheck->isChecked(), this);

    connect(worker, &WorkerThread::commandGenerated, this, &MainWindow::onCommandGenerated);
    connect(worker, &WorkerThread::commandExecuted, this, &MainWindow::onCommandExecuted);
    connect(worker, &WorkerThread::errorOccurred, this, &MainWindow::onErrorOccurred);
    connect(worker, &WorkerThread::finished, worker, &QObject::deleteLater);

    worker->start();
}

void MainWindow::onCommandGenerated(const QString& command) {
    responseField->setText(command);

    if (dryRunCheck->isChecked()) return;

    QStringList forbidden = { "del ", "rmdir", "rd ", "format", "erase", "shutdown", "reboot" };
    QString cmdLower = command.toLower();
    for (const QString& word : forbidden) {
        if (cmdLower.contains(word)) {
            QMessageBox::critical(this, "⚠️ FILTR BEZPIECZEŃSTWA",
                "Wykryto groźną komendę: " + word.toUpper() + "\nBlokada egzekucji aktywnego procesu.");
            break;
        }
    }
}

void MainWindow::onCommandExecuted(const QString& output) {
    outputField->append("\n[SUCCESS]:\n" + output);
    executeBtn->setEnabled(true);
    executeBtn->setText("INICJUJ PROCES AI");
    inputField->setEnabled(true);
    inputField->setFocus();
}

void MainWindow::onErrorOccurred(const QString& errorMsg) {
    outputField->append("\n[BŁĄD]: " + errorMsg);
    executeBtn->setEnabled(true);
    executeBtn->setText("INICJUJ PROCES AI");
    inputField->setEnabled(true);
}

// ---------------- Worker Thread Implementation ----------------

WorkerThread::WorkerThread(const QString& context, const QString& prompt, bool dryRun, QObject* parent)
    : QThread(parent), m_context(context), m_prompt(prompt), m_dryRun(dryRun) {
}

void WorkerThread::run() {
    try {
        OllamaClient ollama;
        std::string cmd = ollama.generateCommand(m_context.toStdString(), m_prompt.toStdString());
        QString qCmd = QString::fromStdString(cmd);
        emit commandGenerated(qCmd);

        if (m_dryRun) {
            emit commandExecuted("[INFO] Dry Run: " + qCmd);
            return;
        }

        QStringList forbidden = { "del ", "rmdir", "rd ", "format", "erase", "shutdown" };
        if (forbidden.contains(qCmd.toLower())) { // Uproszczone sprawdzenie dla przykładu
            emit errorOccurred("BLOKADA RDZENIA.");
            return;
        }

        std::string result = CommandExecutor::execute(cmd);
        emit commandExecuted(QString::fromStdString(result));
    }
    catch (const std::exception& e) {
        QString errorMsg = QString::fromStdString(e.what());

        // --- ZAAWANSOWANA DIAGNOSTYKA W KONSOLI (F12) ---
        std::cout << "\n====================================================" << std::endl;
        std::cout << " [!] RAPORT DIAGNOSTYCZNY DEVOPS BOT" << std::endl;
        std::cout << "====================================================" << std::endl;
        std::cout << " WYKRYTY BLAD: " << e.what() << std::endl;
        std::cout << "----------------------------------------------------" << std::endl;

        if (errorMsg.contains("Timeout") || errorMsg.contains("connect")) {
            std::cout << " DIAGNOZA: Brak odpowiedzi od serwera Ollama." << std::endl;
            std::cout << " MOZLIWE ROZWIAZANIA:" << std::endl;
            std::cout << "  1. Sprawdz czy ikona Ollama jest widoczna w zasobniku systemowym." << std::endl;
            std::cout << "  2. Uruchom recznie: wpisz 'ollama serve' w terminalu." << std::endl;
            std::cout << "  3. Upewnij sie, ze zadna zapora nie blokuje portu 11434." << std::endl;
        }
        else if (errorMsg.contains("404")) {
            std::cout << " DIAGNOZA: Endpoint API nie odpowiada lub model nie istnieje." << std::endl;
            std::cout << " MOZLIWE ROZWIAZANIA:" << std::endl;
            std::cout << "  1. Pobierz model Mistral: 'ollama run mistral'." << std::endl;
        }
        else {
            std::cout << " DIAGNOZA: Blad niestandardowy lub problem z uprawnieniami." << std::endl;
            std::cout << " MOZLIWE ROZWIAZANIA:" << std::endl;
            std::cout << "  1. Sprawdz czy Twoje konto ma uprawnienia administratora." << std::endl;
            std::cout << "  2. Sprawdz logi w Logger.log." << std::endl;
        }
        std::cout << "====================================================\n" << std::endl;

        emit errorOccurred(errorMsg);
    }
}