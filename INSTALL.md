# Installation Guide

[Windows](#install-steps-windows)

[WSL](#install-steps-wsl-ubuntu)

[Linux](#install-steps-linux)

[Visual Studio Code](#visual-studio-code-setup)

## Install Steps (Windows)

### Install GCC Compiler

Follow the [Visual Studio Code C++ Guide](https://code.visualstudio.com/docs/languages/cpp) to install the required features for C++ development, make sure to use the minGW-x64 compiler platform to be able to use GCC.

### Install CMake For Windows

Install the latest CMake release. Note that other versions may be required to proved functionality support as CMake attempts to be backwards compatible but is not perfect.

## Install Steps (WSL Ubuntu)

### Install Build Essentials (G++ / GCC)

All required packages to build and use Ocean as it is a C++ engine with a CMake build system.

```bash
sudo apt update && sudo apt install build-essential
sudo apt install gdb
sudo apt install clang
sudo apt install cmake
sudo apt install ninja-build
sudo apt install git
```

### Install Vulkan Drivers For Linux

#### AMD

It’s best to enable a PPA for the latest Mesa drivers. There is a PPA that packages and releases the latest changes straight from Mesa’s Git. Add the PPA to your system and update. Then, upgrade your system. It will automatically upgrade your existing Mesa packages.

```bash
sudo add-apt-repository ppa:oibaf/graphics-drivers
sudo apt update
sudo apt upgrade
```

When it’s done, install the Vulkan packages.

```bash
sudo apt install libvulkan1 mesa-vulkan-drivers
```

#### NVIDIA

Ubuntu also has a great repository for the NVIDIA proprietary drivers. Add it to your system, and update Apt.

```bash
sudo add-apt-repository ppa:graphics-drivers/ppa
sudo apt upgrade
```

Now, install your drivers and Vulkan.

```bash
sudo apt install nvidia-graphics-drivers-396 nvidia-settings vulkan
```

### Install The Vulkan SDK

#### Ubuntu 24.04 (Noble)

```bash
wget -qO - https://packages.lunarg.com/lunarg-signing-key-pub.asc | sudo apt-key add -

sudo wget -qO /etc/apt/sources.list.d/lunarg-vulkan-1.3.296-noble.list https://packages.lunarg.com/vulkan/1.3.296/lunarg-vulkan-1.3.296-noble.list

sudo apt update
sudo apt install vulkan-sdk
```

#### Ubuntu 22.04 (Jammy)

```bash
wget -qO- https://packages.lunarg.com/lunarg-signing-key-pub.asc | sudo tee /etc/apt/trusted.gpg.d/lunarg.asc

sudo wget -qO /etc/apt/sources.list.d/lunarg-vulkan-1.3.296-jammy.list https://packages.lunarg.com/vulkan/1.3.296/lunarg-vulkan-1.3.296-jammy.list

sudo apt update
sudo apt install vulkan-sdk
```

### Verify Vulkan Installation

Verify the installation of the Vulkan SDK (and the drivers) by running:

#### _Vulkan Installation Analyzer (VIA)_

```bash
vkvia
```

#### _Vulkan Info_

```bash
vulkaninfo
```

#### _Vulkan Cube_

```bash
vkcube
```

## Install Steps (Linux)

TODO

## Visual Studio Code Setup

Once complete with Ubuntu package setup, run VS Code with the following command.

```bash
cd ./directory/Ocean
code ./
```

### Visual Studio Code Extensions

Depending on running in WSL or on a dedicated Linux system, the following extensions are needed or recommended for development in Ocean.

| Extension                                                                                                          | Required |
| ------------------------------------------------------------------------------------------------------------------ | -------- |
| [C/C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)                                    | Yes      |
| [clangd](https://marketplace.visualstudio.com/items?itemName=llvm-vs-code-extensions.vscode-clangd)                | No       |
| [CMake Integration](https://marketplace.visualstudio.com/items?itemName=go2sh.cmake-integration-vscode)            | Yes      |
| [CMake Language Support](https://marketplace.visualstudio.com/items?itemName=josetr.cmake-language-support-vscode) | No       |
| [CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools)                           | No       |
| [Shader Language Support](https://marketplace.visualstudio.com/items?itemName=slevesque.shader)                    | No       |

Note for the clangd extension, the Microsoft C/C++ intellisense will need to be disabled to prevent conflicts.
