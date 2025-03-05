# Visual Studio Setup Instructions

This document explains how to set up the dungeon generator project in Visual Studio.

## Creating a New Project

1. Open Visual Studio
2. Select `Create a new project`
3. Choose `Console App` from the C++ project templates
4. Name the project `DungeonGenerator` and select a location
5. Click `Create`

## Adding Existing Files

1. In Solution Explorer, right-click on the project (not the solution)
2. Select `Add` → `Existing Item...`
3. Navigate to where you saved the source files
4. Select all the `.cpp` and `.h` files
5. Click `Add`

## Setting Up the Project

1. Right-click on the project in Solution Explorer
2. Select `Properties`
3. Make sure `Configuration` is set to `All Configurations`
4. Under `C/C++` → `Language`, ensure:
   - `C++ Language Standard` is set to `C++17` or newer
   - `Conformance mode` is set to `Yes`
5. Click `Apply` then `OK`

## Building and Running

1. Select `Build` → `Build Solution` (or press F7)
2. If the build succeeds, select `Debug` → `Start Without Debugging` (or press Ctrl+F5)
3. The program will run and generate a dungeon.txt file

## Debugging

1. If you want to debug the application, you can set breakpoints by clicking in the left margin of code lines
2. Then select `Debug` → `Start Debugging` (or press F5)

## Command Line Arguments

To pass command line arguments to your application:

1. Right-click on the project in Solution Explorer
2. Select `Properties`
3. Go to `Debugging`
4. In the `Command Arguments` field, enter your arguments (e.g., `80 60` for an 80x60 dungeon)
5. Click `Apply` then `OK`

## Common Issues

1. **Compiler Errors**: Make sure you've set the C++ standard to C++17 or newer
2. **Linker Errors**: Ensure all source files (.cpp) are added to the project
3. **Runtime Errors**: Check file paths if the application can't write to disk
