cmake_minimum_required(VERSION 3.12) 

project(GeeksMessenger VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Ensures the compiler must support C++20
set(CMAKE_CXX_EXTENSIONS OFF)      # Disables compiler-specific extensions (optional, but recommended for strict C++20 compliance)

# Enable testing early
enable_testing()

# Add subdirectories
add_subdirectory(src)
add_subdirectory(tests)

# Ensure modern policy behavior
cmake_policy(SET CMP0167 NEW)

# Explicitly request Boost.System
find_package(Boost 1.83 REQUIRED COMPONENTS system)

# Include headers
include_directories(${Boost_INCLUDE_DIRS})

# CPack Configuration
set(CPACK_PACKAGE_NAME "geeks-messenger-client")
set(CPACK_PACKAGE_VENDOR "RIP-Skillan")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A C++ CLI Chat Client")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_CONTACT "maintainer@example.com")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE") # Ensure you have a LICENSE file or remove this

# Debian specific
set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "RIP-Skillan")

include(CPack)

