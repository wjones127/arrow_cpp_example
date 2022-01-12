# Simple C++ Project Using Arrow

This uses vcpkg and CMake to make building a C++ project with Arrow easy.

## Prerequisites

 * CMake
 * Ninja

## How to build

```
git clone https://github.com/microsoft/vcpkg
cmake --preset vcpkg-ninja .
cmake --build out/build/vcpkg-ninja -j8
```