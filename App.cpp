/**************************************************************************
 * Copyright 2017 All rights reserved. 									  *	
 * Distributed under the terms of the MIT license.						  *
 *																		  *
 * Author:																  *	
 *		Khaled Berraoui <khallebal@gmail.com>							  *
 *																		  *
 **************************************************************************/

#include "App.h"
#include "RollerWindow.h"


App::App(void)
	:	BApplication("application/x-vnd.kb-roller")
{
	RollerWindow *win = new RollerWindow();
	win->Show();
}

bool App::QuitRequested()
{
	Quit();
	return true;
}


int main(void)
{
	App *app = new App();
	app->Run();
	delete app;
	return 0;
}
