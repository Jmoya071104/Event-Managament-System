#pragma once
#include "EventManager.h"
#include "UserSession.h"
#include "EventClass.h"

using namespace System;
using namespace System::IO;
using namespace System::Windows::Forms;
using namespace System::Collections::Generic;

// Saves and loads app state to/from %APPDATA%\EventApp\.
// Format: pipe-delimited plain text — no external libs required.
// Load/Save failures are caught; on load failure seed data is used instead.
public ref class PersistenceService
{
private:
    static String^ AppFolder = Path::Combine(
        Environment::GetFolderPath(Environment::SpecialFolder::ApplicationData),
        L"EventApp");

    static String^ EventsFile  = Path::Combine(AppFolder, L"events.txt");
    static String^ SessionFile = Path::Combine(AppFolder, L"session.txt");

    // Date format used for serialization
    static String^ DateFmt = L"yyyy-MM-dd HH:mm";

    static String^ Escape(String^ s)
    {
        // Replace pipe characters in field values so they don't break parsing
        return s->Replace(L"|", L"[pipe]")->Replace(L"\r\n", L" ")->Replace(L"\n", L" ");
    }

    static String^ Unescape(String^ s)
    {
        return s->Replace(L"[pipe]", L"|");
    }

public:
    // Returns true if events.txt existed and was loaded successfully.
    // Returns false if file did not exist (caller should seed).
    // Throws nothing — errors shown as non-fatal MessageBox.
    static bool LoadEvents(EventManager^ manager)
    {
        if (!File::Exists(EventsFile))
            return false;

        try
        {
            StreamReader^ reader = gcnew StreamReader(EventsFile);
            String^ line;
            while ((line = reader->ReadLine()) != nullptr)
            {
                if (String::IsNullOrWhiteSpace(line)) continue;
                array<String^>^ parts = line->Split(L'|');
                if (parts->Length < 11) continue;

                int      id              = Int32::Parse(parts[0]);
                String^  name            = Unescape(parts[1]);
                String^  runner          = Unescape(parts[2]);
                String^  location        = Unescape(parts[3]);
                String^  description     = Unescape(parts[4]);
                String^  category        = Unescape(parts[5]);
                DateTime start           = DateTime::ParseExact(parts[6], DateFmt, nullptr);
                DateTime end             = DateTime::ParseExact(parts[7], DateFmt, nullptr);
                int      capacity        = Int32::Parse(parts[8]);
                int      registeredCount = Int32::Parse(parts[9]);
                String^  imagePath       = Unescape(parts[10]);

                System::Drawing::Image^ icon = nullptr;
                if (!String::IsNullOrEmpty(imagePath) && File::Exists(imagePath))
                {
                    try { icon = System::Drawing::Image::FromFile(imagePath); }
                    catch (...) { icon = nullptr; }
                }

                EventClass^ ev = EventClass::Restore(
                    id, name, runner, location, description, category,
                    start, end, capacity, registeredCount, imagePath, icon);
                manager->events->Add(ev);
            }
            reader->Close();
            manager->SortEventsByStartDate();
            return true;
        }
        catch (Exception^ ex)
        {
            MessageBox::Show(
                L"Could not load saved events: " + ex->Message + L"\nStarting with sample data.",
                L"Load Warning", MessageBoxButtons::OK, MessageBoxIcon::Warning);
            return false;
        }
    }

    static void LoadSession(UserSession^ session)
    {
        if (!File::Exists(SessionFile)) return;

        try
        {
            StreamReader^ reader = gcnew StreamReader(SessionFile);

            // Line 1: registered IDs
            String^ regLine = reader->ReadLine();
            if (!String::IsNullOrWhiteSpace(regLine))
            {
                for each (String^ token in regLine->Split(L','))
                {
                    int id;
                    if (Int32::TryParse(token->Trim(), id))
                        session->Register(id);
                }
            }

            // Line 2: saved IDs
            String^ savLine = reader->ReadLine();
            if (!String::IsNullOrWhiteSpace(savLine))
            {
                for each (String^ token in savLine->Split(L','))
                {
                    int id;
                    if (Int32::TryParse(token->Trim(), id))
                        session->Save(id);
                }
            }
            reader->Close();
        }
        catch (Exception^)
        {
            // Non-fatal: session simply starts empty
        }
    }

    static void SaveEvents(EventManager^ manager)
    {
        try
        {
            if (!Directory::Exists(AppFolder))
                Directory::CreateDirectory(AppFolder);

            StreamWriter^ writer = gcnew StreamWriter(EventsFile, false);
            for each (EventClass^ ev in manager->events)
            {
                writer->WriteLine(
                    ev->Id + L"|" +
                    Escape(ev->Name) + L"|" +
                    Escape(ev->Runner) + L"|" +
                    Escape(ev->Location) + L"|" +
                    Escape(ev->Description) + L"|" +
                    Escape(ev->Category) + L"|" +
                    ev->StartDateTime.ToString(DateFmt) + L"|" +
                    ev->EndDateTime.ToString(DateFmt) + L"|" +
                    ev->Capacity + L"|" +
                    ev->RegisteredCount + L"|" +
                    Escape(ev->ImagePath));
            }
            writer->Close();
        }
        catch (Exception^ ex)
        {
            MessageBox::Show(
                L"Could not save events: " + ex->Message,
                L"Save Warning", MessageBoxButtons::OK, MessageBoxIcon::Warning);
        }
    }

    static void SaveSession(UserSession^ session)
    {
        try
        {
            if (!Directory::Exists(AppFolder))
                Directory::CreateDirectory(AppFolder);

            StreamWriter^ writer = gcnew StreamWriter(SessionFile, false);

            // Line 1: registered IDs
            List<String^>^ regTokens = gcnew List<String^>();
            for each (int id in session->RegisteredIds)
                regTokens->Add(id.ToString());
            writer->WriteLine(String::Join(L",", regTokens->ToArray()));

            // Line 2: saved IDs
            List<String^>^ savTokens = gcnew List<String^>();
            for each (int id in session->SavedIds)
                savTokens->Add(id.ToString());
            writer->WriteLine(String::Join(L",", savTokens->ToArray()));

            writer->Close();
        }
        catch (Exception^ ex)
        {
            MessageBox::Show(
                L"Could not save session: " + ex->Message,
                L"Save Warning", MessageBoxButtons::OK, MessageBoxIcon::Warning);
        }
    }

    // Convenience: save both events and session in one call
    static void Save(EventManager^ manager, UserSession^ session)
    {
        SaveEvents(manager);
        SaveSession(session);
    }

    // Convenience: load both; returns false if events file was missing (seed needed)
    static bool Load(EventManager^ manager, UserSession^ session)
    {
        bool hadEvents = LoadEvents(manager);
        LoadSession(session);
        return hadEvents;
    }
};
