cmake_minimum_required(VERSION 3.10)
project(translater_prj)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_path(BOOST_INCLUDE_DIR
        NAMES boost/beast.hpp
        PATHS /opt/homebrew/include
        /opt/homebrew/opt/boost/include
        /usr/local/include
        /usr/include
        REQUIRED
)

add_executable(translater_prj
        src/main.cpp
        src/iMacos.cpp
        src/ApiHandler.cpp
        include/ApiHandler.h
        src/MainHandler.cpp
        include/MainHandler.h
)

target_include_directories(translater_prj PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/libs
        ${BOOST_INCLUDE_DIR})

if(APPLE)
  find_package(OpenSSL REQUIRED)
  target_link_libraries(translater_prj
        "-framework CoreFoundation"
        "-framework CoreGraphics"
        "-framework ApplicationServices"
  )
  target_link_libraries(translater_prj
        ${Boost_LIBRARIES}
        OpenSSL::SSL
        OpenSSL::Crypto
  )
  target_compile_definitions(translater_prj PRIVATE
        BOOST_BEAST_USE_STD_ERIC=1
        BOOST_ASIO_USE_STD_ERIC=1
  )
endif()
