# PeakSat OBC Software

In this repository is where all of the OBC EQM Software is housed. This is the final version that will be used on the satellite.
The software runs on the Microchip ATSAMV71Q21B Microcontroller Unit, which is a 32-bit ARM Cortex-M7 core.

![alt text](https://github.com/PeakSat/OBC_Software/blob/main/readme_logo/peaksat_patch_full_2x.png?raw=true)

------------

## Build

The dependencies of this repository are managed through the [conan](https://conan.io/) package manager.
Make sure you have the following tools installed and on your PATH.

| **Tool**                              | **Version** |
|---------------------------------------|-------------|
| ARM GCC (arm-none-eabi-gcc --version) | 13.3        |
| CMake   (cmake --version)             | 3.30.1      |
| Ninja   (ninja --version)             | 1.12.1      |
| Conan   (conan -v)                    | 2.5.0       |
| OpenOCD (openocd -v)                  | 0.12.0      |

### Step 1: Clone the repo
   ```shell
   git clone git@gitlab.com:.git
   cd obc-eqm-software
   ```
### Step 2: Execute the following commands
1. (If you haven't already) create a conan profile for your system:
   ```shell
   conan profile detect
   ```
2. Download all dependencies and build the project through conan:
   ```shell
   conan install . --output-folder=cmake-build-debug --build="*" -u -pr conan-arm-profile
   ```
3. Download all submodules:
   ```shell
   conan source .
   ```
4. Add CMake flags:
   ```shell
   cmake -B cmake-build-debug/build/Debug -DCMAKE_TOOLCHAIN_FILE="cmake-build-debug/build/Debug/generators/conan_toolchain.cmake" -DCMAKE_CXX_COMPILER="arm-none-eabi-g++" -DCMAKE_C_COMPILER="arm-none-eabi-gcc" -DCMAKE_BUILD_TYPE=Debug .
   ```
5. Build the project:
   ```shell
   cd cmake-build-debug/build/Debug
   ninja
   ```
------------

## Uploading the software

After Building the software the resulting binary should be at `cmake-build-debug/build/Debug/OBC_software.elf`

Upload the software using the following command.
```shell
openocd -f  "atmel_samv71_xplained_ultra.cfg" -c "program cmake-build-debug/build/Debug/OBC_software.elf reset"
```
### Attaching a debugger
OpenOCD automatically opens a debug server to which you can connect using
GDB if needed, as follows:
```shell
arm-none-eabi-gdb /path/to/compiled/binary_with_symbols.elf
```
Within GDB, run the following command to connect:
`target extended-remote localhost:3333`

------------
