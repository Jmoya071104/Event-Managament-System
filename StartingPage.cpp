
#include "StartingPage.h"
using namespace System;
using namespace System::Windows::Forms;
[STAThreadAttribute]

int main(array<String^>^ args) {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	GroupProject::StartingPage form;
	Application::Run(% form);
}

