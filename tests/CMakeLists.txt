# 1. Pobieranie GoogleTest (GTest) z sieci
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
# Zapobieganie konfliktom bibliotek na Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# 2. Definicja pliku wykonywalnego testów
# Musimy dołączyć test_command_executor.cpp ORAZ plik źródłowy, który testujemy
add_executable(unit_tests 
    test_command_executor.cpp 
    ../src/CommandExecutor.cpp
    ../src/Logger.cpp
)

# 3. Linkowanie bibliotek
# Dodajemy GTest oraz niezbędne nagłówki
target_link_libraries(unit_tests 
    PRIVATE 
    GTest::gtest
)

# Pozwalamy testom widzieć pliki nagłówkowe z folderu src
target_include_directories(unit_tests PRIVATE ../src)

# 4. REJESTRACJA TESTÓW W CTEST (Kluczowe!)
# Bez tej linijki ctest zawsze powie "No tests found"
add_test(NAME CommandExecutorTests COMMAND unit_tests)
