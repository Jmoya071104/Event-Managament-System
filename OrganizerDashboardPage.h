#pragma once
#include "AppState.h"
#include "PersistenceService.h"
// EventEditorPage and AttendeeManagementPage are included after their definitions
// to avoid forward-declare issues — include order is managed by the .cpp files.

namespace GroupProject {

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;
using namespace System::Collections::Generic;

// Forward declarations — full headers included in the .cpp that instantiates these.
ref class EventEditorPage;
ref class AttendeeManagementPage;

public ref class OrganizerDashboardPage : public System::Windows::Forms::Form
{
private:
    Form^            _callerForm;
    FlowLayoutPanel^ _eventsFLP;
    Label^           _statsTotalEvents;
    Label^           _statsTotalAttendees;
    Label^           _statsUpcoming;

public:
    OrganizerDashboardPage(Form^ callerForm)
    {
        _callerForm = callerForm;
        InitializeComponent();
    }

protected:
    ~OrganizerDashboardPage()
    {
        if (components) delete components;
    }

private:
    System::ComponentModel::Container^ components;

    void InitializeComponent()
    {
        this->SuspendLayout();
        this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
        this->AutoScaleMode       = System::Windows::Forms::AutoScaleMode::Font;
        this->ClientSize          = System::Drawing::Size(1200, 800);
        this->Name                = L"OrganizerDashboardPage";
        this->Text                = L"Organizer Dashboard";
        this->Load               += gcnew System::EventHandler(this, &OrganizerDashboardPage::OnLoad);
        this->FormClosed         += gcnew FormClosedEventHandler(this, &OrganizerDashboardPage::OnFormClosed);
        this->ResumeLayout(false);
    }

    void OnLoad(System::Object^, System::EventArgs^)
    {
        this->WindowState = FormWindowState::Maximized;
        BuildUI();
        RefreshList();
    }

    void OnFormClosed(System::Object^, FormClosedEventArgs^)
    {
        if (_callerForm != nullptr)
            _callerForm->Show();
    }

    // -----------------------------------------------------------------------
    // UI construction
    // -----------------------------------------------------------------------
    void BuildUI()
    {
        int W = this->ClientSize.Width;
        int H = this->ClientSize.Height;

        // ── Top bar ────────────────────────────────────────────────────────
        Panel^ topBar = gcnew Panel();
        topBar->Dock      = DockStyle::Top;
        topBar->Height    = 60;
        topBar->BackColor = Color::FromArgb(30, 30, 60);
        this->Controls->Add(topBar);

        Button^ backBtn   = gcnew Button();
        backBtn->Text      = L"< Back";
        backBtn->Location  = Point(10, 13);
        backBtn->Size      = System::Drawing::Size(90, 34);
        backBtn->ForeColor = Color::White;
        backBtn->FlatStyle = FlatStyle::Flat;
        backBtn->Click    += gcnew System::EventHandler(this, &OrganizerDashboardPage::BackBtn_Click);
        topBar->Controls->Add(backBtn);

        Label^ titleLbl   = gcnew Label();
        titleLbl->Text      = L"Organizer Dashboard";
        titleLbl->Font      = gcnew System::Drawing::Font(L"Segoe UI", 16, FontStyle::Bold);
        titleLbl->ForeColor = Color::White;
        titleLbl->TextAlign = ContentAlignment::MiddleCenter;
        titleLbl->Location  = Point(110, 8);
        titleLbl->AutoSize  = true;
        topBar->Controls->Add(titleLbl);

        Button^ newEventBtn   = gcnew Button();
        newEventBtn->Text      = L"+ Create New Event";
        newEventBtn->Size      = System::Drawing::Size(170, 34);
        newEventBtn->Location  = Point(W - 190, 13);
        newEventBtn->BackColor = Color::FromArgb(0, 153, 76);
        newEventBtn->ForeColor = Color::White;
        newEventBtn->FlatStyle = FlatStyle::Flat;
        newEventBtn->Anchor    = static_cast<AnchorStyles>(AnchorStyles::Top | AnchorStyles::Right);
        newEventBtn->Click    += gcnew System::EventHandler(this, &OrganizerDashboardPage::CreateEvent_Click);
        topBar->Controls->Add(newEventBtn);

        // ── Stats bar ──────────────────────────────────────────────────────
        Panel^ statsBar = gcnew Panel();
        statsBar->Dock      = DockStyle::Top;
        statsBar->Height    = 70;
        statsBar->BackColor = Color::FromArgb(245, 245, 250);
        statsBar->Padding   = System::Windows::Forms::Padding(20, 10, 20, 10);
        this->Controls->Add(statsBar);

        _statsTotalEvents    = MakeStatLabel(statsBar, L"Total Events: 0",    20);
        _statsTotalAttendees = MakeStatLabel(statsBar, L"Total Attendees: 0", 220);
        _statsUpcoming       = MakeStatLabel(statsBar, L"Upcoming: 0",        440);

        // ── Scrollable event list ──────────────────────────────────────────
        Panel^ listContainer = gcnew Panel();
        listContainer->Dock        = DockStyle::Fill;
        listContainer->AutoScroll  = true;
        listContainer->Padding     = System::Windows::Forms::Padding(10);
        this->Controls->Add(listContainer);

        _eventsFLP               = gcnew FlowLayoutPanel();
        _eventsFLP->Dock         = DockStyle::Fill;
        _eventsFLP->AutoScroll   = true;
        _eventsFLP->FlowDirection = FlowDirection::TopDown;
        _eventsFLP->WrapContents  = false;
        _eventsFLP->Padding       = System::Windows::Forms::Padding(5);
        listContainer->Controls->Add(_eventsFLP);

        // Force paint order: stats before list
        listContainer->BringToFront();
    }

    Label^ MakeStatLabel(Panel^ parent, String^ text, int x)
    {
        Label^ lbl    = gcnew Label();
        lbl->Text      = text;
        lbl->Font      = gcnew System::Drawing::Font(L"Segoe UI", 12, FontStyle::Bold);
        lbl->ForeColor = Color::FromArgb(50, 50, 80);
        lbl->Location  = Point(x, 18);
        lbl->AutoSize  = true;
        parent->Controls->Add(lbl);
        return lbl;
    }

    // -----------------------------------------------------------------------
    // List population
    // -----------------------------------------------------------------------
    void RefreshList()
    {
        // Update stats
        _statsTotalEvents->Text    = L"Total Events: "    + AppState::Manager->GetTotalEvents();
        _statsTotalAttendees->Text = L"Total Attendees: " + AppState::Manager->GetTotalAttendees();
        _statsUpcoming->Text       = L"Upcoming: "        + AppState::Manager->GetUpcomingEventCount();

        _eventsFLP->Controls->Clear();

        List<EventClass^>^ allEvents = AppState::Manager->GetAllEventsSorted();
        int rowW = _eventsFLP->ClientSize.Width - 20;
        if (rowW < 600) rowW = 600;

        for each (EventClass^ ev in allEvents)
        {
            Panel^ row = BuildEventRow(ev, rowW);
            _eventsFLP->Controls->Add(row);
        }

        if (allEvents->Count == 0)
        {
            Label^ empty    = gcnew Label();
            empty->Text      = L"No events yet. Click \"+ Create New Event\" to get started.";
            empty->Font      = gcnew System::Drawing::Font(L"Segoe UI", 12);
            empty->ForeColor = Color::Gray;
            empty->AutoSize  = true;
            empty->Margin    = System::Windows::Forms::Padding(20);
            _eventsFLP->Controls->Add(empty);
        }
    }

    Panel^ BuildEventRow(EventClass^ ev, int rowW)
    {
        Panel^ row    = gcnew Panel();
        row->Size      = System::Drawing::Size(rowW, 80);
        row->Margin    = System::Windows::Forms::Padding(0, 0, 0, 6);
        row->BackColor = Color::White;
        row->BorderStyle = BorderStyle::FixedSingle;
        row->Tag       = ev->Id;

        // ── Left: thumbnail ──────────────────────────────────────────────
        PictureBox^ thumb  = gcnew PictureBox();
        thumb->Size         = System::Drawing::Size(70, 70);
        thumb->Location     = Point(5, 5);
        thumb->SizeMode     = PictureBoxSizeMode::Zoom;
        thumb->BorderStyle  = BorderStyle::FixedSingle;
        if (ev->Icon != nullptr)
            thumb->Image = ev->Icon;
        else
            thumb->BackColor = Color::LightGray;
        row->Controls->Add(thumb);

        // ── Center: name + meta ──────────────────────────────────────────
        Label^ nameLbl   = gcnew Label();
        nameLbl->Text     = ev->Name;
        nameLbl->Font     = gcnew System::Drawing::Font(L"Segoe UI", 11, FontStyle::Bold);
        nameLbl->Location = Point(85, 8);
        nameLbl->Size     = System::Drawing::Size(rowW - 340, 26);
        nameLbl->AutoEllipsis = true;
        row->Controls->Add(nameLbl);

        String^ meta = ev->Category + L"  |  "
            + ev->StartDateTime.ToString(L"MMM dd, yyyy  h:mm tt")
            + L"  |  "
            + ev->RegisteredCount + L" / "
            + (ev->Capacity == 0 ? L"∞" : ev->Capacity.ToString()) + L" registered";

        Label^ metaLbl   = gcnew Label();
        metaLbl->Text     = meta;
        metaLbl->Font     = gcnew System::Drawing::Font(L"Segoe UI", 9);
        metaLbl->ForeColor = Color::Gray;
        metaLbl->Location = Point(85, 36);
        metaLbl->Size     = System::Drawing::Size(rowW - 340, 22);
        metaLbl->AutoEllipsis = true;
        row->Controls->Add(metaLbl);

        // ── Right: action buttons ─────────────────────────────────────────
        int btnRight = rowW - 10;

        Button^ attendBtn   = MakeRowButton(L"Attendees", btnRight - 110, 22, Color::FromArgb(0, 102, 204));
        attendBtn->Tag       = ev->Id;
        attendBtn->Click    += gcnew System::EventHandler(this, &OrganizerDashboardPage::AttendeesBtn_Click);
        row->Controls->Add(attendBtn);

        Button^ editBtn     = MakeRowButton(L"Edit", btnRight - 220, 22, Color::FromArgb(204, 102, 0));
        editBtn->Tag         = ev->Id;
        editBtn->Click      += gcnew System::EventHandler(this, &OrganizerDashboardPage::EditBtn_Click);
        row->Controls->Add(editBtn);

        return row;
    }

    Button^ MakeRowButton(String^ text, int x, int y, Color bg)
    {
        Button^ b    = gcnew Button();
        b->Text       = text;
        b->Size       = System::Drawing::Size(100, 36);
        b->Location   = Point(x, y);
        b->BackColor  = bg;
        b->ForeColor  = Color::White;
        b->FlatStyle  = FlatStyle::Flat;
        b->FlatAppearance->BorderSize = 0;
        return b;
    }

    // -----------------------------------------------------------------------
    // Event handlers
    // -----------------------------------------------------------------------
    void BackBtn_Click(System::Object^, System::EventArgs^)
    {
        this->Close();
    }

    void CreateEvent_Click(System::Object^, System::EventArgs^);   // defined after EventEditorPage include

    void OnEditorClosed(System::Object^, FormClosedEventArgs^)
    {
        RefreshList(); // re-draw list so updated image/fields are visible immediately
    }

    void EditBtn_Click(System::Object^ sender, System::EventArgs^)
    {
        Button^ btn = safe_cast<Button^>(sender);
        int id      = safe_cast<int>(btn->Tag);
        OpenEditorForEvent(id);
    }

    void AttendeesBtn_Click(System::Object^ sender, System::EventArgs^)
    {
        Button^ btn = safe_cast<Button^>(sender);
        int id      = safe_cast<int>(btn->Tag);
        OpenAttendeeManager(id);
    }

    void OpenEditorForEvent(int eventId);     // body defined after EventEditorPage include
    void OpenAttendeeManager(int eventId);    // body defined after AttendeeManagementPage include
};

} // namespace GroupProject

// ── Inline bodies that depend on the full definitions of the other pages ────
// These are included here AFTER the class definition so the compiler sees
// OrganizerDashboardPage fully before EventEditorPage / AttendeeManagementPage
// try to use it.
#include "EventEditorPage.h"
#include "AttendeeManagementPage.h"

namespace GroupProject {

inline void OrganizerDashboardPage::CreateEvent_Click(System::Object^, System::EventArgs^)
{
    EventEditorPage^ editor = gcnew EventEditorPage(-1, this);  // -1 = create mode
    editor->FormClosed += gcnew FormClosedEventHandler(this, &OrganizerDashboardPage::OnEditorClosed);
    this->Hide();
    editor->Show();
}

inline void OrganizerDashboardPage::OpenEditorForEvent(int eventId)
{
    EventEditorPage^ editor = gcnew EventEditorPage(eventId, this);
    editor->FormClosed += gcnew FormClosedEventHandler(this, &OrganizerDashboardPage::OnEditorClosed);
    this->Hide();
    editor->Show();
}

inline void OrganizerDashboardPage::OpenAttendeeManager(int eventId)
{
    AttendeeManagementPage^ page = gcnew AttendeeManagementPage(eventId, this);
    page->FormClosed += gcnew FormClosedEventHandler(this, &OrganizerDashboardPage::OnEditorClosed);
    this->Hide();
    page->Show();
}

} // namespace GroupProject
