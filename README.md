# Event Management System

A Windows Forms application built with C++/CLI (.NET) for managing events.

## Features

- Create and manage events with details such as name, date, location, and capacity
- Browse and search through existing events
- Simple graphical interface with a starting page and events page

## Requirements

- Windows 10 or later
- Visual Studio 2022 with the **Desktop development with C++** workload and **.NET desktop development** workload installed

## Build & Run

1. Open `GroupProject.sln` in Visual Studio 2022
2. Select the **Debug** or **Release** configuration and **x64** platform
3. Press **F5** to build and run

## Project Structure

| File | Description |
|---|---|
| `GroupProject.sln` | Visual Studio solution file |
| `GroupProject.vcxproj` | C++/CLI project file |
| `EventClass.h` | Event data model |
| `EventManager.h` | Event management logic |
| `StartingPage.h / .cpp` | Application entry/home screen |
| `EventsPage.h / .cpp` | Events listing and management screen |
