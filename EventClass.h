#pragma once
using namespace System;
using namespace System::Drawing;

// Valid category constants for filtering consistency
public ref class EventCategories {
public:
    static String^ Music    = L"Music";
    static String^ Sports   = L"Sports";
    static String^ Tech     = L"Tech";
    static String^ Art      = L"Art";
    static String^ Food     = L"Food";
    static String^ Academic = L"Academic";
    static String^ Other    = L"Other";
};

public ref class EventClass
{
private:
    // Auto-incrementing ID counter shared across all instances
    static int _nextId = 1;

public:
    int      Id;
    String^  Name;
    String^  Runner;       // Organizer name
    String^  Location;
    String^  Description;
    String^  Category;     // Use EventCategories constants
    DateTime StartDateTime;
    DateTime EndDateTime;
    int      Capacity;         // 0 = unlimited
    int      RegisteredCount;
    String^  ImagePath;        // File path for persistence; may be empty
    Image^   Icon;             // Loaded from ImagePath at runtime; may be nullptr

    // Full constructor used by SeedData and PersistenceService
    EventClass(
        String^ name,
        String^ runner,
        String^ location,
        String^ description,
        String^ category,
        DateTime start,
        DateTime end,
        int capacity,
        String^ imagePath,
        Image^  icon)
    {
        Id              = _nextId++;
        Name            = name;
        Runner          = runner;
        Location        = location;
        Description     = description;
        Category        = category;
        StartDateTime   = start;
        EndDateTime     = end;
        Capacity        = capacity;
        RegisteredCount = 0;
        ImagePath       = imagePath;
        Icon            = icon;
    }

    // Legacy constructor used by the existing Create Event panel (no category/capacity)
    // Category defaults to "Other", Capacity to 0 (unlimited)
    EventClass(
        String^  name,
        String^  runner,
        String^  location,
        String^  description,
        DateTime start,
        DateTime end,
        Image^   icon)
    {
        Id              = _nextId++;
        Name            = name;
        Runner          = runner;
        Location        = location;
        Description     = description;
        Category        = EventCategories::Other;
        StartDateTime   = start;
        EndDateTime     = end;
        Capacity        = 0;
        RegisteredCount = 0;
        ImagePath       = L"";
        Icon            = icon;
    }

    // Used by PersistenceService to restore an event with a known Id and RegisteredCount
    static EventClass^ Restore(
        int      id,
        String^  name,
        String^  runner,
        String^  location,
        String^  description,
        String^  category,
        DateTime start,
        DateTime end,
        int      capacity,
        int      registeredCount,
        String^  imagePath,
        Image^   icon)
    {
        EventClass^ ev = gcnew EventClass(name, runner, location, description,
                                          category, start, end, capacity, imagePath, icon);
        ev->Id              = id;
        ev->RegisteredCount = registeredCount;
        // Ensure the next auto-ID is always higher than any restored ID
        if (id >= _nextId) _nextId = id + 1;
        return ev;
    }
};

