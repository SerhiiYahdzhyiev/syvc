# SYVC - Serhii Yahdzhyiev's Version Control

**SYVC** is an educational project for university, providing a simplified Version Control System (VCS).

## Building the Project

### Windows

To build the project on Windows, follow these steps:

1. Ensure you have [CMake](https://cmake.org/download/) installed.
2. Open a command prompt and navigate to the root directory of the project.
3. Run the following commands:

    ```bash
    mkdir build
    cd build
    cmake ..
    nmake
    ```

### Unix-based OS (Linux, macOS)

To build the project on Unix-based operating systems, follow these steps:

1. Ensure you have [CMake](https://cmake.org/download/) installed.
2. Open a terminal and navigate to the root directory of the project.
3. Run the following commands:

    ```bash
    mkdir build
    cd build
    cmake ..
    make
    ```

## Usage

Once the project is built, you can use the executable named `syvc` from the command line. The executable provides a help command for detailed instructions:

```bash
./syvc help
```


### Adding to PATH

For convenient usage across the filesystem, you can add the path to the **syvc** executable to your **PATH** environment variable.

