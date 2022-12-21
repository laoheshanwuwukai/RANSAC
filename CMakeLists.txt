cmake_minimum_required(VERSION 2.10)

project(RANSAC)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
)

add_executable(
    RANSAC
    main.cpp
    src/lineParameterEstimator.cpp
    src/RANSAC.cpp
)


