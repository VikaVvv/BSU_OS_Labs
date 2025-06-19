add_executable(marker_threads
    main.cpp
    marker_thread.cpp
)

target_include_directories(marker_threads PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Для MacOS добавляем дополнительные флаги
if(APPLE)
    target_link_options(marker_threads PRIVATE
        -Wl,-rpath,@loader_path
    )
endif()