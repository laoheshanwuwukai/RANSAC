cmake_minimum_required(VERSION 2.10)

project(RANSAC)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
)

set(RANSAC_EXAMPLE_HDRS
    include/lineParameterEstimator.h
    include/ParameterEstimator.h
    include/Point2D.h
    include/RANSAC.h)
    
set(RANSAC_EXAMPLE_SRCS
    main.cpp
    src/lineParameterEstimator.cpp
    src/RANSAC.cpp)

add_executable(
    RANSAC
    ${RANSAC_EXAMPLE_HDRS}
    ${RANSAC_EXAMPLE_SRCS}
)


