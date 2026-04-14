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
        topBar->Height    = 62;
        topBar->BackColor = Color::FromArgb(24, 28, 58);
        this->BackColor   = Color::FromArgb(245, 246, 250);
        this->Controls->Add(topBar);

        Button^ backBtn   = gcnew Button();
        backBtn->Text      = L"Back";
        backBtn->Location  = Point(10, 14);
        backBtn->Size      = System::Drawing::Size(80, 34);
        backBtn->ForeColor = Color::White;
        backBtn->FlatStyle = FlatStyle::Flat;
        backBtn->FlatAppearance->BorderColor = Color::FromArgb(80, 85, 130);
        backBtn->Click    += gcnew System::EventHandler(this, &OrganizerDashboardPage::BackBtn_Click);
        topBar->Controls->Add(backBtn);

        Label^ titleLbl   = gcnew Label();
        titleLbl->Text      = L"Organizer Dashboard";
        titleLbl->Font      = gcnew System::Drawing::Font(L"Segoe UI", 15, FontStyle::Bold);
        titleLbl->ForeColor = Color::White;
        titleLbl->TextAlign = ContentAlignment::MiddleLeft;
        titleLbl->Location  = Point(100, 0);
        titleLbl->Size      = System::Drawing::Size(W - 320, 62);
        titleLbl->AutoSize  = false;
        topBar->Controls->Add(titleLbl);

        Button^ newEventBtn   = gcnew Button();
        newEventBtn->Text      = L"+ Create New Event";
        newEventBtn->Size      = System::Drawing::Size(168, 34);
        newEventBtn->Location  = Point(W - 186, 14);
        newEventBtn->BackColor = Color::FromArgb(25, 135, 84);
        newEventBtn->ForeColor = Color::White;
        newEventBtn->FlatStyle = FlatStyle::Flat;
        newEventBtn->FlatAppearance->BorderSize = 0;
        newEventBtn->Anchor    = static_cast<AnchorStyles>(AnchorStyles::Top | AnchorStyles::Right);
        newEventBtn->Click    += gcnew System::EventHandler(this, &OrganizerDashboardPage::CreateEvent_Click);
        topBar->Controls->Add(newEventBtn);

        // ── Stats bar ──────────────────────────────────────────────────────
        Panel^ statsBar = gcnew Panel();
        statsBar->Dock      = DockStyle::Top;
        statsBar->Height    = 84;
        statsBar->BackColor = Color::FromArgb(248, 249, 252);
        statsBar->Padding   = System::Windows::Forms::Padding(10, 10, 10, 10);
        this->Controls->Add(statsBar);

        _statsTotalEvents    = MakeStatCard(statsBar, L"Total Events",    L"0", Color::FromArgb(67, 97, 238),  10);
        _statsTotalAttendees = MakeStatCard(statsBar, L"Total Attendees", L"0", Color::FromArgb(25, 135, 84),  220);
        _statsUpcoming       = MakeStatCard(statsBar, L"Upcoming",        L"0", Color::FromArgb(220, 140, 0),  430);

        // ── Scrollable event list ──────────────────────────────────────────
        Panel^ listContainer = gcnew Panel();
        listContainer->Dock        = DockStyle::Fill;
        listContainer->AutoScroll  = true;
        listContainer->Padding     = System::Windows::Forms::Padding(12, 8, 12, 8);
        this->Controls->Add(listContainer);

        _eventsFLP               = gcnew FlowLayoutPanel();
        _eventsFLP->Dock         = DockStyle::Fill;
        _eventsFLP->AutoScroll   = true;
        _eventsFLP->FlowDirection = FlowDirection::TopDown;
        _eventsFLP->WrapContents  = false;
        _eventsFLP->Padding       = System::Windows::Forms::Padding(4);
        listContainer->Controls->Add(_eventsFLP);

        listContainer->BringToFront();
    }

    Label^ MakeStatCard(Panel^ parent, String^ label, String^ valueText, Color accent, int x)
    {
        Panel^ card    = gcnew Panel();
        card->Location = Point(x, 8);
        card->Size     = System::Drawing::Size(190, 64);
        card->BackColor = Color::White;
        card->BorderStyle = BorderStyle::FixedSingle;
        parent->Controls->Add(card);

        Panel^ accentBar = gcnew Panel();
        accentBar->Location = Point(0, 0);
        accentBar->Size     = System::Drawing::Size(5, 64);
        accentBar->BackColor = accent;
        card->Controls->Add(accentBar);

        Label^ valLbl    = gcnew Label();
        valLbl->Text      = valueText;
        valLbl->Font      = gcnew System::Drawing::Font(L"Segoe UI", 18, FontStyle::Bold);
        valLbl->ForeColor = accent;
        valLbl->Location  = Point(14, 8);
        valLbl->Size      = System::Drawing::Size(160, 30);
        card->Controls->Add(valLbl);

        Label^ nameLbl    = gcnew Label();
        nameLbl->Text      = label;
        nameLbl->Font      = gcnew System::Drawing::Font(L"Segoe UI", 9);
        nameLbl->ForeColor = Color::FromArgb(110, 110, 140);
        nameLbl->Location  = Point(14, 40);
        nameLbl->Size      = System::Drawing::Size(160, 18);
        card->Controls->Add(nameLbl);

        return valLbl;  // returned so RefreshList can update the number
    }

    Label^ MakeStatLabel(Panel^ parent, String^ text, int x)
    {
        // Legacy: kept for binary compat; use MakeStatCard instead
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
        // Update stat cards
        _statsTotalEvents->Text    = AppState::Manager->GetTotalEvents().ToString();
        _statsTotalAttendees->Text = AppState::Manager->GetTotalAttendees().ToString();
        _statsUpcoming->Text       = AppState::Manager->GetUpcomingEventCount().ToString();

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
            empty->ForeColor = Color::FromArgb(150, 152, 180);
            empty->AutoSize  = true;
            empty->Margin    = System::Windows::Forms::Padding(20);
            _eventsFLP->Controls->Add(empty);
        }
    }

    Panel^ BuildEventRow(EventClass^ ev, int rowW)
    {
        Panel^ row    = gcnew Panel();
        row->Size      = System::Drawing::Size(rowW, 86);
        row->Margin    = System::Windows::Forms::Padding(0, 0, 0, 8);
        row->BackColor = Color::White;
        row->BorderStyle = BorderStyle::FixedSingle;
        row->Tag       = ev->Id;

        // Left category stripe
        Panel^ stripe    = gcnew Panel();
        stripe->Size     = System::Drawing::Size(5, 86);
        stripe->Location = Point(0, 0);
        stripe->BackColor = EventDetailsPage::CategoryColor(ev->Category);
        row->Controls->Add(stripe);

        // Thumbnail
        PictureBox^ thumb  = gcnew PictureBox();
        thumb->Size         = System::Drawing::Size(70, 70);
        thumb->Location     = Point(14, 8);
        thumb->SizeMode     = PictureBoxSizeMode::Zoom;
        if (ev->Icon != nullptr)
            thumb->Image = ev->Icon;
        else
            thumb->BackColor = Color::FromArgb(228, 230, 240);
        row->Controls->Add(thumb);

        // ── Center: name + meta ──────────────────────────────────────────
        Label^ nameLbl   = gcnew Label();
        nameLbl->Text     = ev->Name;
        nameLbl->Font     = gcnew System::Drawing::Font(L"Segoe UI", 11, FontStyle::Bold);
        nameLbl->ForeColor = Color::FromArgb(30, 30, 50);
        nameLbl->Location = Point(94, 10);
        nameLbl->Size     = System::Drawing::Size(rowW - 340, 26);
        nameLbl->AutoEllipsis = true;
        row->Controls->Add(nameLbl);

        String^ meta = ev->Category 
            + L" |  "
            + ev->StartDateTime.ToString(L"MMM dd, yyyy  h:mm tt")
            + L" | "
            + ev->RegisteredCount 
            + L" | "
            + (ev->Capacity == 0 ? L"unlimited" : ev->Capacity.ToString()) + L" registered";

        Label^ metaLbl   = gcnew Label();
        metaLbl->Text     = meta;
        metaLbl->Font     = gcnew System::Drawing::Font(L"Segoe UI", 9);
        metaLbl->ForeColor = Color::FromArgb(110, 110, 140);
        metaLbl->Location = Point(94, 40);
        metaLbl->Size     = System::Drawing::Size(rowW - 340, 22);
        metaLbl->AutoEllipsis = true;
        row->Controls->Add(metaLbl);

        // ── Right: action buttons ─────────────────────────────────────────
        int btnRight = rowW - 10;

        Button^ attendBtn   = MakeRowButton(L"Attendees", btnRight - 114, 25, Color::FromArgb(67, 97, 238));
        attendBtn->Tag       = ev->Id;
        attendBtn->Click    += gcnew System::EventHandler(this, &OrganizerDashboardPage::AttendeesBtn_Click);
        row->Controls->Add(attendBtn);

        Button^ editBtn     = MakeRowButton(L"Edit", btnRight - 228, 25, Color::FromArgb(220, 140, 0));
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
