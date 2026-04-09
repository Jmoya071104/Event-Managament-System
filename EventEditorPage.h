#pragma once
#include "AppState.h"
#include "PersistenceService.h"

namespace GroupProject {

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;

// Single page that handles both Create (eventId == -1) and Edit (eventId >= 0) modes.
// On save:  updates/adds the event, calls PersistenceService::Save, then closes.
// On delete (edit mode only): confirms, removes event, saves, then closes.
// Caller (OrganizerDashboardPage) is shown again via NotifyChanged() + OnFormClosed.
public ref class EventEditorPage : public System::Windows::Forms::Form
{
private:
    int    _eventId;    // -1 = create mode
    Form^  _callerForm;
    String^ _selectedImagePath;

    // Form controls
    TextBox^         _tb_name;
    TextBox^         _tb_runner;
    TextBox^         _tb_location;
    TextBox^         _tb_description;
    ComboBox^        _cb_category;
    DateTimePicker^  _dtp_start;
    DateTimePicker^  _dtp_end;
    NumericUpDown^   _nud_capacity;
    PictureBox^      _imagePreview;
    Button^          _deleteBtn;

public:
    EventEditorPage(int eventId, Form^ callerForm)
    {
        _eventId           = eventId;
        _callerForm        = callerForm;
        _selectedImagePath = L"";
        InitializeComponent();
    }

protected:
    ~EventEditorPage()
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
        this->ClientSize          = System::Drawing::Size(900, 800);
        this->Name                = L"EventEditorPage";
        this->Text                = (_eventId == -1) ? L"Create Event" : L"Edit Event";
        this->Load               += gcnew System::EventHandler(this, &EventEditorPage::OnLoad);
        this->FormClosed         += gcnew FormClosedEventHandler(this, &EventEditorPage::OnFormClosed);
        this->ResumeLayout(false);
    }

    void OnFormClosed(System::Object^, FormClosedEventArgs^)
    {
        if (_callerForm != nullptr)
            _callerForm->Show();
    }

    void OnLoad(System::Object^, System::EventArgs^)
    {
        this->WindowState = FormWindowState::Maximized;
        BuildUI();
        if (_eventId != -1)
            PopulateFields();
    }

    // -----------------------------------------------------------------------
    // UI construction
    // -----------------------------------------------------------------------
    void BuildUI()
    {
        int W = this->ClientSize.Width;

        // ── Top bar ─────────────────────────────────────────────────────────
        Panel^ topBar = gcnew Panel();
        topBar->Dock      = DockStyle::Top;
        topBar->Height    = 55;
        topBar->BackColor = Color::FromArgb(30, 30, 60);
        this->Controls->Add(topBar);

        Button^ backBtn    = gcnew Button();
        backBtn->Text       = L"< Back";
        backBtn->Location   = Point(10, 11);
        backBtn->Size       = System::Drawing::Size(90, 33);
        backBtn->ForeColor  = Color::White;
        backBtn->FlatStyle  = FlatStyle::Flat;
        backBtn->Click     += gcnew System::EventHandler(this, &EventEditorPage::BackBtn_Click);
        topBar->Controls->Add(backBtn);

        Label^ titleLbl    = gcnew Label();
        titleLbl->Text      = (_eventId == -1) ? L"Create New Event" : L"Edit Event";
        titleLbl->Font      = gcnew System::Drawing::Font(L"Segoe UI", 15, FontStyle::Bold);
        titleLbl->ForeColor = Color::White;
        titleLbl->Location  = Point(115, 12);
        titleLbl->AutoSize  = true;
        topBar->Controls->Add(titleLbl);

        // ── Scrollable body ─────────────────────────────────────────────────
        Panel^ body = gcnew Panel();
        body->Dock       = DockStyle::Fill;
        body->AutoScroll = true;
        body->Padding    = System::Windows::Forms::Padding(30, 20, 30, 20);
        this->Controls->Add(body);

        int cx   = 30;    // column x for labels
        int  vx  = 210;   // column x for inputs
        int  fw  = 500;   // input field width
        int  y   = 20;
        int  lh  = 30;    // label height
        int  ih  = 32;    // input height
        int  gap = 12;    // gap between rows

        // Helper lambdas replaced by inline code below

        // Event Name
        AddLabel(body, L"Event Name *", cx, y, lh);
        _tb_name = AddTextBox(body, vx, y, fw, ih, L"tb_name");
        y += ih + gap;

        // Organizer / Runner
        AddLabel(body, L"Organizer *", cx, y, lh);
        _tb_runner = AddTextBox(body, vx, y, fw, ih, L"tb_runner");
        y += ih + gap;

        // Location
        AddLabel(body, L"Location *", cx, y, lh);
        _tb_location = AddTextBox(body, vx, y, fw, ih, L"tb_location");
        y += ih + gap;

        // Category
        AddLabel(body, L"Category *", cx, y, lh);
        _cb_category = gcnew ComboBox();
        _cb_category->DropDownStyle = ComboBoxStyle::DropDownList;
        _cb_category->Location      = Point(vx, y);
        _cb_category->Size          = System::Drawing::Size(200, ih);
        _cb_category->Font          = gcnew System::Drawing::Font(L"Segoe UI", 10);
        _cb_category->Items->AddRange(gcnew array<System::Object^> {
            L"Music", L"Sports", L"Tech", L"Art", L"Food", L"Academic", L"Other" });
        _cb_category->SelectedIndex = 6; // "Other" default
        body->Controls->Add(_cb_category);
        y += ih + gap;

        // Start DateTime
        AddLabel(body, L"Start Date/Time *", cx, y, lh);
        _dtp_start = gcnew DateTimePicker();
        _dtp_start->Format       = DateTimePickerFormat::Custom;
        _dtp_start->CustomFormat = L"MM/dd/yyyy  hh:mm tt";
        _dtp_start->ShowCheckBox = false;
        _dtp_start->Location     = Point(vx, y);
        _dtp_start->Size         = System::Drawing::Size(300, ih);
        _dtp_start->Font         = gcnew System::Drawing::Font(L"Segoe UI", 10);
        _dtp_start->Value        = DateTime::Now.AddDays(1);
        body->Controls->Add(_dtp_start);
        y += ih + gap;

        // End DateTime
        AddLabel(body, L"End Date/Time *", cx, y, lh);
        _dtp_end = gcnew DateTimePicker();
        _dtp_end->Format         = DateTimePickerFormat::Custom;
        _dtp_end->CustomFormat   = L"MM/dd/yyyy  hh:mm tt";
        _dtp_end->ShowCheckBox   = false;
        _dtp_end->Location       = Point(vx, y);
        _dtp_end->Size           = System::Drawing::Size(300, ih);
        _dtp_end->Font           = gcnew System::Drawing::Font(L"Segoe UI", 10);
        _dtp_end->Value          = DateTime::Now.AddDays(1).AddHours(2);
        body->Controls->Add(_dtp_end);
        y += ih + gap;

        // Capacity
        AddLabel(body, L"Capacity *", cx, y, lh);
        _nud_capacity = gcnew NumericUpDown();
        _nud_capacity->Location  = Point(vx, y);
        _nud_capacity->Size      = System::Drawing::Size(120, ih);
        _nud_capacity->Font      = gcnew System::Drawing::Font(L"Segoe UI", 10);
        _nud_capacity->Minimum   = 0;
        _nud_capacity->Maximum   = 100000;
        _nud_capacity->Value     = 0;
        body->Controls->Add(_nud_capacity);

        Label^ capHint    = gcnew Label();
        capHint->Text      = L"(0 = unlimited)";
        capHint->Font      = gcnew System::Drawing::Font(L"Segoe UI", 9);
        capHint->ForeColor = Color::Gray;
        capHint->Location  = Point(vx + 130, y + 6);
        capHint->AutoSize  = true;
        body->Controls->Add(capHint);
        y += ih + gap;

        // Description
        AddLabel(body, L"Description *", cx, y, lh);
        _tb_description = gcnew TextBox();
        _tb_description->Multiline  = true;
        _tb_description->Location   = Point(vx, y);
        _tb_description->Size       = System::Drawing::Size(fw, 100);
        _tb_description->Font       = gcnew System::Drawing::Font(L"Segoe UI", 10);
        _tb_description->ScrollBars = ScrollBars::Vertical;
        body->Controls->Add(_tb_description);
        y += 100 + gap;

        // Image
        AddLabel(body, L"Image Path", cx, y, lh);
        TextBox^ tb_img = gcnew TextBox();
        tb_img->Name     = L"tb_img";
        tb_img->Location = Point(vx, y);
        tb_img->Size     = System::Drawing::Size(fw - 120, ih);
        tb_img->Font     = gcnew System::Drawing::Font(L"Segoe UI", 10);
        tb_img->ReadOnly = true;
        body->Controls->Add(tb_img);

        Button^ browseBtn   = gcnew Button();
        browseBtn->Text      = L"Browse...";
        browseBtn->Location  = Point(vx + fw - 110, y);
        browseBtn->Size      = System::Drawing::Size(110, ih);
        browseBtn->FlatStyle = FlatStyle::Flat;
        browseBtn->Click    += gcnew System::EventHandler(this, &EventEditorPage::BrowseImage_Click);
        body->Controls->Add(browseBtn);
        y += ih + gap;

        _imagePreview           = gcnew PictureBox();
        _imagePreview->Name      = L"imgPreview";
        _imagePreview->Location  = Point(vx, y);
        _imagePreview->Size      = System::Drawing::Size(160, 120);
        _imagePreview->SizeMode  = PictureBoxSizeMode::Zoom;
        _imagePreview->BorderStyle = BorderStyle::FixedSingle;
        _imagePreview->BackColor = Color::LightGray;
        body->Controls->Add(_imagePreview);
        y += 120 + gap * 2;

        // ── Save button ──────────────────────────────────────────────────────
        Button^ saveBtn   = gcnew Button();
        saveBtn->Text      = (_eventId == -1) ? L"Create Event" : L"Save Changes";
        saveBtn->Location  = Point(vx, y);
        saveBtn->Size      = System::Drawing::Size(200, 40);
        saveBtn->BackColor = Color::FromArgb(0, 153, 76);
        saveBtn->ForeColor = Color::White;
        saveBtn->FlatStyle = FlatStyle::Flat;
        saveBtn->Click    += gcnew System::EventHandler(this, &EventEditorPage::SaveBtn_Click);
        body->Controls->Add(saveBtn);

        // ── Delete button (edit mode only) ────────────────────────────────────
        if (_eventId != -1)
        {
            _deleteBtn           = gcnew Button();
            _deleteBtn->Text      = L"Delete Event";
            _deleteBtn->Location  = Point(vx + 220, y);
            _deleteBtn->Size      = System::Drawing::Size(160, 40);
            _deleteBtn->BackColor = Color::FromArgb(192, 0, 0);
            _deleteBtn->ForeColor = Color::White;
            _deleteBtn->FlatStyle = FlatStyle::Flat;
            _deleteBtn->Click    += gcnew System::EventHandler(this, &EventEditorPage::DeleteBtn_Click);
            body->Controls->Add(_deleteBtn);
        }
    }

    // Helpers for building labels / textboxes consistently
    void AddLabel(Panel^ parent, String^ text, int x, int y, int h)
    {
        Label^ lbl    = gcnew Label();
        lbl->Text      = text;
        lbl->Font      = gcnew System::Drawing::Font(L"Segoe UI", 10, FontStyle::Bold);
        lbl->Location  = Point(x, y + 6);
        lbl->Size      = System::Drawing::Size(175, h);
        lbl->TextAlign = ContentAlignment::MiddleLeft;
        parent->Controls->Add(lbl);
    }

    TextBox^ AddTextBox(Panel^ parent, int x, int y, int w, int h, String^ name)
    {
        TextBox^ tb = gcnew TextBox();
        tb->Name     = name;
        tb->Location = Point(x, y);
        tb->Size     = System::Drawing::Size(w, h);
        tb->Font     = gcnew System::Drawing::Font(L"Segoe UI", 10);
        parent->Controls->Add(tb);
        return tb;
    }

    // -----------------------------------------------------------------------
    // Populate fields from existing event (edit mode)
    // -----------------------------------------------------------------------
    void PopulateFields()
    {
        EventClass^ ev = AppState::Manager->GetEventById(_eventId);
        if (ev == nullptr) return;

        _tb_name->Text        = ev->Name;
        _tb_runner->Text      = ev->Runner;
        _tb_location->Text    = ev->Location;
        _tb_description->Text = ev->Description;
        _dtp_start->Value     = ev->StartDateTime;
        _dtp_end->Value       = ev->EndDateTime;
        _nud_capacity->Value  = ev->Capacity;

        // Category ComboBox
        int catIdx = _cb_category->Items->IndexOf(ev->Category);
        _cb_category->SelectedIndex = (catIdx >= 0) ? catIdx : 6;

        // Image
        _selectedImagePath = ev->ImagePath;
        if (!String::IsNullOrEmpty(ev->ImagePath))
        {
            // Show path in the readonly textbox
            Panel^ body = safe_cast<Panel^>(_imagePreview->Parent);
            for each (Control^ ctrl in body->Controls)
            {
                TextBox^ tb = dynamic_cast<TextBox^>(ctrl);
                if (tb != nullptr && tb->Name == L"tb_img")
                {
                    tb->Text = ev->ImagePath;
                    break;
                }
            }
            if (ev->Icon != nullptr)
                _imagePreview->Image = ev->Icon;
        }
    }

    // -----------------------------------------------------------------------
    // Validation
    // -----------------------------------------------------------------------
    bool ValidateInput(bool isCreate, int currentRegistered)
    {
        if (String::IsNullOrWhiteSpace(_tb_name->Text)
            || String::IsNullOrWhiteSpace(_tb_runner->Text)
            || String::IsNullOrWhiteSpace(_tb_location->Text)
            || String::IsNullOrWhiteSpace(_tb_description->Text))
        {
            MessageBox::Show(L"Please fill in all required fields (marked with *).",
                L"Validation", MessageBoxButtons::OK, MessageBoxIcon::Warning);
            return false;
        }

        if (isCreate && _dtp_start->Value < DateTime::Now)
        {
            MessageBox::Show(L"Start date/time cannot be in the past when creating an event.",
                L"Validation", MessageBoxButtons::OK, MessageBoxIcon::Warning);
            return false;
        }

        if (_dtp_end->Value <= _dtp_start->Value)
        {
            MessageBox::Show(L"End date/time must be after start date/time.",
                L"Validation", MessageBoxButtons::OK, MessageBoxIcon::Warning);
            return false;
        }

        int cap = (int)_nud_capacity->Value;
        if (cap < 0)
        {
            MessageBox::Show(L"Capacity must be 0 (unlimited) or a positive number.",
                L"Validation", MessageBoxButtons::OK, MessageBoxIcon::Warning);
            return false;
        }

        if (!isCreate && cap > 0 && cap < currentRegistered)
        {
            MessageBox::Show(
                L"Capacity (" + cap + L") cannot be lower than the current registered count ("
                + currentRegistered + L").",
                L"Validation", MessageBoxButtons::OK, MessageBoxIcon::Warning);
            return false;
        }

        return true;
    }

    // -----------------------------------------------------------------------
    // Event handlers
    // -----------------------------------------------------------------------
    void BackBtn_Click(System::Object^, System::EventArgs^)
    {
        this->Close();
    }

    void BrowseImage_Click(System::Object^, System::EventArgs^)
    {
        OpenFileDialog^ dlg = gcnew OpenFileDialog();
        dlg->Filter = L"Image Files|*.jpg;*.jpeg;*.png;*.bmp;*.gif";
        dlg->Title  = L"Select Event Image";
        if (dlg->ShowDialog() == System::Windows::Forms::DialogResult::OK)
        {
            _selectedImagePath = dlg->FileName;

            // Update path textbox
            Panel^ body = safe_cast<Panel^>(_imagePreview->Parent);
            for each (Control^ ctrl in body->Controls)
            {
                TextBox^ tb = dynamic_cast<TextBox^>(ctrl);
                if (tb != nullptr && tb->Name == L"tb_img")
                {
                    tb->Text = _selectedImagePath;
                    break;
                }
            }

            try
            {
                _imagePreview->Image    = System::Drawing::Image::FromFile(_selectedImagePath);
                _imagePreview->SizeMode = PictureBoxSizeMode::Zoom;
            }
            catch (Exception^)
            {
                MessageBox::Show(L"Could not load the selected image.", L"Image Error",
                    MessageBoxButtons::OK, MessageBoxIcon::Warning);
            }
        }
    }

    void SaveBtn_Click(System::Object^, System::EventArgs^)
    {
        bool isCreate = (_eventId == -1);
        int  existingRegistered = 0;

        if (!isCreate)
        {
            EventClass^ existing = AppState::Manager->GetEventById(_eventId);
            if (existing != nullptr)
                existingRegistered = existing->RegisteredCount;
        }

        if (!ValidateInput(isCreate, existingRegistered))
            return;

        // Load image
        System::Drawing::Image^ icon = nullptr;
        if (!String::IsNullOrEmpty(_selectedImagePath)
            && System::IO::File::Exists(_selectedImagePath))
        {
            try { icon = System::Drawing::Image::FromFile(_selectedImagePath); }
            catch (Exception^) { icon = nullptr; }
        }

        String^ cat = safe_cast<String^>(_cb_category->SelectedItem);

        if (isCreate)
        {
            EventClass^ newEv = gcnew EventClass(
                _tb_name->Text->Trim(),
                _tb_runner->Text->Trim(),
                _tb_location->Text->Trim(),
                _tb_description->Text->Trim(),
                cat,
                _dtp_start->Value,
                _dtp_end->Value,
                (int)_nud_capacity->Value,
                _selectedImagePath,
                icon);
            AppState::Manager->addEvent(newEv);
            PersistenceService::Save(AppState::Manager, AppState::Session);

            MessageBox::Show(L"Event \"" + newEv->Name + L"\" created successfully.",
                L"Success", MessageBoxButtons::OK, MessageBoxIcon::Information);
        }
        else
        {
            EventClass^ updated = gcnew EventClass(
                _tb_name->Text->Trim(),
                _tb_runner->Text->Trim(),
                _tb_location->Text->Trim(),
                _tb_description->Text->Trim(),
                cat,
                _dtp_start->Value,
                _dtp_end->Value,
                (int)_nud_capacity->Value,
                _selectedImagePath,
                icon);
            AppState::Manager->UpdateEvent(_eventId, updated);
            PersistenceService::Save(AppState::Manager, AppState::Session);

            MessageBox::Show(L"Event updated successfully.",
                L"Success", MessageBoxButtons::OK, MessageBoxIcon::Information);
        }

        this->Close();
    }

    void DeleteBtn_Click(System::Object^, System::EventArgs^)
    {
        EventClass^ ev = AppState::Manager->GetEventById(_eventId);
        String^ evName = (ev != nullptr) ? ev->Name : L"this event";

        System::Windows::Forms::DialogResult result = MessageBox::Show(
            L"Are you sure you want to permanently delete \"" + evName + L"\"?\n"
            L"This will remove all registrations for this event.",
            L"Confirm Delete",
            MessageBoxButtons::YesNo,
            MessageBoxIcon::Warning);

        if (result != System::Windows::Forms::DialogResult::Yes)
            return;

        AppState::Manager->DeleteEvent(_eventId, AppState::Session);
        PersistenceService::Save(AppState::Manager, AppState::Session);

        MessageBox::Show(L"Event deleted.", L"Deleted",
            MessageBoxButtons::OK, MessageBoxIcon::Information);
        this->Close();
    }
};

} // namespace GroupProject
