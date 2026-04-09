#pragma once
#include "EventManager.h"
#include "UserSession.h"

// Static shared state holder — accessible from any form without constructor chaining.
// Initialize() must be called once from StartingPage before any form is shown.
public ref class AppState
{
public:
    static EventManager^ Manager;
    static UserSession^  Session;

    static void Initialize()
    {
        Manager = gcnew EventManager();
        Session = gcnew UserSession();
    }
};
