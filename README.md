# CH32V003 SDK Template

## Overview
This repository provides a custom SDK template for the CH32V003 microcontroller series. It aims to offer a modern, CMake-based build system and a clean, well-structured codebase to simplify development and overcome the limitations of the official WCH SDK.

## Usage
1. Compile your own GCC toolchain with support for the `rv32ec` architecture, as most standard GCC distributions lack this support.
2. Configure and build the project using CMake by specifying your GCC toolchain path and prefix:
```bash
cmake -B build -DCOMPILER_PATH=<your_path> -DCOMPILER_PREFIX=<your_prefix> && make -C build/
```
3. Upload the compiled binary to your device using `wlink`:
```bash
wlink -v flash build/template.bin
```

## Why Use This Custom Template?

The official WCH SDK is limited in several ways:
- It lacks a CMakeLists.txt or Makefile, making it difficult to build with modern tools.
- Documentation and build instructions are sparse or missing.
- The SDK heavily mimics the STM32 HAL, which may not be ideal for all users.

This template addresses these issues by providing:
- A clean, modular codebase.
- A CMake-based build system for easy integration and customization.
- Clear instructions for building and flashing the firmware.

## Features
- Support for CH32V003 microcontrollers.
- Modular peripheral drivers.
- Example main application.
- Easy toolchain configuration.

## Getting Help
For issues or questions, please open an issue on the GitHub repository.

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
