/**************************************************************************
 * Copyright 2017 Khaled Berraoui <khallebal@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 **************************************************************************/

#include "App.h"
#include "RollerWindow.h"


App::App(void)
	:	BApplication("application/x-vnd.kb-roller")
{
	RollerWindow *win = new RollerWindow();
	win->Show();
}

int main(void)
{
	App *app = new App();
	app->Run();
	delete app;
	return 0;
}
