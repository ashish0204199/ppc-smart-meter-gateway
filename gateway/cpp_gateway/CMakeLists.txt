cmake_minimum_required(VERSION 3.16)

project(MeterBridgeGateway LANGUAGES  CXX)

add_executable(meterbridge_gateway src/main.cpp src/telemetry_parser.cpp)
target_include_directories(meterbridge_gateway PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(meterbridge_gateway PRIVATE cxx_std_17)
