cmake_minimum_required(VERSION 3.16)

# NAPRAWA BŁĘDU NOWEJ WERSJI CMAKE (4.x)
# To zastępuje konieczność wpisywania flagi w terminalu
if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

project(DevOpsBotAI VERSION 1.0 LANGUAGES CXX)

# Włączenie automatycznej obsługi narzędzi Qt
set(CMAKE_AUTOMOC ON) 
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Obsługa wyjątków dla MSVC (zostawiamy, nie przeszkadza w MinGW)
if(MSVC)
    add_compile_options(/EHsc)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Znajdź Qt6
find_package(Qt6 COMPONENTS Widgets REQUIRED)

# 2. Automatyczne pobieranie biblioteki CURL
include(FetchContent)
FetchContent_Declare(
  curl
  URL https://github.com/curl/curl/releases/download/curl-8_4_0/curl-8.4.0.tar.gz
)

# Konfiguracja CURL - wyłączamy zbędne rzeczy
set(BUILD_CURL_EXE OFF CACHE INTERNAL "")
set(BUILD_TESTING OFF CACHE INTERNAL "") # To wyłącza tylko testy wewnątrz biblioteki CURL
set(CURL_USE_LIBPSL OFF CACHE INTERNAL "")
set(HTTP_ONLY ON CACHE INTERNAL "")
set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "")

FetchContent_MakeAvailable(curl)

# 3. Automatyczne pobieranie nlohmann/json
FetchContent_Declare(
    json 
    URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz
)
FetchContent_MakeAvailable(json)

# Pliki źródłowe
set(SOURCES
    src/main.cpp
    src/MainWindow.cpp
    src/OllamaClient.cpp
    src/CommandExecutor.cpp
    src/Logger.cpp
)

# Pliki nagłówkowe
set(HEADERS
    src/MainWindow.h
    src/OllamaClient.h
    src/CommandExecutor.h
    src/Logger.h
)

# Tworzenie pliku wykonywalnego
qt_add_executable(DevOpsBotAI WIN32 ${SOURCES} ${HEADERS})

# Linkowanie bibliotek
target_link_libraries(DevOpsBotAI PRIVATE
    Qt6::Widgets
    libcurl
    nlohmann_json::nlohmann_json
)

# --- SEKCJA TESTÓW (TERAZ ODBLOKOWANA) ---
# To sprawi, że komenda 'ctest' w folderze build zacznie działać
enable_testing()
add_subdirectory(tests)
