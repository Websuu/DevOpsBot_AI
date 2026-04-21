============================================================
PROJEKT: DevOpsBot AI - Inteligentny Asystent Systemowy
PRZEDMIOT: Języki Programowania Obiektowego 2025/2026
AUTORZY: Mateusz Kołodziejczak, Maciej Kwiatkowski
============================================================

1. CEL I OPIS PROJEKTU
Aplikacja DevOpsBot AI integruje lokalne modele językowe (LLM) z terminalem 
systemu Windows. Pozwala na sterowanie komputerem za pomocą języka 
naturalnego. System automatycznie generuje odpowiednie komendy shellowe 
i wykonuje je w bezpiecznym środowisku.

2. KLUCZOWE FUNKCJE (v1.2.0)
- INTEGRACJA AI: Komunikacja z Ollama API (localhost:11434).
- TRYB DIAGNOSTYCZNY (F12): Ukryta konsola diagnostyczna z systemem 
  podpowiedzi naprawczych (Self-Healing Advice).
- BEZPIECZEŃSTWO: Podwójna blokada komend destrukcyjnych (Blacklist: del, 
  rmdir, format itp.).
- TRYB BEZPIECZNY (DRY RUN): Możliwość samej generacji tekstu bez 
  wykonywania operacji na systemie.
- MULTITHREADING: GUI oparte na QThread - aplikacja nie zamarza podczas 
  pracy modelu AI.

3. INSTRUKCJA URUCHOMIENIA
a) Przygotowanie AI:
   - Zainstaluj Ollama.
   - Uruchom w terminalu: 'ollama serve'.
   - Pobierz model: 'ollama run mistral' (zalecany).

b) Budowanie projektu:
   1. Sklonuj repozytorium: 
      git clone https://github.com/Websuu/JPO_DevOpsBot_AI.git
   2. Wejdź do folderu i skonfiguruj CMake:
      cmake -B build -DCMAKE_BUILD_TYPE=Release
   3. Skompiluj projekt:
      cmake --build build --config Release

c) Uruchomienie:
   Aplikacja znajduje się w: ./build/Release/JPO_DevOpsBot_AI.exe

4. TESTY JEDNOSTKOWE
Projekt posiada wbudowane testy modułu CommandExecutor:
   cd build
   ctest --output-on-failure

5. AUTORZY
- Mateusz Kołodziejczak
- Maciej Kwiatkowski

Projekt zrealizowany w ramach przedmiotu Języki Programowania Obiektowego (2025/2026).
============================================================