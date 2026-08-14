cmake_minimum_required(VERSION 3.16)

project(MeterBridgeGateway LANGUAGES  CXX)

add_executable(meterbridge_gateway
		src/main.cpp
)
target_compile_features(meterbridge_gateway PRIVATE cxx_std_17)
