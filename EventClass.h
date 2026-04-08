#pragma once
using namespace System;

public ref class EventClass
{
public:
    String^ Name;
    String^ Runner;
    String^ Location;
    String^ Description;
    DateTime StartDate;
    DateTime EndDate;
    System::Drawing::Image^ Icon;

    // Constructor
    EventClass(
        String^ name,
        String^ runner,
        String^ location,
        String^ description,
        DateTime start,
        DateTime end, System::Drawing::Image^ icon)
    {
        Name = name;
        Runner = runner;
        Location = location;
        Description = description;
        StartDate = start;
        EndDate = end;
        Icon = icon;
    }
};

