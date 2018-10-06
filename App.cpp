/**************************************************************************
 * Copyright 2017 All rights reserved. 									  *
 * Distributed under the terms of the MIT license.						  *
 *																		  *
 * Author:																  *
 *		Khaled Berraoui <khallebal@gmail.com>							  *
 *																		  *
 **************************************************************************/
#include "App.h"
#include "SettingsWindow.h"

#include <Deskbar.h>
#include <Entry.h>
#include <Roster.h>
#include <Window.h>

const char* kRollerSignature = "application/x-vnd.kb-roller";
const char* kDeskbarSignature = "application/x-vnd.Be-TSKB";
const char *kRollerDeskbarName = "RollerDeskbarView";

App::App(void)
	:	BApplication(kRollerSignature)
{
}

App::~App()
{
}

void App::ReadyToRun()
{
	BDeskbar deskbar;
	entry_ref appref;
	bool isInDeskbar;
	isInDeskbar = deskbar.HasItem(kRollerDeskbarName);

	if (!isInDeskbar) {
		be_roster->FindApp(kRollerSignature, &appref);
		deskbar.AddItem(&appref);
		Quit();
		return;
	}
	SettingsWindow *win = new SettingsWindow();
		win->Show();

}

int main(void)
{
	App *app = new App();
	app->Run();
	delete app;
	return 0;
}
