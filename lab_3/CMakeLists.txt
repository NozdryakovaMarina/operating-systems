cmake_minimum_required(VERSION 3.10)
project(CowDrawing LANGUAGES C)

# Настройка стандарта C
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Поиск необходимых библиотек
find_package(X11 REQUIRED)

# Добавление исполняемого файла
add_executable(main
    main.c
)

# Подключение библиотек
target_link_libraries(main
    ${X11_LIBRARIES}
)

# Установка путей к заголовочным файлам
target_include_directories(main PRIVATE
    ${X11_INCLUDE_DIR}
)

# Установка дополнительных флагов компиляции
if(CMAKE_COMPILER_IS_GNUCC)
    target_compile_options(main PRIVATE -Wall -Wextra)
endif()