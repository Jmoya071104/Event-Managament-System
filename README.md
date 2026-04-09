# Event Management System

A Windows Forms application built with C++/CLI (.NET) for managing events, featuring a modern, visually consistent UI with dark navy top bars, light gray backgrounds, white content cards, and a cohesive blue-indigo accent color scheme.

## Features

### Attendee
- Browse upcoming events with search and category filtering
- View full event details (organizer, location, date/time, capacity, description)
- Register and cancel registration for events
- Save / unsave events for later
- "My Events" page with tabs for registered and saved events

### Organizer
- Organizer Dashboard with live stats (total events, attendees, upcoming count)
- Create new events with name, organizer, location, category, date/time, capacity, description, and image
- Edit existing events (all fields); changes reflect immediately in the dashboard
- Delete events with confirmation — automatically cleans up all registrations
- Per-event attendee management: searchable roster, check-in tracking, CSV export to desktop

### General
- Persistent storage — events and session data saved to `%APPDATA%\EventApp\`
- Preloaded sample events across Music, Sports, Tech, Art, Food, and Academic categories

## Requirements

- Windows 10 or later
- Visual Studio 2022 with the **Desktop development with C++** workload and **.NET desktop development** workload installed

## Build & Run

1. Open `GroupProject.sln` in Visual Studio 2022
2. Select the **Debug** or **Release** configuration and **x64** platform
3. Press **F5** to build and run

## UI Design

All pages share a unified visual system:

| Token | Color | Usage |
|---|---|---|
| `APP_BG` | `rgb(245, 246, 250)` | Form / page background |
| `TOPBAR` | `rgb(24, 28, 58)` | Dark navy top bar on every page |
| `ACCENT` | `rgb(67, 97, 238)` | Primary buttons, category stripes, stats |
| `DANGER` | `rgb(220, 53, 69)` | Destructive actions (cancel, delete) |
| `SUCCESS` | `rgb(25, 135, 84)` | Create / save-changes buttons |
| `GOLD` | `rgb(255, 193, 7)` | Saved / bookmarked state |
| `TEXT1` | `rgb(30, 30, 50)` | Primary text |
| `TEXT2` | `rgb(110, 110, 140)` | Secondary / meta text |

- **Font:** Segoe UI throughout (replacing all Microsoft Sans Serif)
- **Buttons:** `FlatStyle::Flat`, `BorderSize = 0` for primary actions; `BorderSize = 1` with a subtle border color for secondary actions
- **Cards:** White background, `BorderStyle::FixedSingle`, left 5 px colored category stripe

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
| `OrganizerDashboardPage.h` | Organizer hub: event list, stats, navigation to create/edit/attendees |
| `EventEditorPage.h` | Unified create and edit form for events (with delete in edit mode) |
| `AttendeeManagementPage.h` | Per-event attendee roster with search, check-in, and CSV export |
