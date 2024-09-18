# PeakSat OBC Software

This repo hosts the software that will be running on the PeakSat satellite.
![alt text](https://github.com/PeakSat/OBC_Software/blob/main/readme_logo/peaksat_patch_full_2x.png?raw=true)
------------
Initial setup was done using the [minimal-dev-setup](https://github.com/PeakSat/minimal-dev-setup/tree/master "minimal-dev-setup").

------------

## Build and Upload software
Make sure you have the following tools installed and on your PATH

| **Tool**                              | **Version** |
|---------------------------------------|-------------|
| ARM GCC (arm-none-eabi-gcc --version) | 13.3        |
| CMake   (cmake --version)             | 3.30.1      |
| Ninja   (ninja --version)             | 1.12.1      |
| Conan   (conan -v)                    | 2.5.0       |
| OpenOCD (openocd -v)                  | 0.12.0      |
| Adoptium OpenJDK (java -version)      | >= 8        |

### Step 1: Clone the repo
### Step 2: Execute the following commands
```shell
conan config install ./config/
```
This will add a new Conan profile, named `baremetal-samv71-armv7`. The profile
includes global compiler and CMake settings necessary for building working code
for the SAMV71 platform.
```shell
conan create ./recipes/<package-name> -pr=baremetal-samv71-armv7
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
### Step 3: Build
```shell
conan build . -pr=baremetal-samv71-armv7
```
The resulting binary should be at `build/Debug/OBC_software.elf`
### Step 4: Upload the software
```shell
openocd -f  "atmel_samv71_xplained_ultra.cfg" -c "program build/Debug/OBC_software.elf reset"
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
## VS Code Integration
Instructions to build and debug the project using the VSCode IDE. Complete steps 1-3 from the previous section and continue with the following.
### Step 1: Install VS Code 
[Visual Studio Code](https://code.visualstudio.com/ "Visual Studio Code")
### Step 2: Install necessary plugins
```
Cortex-Debug
CMake
CMake Tools
```
Using the side panel on the IDE, make the project, build and upload it using an STLink.

------------
## Configuring the MCU

The Harmony setup includes the Microchip Harmony Configurator (MHC) tool for
adjusting settings and generating peripheral drivers. **MHC needs Java**
installed and in your PATH to work. If your Java setting is incorrect, CMake
will notify you with an error.

To launch it, use
```shell
cd build/Debug \
ninja mcu_config
```

This MHC instance is under the control of CMake, and can only be used by issuing
this command. 

To create a new configuration, follow this workflow:
1. Click `File -> New Configuration`. Even if editing existing settings, always
create a new configuration.
2. No need to set Location/Project Name/Configuration Name, they don't really
matter in this case. 
3. Select `ATSAMV71Q21B` under `Target Device`. The supporting files for other
devices are missing and thus will not work. Make sure this is correct; otherwise
you have to start the process all over.
4. Click `Finish`. On the `Configuration Database Setup` click `Launch`.
5. Configure away.
6. Click `File -> Save Configuration` (the disk icon), then `File -> Export`. At
the Export dialog, check all checkboxes and specify a folder where you'll get
your settings. **Always save before exporting**, otherwise you won't get files
back.

To edit an existing configuration, follow Steps 1-4, then click `File -> Import`
and specify the directory of your exported configuration files. When done, save
and export back.

**CAUTION**: Do NOT use the code generation feature, it will not work and cause
MHC to hang. Generation will be done automatically by CMake whenever needed.

### Generating peripheral driver code

The Harmony package includes a helper script for MHC, which can be found under
`./conan/recipes/harmony/cmake/MHCHelper.cmake`. This script is implicitly
included after calling `find_package(Harmony)` and generates the code using
MHC at configure time.

In your CMakeLists.txt, add this:
```cmake
add_harmony_config(
        MCU_MODEL ATSAMV71Q21B
        YAML_FILES <list of MHC-exported yml files>
)
```

You can use the generated code by linking in your executable the targets:
- `Harmony::PeripheralDrivers`: generated peripheral drivers.
- `Harmony::Interrupts` : interrupt handler code
- `Harmony::SysInit`: the SYS_Initialize() function that does the clock init.
