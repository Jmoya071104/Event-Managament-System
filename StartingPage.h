#pragma once
#include "AppState.h"
#include "SeedData.h"
#include "PersistenceService.h"
#include "EventDetailsPage.h"
#include "MyEventsPage.h"
#include "EventsPage.h"
#include "EventManager.h"
#include "EventClass.h"

namespace GroupProject {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for StartingPage
	/// </summary>
	public ref class StartingPage : public System::Windows::Forms::Form
	{
	public:
		StartingPage(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~StartingPage()
		{
			if (components)
			{
				delete components;
			}
		}

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

		Label^ TitleLabel = gcnew Label();

		Button^ CreateEventBtn  = gcnew Button();
		Button^ ViewEventsBtn   = gcnew Button();
		Button^ MyEventsBtn     = gcnew Button();
		Button^ ExitBtn         = gcnew Button();

		// EditEventsBtn removed — Phase 2 scope




#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->SuspendLayout();
			// 
			// StartingPage
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(282, 253);
			this->Name = L"StartingPage";
			this->Text = L"StartingPage";
			this->Load += gcnew System::EventHandler(this, &StartingPage::StartingPage_Load);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void StartingPage_Load(System::Object^ sender, System::EventArgs^ e) {
		// --- Initialize shared state and load/seed data on first launch ---
		AppState::Initialize();
		bool hadSavedData = PersistenceService::Load(AppState::Manager, AppState::Session);
		if (!hadSavedData)
			SeedData::Seed(AppState::Manager);

		this->WindowState = FormWindowState::Maximized;

		float windowWidth  = this->ClientSize.Width;
		float windowHeight = this->ClientSize.Height;

		// Title Label
		this->TitleLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 25,
			System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
			static_cast<System::Byte>(0)));
		this->TitleLabel->Location = System::Drawing::Point(windowWidth * 0.25, 0);
		this->TitleLabel->Size = System::Drawing::Size(windowWidth * 0.5, windowHeight * 0.20);
		this->TitleLabel->TabIndex = 0;
		this->TitleLabel->Text = L"Event Management System";
		this->TitleLabel->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
		TitleLabel->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
		this->Controls->Add(TitleLabel);

		// Browse Events button (attendee entry)
		this->ViewEventsBtn->Location = System::Drawing::Point(windowWidth * 0.25, windowHeight * .28);
		this->ViewEventsBtn->Size = System::Drawing::Size(windowWidth * 0.50, windowHeight * 0.13);
		this->ViewEventsBtn->TabIndex = 1;
		this->ViewEventsBtn->Text = L"Browse Events";
		this->ViewEventsBtn->UseVisualStyleBackColor = true;
		this->ViewEventsBtn->Click += gcnew System::EventHandler(this, &StartingPage::ViewEvents_Click);
		this->Controls->Add(ViewEventsBtn);

		// My Events button
		this->MyEventsBtn->Location = System::Drawing::Point(windowWidth * 0.25, windowHeight * .45);
		this->MyEventsBtn->Size = System::Drawing::Size(windowWidth * 0.50, windowHeight * 0.13);
		this->MyEventsBtn->TabIndex = 2;
		this->MyEventsBtn->Text = L"My Events";
		this->MyEventsBtn->UseVisualStyleBackColor = true;
		this->MyEventsBtn->Click += gcnew System::EventHandler(this, &StartingPage::MyEvents_Click);
		this->Controls->Add(MyEventsBtn);

		// Create Event button (organizer — secondary)
		this->CreateEventBtn->Location = System::Drawing::Point(windowWidth * 0.25, windowHeight * .62);
		this->CreateEventBtn->Size = System::Drawing::Size(windowWidth * 0.50, windowHeight * 0.13);
		this->CreateEventBtn->TabIndex = 3;
		this->CreateEventBtn->Text = L"Create Event (Organizer)";
		this->CreateEventBtn->UseVisualStyleBackColor = true;
		this->CreateEventBtn->Click += gcnew System::EventHandler(this, &StartingPage::createEvent_Click);
		this->Controls->Add(CreateEventBtn);
	}

		   //Create Event Button
	private: System::Void createEvent_Click(System::Object^ sender, System::EventArgs^ e) {
		float windowWidth = this->ClientSize.Width;
		float windowHeight = this->ClientSize.Height;
		Panel^ CreateEventPanel = gcnew Panel();

		CreateEventPanel->Size = System::Drawing::Size(windowWidth * .5 , windowHeight * .5);
		CreateEventPanel->Location = Point(windowWidth * 0.25, windowHeight * .3);
		CreateEventPanel->Visible = true;
		CreateEventPanel->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
		this->Controls->Add(CreateEventPanel);
		CreateEventPanel->BringToFront();

		Label^ PanelTitle = gcnew Label();

		Label^ EventName = gcnew Label();
		TextBox^ EventNameTB = gcnew TextBox();

		Label^ EventRunner = gcnew Label();
		TextBox^ EventRunnerTB = gcnew TextBox();

		Label^ ThumbnailImageLbl = gcnew Label();
		Label^ Preview = gcnew Label();
		PictureBox^ ThumbnailImage = gcnew PictureBox();
		Button^ UploadBTN = gcnew Button();

		Label^ EventLocation = gcnew Label();
		TextBox^ EventLocationTB = gcnew TextBox();

		Label^ DescriptionTitle = gcnew Label();
		TextBox^ DescriptionTB = gcnew TextBox();

		Label^ StartLbl = gcnew Label();
		DateTimePicker^ Start = gcnew DateTimePicker();

		Label^ EndLbl = gcnew Label();
		DateTimePicker^ End = gcnew DateTimePicker();

		Button^ CreateEventBTN = gcnew Button();
		Button^ CancelBTN = gcnew Button();

		//Panel Title
		{
			PanelTitle->Font = (gcnew System::Drawing::Font(L"Times New Roman", 12.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			PanelTitle->Location = System::Drawing::Point(0, 0);
			PanelTitle->Size = System::Drawing::Size(CreateEventPanel->Width * .20, CreateEventPanel->Height * 0.1);
			PanelTitle->TabIndex = 0;
			PanelTitle->Text = L"Create a New Event";
			PanelTitle->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			PanelTitle->AutoSize = true;
			CreateEventPanel->Controls->Add(PanelTitle);
		}

		//EventName and text box
		{
			
			EventName->Location = System::Drawing::Point(0, CreateEventPanel->Height * 0.1);
			EventName->Size = System::Drawing::Size(CreateEventPanel->Width * .15, CreateEventPanel->Height * 0.05);
			EventName->Font = (gcnew System::Drawing::Font(L"Times New Roman", 12.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			EventName->TabIndex = 0;
			EventName->Text = L"Event Name";
			EventName->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			CreateEventPanel->Controls->Add(EventName);

			EventNameTB->Font = (gcnew System::Drawing::Font(
				L"Microsoft Sans Serif", 12.0F,
				System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			EventNameTB->Name = "EventNameTB";
			EventNameTB->Location = System::Drawing::Point(CreateEventPanel->Width * 0.15, CreateEventPanel->Height * 0.1);
			EventNameTB->Size = System::Drawing::Size(CreateEventPanel->Width * 0.30, CreateEventPanel->Height * 0.05);
			EventNameTB->BringToFront();
			CreateEventPanel->Controls->Add(EventNameTB);
		}

		//EventRunner and text box
		{
			EventRunner->Font = (gcnew System::Drawing::Font(L"Times New Roman", 12.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			EventRunner->Location = System::Drawing::Point(CreateEventPanel->Width * 0.50, CreateEventPanel->Height * 0.1);
			EventRunner->Size = System::Drawing::Size(CreateEventPanel->Width * .190, CreateEventPanel->Height * 0.05);
			EventRunner->Text = L"Event Runner";
			EventRunner->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			CreateEventPanel->Controls->Add(EventRunner);

			EventRunnerTB->Font = (gcnew System::Drawing::Font(
				L"Microsoft Sans Serif", 12.0F,
				System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			EventRunnerTB->Name = "EventRunnerTB";
			EventRunnerTB->Location = System::Drawing::Point(CreateEventPanel->Width * 0.69, CreateEventPanel->Height * 0.1);
			EventRunnerTB->Size = System::Drawing::Size(CreateEventPanel->Width * 0.30, CreateEventPanel->Height * 0.05);
			CreateEventPanel->Controls->Add(EventRunnerTB);

		}

		//Thumbnail and Upload BTN
		{
			ThumbnailImageLbl->Font = (gcnew System::Drawing::Font(L"Times New Roman", 12.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			ThumbnailImageLbl->Location = System::Drawing::Point(0, CreateEventPanel->Height * 0.18);
			ThumbnailImageLbl->Size = System::Drawing::Size(CreateEventPanel->Width * .20, CreateEventPanel->Height * 0.1);
			ThumbnailImageLbl->TabIndex = 0;
			ThumbnailImageLbl->Text = L"Thumbnail Image (Optional)";
			ThumbnailImageLbl->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			ThumbnailImageLbl->AutoSize = true;
			CreateEventPanel->Controls->Add(ThumbnailImageLbl);

			Preview->Font = (gcnew System::Drawing::Font(L"Times New Roman", 12.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			Preview->Location = System::Drawing::Point(CreateEventPanel->Width * 0.045, CreateEventPanel->Height * 0.35);
			Preview->Size = System::Drawing::Size(CreateEventPanel->Height * 0.35, CreateEventPanel->Height * 0.1);
			Preview->TabIndex = 0;
			Preview->Text = L"Image Preview";
			Preview->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			Preview->AutoSize = true;
			CreateEventPanel->Controls->Add(Preview);
			
			ThumbnailImage->Name = "ThumbnailImage"; // MUST match the string you used
			ThumbnailImage->Size = System::Drawing::Size(CreateEventPanel->Height * 0.35, CreateEventPanel->Height * 0.35);
			ThumbnailImage->Location = Point(CreateEventPanel->Width * 0.025, CreateEventPanel->Height * 0.4);
			ThumbnailImage->BorderStyle = BorderStyle::FixedSingle;
			CreateEventPanel->Controls->Add(ThumbnailImage);

			UploadBTN->Location = System::Drawing::Point(CreateEventPanel->Width * 0.30, CreateEventPanel->Height * 0.17);
			UploadBTN->Size = System::Drawing::Size(CreateEventPanel->Width * 0.1, CreateEventPanel->Height * 0.10);
			UploadBTN->TabIndex = 1;
			UploadBTN->Text = L"Upload";
			UploadBTN->UseVisualStyleBackColor = true;
			UploadBTN->BringToFront();
			UploadBTN->Click += gcnew System::EventHandler(this, &StartingPage::UploadImageBtn_Click);
			CreateEventPanel->Controls->Add(UploadBTN);


		}

		//Event Location and text box
		{
			EventLocation->Font = (gcnew System::Drawing::Font(L"Times New Roman", 12.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			EventLocation->Location = System::Drawing::Point(CreateEventPanel->Width * 0.50, CreateEventPanel->Height * 0.18);
			EventLocation->Size = System::Drawing::Size(CreateEventPanel->Width * .190, CreateEventPanel->Height * 0.1);
			EventLocation->Text = L"Event Location";
			EventLocation->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			CreateEventPanel->Controls->Add(EventLocation);

			EventLocationTB->Font = (gcnew System::Drawing::Font(
				L"Microsoft Sans Serif", 12.0F,
				System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			EventLocationTB->Name = "EventLocationTB";
			EventLocationTB->Location = System::Drawing::Point(CreateEventPanel->Width * 0.69, CreateEventPanel->Height * 0.18);
			EventLocationTB->Size = System::Drawing::Size(CreateEventPanel->Width * 0.30, CreateEventPanel->Height * 0.05);
			CreateEventPanel->Controls->Add(EventLocationTB);

		}


		//Description Label
		{
			DescriptionTitle->Font = (gcnew System::Drawing::Font(L"Times New Roman", 12.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			DescriptionTitle->Location = System::Drawing::Point(CreateEventPanel->Width * 0.25, CreateEventPanel->Height * 0.30);
			DescriptionTitle->Size = System::Drawing::Size(CreateEventPanel->Width * .2, CreateEventPanel->Height * 0.1);
			DescriptionTitle->Text = L"Description";
			DescriptionTitle->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			CreateEventPanel->Controls->Add(DescriptionTitle);

			DescriptionTB->Font = (gcnew System::Drawing::Font(
				L"Microsoft Sans Serif", 12.0F,
				System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			DescriptionTB->Name = "DescriptionTB";
			DescriptionTB->Multiline = true;
			DescriptionTB->Location = System::Drawing::Point(CreateEventPanel->Width * 0.25, CreateEventPanel->Height * 0.4);
			DescriptionTB->Size = System::Drawing::Size(CreateEventPanel->Width * 0.70, CreateEventPanel->Height * 0.35);
			CreateEventPanel->Controls->Add(DescriptionTB);
		}

		//Start Time
		{
			StartLbl->Font = (gcnew System::Drawing::Font(L"Times New Roman", 12.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			StartLbl->Location = System::Drawing::Point(0, CreateEventPanel->Height * 0.74);
			StartLbl->Size = System::Drawing::Size(CreateEventPanel->Width * .2, CreateEventPanel->Height * 0.1);
			StartLbl->Text = L"Start date and Time";
			StartLbl->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			CreateEventPanel->Controls->Add(StartLbl);

			Start->Font = (gcnew System::Drawing::Font(
				L"Microsoft Sans Serif", 12.0F,
				System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			Start->Name = "StartDT";
			Start->Location = System::Drawing::Point(CreateEventPanel->Width * .2, CreateEventPanel->Height * 0.76);
			Start->Size = System::Drawing::Size(CreateEventPanel->Width * .3, CreateEventPanel->Height * 0.1);
			Start->Format = System::Windows::Forms::DateTimePickerFormat::Custom;
			Start->CustomFormat = L"MM/dd/yyyy hh:mm tt";
			// Optional: event handler
			//this->EventStartPicker->ValueChanged += gcnew System::EventHandler(
				//this, &MyForm::EventStartPicker_ValueChanged);

			CreateEventPanel->Controls->Add(Start);
		}

		//End Time
		{
			EndLbl->Font = (gcnew System::Drawing::Font(L"Times New Roman", 12.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			EndLbl->Location = System::Drawing::Point(CreateEventPanel->Width * 0.50, CreateEventPanel->Height * 0.74);
			EndLbl->Size = System::Drawing::Size(CreateEventPanel->Width * .2, CreateEventPanel->Height * 0.1);
			EndLbl->Text = L"End date and Time";
			EndLbl->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			CreateEventPanel->Controls->Add(EndLbl);

			End->Font = (gcnew System::Drawing::Font(
				L"Microsoft Sans Serif", 12.0F,
				System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			End->Name = "EndDT";
			End->Location = System::Drawing::Point(CreateEventPanel->Width * .7, CreateEventPanel->Height * 0.76);
			End->Size = System::Drawing::Size(CreateEventPanel->Width * .3, CreateEventPanel->Height * 0.1);
			End->Format = System::Windows::Forms::DateTimePickerFormat::Custom;
			End->CustomFormat = L"MM/dd/yyyy hh:mm tt";
			// Optional: event handler
			//this->EventStartPicker->ValueChanged += gcnew System::EventHandler(
				//this, &MyForm::EventStartPicker_ValueChanged);

			CreateEventPanel->Controls->Add(End);
		}

		//CreateEventBtn
		{
			CreateEventBTN->Location = System::Drawing::Point(CreateEventPanel->Width * 0.70, CreateEventPanel->Height * 0.85);
			CreateEventBTN->Size = System::Drawing::Size(CreateEventPanel->Width * 0.3, CreateEventPanel->Height * 0.15);
			CreateEventBTN->TabIndex = 1;
			CreateEventBTN->Text = L"Create Event";
			CreateEventBTN->UseVisualStyleBackColor = true;
			CreateEventBTN->BringToFront();
			CreateEventBTN->Click += gcnew System::EventHandler(this, &StartingPage::CreateEventInstance_Click);

			CreateEventPanel->Controls->Add(CreateEventBTN);
		}

		//CancelBTN
		{
			CancelBTN->Location = System::Drawing::Point(CreateEventPanel->Width * 0, CreateEventPanel->Height * 0.85);
			CancelBTN->Size = System::Drawing::Size(CreateEventPanel->Width * 0.3, CreateEventPanel->Height * 0.15);
			CancelBTN->TabIndex = 1;
			CancelBTN->Text = L"Cancel";
			CancelBTN->UseVisualStyleBackColor = true;
			CancelBTN->BringToFront();
			CancelBTN->Click += gcnew System::EventHandler(this, &StartingPage::CreateEventCancel_Click);
			CreateEventPanel->Controls->Add(CancelBTN);
		}





	}

	private: System::Void CreateEventCancel_Click(System::Object^ sender, System::EventArgs^ e)
	{
		System::Windows::Forms::DialogResult result;

		result = MessageBox::Show(
			"Are you sure you want to cancel?",
			"Confirm",
			MessageBoxButtons::YesNo,
			MessageBoxIcon::Question
		);

		if (result == System::Windows::Forms::DialogResult::Yes) {
			Panel^ panel = dynamic_cast<Panel^>((safe_cast<Control^>(sender))->Parent);
			panel->Parent->Controls->Remove(panel);

		}
	}

	private: System::Void CreateEventInstance_Click(System::Object^ sender, System::EventArgs^ e) {
		Button^ btn = safe_cast<Button^>(sender);
		Panel^ panel = safe_cast<Panel^>(btn->Parent);

		String^ EventName        = panel->Controls["EventNameTB"]->Text;
		String^ EventRunner      = panel->Controls["EventRunnerTB"]->Text;
		String^ EventLocation    = panel->Controls["EventLocationTB"]->Text;
		String^ EventDescription = panel->Controls["DescriptionTB"]->Text;
		DateTime StartValue = safe_cast<DateTimePicker^>(panel->Controls["StartDT"])->Value;
		DateTime EndValue   = safe_cast<DateTimePicker^>(panel->Controls["EndDT"])->Value;
		Image^   icon       = safe_cast<PictureBox^>(panel->Controls["ThumbnailImage"])->Image;

		if (String::IsNullOrEmpty(EventName)
			|| String::IsNullOrEmpty(EventRunner)
			|| String::IsNullOrEmpty(EventLocation)
			|| String::IsNullOrEmpty(EventDescription))
		{
			MessageBox::Show("Please Fill In All Necessary Fields");
		}
		else if (StartValue < DateTime::Now || EndValue < DateTime::Now)
		{
			MessageBox::Show("Please Ensure Start Time and End Times are not in the past");
		}
		else if (EndValue < StartValue)
		{
			MessageBox::Show("End Date Cannot Be Before Start Date");
		}
		else
		{
			// Legacy constructor: Category defaults to "Other", Capacity = 0 (unlimited)
			EventClass^ newEvent = gcnew EventClass(
				EventName, EventRunner, EventLocation, EventDescription,
				StartValue, EndValue, icon);

			AppState::Manager->addEvent(newEvent);
			PersistenceService::Save(AppState::Manager, AppState::Session);

			MessageBox::Show("Event Created");

			panel->Controls["EventNameTB"]->Text     = L"";
			panel->Controls["EventRunnerTB"]->Text   = L"";
			panel->Controls["EventLocationTB"]->Text = L"";
			panel->Controls["DescriptionTB"]->Text   = L"";
			safe_cast<DateTimePicker^>(panel->Controls["StartDT"])->Value = DateTime::Now;
			safe_cast<DateTimePicker^>(panel->Controls["EndDT"])->Value   = DateTime::Now;
			panel->Parent->Controls->Remove(panel);
		}
	}

		   //ViewEventBTN
	private: System::Void ViewEvents_Click(System::Object^ sender, System::EventArgs^ e) {
		EventsPage^ newForm = gcnew EventsPage(this);
		this->Hide();
		newForm->Show();
	}

	private: System::Void MyEvents_Click(System::Object^ sender, System::EventArgs^ e) {
		// Forward declaration included via MyEventsPage.h
		MyEventsPage^ myPage = gcnew MyEventsPage(this);
		this->Hide();
		myPage->Show();
	}

	private: System::Void UploadImageBtn_Click(System::Object^ sender, System::EventArgs^ e) {
		Button^ btn = safe_cast<Button^>(sender);
		Panel^ panel = safe_cast<Panel^>(btn->Parent);

		OpenFileDialog^ openFile = gcnew OpenFileDialog();

		openFile->Filter = "Image Files|*.jpg;*.jpeg;*.png;*.bmp;*.gif";
		openFile->Title = "Select an Event Icon";

		if (openFile->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
			try {
				PictureBox^ pic = safe_cast<PictureBox^>(panel->Controls["ThumbnailImage"]);

				pic->Image = System::Drawing::Image::FromFile(openFile->FileName);
				pic->SizeMode = PictureBoxSizeMode::StretchImage;
			}
			catch (Exception^ ex) {
				MessageBox::Show("Error loading image: " + ex->Message);
			}
		}
	}
		   


		  

	};
}
