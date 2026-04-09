#pragma once
#include "AppState.h"
#include "PersistenceService.h"

namespace GroupProject {

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;
using namespace System::Collections::Generic;

// Shows a demo attendee roster generated from an event's RegisteredCount.
// Supports name/email search and per-row check-in toggle.
// "Export CSV" writes a file to the desktop.
public ref class AttendeeManagementPage : public System::Windows::Forms::Form
{
private:
    int    _eventId;
    Form^  _callerForm;

    // Generated demo attendee data
    ref struct Attendee {
        String^ Name;
        String^ Email;
        bool    CheckedIn;
        int     Index;
    };

    List<Attendee^>^  _attendees;
    List<Attendee^>^  _filtered;
    TextBox^          _searchTB;
    FlowLayoutPanel^  _listFLP;
    Label^            _statLbl;

public:
    AttendeeManagementPage(int eventId, Form^ callerForm)
    {
        _eventId    = eventId;
        _callerForm = callerForm;
        _attendees  = gcnew List<Attendee^>();
        _filtered   = gcnew List<Attendee^>();
        InitializeComponent();
    }

protected:
    ~AttendeeManagementPage()
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
        this->ClientSize          = System::Drawing::Size(900, 700);
        this->Name                = L"AttendeeManagementPage";
        this->Text                = L"Attendee Management";
        this->Load               += gcnew System::EventHandler(this, &AttendeeManagementPage::OnLoad);
        this->FormClosed         += gcnew FormClosedEventHandler(this, &AttendeeManagementPage::OnFormClosed);
        this->ResumeLayout(false);
    }

    void OnLoad(System::Object^, System::EventArgs^)
    {
        this->WindowState = FormWindowState::Maximized;
        GenerateAttendees();
        BuildUI();
        ApplyFilter(L"");
    }

    void OnFormClosed(System::Object^, FormClosedEventArgs^)
    {
        if (_callerForm != nullptr)
            _callerForm->Show();
    }

    // -----------------------------------------------------------------------
    // Generate demo attendees from RegisteredCount
    // -----------------------------------------------------------------------
    void GenerateAttendees()
    {
        EventClass^ ev = AppState::Manager->GetEventById(_eventId);
        if (ev == nullptr) return;

        // Deterministic demo names so the list looks realistic
        array<String^>^ firstNames = {
            L"Alice", L"Bob", L"Carlos", L"Diana", L"Ethan",
            L"Fiona", L"George", L"Hannah", L"Ivan", L"Julia",
            L"Kevin", L"Laura", L"Marcus", L"Nina", L"Oscar",
            L"Paula", L"Quinn", L"Rachel", L"Samuel", L"Tina"
        };
        array<String^>^ lastNames = {
            L"Smith", L"Johnson", L"Williams", L"Jones", L"Brown",
            L"Davis", L"Miller", L"Wilson", L"Moore", L"Taylor",
            L"Anderson", L"Thomas", L"Jackson", L"White", L"Harris",
            L"Martin", L"Thompson", L"Garcia", L"Martinez", L"Robinson"
        };
        array<String^>^ domains = {
            L"gmail.com", L"yahoo.com", L"outlook.com",
            L"hotmail.com", L"university.edu"
        };

        int count = ev->RegisteredCount;
        _attendees->Clear();

        for (int i = 0; i < count; i++)
        {
            Attendee^ a   = gcnew Attendee();
            a->Index       = i;
            a->Name        = firstNames[i % firstNames->Length]
                             + L" "
                             + lastNames[(i / firstNames->Length) % lastNames->Length];
            String^ suffix = (i > 0) ? i.ToString() : L"";
            a->Email       = (firstNames[i % firstNames->Length]->ToLower()
                             + L"."
                             + lastNames[(i / firstNames->Length) % lastNames->Length]->ToLower()
                             + suffix
                             + L"@"
                             + domains[i % domains->Length]);
            a->CheckedIn   = false;
            _attendees->Add(a);
        }
    }

    // -----------------------------------------------------------------------
    // UI construction
    // -----------------------------------------------------------------------
    void BuildUI()
    {
        EventClass^ ev = AppState::Manager->GetEventById(_eventId);
        String^ evName = (ev != nullptr) ? ev->Name : L"Unknown Event";
        int      count = (ev != nullptr) ? ev->RegisteredCount : 0;
        int      cap   = (ev != nullptr && ev->Capacity > 0) ? ev->Capacity : -1;

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
        backBtn->Click    += gcnew System::EventHandler(this, &AttendeeManagementPage::BackBtn_Click);
        topBar->Controls->Add(backBtn);

        Label^ titleLbl   = gcnew Label();
        titleLbl->Text      = L"Attendees — " + evName;
        titleLbl->Font      = gcnew System::Drawing::Font(L"Segoe UI", 14, FontStyle::Bold);
        titleLbl->ForeColor = Color::White;
        titleLbl->Location  = Point(115, 14);
        titleLbl->AutoSize  = true;
        topBar->Controls->Add(titleLbl);

        // Export CSV button
        Button^ exportBtn   = gcnew Button();
        exportBtn->Text      = L"Export CSV";
        exportBtn->Location  = Point(W - 140, 13);
        exportBtn->Size      = System::Drawing::Size(120, 34);
        exportBtn->BackColor = Color::FromArgb(60, 60, 100);
        exportBtn->ForeColor = Color::White;
        exportBtn->FlatStyle = FlatStyle::Flat;
        exportBtn->Anchor    = static_cast<AnchorStyles>(AnchorStyles::Top | AnchorStyles::Right);
        exportBtn->Click    += gcnew System::EventHandler(this, &AttendeeManagementPage::ExportCSV_Click);
        topBar->Controls->Add(exportBtn);

        // ── Stats bar ──────────────────────────────────────────────────────
        Panel^ statsBar = gcnew Panel();
        statsBar->Dock      = DockStyle::Top;
        statsBar->Height    = 45;
        statsBar->BackColor = Color::FromArgb(245, 245, 250);
        statsBar->Padding   = System::Windows::Forms::Padding(15, 8, 15, 8);
        this->Controls->Add(statsBar);

        String^ capStr     = (cap >= 0) ? cap.ToString() : L"∞";
        String^ statsText  = L"Registered: " + count + L"   /   Capacity: " + capStr;
        _statLbl           = gcnew Label();
        _statLbl->Text      = statsText;
        _statLbl->Font      = gcnew System::Drawing::Font(L"Segoe UI", 11, FontStyle::Bold);
        _statLbl->ForeColor = Color::FromArgb(50, 50, 80);
        _statLbl->Location  = Point(15, 10);
        _statLbl->AutoSize  = true;
        statsBar->Controls->Add(_statLbl);

        // ── Search bar ─────────────────────────────────────────────────────
        Panel^ searchBar = gcnew Panel();
        searchBar->Dock      = DockStyle::Top;
        searchBar->Height    = 45;
        searchBar->BackColor = Color::White;
        searchBar->Padding   = System::Windows::Forms::Padding(10, 7, 10, 7);
        this->Controls->Add(searchBar);

        Label^ searchLbl   = gcnew Label();
        searchLbl->Text     = L"Search:";
        searchLbl->Font     = gcnew System::Drawing::Font(L"Segoe UI", 10);
        searchLbl->Location = Point(10, 12);
        searchLbl->AutoSize = true;
        searchBar->Controls->Add(searchLbl);

        _searchTB           = gcnew TextBox();
        _searchTB->Location  = Point(75, 8);
        _searchTB->Size      = System::Drawing::Size(340, 28);
        _searchTB->Font      = gcnew System::Drawing::Font(L"Segoe UI", 10);
        _searchTB->TextChanged += gcnew System::EventHandler(this, &AttendeeManagementPage::Search_Changed);
        searchBar->Controls->Add(_searchTB);

        // ── Header row ─────────────────────────────────────────────────────
        Panel^ headerRow = gcnew Panel();
        headerRow->Dock      = DockStyle::Top;
        headerRow->Height    = 36;
        headerRow->BackColor = Color::FromArgb(220, 220, 240);
        this->Controls->Add(headerRow);
        BuildHeaderRow(headerRow);

        // ── Scrollable attendee list ───────────────────────────────────────
        Panel^ listContainer = gcnew Panel();
        listContainer->Dock       = DockStyle::Fill;
        listContainer->AutoScroll = true;
        this->Controls->Add(listContainer);

        _listFLP               = gcnew FlowLayoutPanel();
        _listFLP->Dock         = DockStyle::Fill;
        _listFLP->AutoScroll   = true;
        _listFLP->FlowDirection = FlowDirection::TopDown;
        _listFLP->WrapContents  = false;
        listContainer->Controls->Add(_listFLP);

        // Fix paint order so Fill comes last
        listContainer->BringToFront();
    }

    void BuildHeaderRow(Panel^ hdrPanel)
    {
        AddHdrLabel(hdrPanel, L"#",         10,  40);
        AddHdrLabel(hdrPanel, L"Name",      55,  220);
        AddHdrLabel(hdrPanel, L"Email",     285, 320);
        AddHdrLabel(hdrPanel, L"Check-In",  615, 120);
    }

    void AddHdrLabel(Panel^ parent, String^ text, int x, int w)
    {
        Label^ lbl    = gcnew Label();
        lbl->Text      = text;
        lbl->Font      = gcnew System::Drawing::Font(L"Segoe UI", 10, FontStyle::Bold);
        lbl->Location  = Point(x, 8);
        lbl->Size      = System::Drawing::Size(w, 22);
        lbl->ForeColor = Color::FromArgb(40, 40, 80);
        parent->Controls->Add(lbl);
    }

    // -----------------------------------------------------------------------
    // Filter + list rendering
    // -----------------------------------------------------------------------
    void ApplyFilter(String^ query)
    {
        _filtered->Clear();
        String^ q = query->Trim()->ToLower();

        for each (Attendee^ a in _attendees)
        {
            if (String::IsNullOrEmpty(q)
                || a->Name->ToLower()->Contains(q)
                || a->Email->ToLower()->Contains(q))
            {
                _filtered->Add(a);
            }
        }

        RenderList();
    }

    void RenderList()
    {
        _listFLP->Controls->Clear();

        int W = _listFLP->ClientSize.Width - 20;
        if (W < 500) W = 500;

        for each (Attendee^ a in _filtered)
        {
            Panel^ row = BuildAttendeeRow(a, W);
            _listFLP->Controls->Add(row);
        }

        if (_filtered->Count == 0)
        {
            Label^ empty    = gcnew Label();
            empty->Text      = L"No attendees found.";
            empty->Font      = gcnew System::Drawing::Font(L"Segoe UI", 11);
            empty->ForeColor = Color::Gray;
            empty->AutoSize  = true;
            empty->Margin    = System::Windows::Forms::Padding(20);
            _listFLP->Controls->Add(empty);
        }

        // Update check-in count in stats label
        int checkedIn = 0;
        for each (Attendee^ a in _attendees)
            if (a->CheckedIn) checkedIn++;
        UpdateStatLabel(checkedIn);
    }

    Panel^ BuildAttendeeRow(Attendee^ a, int rowW)
    {
        Panel^ row    = gcnew Panel();
        row->Size      = System::Drawing::Size(rowW, 44);
        row->Margin    = System::Windows::Forms::Padding(0, 0, 0, 2);
        row->BackColor = (a->Index % 2 == 0) ? Color::White : Color::FromArgb(248, 248, 252);
        row->BorderStyle = BorderStyle::FixedSingle;
        row->Tag       = a->Index;

        Label^ numLbl   = gcnew Label();
        numLbl->Text     = (a->Index + 1).ToString();
        numLbl->Font     = gcnew System::Drawing::Font(L"Segoe UI", 9);
        numLbl->ForeColor = Color::Gray;
        numLbl->Location = Point(10, 13);
        numLbl->Size     = System::Drawing::Size(40, 20);
        row->Controls->Add(numLbl);

        Label^ nameLbl   = gcnew Label();
        nameLbl->Text     = a->Name;
        nameLbl->Font     = gcnew System::Drawing::Font(L"Segoe UI", 10, FontStyle::Bold);
        nameLbl->Location = Point(55, 12);
        nameLbl->Size     = System::Drawing::Size(220, 22);
        row->Controls->Add(nameLbl);

        Label^ emailLbl   = gcnew Label();
        emailLbl->Text     = a->Email;
        emailLbl->Font     = gcnew System::Drawing::Font(L"Segoe UI", 9);
        emailLbl->ForeColor = Color::FromArgb(80, 80, 120);
        emailLbl->Location = Point(285, 13);
        emailLbl->Size     = System::Drawing::Size(320, 20);
        emailLbl->AutoEllipsis = true;
        row->Controls->Add(emailLbl);

        CheckBox^ checkIn   = gcnew CheckBox();
        checkIn->Text        = a->CheckedIn ? L"Checked In" : L"Check In";
        checkIn->Font        = gcnew System::Drawing::Font(L"Segoe UI", 9);
        checkIn->Location    = Point(615, 11);
        checkIn->Size        = System::Drawing::Size(120, 22);
        checkIn->Checked     = a->CheckedIn;
        checkIn->Tag         = a->Index;
        checkIn->CheckedChanged += gcnew System::EventHandler(this, &AttendeeManagementPage::CheckIn_Changed);
        row->Controls->Add(checkIn);

        return row;
    }

    void UpdateStatLabel(int checkedIn)
    {
        EventClass^ ev = AppState::Manager->GetEventById(_eventId);
        if (ev == nullptr) return;
        int  count  = ev->RegisteredCount;
        int  cap    = ev->Capacity;
        String^ capStr = (cap > 0) ? cap.ToString() : L"∞";
        _statLbl->Text = L"Registered: " + count
            + L"   /   Capacity: " + capStr
            + L"   |   Checked In: " + checkedIn + L" / " + count;
    }

    // -----------------------------------------------------------------------
    // Event handlers
    // -----------------------------------------------------------------------
    void BackBtn_Click(System::Object^, System::EventArgs^)
    {
        this->Close();
    }

    void Search_Changed(System::Object^, System::EventArgs^)
    {
        ApplyFilter(_searchTB->Text);
    }

    void CheckIn_Changed(System::Object^ sender, System::EventArgs^)
    {
        CheckBox^ cb  = safe_cast<CheckBox^>(sender);
        int idx        = safe_cast<int>(cb->Tag);

        for each (Attendee^ a in _attendees)
        {
            if (a->Index == idx)
            {
                a->CheckedIn = cb->Checked;
                cb->Text     = cb->Checked ? L"Checked In" : L"Check In";
                break;
            }
        }

        // Update stats
        int checkedIn = 0;
        for each (Attendee^ a in _attendees)
            if (a->CheckedIn) checkedIn++;
        UpdateStatLabel(checkedIn);
    }

    void ExportCSV_Click(System::Object^, System::EventArgs^)
    {
        EventClass^ ev = AppState::Manager->GetEventById(_eventId);
        String^ evName = (ev != nullptr) ? ev->Name : L"event";

        // Sanitize name for filename
        String^ safe = evName->Replace(L" ", L"_")
                             ->Replace(L"/", L"-")
                             ->Replace(L"\\", L"-")
                             ->Replace(L":", L"-");

        String^ desktop = Environment::GetFolderPath(Environment::SpecialFolder::Desktop);
        String^ path    = System::IO::Path::Combine(desktop, safe + L"_attendees.csv");

        try
        {
            System::IO::StreamWriter^ sw = gcnew System::IO::StreamWriter(path, false);
            sw->WriteLine(L"#,Name,Email,CheckedIn");
            for each (Attendee^ a in _attendees)
            {
                sw->WriteLine(
                    (a->Index + 1).ToString() + L","
                    + L"\"" + a->Name  + L"\","
                    + L"\"" + a->Email + L"\","
                    + (a->CheckedIn ? L"Yes" : L"No"));
            }
            sw->Close();
            MessageBox::Show(L"Exported to:\n" + path, L"Export Successful",
                MessageBoxButtons::OK, MessageBoxIcon::Information);
        }
        catch (Exception^ ex)
        {
            MessageBox::Show(L"Export failed: " + ex->Message, L"Export Error",
                MessageBoxButtons::OK, MessageBoxIcon::Error);
        }
    }
};

} // namespace GroupProject
