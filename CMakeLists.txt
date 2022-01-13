cmake_minimum_required(VERSION 3.0)

project(ArrowMinimalExample)

# option(ARROW_LINK_SHARED "Link to the Arrow shared library" ON)

find_package(Arrow REQUIRED)

set(CMAKE_CXX_STANDARD 11)

message(STATUS "Arrow version: ${ARROW_VERSION}")
message(STATUS "Arrow SO version: ${ARROW_FULL_SO_VERSION}")

add_executable(arrow_example example.cc)

target_include_directories(arrow_example PUBLIC ${CMAKE_BINARY_DIR}/vcpkg_installed/${VCPKG_TARGET_TRIPLET}/include)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_link_directories(arrow_example PUBLIC ${CMAKE_BINARY_DIR}/vcpkg_installed/${VCPKG_TARGET_TRIPLET}/lib)
else()
  target_link_directories(arrow_example PUBLIC ${CMAKE_BINARY_DIR}/vcpkg_installed/${VCPKG_TARGET_TRIPLET}/debug/lib)
endif()

# if(ARROW_LINK_SHARED)
#   target_link_libraries(arrow_example PRIVATE arrow_shared)
# else()
  set(THREADS_PREFER_PTHREAD_FLAG ON)
  find_package(Threads REQUIRED)
  target_link_libraries(arrow_example PRIVATE arrow_static parquet brotlicommon brotlidec brotlienc lz4 thrift utf8proc Threads::Threads)
# endif()