# Project to learn about 3D

## Linux install

The following packages are required:

```
sudo apt install libgl-dev libwayland-dev libxkbcommon-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev
```

## Windows Setup

Install chocolatey using a powerShell (admin):

```shell
Set-ExecutionPolicy Bypass -Scope Process -Force; [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))
```

Then install building tools:

```shell
choco install cmake.portable -y
choco install make ninja -y
choco install mingw -y
```

Using VS Code and Mingw, you will need to specify where to search for system headers. Set the following in your `settings.json` file:

```json
"clangd.arguments": [
    "-log=verbose",
    "-pretty",
    "--background-index",
    "--clang-tidy",
    "--compile-commands-dir=${workspaceFolder}/build/",
    "--query-driver=C:\\ProgramData\\mingw64\\mingw64\\bin\\g++.exe", // Replace with the path where g++ is installed
    "-header-insertion=never"
  ],
```

With the cmake extension installed for vscode: Open the Command Palette (Ctrl+Shift+P) and run CMake: Select a Kit. Then configure everything that you need using the settings.json file