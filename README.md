# Event Management System

A Windows Forms application built with C++/CLI (.NET) for managing events.

## Features

- Browse upcoming events with search and category filtering
- View full event details (organizer, location, date/time, capacity, description)
- Register and cancel registration for events
- Save / unsave events for later
- "My Events" page with tabs for registered and saved events
- Create new events as an organizer
- Persistent storage — event and session data saved to `%APPDATA%\EventApp\`
- Preloaded sample events across Music, Sports, Tech, Art, Food, and Academic categories

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
| `EventClass.h` | Event data model (`EventClass`, `EventCategories`) |
| `UserSession.h` | Tracks the attendee's registered and saved event IDs |
| `AppState.h` | Static singletons: `AppState::Manager` and `AppState::Session` |
| `EventManager.h` | Event management logic (CRUD, search, filter, register, save) |
| `SeedData.h` | 8 hardcoded sample events loaded on first run |
| `PersistenceService.h` | Load/save events and session to pipe-delimited text files |
| `StartingPage.h / .cpp` | Application entry / home screen |
| `EventsPage.h / .cpp` | Browse events with search, category filter, and card layout |
| `EventDetailsPage.h` | Full event detail page with Register / Save actions |
| `MyEventsPage.h` | Attendee page with Registered and Saved Events tabs |
