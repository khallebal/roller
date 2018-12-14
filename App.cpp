/*
 * Copyright 2017 All rights reserved.
 * Distributed under the terms of the MIT license.
 *
 * Author:
 *		Khaled Berraoui <khallebal@gmail.com>
 *
 */
#include "App.h"
#include "SettingsWindow.h"

#include <Deskbar.h>
#include <Entry.h>

const char* kRollerSignature = "application/x-vnd.kb-roller";
const char* kDeskbarSignature = "application/x-vnd.Be-TSKB";
const char *kReplicantName = "RollerDeskbarView";

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
	bool inDeskbar;
	inDeskbar = deskbar.HasItem(kReplicantName);
	if (!inDeskbar) {

	image_info info;
	entry_ref ref;
	if (our_image(info) == B_OK
		&& get_ref_for_path(info.name, &ref) == B_OK) {
		deskbar.AddItem(&ref);

		Quit();
		return;
		}
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
