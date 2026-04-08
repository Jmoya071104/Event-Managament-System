#pragma once
#include "EventClass.h"

using namespace System;
using namespace System::Collections::Generic;

public ref class EventManager {
public: 
	List<EventClass^>^ events;

	EventManager() {
		events = gcnew List<EventClass^>;

	}

	void addEvent(EventClass^ newEvent) {
		events->Add(newEvent);
		SortEventsByStartDate();
	}

	
	static int CompareByStartDate(EventClass^ a, EventClass^ b) {
		return DateTime::Compare(a->StartDate, b->StartDate);
	}

	void SortEventsByStartDate() {
		events->Sort(gcnew Comparison<EventClass^>(CompareByStartDate));
	}


};
