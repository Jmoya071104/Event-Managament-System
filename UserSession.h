#pragma once
using namespace System;
using namespace System::Collections::Generic;

// Tracks which events the current attendee has registered for or saved.
// This is the single source of truth for attendee state within a session.
public ref class UserSession
{
public:
    List<int>^ RegisteredIds;
    List<int>^ SavedIds;

    UserSession()
    {
        RegisteredIds = gcnew List<int>();
        SavedIds      = gcnew List<int>();
    }

    void Register(int id)           { RegisteredIds->Add(id); }
    void CancelRegistration(int id) { RegisteredIds->Remove(id); }
    void Save(int id)               { SavedIds->Add(id); }
    void Unsave(int id)             { SavedIds->Remove(id); }
    bool IsRegistered(int id)       { return RegisteredIds->Contains(id); }
    bool IsSaved(int id)            { return SavedIds->Contains(id); }
};
