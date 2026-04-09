#pragma once
#include "AppState.h"
#include "PersistenceService.h"
#include "EventDetailsPage.h"

namespace GroupProject {

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;
using namespace System::Collections::Generic;

// Shows the attendee's registered and saved events in two tabs.
// Caller form is shown again when this form closes.
public ref class MyEventsPage : public System::Windows::Forms::Form
{
private:
    Form^ _callerForm;

    TabControl^     _tabs;
    FlowLayoutPanel^ _registeredPanel;
    FlowLayoutPanel^ _savedPanel;

public:
    MyEventsPage(Form^ callerForm)
    {
        _callerForm = callerForm;
        InitializeComponent();
    }

protected:
    ~MyEventsPage()
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
        this->Name = L"MyEventsPage";
        this->Text = L"My Events";
        this->Load       += gcnew System::EventHandler(this, &MyEventsPage::OnLoad);
        this->VisibleChanged += gcnew System::EventHandler(this, &MyEventsPage::OnVisibleChanged);
        this->FormClosed += gcnew FormClosedEventHandler(this, &MyEventsPage::OnFormClosed);
        this->ResumeLayout(false);
    }

    void OnLoad(System::Object^, System::EventArgs^)
    {
        this->WindowState = FormWindowState::Maximized;
        BuildUI();
        PopulateTabs();
    }

    void OnFormClosed(System::Object^, FormClosedEventArgs^)
    {
        if (_callerForm != nullptr)
            _callerForm->Show();
    }

    void OnVisibleChanged(System::Object^, System::EventArgs^)
    {
        if (!this->Visible || _registeredPanel == nullptr) return;
        PopulateTabs();
    }

    void BuildUI()
    {
        int W = this->ClientSize.Width;
        int H = this->ClientSize.Height;

        // Title
        Label^ title = gcnew Label();
        title->Text      = L"My Events";
        title->Font      = gcnew System::Drawing::Font(L"Microsoft Sans Serif", 20, FontStyle::Bold);
        title->Location  = Point(0, 0);
        title->Size      = System::Drawing::Size(W, 55);
        title->TextAlign = ContentAlignment::MiddleCenter;
        this->Controls->Add(title);

        // Back button
        Button^ backBtn = gcnew Button();
        backBtn->Text     = L"< Back";
        backBtn->Location = Point(10, 10);
        backBtn->Size     = System::Drawing::Size(90, 35);
        backBtn->Click   += gcnew System::EventHandler(this, &MyEventsPage::BackBtn_Click);
        this->Controls->Add(backBtn);
        backBtn->BringToFront();

        // TabControl
        _tabs = gcnew TabControl();
        _tabs->Location = Point(10, 65);
        _tabs->Size     = System::Drawing::Size(W - 20, H - 75);
        _tabs->Font     = gcnew System::Drawing::Font(L"Microsoft Sans Serif", 11);
        this->Controls->Add(_tabs);

        // Registered tab
        TabPage^ regPage = gcnew TabPage(L"Registered Events");
        _registeredPanel = MakeScrollPanel();
        regPage->Controls->Add(_registeredPanel);
        _tabs->TabPages->Add(regPage);

        // Saved tab
        TabPage^ savPage = gcnew TabPage(L"Saved Events");
        _savedPanel = MakeScrollPanel();
        savPage->Controls->Add(_savedPanel);
        _tabs->TabPages->Add(savPage);
    }

    static FlowLayoutPanel^ MakeScrollPanel()
    {
        FlowLayoutPanel^ p = gcnew FlowLayoutPanel();
        p->Dock           = DockStyle::Fill;
        p->AutoScroll     = true;
        p->FlowDirection  = FlowDirection::TopDown;
        p->WrapContents   = false;
        p->Padding        = System::Windows::Forms::Padding(10);
        return p;
    }

    void PopulateTabs()
    {
        // Registered events
        _registeredPanel->Controls->Clear();
        List<EventClass^>^ regEvents = AppState::Manager->GetRegisteredEvents(AppState::Session);
        if (regEvents->Count == 0)
            _registeredPanel->Controls->Add(MakeEmptyLabel(L"You have not registered for any events yet."));
        else
            for each (EventClass^ ev in regEvents)
                _registeredPanel->Controls->Add(MakeEventCard(ev, true));

        // Saved events
        _savedPanel->Controls->Clear();
        List<EventClass^>^ savEvents = AppState::Manager->GetSavedEvents(AppState::Session);
        if (savEvents->Count == 0)
            _savedPanel->Controls->Add(MakeEmptyLabel(L"You have not saved any events yet."));
        else
            for each (EventClass^ ev in savEvents)
                _savedPanel->Controls->Add(MakeEventCard(ev, false));
    }

    static Label^ MakeEmptyLabel(String^ text)
    {
        Label^ lbl = gcnew Label();
        lbl->Text      = text;
        lbl->Font      = gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12);
        lbl->ForeColor = Color::Gray;
        lbl->Size      = System::Drawing::Size(600, 60);
        lbl->TextAlign = ContentAlignment::MiddleCenter;
        lbl->Margin    = System::Windows::Forms::Padding(20);
        return lbl;
    }

    // isRegistered = true → show Cancel Registration button
    // isRegistered = false → show Unsave button
    Panel^ MakeEventCard(EventClass^ ev, bool isRegisteredTab)
    {
        Panel^ card = gcnew Panel();
        card->Size        = System::Drawing::Size(this->ClientSize.Width - 60, 110);
        card->BorderStyle = BorderStyle::FixedSingle;
        card->BackColor   = Color::White;
        card->Margin      = System::Windows::Forms::Padding(0, 0, 0, 8);
        card->Tag         = ev->Id.ToString(); // store for action lookup

        // Thumbnail
        PictureBox^ thumb = gcnew PictureBox();
        thumb->Size      = System::Drawing::Size(90, 90);
        thumb->Location  = Point(8, 10);
        thumb->SizeMode  = PictureBoxSizeMode::StretchImage;
        thumb->Image     = ev->Icon;
        thumb->BackColor = Color::LightGray;
        card->Controls->Add(thumb);

        int tx = 110;

        // Title
        Label^ titleLbl = gcnew Label();
        titleLbl->Text      = ev->Name;
        titleLbl->Font      = gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, FontStyle::Bold);
        titleLbl->Location  = Point(tx, 8);
        titleLbl->Size      = System::Drawing::Size(card->Width - tx - 230, 24);
        titleLbl->AutoEllipsis = true;
        card->Controls->Add(titleLbl);

        // Organizer / location / date
        String^ meta = ev->Runner + L"  |  " + ev->Location + L"\n"
                     + ev->StartDateTime.ToString(L"MMM dd, yyyy  hh:mm tt");
        Label^ metaLbl = gcnew Label();
        metaLbl->Text     = meta;
        metaLbl->Font     = gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9);
        metaLbl->Location = Point(tx, 34);
        metaLbl->Size     = System::Drawing::Size(card->Width - tx - 230, 44);
        card->Controls->Add(metaLbl);

        // Category badge
        Label^ catBadge = gcnew Label();
        catBadge->Text      = ev->Category;
        catBadge->Location  = Point(tx, 82);
        catBadge->Size      = System::Drawing::Size(90, 20);
        catBadge->TextAlign = ContentAlignment::MiddleCenter;
        catBadge->BackColor = EventDetailsPage::CategoryColor(ev->Category);
        catBadge->ForeColor = Color::White;
        catBadge->Font      = gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8, FontStyle::Bold);
        card->Controls->Add(catBadge);

        // View Details button
        Button^ detailsBtn = gcnew Button();
        detailsBtn->Text     = L"View Details";
        detailsBtn->Location = Point(card->Width - 220, 10);
        detailsBtn->Size     = System::Drawing::Size(100, 35);
        detailsBtn->Tag      = ev->Id.ToString();
        detailsBtn->Click   += gcnew System::EventHandler(this, &MyEventsPage::ViewDetails_Click);
        card->Controls->Add(detailsBtn);

        // Action button (Cancel Registration or Unsave)
        Button^ actionBtn = gcnew Button();
        actionBtn->Location = Point(card->Width - 220, 55);
        actionBtn->Size     = System::Drawing::Size(100, 35);
        actionBtn->Tag      = ev->Id.ToString();
        if (isRegisteredTab)
        {
            actionBtn->Text     = L"Cancel Reg.";
            actionBtn->Click   += gcnew System::EventHandler(this, &MyEventsPage::CancelReg_Click);
        }
        else
        {
            actionBtn->Text     = L"Unsave";
            actionBtn->Click   += gcnew System::EventHandler(this, &MyEventsPage::Unsave_Click);
        }
        card->Controls->Add(actionBtn);

        return card;
    }

    void BackBtn_Click(System::Object^, System::EventArgs^)
    {
        this->Close();
    }

    void ViewDetails_Click(System::Object^ sender, System::EventArgs^)
    {
        int id = Int32::Parse(safe_cast<Button^>(sender)->Tag->ToString());
        EventDetailsPage^ details = gcnew EventDetailsPage(id, this);
        this->Hide();
        details->Show();
    }

    void CancelReg_Click(System::Object^ sender, System::EventArgs^)
    {
        int id = Int32::Parse(safe_cast<Button^>(sender)->Tag->ToString());
        System::Windows::Forms::DialogResult result = MessageBox::Show(
            L"Cancel your registration for this event?",
            L"Confirm", MessageBoxButtons::YesNo, MessageBoxIcon::Question);
        if (result != System::Windows::Forms::DialogResult::Yes) return;

        AppState::Manager->CancelRegistration(id, AppState::Session);
        PersistenceService::Save(AppState::Manager, AppState::Session);
        PopulateTabs(); // refresh display
    }

    void Unsave_Click(System::Object^ sender, System::EventArgs^)
    {
        int id = Int32::Parse(safe_cast<Button^>(sender)->Tag->ToString());
        AppState::Manager->UnsaveEvent(id, AppState::Session);
        PersistenceService::Save(AppState::Manager, AppState::Session);
        PopulateTabs(); // refresh display
    }
};

} // namespace GroupProject
