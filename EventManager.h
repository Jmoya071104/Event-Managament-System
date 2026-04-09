#pragma once
#include "EventClass.h"
#include "UserSession.h"

using namespace System;
using namespace System::Collections::Generic;

public ref class EventManager
{
public:
    List<EventClass^>^ events;

    EventManager()
    {
        events = gcnew List<EventClass^>();
    }

    // --- Basic management ---

    void addEvent(EventClass^ newEvent)
    {
        events->Add(newEvent);
        SortEventsByStartDate();
    }

    static int CompareByStartDate(EventClass^ a, EventClass^ b)
    {
        return DateTime::Compare(a->StartDateTime, b->StartDateTime);
    }

    void SortEventsByStartDate()
    {
        events->Sort(gcnew Comparison<EventClass^>(CompareByStartDate));
    }

    // --- Query methods ---

    List<EventClass^>^ GetAllEvents()
    {
        return events;
    }

    EventClass^ GetEventById(int id)
    {
        for each (EventClass^ ev in events)
        {
            if (ev->Id == id) return ev;
        }
        return nullptr;
    }

    // Case-insensitive search across Name, Runner, and Location
    List<EventClass^>^ SearchEvents(String^ query)
    {
        List<EventClass^>^ results = gcnew List<EventClass^>();
        if (String::IsNullOrWhiteSpace(query))
            return gcnew List<EventClass^>(events);

        String^ q = query->ToLower()->Trim();
        for each (EventClass^ ev in events)
        {
            if (ev->Name->ToLower()->Contains(q)
                || ev->Runner->ToLower()->Contains(q)
                || ev->Location->ToLower()->Contains(q))
            {
                results->Add(ev);
            }
        }
        return results;
    }

    // Returns matching events; "All" or empty string returns everything
    List<EventClass^>^ FilterByCategory(String^ category)
    {
        if (String::IsNullOrEmpty(category)
            || category == L"All"
            || category == L"All Categories")
        {
            return gcnew List<EventClass^>(events);
        }

        List<EventClass^>^ results = gcnew List<EventClass^>();
        for each (EventClass^ ev in events)
        {
            if (ev->Category == category)
                results->Add(ev);
        }
        return results;
    }

    // Search + category combined (both filters applied together)
    List<EventClass^>^ SearchAndFilter(String^ query, String^ category)
    {
        List<EventClass^>^ afterSearch   = SearchEvents(query);
        List<EventClass^>^ afterCategory = gcnew List<EventClass^>();

        bool allCats = String::IsNullOrEmpty(category)
                       || category == L"All"
                       || category == L"All Categories";

        for each (EventClass^ ev in afterSearch)
        {
            if (allCats || ev->Category == category)
                afterCategory->Add(ev);
        }
        return afterCategory;
    }

    // --- Attendee actions ---

    // Returns true on success; false if already registered or event is full
    bool RegisterForEvent(int id, UserSession^ session)
    {
        EventClass^ ev = GetEventById(id);
        if (ev == nullptr)                          return false;
        if (session->IsRegistered(id))              return false;
        if (ev->Capacity > 0 && ev->RegisteredCount >= ev->Capacity) return false;

        ev->RegisteredCount++;
        session->Register(id);
        return true;
    }

    // Returns true on success; false if not currently registered
    bool CancelRegistration(int id, UserSession^ session)
    {
        EventClass^ ev = GetEventById(id);
        if (ev == nullptr)                 return false;
        if (!session->IsRegistered(id))    return false;

        if (ev->RegisteredCount > 0) ev->RegisteredCount--;
        session->CancelRegistration(id);
        return true;
    }

    void SaveEvent(int id, UserSession^ session)
    {
        session->Save(id);
    }

    void UnsaveEvent(int id, UserSession^ session)
    {
        session->Unsave(id);
    }

    // Convenience: get only events the attendee has registered for
    List<EventClass^>^ GetRegisteredEvents(UserSession^ session)
    {
        List<EventClass^>^ results = gcnew List<EventClass^>();
        for each (EventClass^ ev in events)
        {
            if (session->IsRegistered(ev->Id))
                results->Add(ev);
        }
        return results;
    }

    // Convenience: get only events the attendee has saved
    List<EventClass^>^ GetSavedEvents(UserSession^ session)
    {
        List<EventClass^>^ results = gcnew List<EventClass^>();
        for each (EventClass^ ev in events)
        {
            if (session->IsSaved(ev->Id))
                results->Add(ev);
        }
        return results;
    }
};
