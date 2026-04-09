#pragma once
#include "AppState.h"
#include "PersistenceService.h"

namespace GroupProject {

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;

// Shows full details for a single event.
// Handles Register/Cancel, Save/Unsave actions.
// Takes a caller Form^ and an event ID — no direct reference to other page classes needed.
public ref class EventDetailsPage : public System::Windows::Forms::Form
{
private:
    int    _eventId;
    Form^  _callerForm;

    // Controls updated dynamically after actions
    Label^  _capacityLabel;
    Button^ _registerBtn;
    Button^ _cancelRegBtn;
    Button^ _saveBtn;

public:
    EventDetailsPage(int eventId, Form^ callerForm)
    {
        _eventId    = eventId;
        _callerForm = callerForm;
        InitializeComponent();
    }

protected:
    ~EventDetailsPage()
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
        this->Name                = L"EventDetailsPage";
        this->Text                = L"Event Details";
        this->Load               += gcnew System::EventHandler(this, &EventDetailsPage::OnLoad);
        this->FormClosed         += gcnew FormClosedEventHandler(this, &EventDetailsPage::OnFormClosed);
        this->ResumeLayout(false);
    }

    void OnLoad(System::Object^ sender, System::EventArgs^ e)
    {
        this->WindowState = FormWindowState::Maximized;
        BuildUI();
    }

    void OnFormClosed(System::Object^ sender, FormClosedEventArgs^ e)
    {
        if (_callerForm != nullptr)
            _callerForm->Show();
    }

    // Helper: create a right-column info label and advance ry
    Label^ MakeDetailLabel(String^ text, bool bold, int fontSize, int rxLeft, int formW, int% ry)
    {
        const int lineH = 32;
        Label^ lbl    = gcnew Label();
        lbl->Text     = text;
        lbl->Location = Point(rxLeft, ry);
        lbl->Size     = System::Drawing::Size(formW - rxLeft - 20, lineH);
        lbl->Font     = gcnew System::Drawing::Font(L"Microsoft Sans Serif",
                            (float)fontSize,
                            bold ? FontStyle::Bold : FontStyle::Regular);
        lbl->AutoSize = false;
        ry += lineH + 4;
        return lbl;
    }

    void BuildUI()
    {
        EventClass^ ev = AppState::Manager->GetEventById(_eventId);
        if (ev == nullptr)
        {
            MessageBox::Show(L"Event not found.", L"Error");
            this->Close();
            return;
        }

        int W = this->ClientSize.Width;
        int H = this->ClientSize.Height;

        // Back button
        Button^ backBtn = gcnew Button();
        backBtn->Text     = L"< Back";
        backBtn->Location = Point(10, 10);
        backBtn->Size     = System::Drawing::Size(100, 35);
        backBtn->Click   += gcnew System::EventHandler(this, &EventDetailsPage::BackBtn_Click);
        this->Controls->Add(backBtn);

        int contentTop = 60;

        // Event image (left column)
        PictureBox^ imgBox = gcnew PictureBox();
        imgBox->Size      = System::Drawing::Size(280, 220);
        imgBox->Location  = Point(20, contentTop);
        imgBox->BorderStyle = BorderStyle::FixedSingle;
        imgBox->SizeMode  = PictureBoxSizeMode::StretchImage;
        imgBox->Image     = ev->Icon;
        if (ev->Icon == nullptr)
        {
            imgBox->BackColor = Color::LightGray;
            Label^ noImg = gcnew Label();
            noImg->Text      = L"No Image";
            noImg->TextAlign = ContentAlignment::MiddleCenter;
            noImg->Dock      = DockStyle::Fill;
            imgBox->Controls->Add(noImg);
        }
        this->Controls->Add(imgBox);

        // Right column: title, organizer, category, location, date
        int rxLeft = 320;
        int ry     = contentTop;

        this->Controls->Add(MakeDetailLabel(ev->Name, true, 18, rxLeft, W, ry));

        Label^ catLbl = gcnew Label();
        catLbl->Text      = ev->Category;
        catLbl->Location  = Point(rxLeft, ry);
        catLbl->Size      = System::Drawing::Size(120, 26);
        catLbl->TextAlign = ContentAlignment::MiddleCenter;
        catLbl->BackColor = CategoryColor(ev->Category);
        catLbl->ForeColor = Color::White;
        catLbl->Font      = gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, FontStyle::Bold);
        ry += 32;
        this->Controls->Add(catLbl);

        this->Controls->Add(MakeDetailLabel(L"Organizer: " + ev->Runner, false, 11, rxLeft, W, ry));
        this->Controls->Add(MakeDetailLabel(L"Location: "  + ev->Location, false, 11, rxLeft, W, ry));
        this->Controls->Add(MakeDetailLabel(L"Start: " + ev->StartDateTime.ToString(L"MMMM dd, yyyy  hh:mm tt"), false, 11, rxLeft, W, ry));
        this->Controls->Add(MakeDetailLabel(L"End:   " + ev->EndDateTime.ToString(L"MMMM dd, yyyy  hh:mm tt"), false, 11, rxLeft, W, ry));

        // Capacity label (stored as member so it can be refreshed)
        const int lineH  = 32;
        _capacityLabel = gcnew Label();
        _capacityLabel->Location = Point(rxLeft, ry);
        _capacityLabel->Size     = System::Drawing::Size(W - rxLeft - 20, lineH);
        _capacityLabel->Font     = gcnew System::Drawing::Font(L"Microsoft Sans Serif", 11);
        ry += lineH + 4;
        this->Controls->Add(_capacityLabel);
        RefreshCapacityLabel(ev);

        // Description (below image + meta, full width)
        int descTop = Math::Max(imgBox->Bottom + 16, ry + 16);
        Label^ descTitle = gcnew Label();
        descTitle->Text     = L"Description";
        descTitle->Location = Point(20, descTop);
        descTitle->Size     = System::Drawing::Size(W - 40, 28);
        descTitle->Font     = gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, FontStyle::Bold);
        this->Controls->Add(descTitle);

        RichTextBox^ descBox = gcnew RichTextBox();
        descBox->Text      = ev->Description;
        descBox->ReadOnly  = true;
        descBox->Location  = Point(20, descTop + 32);
        descBox->Size      = System::Drawing::Size(W - 40, H - (descTop + 32) - 70);
        descBox->Font      = gcnew System::Drawing::Font(L"Microsoft Sans Serif", 11);
        descBox->BorderStyle = BorderStyle::FixedSingle;
        descBox->ScrollBars = RichTextBoxScrollBars::Vertical;
        this->Controls->Add(descBox);

        // Action buttons at bottom
        int btnY   = H - 60;
        int btnW   = 200;
        int btnH   = 40;
        int btnGap = 20;
        int btnX   = 20;

        _registerBtn = gcnew Button();
        _registerBtn->Text     = L"Register";
        _registerBtn->Location = Point(btnX, btnY);
        _registerBtn->Size     = System::Drawing::Size(btnW, btnH);
        _registerBtn->Click   += gcnew System::EventHandler(this, &EventDetailsPage::RegisterBtn_Click);
        this->Controls->Add(_registerBtn);
        btnX += btnW + btnGap;

        _cancelRegBtn = gcnew Button();
        _cancelRegBtn->Text     = L"Cancel Registration";
        _cancelRegBtn->Location = Point(btnX, btnY);
        _cancelRegBtn->Size     = System::Drawing::Size(btnW, btnH);
        _cancelRegBtn->Click   += gcnew System::EventHandler(this, &EventDetailsPage::CancelRegBtn_Click);
        this->Controls->Add(_cancelRegBtn);
        btnX += btnW + btnGap;

        _saveBtn = gcnew Button();
        _saveBtn->Location = Point(btnX, btnY);
        _saveBtn->Size     = System::Drawing::Size(btnW, btnH);
        _saveBtn->Click   += gcnew System::EventHandler(this, &EventDetailsPage::SaveBtn_Click);
        this->Controls->Add(_saveBtn);

        RefreshActionButtons(ev);
    }

    // Refresh capacity display string
    void RefreshCapacityLabel(EventClass^ ev)
    {
        if (ev->Capacity == 0)
            _capacityLabel->Text = L"Capacity: Unlimited";
        else
            _capacityLabel->Text = L"Capacity: " + ev->RegisteredCount
                                 + L" / " + ev->Capacity + L" registered";
    }

    // Enable/disable and relabel action buttons based on current state
    void RefreshActionButtons(EventClass^ ev)
    {
        bool isReg  = AppState::Session->IsRegistered(ev->Id);
        bool isSav  = AppState::Session->IsSaved(ev->Id);
        bool isFull = ev->Capacity > 0 && ev->RegisteredCount >= ev->Capacity;

        _registerBtn->Enabled   = !isReg && !isFull;
        _registerBtn->Text      = isFull && !isReg ? L"Event Full" : L"Register";

        _cancelRegBtn->Enabled  = isReg;
        _cancelRegBtn->Visible  = isReg;

        _saveBtn->Text      = isSav ? L"\u2605 Saved" : L"\u2606 Save";
        _saveBtn->BackColor = isSav ? Color::Gold : SystemColors::Control;
    }

public:
    // Public so other pages (e.g. MyEventsPage, EventsPage) can use consistent category colors
    static Color CategoryColor(String^ category)
    {
        if (category == EventCategories::Music)    return Color::FromArgb(142, 68, 173);
        if (category == EventCategories::Sports)   return Color::FromArgb(39, 174, 96);
        if (category == EventCategories::Tech)     return Color::FromArgb(41, 128, 185);
        if (category == EventCategories::Art)      return Color::FromArgb(211, 84, 0);
        if (category == EventCategories::Food)     return Color::FromArgb(192, 57, 43);
        if (category == EventCategories::Academic) return Color::FromArgb(52, 73, 94);
        return Color::FromArgb(100, 100, 100); // Other
    }

private:
    void BackBtn_Click(System::Object^ sender, System::EventArgs^ e)
    {
        this->Close(); // OnFormClosed will show _callerForm
    }

    void RegisterBtn_Click(System::Object^ sender, System::EventArgs^ e)
    {
        bool ok = AppState::Manager->RegisterForEvent(_eventId, AppState::Session);
        if (!ok)
        {
            MessageBox::Show(L"Could not register. The event may be full or you are already registered.",
                             L"Registration", MessageBoxButtons::OK, MessageBoxIcon::Information);
            return;
        }
        PersistenceService::Save(AppState::Manager, AppState::Session);
        EventClass^ ev = AppState::Manager->GetEventById(_eventId);
        RefreshCapacityLabel(ev);
        RefreshActionButtons(ev);
    }

    void CancelRegBtn_Click(System::Object^ sender, System::EventArgs^ e)
    {
        System::Windows::Forms::DialogResult result = MessageBox::Show(
            L"Cancel your registration for this event?",
            L"Confirm", MessageBoxButtons::YesNo, MessageBoxIcon::Question);
        if (result != System::Windows::Forms::DialogResult::Yes) return;

        AppState::Manager->CancelRegistration(_eventId, AppState::Session);
        PersistenceService::Save(AppState::Manager, AppState::Session);
        EventClass^ ev = AppState::Manager->GetEventById(_eventId);
        RefreshCapacityLabel(ev);
        RefreshActionButtons(ev);
    }

    void SaveBtn_Click(System::Object^ sender, System::EventArgs^ e)
    {
        if (AppState::Session->IsSaved(_eventId))
            AppState::Manager->UnsaveEvent(_eventId, AppState::Session);
        else
            AppState::Manager->SaveEvent(_eventId, AppState::Session);

        PersistenceService::Save(AppState::Manager, AppState::Session);
        EventClass^ ev = AppState::Manager->GetEventById(_eventId);
        RefreshActionButtons(ev);
    }
};

} // namespace GroupProject
