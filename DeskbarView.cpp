/**************************************************************************
 * Copyright 2017 All rights reserved. 										*
 * Distributed under the terms of the MIT license.							*
 *																			*
 * Author:																	*
 *		Khaled Berraoui <khallebal@gmail.com>								*
 *																			*
 **************************************************************************/

#include "App.h"
#include "DeskbarView.h"
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
#include <strings.h>

const uint32 kPause = 'puse';
const uint32 kResume = 'rsme';
const uint32 kNext = 'next';
const uint32 kPrevious = 'prvs';
const uint32 kSettingsWin = 'sttg';


BView *instantiate_deskbar_item()
{
	return new DeskbarView();
}

DeskbarView::DeskbarView(bool inDeskbar)
	:	BView(BRect(0, 0, 15, 15), kRollerDeskbarName,
		B_FOLLOW_ALL, B_WILL_DRAW),
		fInDeskbar(inDeskbar)
{
	Init();
}

DeskbarView::DeskbarView(BMessage *message)
	:	BView(message),
	fInDeskbar(false)
{
		app_info info;
	if (be_app->GetAppInfo(&info) == B_OK
		&& !strcasecmp(info.signature, kDeskbarSignature))
		fInDeskbar = true;

	Init();
}

DeskbarView::~DeskbarView()
{
	delete fIcon;
}

void DeskbarView::Init()
{
	entry_ref ref;
	be_roster->FindApp(kRollerSignature, &ref);

	BFile			file(&ref, B_READ_ONLY);
	BAppFileInfo	appFileInfo(&file);
	fIcon = new BBitmap(BRect(0,0,15,15), B_CMAP8);
	appFileInfo.GetIcon(fIcon, B_MINI_ICON);
}

void DeskbarView::_Quit()
{
	if (fInDeskbar) {
		BDeskbar deskbar;
		deskbar.RemoveItem(kRollerDeskbarName);
	} else
		be_app->PostMessage(B_QUIT_REQUESTED);
}

DeskbarView *DeskbarView::Instantiate(BMessage *message)
{
	if (validate_instantiation(message, kRollerDeskbarName))
	return new DeskbarView(message);

	return NULL;

}

status_t DeskbarView::Archive(BMessage *message, bool deep) const
{
	BView::Archive(message, deep);
	message->AddString("add_on",kRollerSignature);
	message->AddString("class",kRollerDeskbarName);

	return B_OK;
}

void DeskbarView::AttachedToWindow()
{
	BView::AttachedToWindow();

	SetViewColor(Parent()->ViewColor());

}

void DeskbarView::Draw(BRect rect)
{
	SetDrawingMode( B_OP_ALPHA );
	DrawBitmap(fIcon, BPoint(0.0, 0.0));
}

void DeskbarView::MessageReceived(BMessage *message)
{
	switch (message->what) {
		case B_ABOUT_REQUESTED: {
			BAboutWindow *window = new BAboutWindow(
			"roller", kRollerSignature);

			const char *header = {
			"\t   **ROLLER**"
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
			deskbar.RemoveItem(kRollerDeskbarName);
			break;
		}
		case kSettingsWin: {
				be_roster->Launch(kRollerSignature);
			break;
		}
		default: {
			BView::MessageReceived(message);
			break;
		}
	}
}

void DeskbarView::MouseDown(BPoint where)
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

void DeskbarView::LeftClick(BPoint where)
{
		BPopUpMenu *popup = new BPopUpMenu("popup", false, false);
		popup->AddItem(new BMenuItem("Pause",
		new BMessage(kPause)));
		popup->AddItem(new BMenuItem("Resume",
		new BMessage(kResume)));

		popup->AddSeparatorItem();

		popup->AddItem(new BMenuItem("Previous",
		new BMessage(kPrevious)));
		popup->AddItem(new BMenuItem("Next",
		new BMessage(kNext)));
		popup->SetAsyncAutoDestruct(true);
		popup->SetTargetForItems(this);
		ConvertToScreen(&where);
		popup->Go(where, true, true, true);
}

void DeskbarView::RightClick(BPoint where)
{
		BPopUpMenu *popup = new BPopUpMenu("popup", false, false);
		popup->AddItem(new BMenuItem("Settings" B_UTF8_ELLIPSIS,
		new BMessage(kSettingsWin)));
		popup->AddItem(new BMenuItem("About",
		new BMessage(B_ABOUT_REQUESTED)));

		popup->AddSeparatorItem();

	if (fInDeskbar) {
		popup->AddItem(new BMenuItem("Quit",
		new BMessage(B_QUIT_REQUESTED)));
	}
		popup->SetAsyncAutoDestruct(true);
		popup->SetTargetForItems(this);
		ConvertToScreen(&where);
		popup->Go(where, true, true, true);
}
