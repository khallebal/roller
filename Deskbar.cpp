/**************************************************************************
 * Copyright 2017 All rights reserved. 									     												*	
 * Distributed under the terms of the MIT license.						  											*
 *																		 																			*
 * Author:																  																	*	
 *		Khaled Berraoui <khallebal@gmail.com>							  											*
 *																		  																			*
 **************************************************************************/

#include "App.h"
#include "Deskbar.h"
#include "SettingsWindow.h"

#include <AboutWindow.h>
#include <AppFileInfo.h>
#include <Bitmap.h>
#include <Deskbar.h>
#include <Entry.h>
#include <File.h>
#include <MenuItem.h>
#include <Region.h>
#include <PopUpMenu.h>
#include <Roster.h>
#include <View.h>
#include <Window.h>

#include <stdio.h>


const uint32 M_Pause = 'puse';
const uint32 M_Resume = 'rsme';
const uint32 M_Next = 'next';
const uint32 M_Previous = 'prvs';
const uint32 M_Settings = 'sttg';
//const uint32 M_Quit = 'quit';


BView *instantiate_deskbar_item()
{
	return new Deskbar();
}

Deskbar::Deskbar()
	:	BView(BRect(0, 0, 15, 15), kRollerDeskbarItem,
		B_FOLLOW_ALL, B_WILL_DRAW)
{
	Init();
}

Deskbar::Deskbar(BMessage *message)
	:	BView(message)
{
	Init();
}

Deskbar::~Deskbar()
{

	delete fIcon;
}

void Deskbar::Init()
{
	entry_ref ref;
	be_roster->FindApp(M_Roller_Signature, &ref);
	
	BFile			file(&ref, B_READ_ONLY);
	BAppFileInfo	appFileInfo(&file);
	fIcon = new BBitmap(BRect(0,0,15,15), B_CMAP8);
	appFileInfo.GetIcon(fIcon, B_MINI_ICON);
}

Deskbar *Deskbar::Instantiate(BMessage *message)
{
	if (validate_instantiation(message, kRollerDeskbarItem))
	return new Deskbar(message);

	return NULL;
	
}

	
status_t Deskbar::Archive(BMessage *message, bool deep) const
{
	BView::Archive(message, deep);
	message->AddString("add_on",M_Roller_Signature);
	message->AddString("class",kRollerDeskbarItem);

	return B_OK;
}


void Deskbar::AttachedToWindow()
{
	BView::AttachedToWindow();

	SetViewColor(Parent()->ViewColor());

}


void Deskbar::Draw(BRect rect)
{
	SetDrawingMode( B_OP_ALPHA );
	DrawBitmap(fIcon, BPoint(0.0, 0.0));
}


void Deskbar::MessageReceived(BMessage *message)
{
	switch (message->what) {
		case B_ABOUT_REQUESTED: {
			BAboutWindow *window = new BAboutWindow(
			"roller", M_Roller_Signature);

			const char *header = {
			"**ROLLER**"
			};
			
			const char *contents[] = {
			"\tA native wallpaper\n" 
			"\tchanger app for Haiku.",
				NULL
			};

			const char* authors[] = {
			"\tKhaled Berraoui.\n",
				NULL
			};

			window->AddText(header, contents);
			window->AddCopyright(2017, "Khaled Berraoui");
			window->AddAuthors(authors);

			window->Show();
			break;
		}
		case B_QUIT_REQUESTED: {
			BDeskbar deskbar;
			if (deskbar.HasItem(kRollerDeskbarItem)) {
				deskbar.RemoveItem(kRollerDeskbarItem);
			} else
				be_app->PostMessage(B_QUIT_REQUESTED);

			break;
		}
		case M_Settings: {
				be_roster->Launch(M_Roller_Signature);
			break;
		}
		default: {
			BView::MessageReceived(message);
			break;
		}
	}
}


void Deskbar::MouseDown(BPoint where)
{
	BPoint location;
	uint32 buttons;

	GetMouse(&location, &buttons);
	

	if (buttons & B_PRIMARY_MOUSE_BUTTON) {
		LeftClick(where);
	} else
	if (buttons & B_SECONDARY_MOUSE_BUTTON) {
		RightClick(where);
		
	}
}




void Deskbar::LeftClick(BPoint where)
{
		BPopUpMenu *popup = new BPopUpMenu("popup", false, false);
		popup->AddItem(new BMenuItem("Pause",
		new BMessage(M_Pause)));
		popup->AddItem(new BMenuItem("Resume",
		new BMessage(M_Resume)));

		popup->AddSeparatorItem();

		popup->AddItem(new BMenuItem("Previous",
		new BMessage(M_Previous)));
		popup->AddItem(new BMenuItem("Next",
		new BMessage(M_Next)));
		popup->SetAsyncAutoDestruct(true);
		popup->SetTargetForItems(this);
		ConvertToScreen(&where);
		popup->Go(where, true, true, true);
}


void Deskbar::RightClick(BPoint where)
{
		BPopUpMenu *popup = new BPopUpMenu("popup", false, false);
		popup->AddItem(new BMenuItem("Settings",
		new BMessage(M_Settings)));
		popup->AddItem(new BMenuItem("About",
		new BMessage(B_ABOUT_REQUESTED)));

		popup->AddSeparatorItem();

		popup->AddItem(new BMenuItem("Quit",
		new BMessage(B_QUIT_REQUESTED)));
		popup->SetAsyncAutoDestruct(true);
		popup->SetTargetForItems(this);
		ConvertToScreen(&where);
		popup->Go(where, true, true, true);
}

