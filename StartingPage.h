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
		Label^  TitleLabel     = gcnew Label();
		Button^ OrganizerBtn  = gcnew Button();
		Button^ ViewEventsBtn  = gcnew Button();
		Button^ MyEventsBtn    = gcnew Button();

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->SuspendLayout();
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(282, 253);
			this->Name = L"StartingPage";
			this->Text = L"StartingPage";
			this->Load += gcnew System::EventHandler(this, &StartingPage::StartingPage_Load);
			this->ResumeLayout(false);
		}
#pragma endregion

	private: System::Void StartingPage_Load(System::Object^ sender, System::EventArgs^ e)
	{
		AppState::Initialize();
		bool hadSavedData = PersistenceService::Load(AppState::Manager, AppState::Session);
		if (!hadSavedData) SeedData::Seed(AppState::Manager);
		this->WindowState = FormWindowState::Maximized;
		float W = (float)this->ClientSize.Width;
		float H = (float)this->ClientSize.Height;
		this->TitleLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 25, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
		this->TitleLabel->Location  = System::Drawing::Point((int)(W*0.25), 0);
		this->TitleLabel->Size      = System::Drawing::Size((int)(W*0.5), (int)(H*0.20));
		this->TitleLabel->Text      = L"Event Management System";
		this->TitleLabel->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
		this->TitleLabel->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
		this->Controls->Add(TitleLabel);
		this->ViewEventsBtn->Location = System::Drawing::Point((int)(W*0.25),(int)(H*.28));
		this->ViewEventsBtn->Size     = System::Drawing::Size((int)(W*0.50),(int)(H*0.13));
		this->ViewEventsBtn->Text     = L"Browse Events";
		this->ViewEventsBtn->UseVisualStyleBackColor = true;
		this->ViewEventsBtn->Click   += gcnew System::EventHandler(this, &StartingPage::ViewEvents_Click);
		this->Controls->Add(ViewEventsBtn);
		this->MyEventsBtn->Location = System::Drawing::Point((int)(W*0.25),(int)(H*.45));
		this->MyEventsBtn->Size     = System::Drawing::Size((int)(W*0.50),(int)(H*0.13));
		this->MyEventsBtn->Text     = L"My Events";
		this->MyEventsBtn->UseVisualStyleBackColor = true;
		this->MyEventsBtn->Click   += gcnew System::EventHandler(this, &StartingPage::MyEvents_Click);
		this->Controls->Add(MyEventsBtn);
		this->OrganizerBtn->Location = System::Drawing::Point((int)(W*0.25),(int)(H*.62));
		this->OrganizerBtn->Size     = System::Drawing::Size((int)(W*0.50),(int)(H*0.13));
		this->OrganizerBtn->Text     = L"Organizer Dashboard";
		this->OrganizerBtn->UseVisualStyleBackColor = true;
		this->OrganizerBtn->Click   += gcnew System::EventHandler(this, &StartingPage::OrganizerDashboard_Click);
		this->Controls->Add(OrganizerBtn);
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
