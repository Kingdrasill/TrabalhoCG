# CMakeList.txt: projeto de CMake para TrabalhoFinal, incluir origem e definir
# lógica específica a um projeto aqui.
#
cmake_minimum_required(VERSION 3.12)

project(TrabalhoFinal)

add_executable(TrabalhoFinal "TrabalhoFinal.cpp"  "Arquivo.cpp" "Textura.cpp"   "dependencias/glad/src/glad.c" "Shader.h" "Camera.h" "Light.h" "Geometria.h" "Geometria.cpp" "Objetos.h")

target_include_directories(TrabalhoFinal PRIVATE dependencias/glm
												 dependencias/stb
												 dependencias/glfw/include
												 dependencias/glad/include
												 dependencias/glew/include
)

target_link_directories(TrabalhoFinal PRIVATE dependencias/glfw/lib-vc2022
											  dependencias/glew/lib/Release/x64
)

target_link_libraries(TrabalhoFinal PRIVATE glfw3.lib
											glew32.lib
											opengl32.lib
)

add_custom_command(TARGET TrabalhoFinal POST_BUILD
				   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/dependencias/glew/bin/Release/x64/glew32.dll" "${CMAKE_BINARY_DIR}"
)
# TODO: Adicione testes e instale destinos, se necessário.
