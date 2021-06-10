cmake_minimum_required(VERSION 3.19)
project(animated_skeleton)

set(CMAKE_CXX_STANDARD 14)

add_executable(animated_skeleton main.cpp Bone.cpp Bone.h Game.cpp Game.h Scene.cpp Scene.h Skeleton.cpp Skeleton.h Vertex.h)
target_link_libraries(animated_skeleton -lglut -lGL -lGLU -lGLEW -lglfw)