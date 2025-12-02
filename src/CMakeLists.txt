# mygame/src/CMakeLists.txt

# Lista de fontes do projeto
set(MYGAME_SOURCES
    main.cpp
)

# Arquivo fonte do glad
add_library(glad STATIC ${CMAKE_SOURCE_DIR}/third-party/glad/src/glad.c)

target_include_directories(glad
    PUBLIC
        ${CMAKE_SOURCE_DIR}/third-party/glad/include
)

# Add Headers
#set(MYGAME_HEADERS
#    ${CMAKE_SOURCE_DIR}/include/mygame/Game.h
#    ${CMAKE_SOURCE_DIR}/include/mygame/Renderer.h
#)

add_executable(opengl
    ${MYGAME_SOURCES}
#    ${MYGAME_HEADERS}
)

target_link_libraries(opengl
    PRIVATE
        SDL3::SDL3
        OpenGL::GL
        glad
)

target_include_directories(opengl
    PRIVATE
        #${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/third-party/glad/include
)
