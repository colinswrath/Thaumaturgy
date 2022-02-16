This is a simple utility plugin for use with Thaumaturgy - An Enchanting Overhaul: https://www.nexusmods.com/skyrimspecialedition/mods/57138  

## What does it do?  

**Current functionality includes:**  
`Apply set mult to disenchanting XP` - Can be adjusted in the ini

## Requirements
* [CMake](https://cmake.org/)
	* Add this to your `PATH`
* [PowerShell](https://github.com/PowerShell/PowerShell/releases/latest)
* [Vcpkg](https://github.com/microsoft/vcpkg)
	* Add the environment variable `VCPKG_ROOT` with the value as the path to the folder containing vcpkg
* [Visual Studio Community 2019](https://visualstudio.microsoft.com/)
	* Desktop development with C++

## Register Visual Studio as a Generator
* Open `x64 Native Tools Command Prompt`
* Run `cmake`
* Close the cmd window

## Building
```
git clone https://github.com/colinswrath/Thaumaturgy.git
cd ExamplePlugin-CommonLibSSE
git submodule update --init --recursive
cmake -B build -S .
```
