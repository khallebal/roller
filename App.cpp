/**************************************************************************
 * Copyright 2017 All rights reserved. 									  													*	
 * Distributed under the terms of the MIT license.						  											*
 *																		  																			*
 * Author:																  																	*	
 *		Khaled Berraoui <khallebal@gmail.com>							  											*
 *																		  																			*
 **************************************************************************/

#include "App.h"
#include "SettingsWindow.h"

#include <Deskbar.h>
#include <Entry.h>
#include <Roster.h>
#include <Window.h>

const char *kRollerDeskbarItem = "RollerDeskbarView";

App::App(void)
	:	BApplication("application/x-vnd.kb-roller")
{
}

void App::ReadyToRun()
{
	BDeskbar deskbar;
	entry_ref appref;

	if (!deskbar.HasItem(kRollerDeskbarItem)) {
		be_roster->FindApp(M_Roller_Signature, &appref),
		deskbar.AddItem(&appref);
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
