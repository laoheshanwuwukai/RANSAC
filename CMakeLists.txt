cmake_minimum_required(VERSION 2.10)

project(RANSAC)

include_directories(
    include
)

add_executable(
    RANSAC
    main.cpp
    src/lineParameterEstimator.cpp
    src/RANSAC.cpp
)


