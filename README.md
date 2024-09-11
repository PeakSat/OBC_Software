# PeakSat OBC Software

This repo hosts the software that will be running on the PeakSat satellite.
------------
Initial setup was done using the [minimal-dev-setup](https://github.com/PeakSat/minimal-dev-setup/tree/master "minimal-dev-setup").

------------

### Build and Upload software
Make sure you have the following tools installed and on your PATH

| **Tool**                              | **Version** |
|---------------------------------------|-------------|
| ARM GCC (arm-none-eabi-gcc --version) | 13.3        |
| CMake   (cmake --version)             | 3.30.1      |
| Ninja   (ninja --version)             | 1.12.1      |
| Conan   (conan -v)                    | 2.5.0       |
| OpenOCD (openocd -v)                  | 0.12.0      |
| Adoptium OpenJDK (java -version)      | >= 8        |

###Step 1: Clone the repo
###Step 2: Execute the following commands
```shell
conan config install ./config/
```
This will add a new Conan profile, named `baremetal-samv71-armv7`. The profile
includes global compiler and CMake settings necessary for building working code
for the SAMV71 platform.
```shell
conan create ./conan/recipes/<package-name> -pr=baremetal-samv71-armv7
```
where `package-name` is the package's directory name. The correct order for
installation (higher on the list = first):
```
cmsis
samv71-dfp
harmony-csp
harmony-mhc
harmony
```
Deviating from this order will just cause Conan to throw an error of missing
dependencies.
###Step 3: Build
```shell
conan build . -pr=baremetal-samv71-armv7
```
The resulting binary should be at `build/Debug/OBC_software.elf`
###Step 4: Upload the software
```shell
openocd -f  "atmel_samv71_xplained_ultra.cfg" -c "program build/Debug/OBC_software.elf reset"
```
###Attaching a debugger
OpenOCD automatically opens a debug server to which you can connect using
GDB if needed, as follows:
```shell
arm-none-eabi-gdb /path/to/compiled/binary_with_symbols.elf
```
Within GDB, run the following command to connect:
`target extended-remote localhost:3333`

------------
###VS Code Integration
Instructions to build and debug the project using the VSCode IDE. Complete steps 1-3 from the previous section and continue with the following.
###Step 1: Install VS Code 
[Visual Studio Code](https://code.visualstudio.com/ "Visual Studio Code")
###Step 2: Install necessary plugins
```
Cortex-Debug
CMake
CMake Tools
```
Using the side panel on the IDE, make the project, build and upload it using an STLink.
