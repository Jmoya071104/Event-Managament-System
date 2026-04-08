#pragma once
#include "EventManager.h"

namespace GroupProject {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for EventsPage
	/// </summary>
	public ref class EventsPage : public System::Windows::Forms::Form
	{
	private:
		EventManager^ Manager;
		Form^ ParentForm;
	public:
		EventsPage(EventManager^ ExistingManager, Form^ parent)
		{
			InitializeComponent();
			Manager = ExistingManager;
			ParentForm = parent;
			this->FormClosed += gcnew FormClosedEventHandler(this, &EventsPage::OnFormClosed);
			//
			//TODO: Add the constructor code here
			//
		}

	protected:


		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		/// 
		~EventsPage()
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

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->SuspendLayout();
			// 
			// EventsPage
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(282, 253);
			this->Name = L"EventsPage";
			this->Text = L"EventsPage";
			this->Load += gcnew System::EventHandler(this, &EventsPage::EventsPage_Load);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void EventsPage_Load(System::Object^ sender, System::EventArgs^ e) {
		this->WindowState = FormWindowState::Maximized;
		float windowWidth = this->ClientSize.Width;
		float windowHeight = this->ClientSize.Height;

		Label^ UpcomingEventsLabel = gcnew Label();
		Panel^ FiltersPanel = gcnew Panel();
		Panel^ DisplayEventsPanel = gcnew Panel();

		//UpcomingEventsLabel
		{
			UpcomingEventsLabel->Font = (gcnew System::Drawing::Font(L"Times New Roman", 12.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			UpcomingEventsLabel->Location = System::Drawing::Point(windowWidth * 0.35, 0);
			UpcomingEventsLabel->Size = System::Drawing::Size(windowWidth * 0.65, windowHeight * 0.15);
			UpcomingEventsLabel->Text = L"Upcoming Events";
			UpcomingEventsLabel->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->Controls->Add(UpcomingEventsLabel);
		}
		
		//FiltersPanel
		{			
			
			Label^ FiltersTitleLbl = gcnew Label();

			Label^ SearchByNameLbl = gcnew Label();
			TextBox^ SearchByNameTB = gcnew TextBox();



			FiltersPanel->Size = System::Drawing::Size(windowWidth * .35, windowHeight);
			FiltersPanel->Location = Point(0, 0);
			FiltersPanel->Visible = true;
			FiltersPanel->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->Controls->Add(FiltersPanel);
			FiltersPanel->BringToFront();

			float filterPanelWidth = FiltersPanel->Width;
			float filterPanelHeight = FiltersPanel->Height;

			//Title
			{
				FiltersTitleLbl->Font = (gcnew System::Drawing::Font(L"Times New Roman", 12.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
					static_cast<System::Byte>(0)));
				FiltersTitleLbl->Location = System::Drawing::Point(filterPanelWidth * .05, filterPanelHeight * .025 );
				FiltersTitleLbl->Size = System::Drawing::Size( filterPanelWidth * .9, filterPanelHeight * .15);
				FiltersTitleLbl->Text = L"Filters";
				FiltersTitleLbl->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
				FiltersTitleLbl->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
				FiltersPanel->Controls->Add(FiltersTitleLbl);
			}
			
			//SearchByName
			{
				SearchByNameLbl->Font = (gcnew System::Drawing::Font(L"Times New Roman", 12.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
					static_cast<System::Byte>(0)));
				SearchByNameLbl->Location = System::Drawing::Point(filterPanelWidth * .05, filterPanelHeight * .18);
				SearchByNameLbl->Size = System::Drawing::Size(filterPanelWidth * .4, filterPanelHeight * .065);
				SearchByNameLbl->Text = L"Search By Event Name";
				SearchByNameLbl->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
				SearchByNameLbl->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
				FiltersPanel->Controls->Add(SearchByNameLbl);


				SearchByNameTB->Font = (gcnew System::Drawing::Font(
					L"Microsoft Sans Serif", 12.0F,
					System::Drawing::FontStyle::Regular,
					System::Drawing::GraphicsUnit::Point,
					static_cast<System::Byte>(0)));
				SearchByNameTB->Name = "EventNameTB";
				SearchByNameTB->Location = System::Drawing::Point(filterPanelWidth * .05, filterPanelHeight * .3);
				SearchByNameTB->Size = System::Drawing::Size(filterPanelWidth * .4, filterPanelHeight * 1);
				SearchByNameTB->BringToFront();
				FiltersPanel->Controls->Add(SearchByNameTB);
			}

			//SearchByLocation
			{}

			//SearchByEventRunner
			{}

			//SearchByDateRange
			{}

			//SearchByExactDate
			{}

			//FilterBTN
			
		}

		//DisplayEventsPanel
		{
			DisplayEventsPanel->Size = System::Drawing::Size(windowWidth * .65, windowHeight * 0.85);
			DisplayEventsPanel->Location = Point(windowWidth * 0.35, windowHeight * 0.15);
			DisplayEventsPanel->Visible = true;
			DisplayEventsPanel->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			DisplayEventsPanel->AutoScroll = true;
			this->Controls->Add(DisplayEventsPanel);
			DisplayEventsPanel->BringToFront();

			displayEvents(DisplayEventsPanel, Manager->events);
		}
	

	}

	private: System::Void OnFormClosed(System::Object^ sender, System::Windows::Forms::FormClosedEventArgs^ e)
	{
		if (ParentForm != nullptr)
		{
			ParentForm->Show();
		}
	}

		   void displayEvents(Panel^ DisplayPanel, List<EventClass^>^ events) {


			   if (events->Count == 0) {
				   return;
			   }
			   float width = DisplayPanel->Width;
			   float height = DisplayPanel->Height;
			   float blankSpace = height * 0.05;

			   for (int i = 0; i < events->Count; i++) {
				   Panel^ IndividualEventPanel = gcnew Panel();

				   IndividualEventPanel->Size = System::Drawing::Size(width * 0.8, height * 0.3);
				   IndividualEventPanel->Location = Point(width * 0.1, blankSpace + i *(height * 0.3 + blankSpace));
				   IndividualEventPanel->Visible = true;
				   IndividualEventPanel->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
				   DisplayPanel->Controls->Add(IndividualEventPanel);
				   IndividualEventPanel->BringToFront();

				   float InPanelWidth = IndividualEventPanel->Width;
				   float InPanelHeight = IndividualEventPanel->Height;

				   Label^ Title = gcnew Label();
				   TextBox^ PlaceHolder = gcnew TextBox();
				   Label^ Location = gcnew Label();
				   Label^ StartTime = gcnew Label();
				   PictureBox^ Logo = gcnew PictureBox();
				   TextBox^ Desc = gcnew TextBox();

				   //Title
				   {
					   Title->Font = (gcnew System::Drawing::Font(L"Times New Roman", 12.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
						   static_cast<System::Byte>(0)));
					   Title->Location = System::Drawing::Point(InPanelWidth * 0.35, InPanelHeight * 0.1);
					   Title->Size = System::Drawing::Size(InPanelWidth * 0.55, InPanelHeight * 0.1);
					   Title->Text = events[i]->Name;
					   Title->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
					   Title->BorderStyle = BorderStyle::FixedSingle;
					   IndividualEventPanel->Controls->Add(Title);
				   }

				   //Logo
				   {
					   Logo->Size = System::Drawing::Size(InPanelHeight * 0.9, InPanelHeight * 0.8);
					   Logo->Location = Point(InPanelWidth * 0.05, InPanelHeight * 0.1);
					   Logo->BorderStyle = BorderStyle::FixedSingle;
					   Logo->Image = events[i]->Icon;
					   Logo->SizeMode = PictureBoxSizeMode::StretchImage;
					   IndividualEventPanel->Controls->Add(Logo);
				   }

				   //Location
				   {
					   Location->Font = (gcnew System::Drawing::Font(L"Times New Roman", 12.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
						   static_cast<System::Byte>(0)));
					   Location->Location = System::Drawing::Point(InPanelWidth * 0.35, InPanelHeight * 0.2);
					   Location->Size = System::Drawing::Size(InPanelWidth * 0.55, InPanelHeight * 0.1);
					   Location->Text = "Location: " + events[i]->Location;
					   Location->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
					   Location->BorderStyle = BorderStyle::FixedSingle;

					   IndividualEventPanel->Controls->Add(Location);
				   }

				   //StartTime
				   {
					   StartTime->Font = (gcnew System::Drawing::Font(L"Times New Roman", 12.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
						   static_cast<System::Byte>(0)));
					   StartTime->Location = System::Drawing::Point(InPanelWidth * 0.35, InPanelHeight * 0.3);
					   StartTime->Size = System::Drawing::Size(InPanelWidth * 0.55, InPanelHeight * 0.1);
					   StartTime->Text = "From: " + events[i]->StartDate.ToString("MMMM dd, hh:mm tt") + " to " + events[i]->EndDate.ToString("MMMM dd, hh:mm tt");
					   StartTime->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
					   StartTime->BorderStyle = BorderStyle::FixedSingle;

					   IndividualEventPanel->Controls->Add(StartTime);
				   }

				  
				   //Description
				   {
					   Desc->Size = System::Drawing::Size(InPanelWidth * 0.60, InPanelHeight * 0.5);
					   Desc->Location = Point(InPanelWidth * 0.35, InPanelHeight * 0.4);
					   Desc->Visible = true;
					   Desc->Multiline = true;
					   Desc->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
					   Desc->Font = gcnew System::Drawing::Font(
						   L"Microsoft Sans Serif",
						   12.0F,
						   System::Drawing::FontStyle::Regular,
						   System::Drawing::GraphicsUnit::Point
					   );
					   Desc->ReadOnly = true;
					   Desc->ScrollBars = ScrollBars::Vertical;
					   Desc->Text = events[i]->Description;
					   IndividualEventPanel->Controls->Add(Desc);

				   }

			   }

	}

	
		   
	};
}
