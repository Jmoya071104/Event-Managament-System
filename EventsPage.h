#pragma once
#include "AppState.h"
#include "PersistenceService.h"
#include "EventDetailsPage.h"
#include "MyEventsPage.h"

namespace GroupProject {

	using namespace System;
	using namespace System::Windows::Forms;
	using namespace System::Drawing;
	using namespace System::Collections::Generic;

	public ref class EventsPage : public System::Windows::Forms::Form
	{
	private:
		Form^            _callerForm;
		TextBox^         _searchTB;
		ComboBox^        _categoryCB;
		FlowLayoutPanel^ _eventsFLP;

	public:
		EventsPage(Form^ callerForm)
		{
			_callerForm = callerForm;
			InitializeComponent();
		}

	protected:
		~EventsPage()
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
			this->ClientSize          = System::Drawing::Size(1100, 750);
			this->Name                = L"EventsPage";
			this->Text                = L"Browse Events";
			this->Load               += gcnew System::EventHandler(this, &EventsPage::EventsPage_Load);
			this->FormClosed         += gcnew FormClosedEventHandler(this, &EventsPage::OnFormClosed);
			this->ResumeLayout(false);
		}

		void EventsPage_Load(System::Object^ sender, System::EventArgs^ e)
		{
			this->WindowState = FormWindowState::Maximized;
			BuildUI();
		}

		void OnFormClosed(System::Object^ sender, System::Windows::Forms::FormClosedEventArgs^ e)
		{
			if (_callerForm != nullptr)
				_callerForm->Show();
		}

		void BuildUI()
		{
			int W       = this->ClientSize.Width;
			int H       = this->ClientSize.Height;
			int filterW = (int)(W * 0.27);

			// ── Left filter panel ────────────────────────────────────────────
			Panel^ filtersPanel = gcnew Panel();
			filtersPanel->Size        = System::Drawing::Size(filterW, H);
			filtersPanel->Location    = Point(0, 0);
			filtersPanel->BorderStyle = BorderStyle::FixedSingle;
			filtersPanel->BackColor   = Color::WhiteSmoke;
			this->Controls->Add(filtersPanel);

			Label^ filterTitleLbl     = gcnew Label();
			filterTitleLbl->Text      = L"Filters";
			filterTitleLbl->Font      = gcnew Drawing::Font(L"Segoe UI", 14.0F, FontStyle::Bold);
			filterTitleLbl->TextAlign = ContentAlignment::MiddleCenter;
			filterTitleLbl->Size      = System::Drawing::Size(filterW - 20, 50);
			filterTitleLbl->Location  = Point(10, 15);
			filtersPanel->Controls->Add(filterTitleLbl);

			Label^ searchLbl    = gcnew Label();
			searchLbl->Text     = L"Search Events:";
			searchLbl->Font     = gcnew Drawing::Font(L"Segoe UI", 10.0F);
			searchLbl->Size     = System::Drawing::Size(filterW - 20, 25);
			searchLbl->Location = Point(10, 80);
			filtersPanel->Controls->Add(searchLbl);

			_searchTB           = gcnew TextBox();
			_searchTB->Font     = gcnew Drawing::Font(L"Segoe UI", 10.0F);
			_searchTB->Size     = System::Drawing::Size(filterW - 20, 30);
			_searchTB->Location = Point(10, 108);
			filtersPanel->Controls->Add(_searchTB);

			Label^ catLbl    = gcnew Label();
			catLbl->Text     = L"Category:";
			catLbl->Font     = gcnew Drawing::Font(L"Segoe UI", 10.0F);
			catLbl->Size     = System::Drawing::Size(filterW - 20, 25);
			catLbl->Location = Point(10, 152);
			filtersPanel->Controls->Add(catLbl);

			_categoryCB                = gcnew ComboBox();
			_categoryCB->Font          = gcnew Drawing::Font(L"Segoe UI", 10.0F);
			_categoryCB->Size          = System::Drawing::Size(filterW - 20, 30);
			_categoryCB->Location      = Point(10, 180);
			_categoryCB->DropDownStyle = ComboBoxStyle::DropDownList;
			_categoryCB->Items->Add(L"All Categories");
			_categoryCB->Items->Add(EventCategories::Music);
			_categoryCB->Items->Add(EventCategories::Sports);
			_categoryCB->Items->Add(EventCategories::Tech);
			_categoryCB->Items->Add(EventCategories::Art);
			_categoryCB->Items->Add(EventCategories::Food);
			_categoryCB->Items->Add(EventCategories::Academic);
			_categoryCB->Items->Add(EventCategories::Other);
			_categoryCB->SelectedIndex = 0;
			filtersPanel->Controls->Add(_categoryCB);

			Button^ filterBtn     = gcnew Button();
			filterBtn->Text       = L"Apply Filter";
			filterBtn->Font       = gcnew Drawing::Font(L"Segoe UI", 10.0F, FontStyle::Bold);
			filterBtn->Size       = System::Drawing::Size(filterW - 20, 38);
			filterBtn->Location   = Point(10, 228);
			filterBtn->BackColor  = Color::SteelBlue;
			filterBtn->ForeColor  = Color::White;
			filterBtn->FlatStyle  = FlatStyle::Flat;
			filterBtn->Click     += gcnew System::EventHandler(this, &EventsPage::FilterBtn_Click);
			filtersPanel->Controls->Add(filterBtn);

			Button^ myEventsBtn    = gcnew Button();
			myEventsBtn->Text      = L"My Events";
			myEventsBtn->Font      = gcnew Drawing::Font(L"Segoe UI", 10.0F, FontStyle::Bold);
			myEventsBtn->Size      = System::Drawing::Size(filterW - 20, 38);
			myEventsBtn->Location  = Point(10, 278);
			myEventsBtn->BackColor = Color::DarkSlateGray;
			myEventsBtn->ForeColor = Color::White;
			myEventsBtn->FlatStyle = FlatStyle::Flat;
			myEventsBtn->Click    += gcnew System::EventHandler(this, &EventsPage::MyEventsBtn_Click);
			filtersPanel->Controls->Add(myEventsBtn);

			Button^ backBtn    = gcnew Button();
			backBtn->Text      = L"< Back";
			backBtn->Font      = gcnew Drawing::Font(L"Segoe UI", 10.0F);
			backBtn->Size      = System::Drawing::Size(filterW - 20, 38);
			backBtn->Location  = Point(10, 328);
			backBtn->FlatStyle = FlatStyle::Flat;
			backBtn->Click    += gcnew System::EventHandler(this, &EventsPage::BackBtn_Click);
			filtersPanel->Controls->Add(backBtn);

			// ── Right events area ────────────────────────────────────────────
			int rightX = filterW + 2;
			int rightW = W - rightX;

			Label^ titleLbl     = gcnew Label();
			titleLbl->Text      = L"Upcoming Events";
			titleLbl->Font      = gcnew Drawing::Font(L"Segoe UI", 16.0F, FontStyle::Bold);
			titleLbl->TextAlign = ContentAlignment::MiddleCenter;
			titleLbl->Size      = System::Drawing::Size(rightW, 55);
			titleLbl->Location  = Point(rightX, 0);
			this->Controls->Add(titleLbl);

			_eventsFLP               = gcnew FlowLayoutPanel();
			_eventsFLP->Size         = System::Drawing::Size(rightW - 10, H - 60);
			_eventsFLP->Location     = Point(rightX + 5, 58);
			_eventsFLP->FlowDirection = FlowDirection::TopDown;
			_eventsFLP->AutoScroll   = true;
			_eventsFLP->WrapContents = false;
			this->Controls->Add(_eventsFLP);

			DisplayEvents(AppState::Manager->GetAllEvents());
		}

		void DisplayEvents(List<EventClass^>^ events)
		{
			_eventsFLP->Controls->Clear();
			int cardW = _eventsFLP->Width - 25;

			if (events->Count == 0)
			{
				Label^ empty     = gcnew Label();
				empty->Text      = L"No events match your search.";
				empty->Font      = gcnew Drawing::Font(L"Segoe UI", 12.0F);
				empty->ForeColor = Color::Gray;
				empty->Size      = System::Drawing::Size(cardW, 60);
				empty->TextAlign = ContentAlignment::MiddleCenter;
				_eventsFLP->Controls->Add(empty);
				return;
			}

			for each (EventClass^ ev in events)
				_eventsFLP->Controls->Add(MakeEventCard(ev, cardW));
		}

		Panel^ MakeEventCard(EventClass^ ev, int cardW)
		{
			int cardH = 130;

			Panel^ card     = gcnew Panel();
			card->Size      = System::Drawing::Size(cardW, cardH);
			card->BorderStyle = BorderStyle::FixedSingle;
			card->BackColor = Color::White;
			card->Margin    = System::Windows::Forms::Padding(0, 0, 0, 6);

			// Thumbnail
			PictureBox^ thumb  = gcnew PictureBox();
			thumb->Size        = System::Drawing::Size(100, 100);
			thumb->Location    = Point(8, 15);
			thumb->SizeMode    = PictureBoxSizeMode::StretchImage;
			thumb->Image       = ev->Icon;
			if (ev->Icon == nullptr) thumb->BackColor = Color::LightGray;
			card->Controls->Add(thumb);

			int textX  = 118;
			int btnRightEdge = cardW - 10;

			// Title
			Label^ titleLbl        = gcnew Label();
			titleLbl->Text         = ev->Name;
			titleLbl->Font         = gcnew Drawing::Font(L"Segoe UI", 11.0F, FontStyle::Bold);
			titleLbl->Location     = Point(textX, 8);
			titleLbl->Size         = System::Drawing::Size(btnRightEdge - textX - 240, 24);
			titleLbl->AutoEllipsis = true;
			card->Controls->Add(titleLbl);

			// Organizer | Location
			Label^ metaLbl        = gcnew Label();
			metaLbl->Text         = ev->Runner + L"  |  " + ev->Location;
			metaLbl->Font         = gcnew Drawing::Font(L"Segoe UI", 9.0F);
			metaLbl->ForeColor    = Color::DimGray;
			metaLbl->Location     = Point(textX, 34);
			metaLbl->Size         = System::Drawing::Size(btnRightEdge - textX - 240, 20);
			metaLbl->AutoEllipsis = true;
			card->Controls->Add(metaLbl);

			// Date range
			Label^ dateLbl        = gcnew Label();
			dateLbl->Text         = ev->StartDateTime.ToString(L"MMM dd, yyyy h:mm tt")
			                      + L"  \x2192  "
			                      + ev->EndDateTime.ToString(L"MMM dd, yyyy h:mm tt");
			dateLbl->Font         = gcnew Drawing::Font(L"Segoe UI", 9.0F);
			dateLbl->ForeColor    = Color::DimGray;
			dateLbl->Location     = Point(textX, 56);
			dateLbl->Size         = System::Drawing::Size(btnRightEdge - textX - 240, 20);
			dateLbl->AutoEllipsis = true;
			card->Controls->Add(dateLbl);

			// Capacity
			String^ capText = (ev->Capacity == 0)
			    ? L"Capacity: Unlimited"
			    : L"Capacity: " + ev->RegisteredCount + L" / " + ev->Capacity;
			Label^ capLbl     = gcnew Label();
			capLbl->Text      = capText;
			capLbl->Font      = gcnew Drawing::Font(L"Segoe UI", 9.0F);
			capLbl->ForeColor = Color::DimGray;
			capLbl->Location  = Point(textX, 78);
			capLbl->Size      = System::Drawing::Size(btnRightEdge - textX - 240, 20);
			card->Controls->Add(capLbl);

			// Category badge
			Label^ badge     = gcnew Label();
			badge->Text      = ev->Category;
			badge->Font      = gcnew Drawing::Font(L"Segoe UI", 8.5F, FontStyle::Bold);
			badge->ForeColor = Color::White;
			badge->BackColor = EventDetailsPage::CategoryColor(ev->Category);
			badge->Size      = System::Drawing::Size(90, 22);
			badge->Location  = Point(textX, 100);
			badge->TextAlign = ContentAlignment::MiddleCenter;
			card->Controls->Add(badge);

			// View Details button
			Button^ detailsBtn    = gcnew Button();
			detailsBtn->Text      = L"View Details";
			detailsBtn->Font      = gcnew Drawing::Font(L"Segoe UI", 9.0F, FontStyle::Bold);
			detailsBtn->Size      = System::Drawing::Size(115, 34);
			detailsBtn->Location  = Point(btnRightEdge - 240, 48);
			detailsBtn->BackColor = Color::SteelBlue;
			detailsBtn->ForeColor = Color::White;
			detailsBtn->FlatStyle = FlatStyle::Flat;
			detailsBtn->Tag       = ev->Id.ToString();
			detailsBtn->Click    += gcnew System::EventHandler(this, &EventsPage::ViewDetails_Click);
			card->Controls->Add(detailsBtn);

			// Save / Unsave toggle
			bool saved         = AppState::Session->IsSaved(ev->Id);
			Button^ saveBtn    = gcnew Button();
			saveBtn->Text      = saved ? L"\u2605 Saved" : L"\u2606 Save";
			saveBtn->Font      = gcnew Drawing::Font(L"Segoe UI", 9.0F);
			saveBtn->Size      = System::Drawing::Size(115, 34);
			saveBtn->Location  = Point(btnRightEdge - 118, 48);
			saveBtn->BackColor = saved ? Color::Gold : Color::LightGray;
			saveBtn->FlatStyle = FlatStyle::Flat;
			saveBtn->Tag       = ev->Id.ToString();
			saveBtn->Click    += gcnew System::EventHandler(this, &EventsPage::SaveToggle_Click);
			card->Controls->Add(saveBtn);

			return card;
		}

		// ── Event handlers ───────────────────────────────────────────────────

		void FilterBtn_Click(System::Object^ sender, System::EventArgs^ e)
		{
			String^ query    = _searchTB->Text->Trim();
			String^ category = safe_cast<String^>(_categoryCB->SelectedItem);
			DisplayEvents(AppState::Manager->SearchAndFilter(query, category));
		}

		void ViewDetails_Click(System::Object^ sender, System::EventArgs^ e)
		{
			Button^ btn = safe_cast<Button^>(sender);
			int id = Int32::Parse(safe_cast<String^>(btn->Tag));
			EventDetailsPage^ page = gcnew EventDetailsPage(id, this);
			this->Hide();
			page->Show();
		}

		void SaveToggle_Click(System::Object^ sender, System::EventArgs^ e)
		{
			Button^ btn = safe_cast<Button^>(sender);
			int id = Int32::Parse(safe_cast<String^>(btn->Tag));
			if (AppState::Session->IsSaved(id))
				AppState::Manager->UnsaveEvent(id, AppState::Session);
			else
				AppState::Manager->SaveEvent(id, AppState::Session);
			PersistenceService::Save(AppState::Manager, AppState::Session);
			bool nowSaved  = AppState::Session->IsSaved(id);
			btn->Text      = nowSaved ? L"\u2605 Saved" : L"\u2606 Save";
			btn->BackColor = nowSaved ? Color::Gold : Color::LightGray;
		}

		void MyEventsBtn_Click(System::Object^ sender, System::EventArgs^ e)
		{
			MyEventsPage^ page = gcnew MyEventsPage(this);
			this->Hide();
			page->Show();
		}

		void BackBtn_Click(System::Object^ sender, System::EventArgs^ e)
		{
			if (_callerForm != nullptr)
				_callerForm->Show();
			this->Close();
		}
	};
}
