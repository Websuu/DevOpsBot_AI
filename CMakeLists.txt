cmake_minimum_required(VERSION 3.16)
project(DevOpsBotAI VERSION 1.0 LANGUAGES CXX)

# W³¹czenie automatycznej obs³ugi narzêdzi Qt
set(CMAKE_AUTOMOC ON) # To naprawi b³êdy "nierozpoznany symbol zewnêtrzny"
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Naprawa ostrze¿enia /EHsc (obs³uga wyj¹tków)
if(MSVC)
    add_compile_options(/EHsc)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. ZnajdŸ Qt6 (Skonfigurowaliœmy to wczeœniej w VS)
find_package(Qt6 COMPONENTS Widgets REQUIRED)

# 2. Automatyczne pobieranie biblioteki CURL (Rozwi¹zuje Twój b³¹d)
include(FetchContent)
FetchContent_Declare(
  curl
  URL https://github.com/curl/curl/releases/download/curl-8_4_0/curl-8.4.0.tar.gz
)

# Wy³¹czamy zbêdne funkcje CURL, ¿eby kompilacja by³a szybka i bezproblemowa
set(BUILD_CURL_EXE OFF CACHE INTERNAL "")
set(BUILD_TESTING OFF CACHE INTERNAL "")
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

# Pliki Ÿród³owe
set(SOURCES
    src/main.cpp
    src/MainWindow.cpp
    src/OllamaClient.cpp
    src/CommandExecutor.cpp
    src/Logger.cpp
)

# Pliki nag³ówkowe
set(HEADERS
    src/MainWindow.h
    src/OllamaClient.h
    src/CommandExecutor.h
    src/Logger.h
)

# Tworzenie pliku wykonywalnego
qt_add_executable(DevOpsBotAI WIN32 ${SOURCES} ${HEADERS})
# Linkowanie bibliotek
# Uwaga: dla FetchContent CURL u¿ywamy nazwy celu 'libcurl'
target_link_libraries(DevOpsBotAI PRIVATE
    Qt6::Widgets
    libcurl
    nlohmann_json::nlohmann_json
)

# Opcjonalne: Testy jednostkowe (mo¿esz odkomentowaæ póŸniej)
# enable_testing()
# add_subdirectory(tests)