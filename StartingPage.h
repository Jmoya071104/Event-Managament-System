#pragma once
#include "AppState.h"
#include "SeedData.h"
#include "PersistenceService.h"
#include "EventDetailsPage.h"
#include "MyEventsPage.h"
#include "EventsPage.h"
#include "EventManager.h"
#include "EventClass.h"
#include "OrganizerDashboardPage.h"

namespace GroupProject {

	using namespace System;
		using namespace System::Windows::Forms;

	public ref class StartingPage : public System::Windows::Forms::Form
	{
	public:
		StartingPage(void) { InitializeComponent(); }
	protected:
		~StartingPage() { if (components) { delete components; } }
	private:
		System::ComponentModel::Container ^components;
		Panel^ _navCard = nullptr;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->SuspendLayout();
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(282, 253);
			this->Name = L"StartingPage";
			this->Text = L"StartingPage";
			this->Load   += gcnew System::EventHandler(this, &StartingPage::StartingPage_Load);
			this->Resize += gcnew System::EventHandler(this, &StartingPage::StartingPage_Resize);
			this->ResumeLayout(false);
		}
#pragma endregion

	private:
		void CenterNavCard()
		{
			if (_navCard == nullptr) return;
			int W  = this->ClientSize.Width;
			int H  = this->ClientSize.Height;
			int cx = (W - _navCard->Width)  / 2;
			int cy = (H - _navCard->Height) / 2 + 20;
			if (cy < 100) cy = 100;
			_navCard->Location = System::Drawing::Point(cx, cy);
		}

		System::Void StartingPage_Resize(System::Object^, System::EventArgs^)
		{
			CenterNavCard();
		}

	private: System::Void StartingPage_Load(System::Object^ sender, System::EventArgs^ e)
	{
		AppState::Initialize();
		bool hadSavedData = PersistenceService::Load(AppState::Manager, AppState::Session);
		if (!hadSavedData) SeedData::Seed(AppState::Manager);
		this->WindowState = FormWindowState::Maximized;
		this->BackColor   = Color::FromArgb(245, 246, 250);

		// ── Top bar ─────────────────────────────────────────────────────────
		Panel^ topBar     = gcnew Panel();
		topBar->Dock      = DockStyle::Top;
		topBar->Height    = 86;
		topBar->BackColor = Color::FromArgb(24, 28, 58);
		this->Controls->Add(topBar);

		Label^ appTitle    = gcnew Label();
		appTitle->Text      = L"Event Management System";
		appTitle->Font      = gcnew Drawing::Font(L"Segoe UI", 22, FontStyle::Bold);
		appTitle->ForeColor = Color::White;
		appTitle->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
		appTitle->Dock      = DockStyle::Top;
		appTitle->Height    = 56;
		topBar->Controls->Add(appTitle);

		Label^ subTitle    = gcnew Label();
		subTitle->Text      = L"Browse events, register, and manage your schedule";
		subTitle->Font      = gcnew Drawing::Font(L"Segoe UI", 10);
		subTitle->ForeColor = Color::FromArgb(170, 178, 220);
		subTitle->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
		subTitle->Dock      = DockStyle::Bottom;
		subTitle->Height    = 28;
		topBar->Controls->Add(subTitle);

		// ── Navigation card (centered on form) ──────────────────────────────
		const int cardW  = 420;
		const int btnW   = 362;
		const int btnH   = 54;
		const int btnX   = (cardW - btnW) / 2;
		const int firstY = 76;
		const int cardH  = firstY + (btnH + 16) * 3 + 20;

		_navCard              = gcnew Panel();
		_navCard->Size        = System::Drawing::Size(cardW, cardH);
		_navCard->BackColor   = Color::White;
		_navCard->BorderStyle = BorderStyle::FixedSingle;
		this->Controls->Add(_navCard);

		Label^ navHdr    = gcnew Label();
		navHdr->Text      = L"What would you like to do?";
		navHdr->Font      = gcnew Drawing::Font(L"Segoe UI", 12, FontStyle::Bold);
		navHdr->ForeColor = Color::FromArgb(30, 30, 50);
		navHdr->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
		navHdr->Size      = System::Drawing::Size(cardW, 44);
		navHdr->Location  = System::Drawing::Point(0, 20);
		_navCard->Controls->Add(navHdr);

		Button^ viewEventsBtn    = gcnew Button();
		viewEventsBtn->Text      = L"Browse Events";
		viewEventsBtn->Font      = gcnew Drawing::Font(L"Segoe UI", 11, FontStyle::Bold);
		viewEventsBtn->Size      = System::Drawing::Size(btnW, btnH);
		viewEventsBtn->Location  = System::Drawing::Point(btnX, firstY);
		viewEventsBtn->BackColor = Color::FromArgb(67, 97, 238);
		viewEventsBtn->ForeColor = Color::White;
		viewEventsBtn->FlatStyle = FlatStyle::Flat;
		viewEventsBtn->FlatAppearance->BorderSize = 0;
		viewEventsBtn->Click    += gcnew System::EventHandler(this, &StartingPage::ViewEvents_Click);
		_navCard->Controls->Add(viewEventsBtn);

		Button^ myEventsBtn    = gcnew Button();
		myEventsBtn->Text      = L"My Events";
		myEventsBtn->Font      = gcnew Drawing::Font(L"Segoe UI", 11, FontStyle::Bold);
		myEventsBtn->Size      = System::Drawing::Size(btnW, btnH);
		myEventsBtn->Location  = System::Drawing::Point(btnX, firstY + (btnH + 16));
		myEventsBtn->BackColor = Color::FromArgb(67, 97, 238);
		myEventsBtn->ForeColor = Color::White;
		myEventsBtn->FlatStyle = FlatStyle::Flat;
		myEventsBtn->FlatAppearance->BorderSize = 0;
		myEventsBtn->Click    += gcnew System::EventHandler(this, &StartingPage::MyEvents_Click);
		_navCard->Controls->Add(myEventsBtn);

		Button^ orgBtn    = gcnew Button();
		orgBtn->Text      = L"Organizer Dashboard";
		orgBtn->Font      = gcnew Drawing::Font(L"Segoe UI", 11, FontStyle::Bold);
		orgBtn->Size      = System::Drawing::Size(btnW, btnH);
		orgBtn->Location  = System::Drawing::Point(btnX, firstY + (btnH + 16) * 2);
		orgBtn->BackColor = Color::FromArgb(50, 55, 100);
		orgBtn->ForeColor = Color::White;
		orgBtn->FlatStyle = FlatStyle::Flat;
		orgBtn->FlatAppearance->BorderSize = 0;
		orgBtn->Click    += gcnew System::EventHandler(this, &StartingPage::OrganizerDashboard_Click);
		_navCard->Controls->Add(orgBtn);

		CenterNavCard();
	}

	private: System::Void ViewEvents_Click(System::Object^ sender, System::EventArgs^ e)
	{
		EventsPage^ newForm = gcnew EventsPage(this);
		this->Hide(); newForm->Show();
	}
	private: System::Void MyEvents_Click(System::Object^ sender, System::EventArgs^ e)
	{
		MyEventsPage^ myPage = gcnew MyEventsPage(this);
		this->Hide(); myPage->Show();
	}
	private: System::Void OrganizerDashboard_Click(System::Object^ sender, System::EventArgs^ e)
	{
		OrganizerDashboardPage^ dashboard = gcnew OrganizerDashboardPage(this);
		this->Hide(); dashboard->Show();
	}
	};
}
