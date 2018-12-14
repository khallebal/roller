/*
 * Copyright 2017 All rights reserved.
 * Distributed under the terms of the MIT license.
 *
 * Author:
 *		Khaled Berraoui <khallebal@gmail.com>
 *
 */

#include <AboutWindow.h>
#include <Bitmap.h>
#include <Deskbar.h>
#include <Entry.h>
#include <File.h>
#include <IconUtils.h>
#include <MenuItem.h>
#include <PopUpMenu.h>
#include <Resources.h>
#include <Roster.h>
#include <View.h>

#include "App.h"
#include "DeskbarView.h"
#include "SettingsWindow.h"


const uint32 kPause = 'puse';
const uint32 kResume = 'rsme';
const uint32 kNext = 'next';
const uint32 kPrevious = 'prvs';
const uint32 kSettingsWin = 'sttg';
const uint32 kQuit = 'quit';


status_t our_image(image_info& image)
{
	int32 cookie = 0;
	while (get_next_image_info(B_CURRENT_TEAM, &cookie, &image) == B_OK) {
		if ((char*)our_image >= (char*)image.text
			&& (char*)our_image <= (char*)image.text + image.text_size)
			return B_OK;
	}

	return B_ERROR;
}

DeskbarView::DeskbarView(BRect frame, uint32 resizingMode)
	:	BView(frame, kReplicantName, resizingMode, B_WILL_DRAW)
{
	Init();
}

DeskbarView::DeskbarView(BMessage *message)
	:	BView(message)
{
	Init();
}

DeskbarView::~DeskbarView()
{
}

DeskbarView *DeskbarView::Instantiate(BMessage *message)
{
	if (validate_instantiation(message, kReplicantName))
	return new DeskbarView(message);

	return NULL;

}

status_t DeskbarView::Archive(BMessage *message, bool deep) const
{
	BView::Archive(message, deep);
	message->AddString("add_on",kRollerSignature);
	message->AddString("class",kReplicantName);

	return B_OK;
}

void DeskbarView::AttachedToWindow()
{
	AdoptParentColors();

	BView::AttachedToWindow();
}

void DeskbarView::Draw(BRect rect)
{
	SetDrawingMode(B_OP_ALPHA);
	DrawBitmap(fIcon);
	SetDrawingMode(B_OP_COPY);
}

void DeskbarView::Init()
{
	fIcon = NULL;

	image_info info;
	if (our_image(info) != B_OK)
		return;

	BFile file(info.name, B_READ_ONLY);
	if (file.InitCheck() != B_OK)
		return;

	BResources resources(&file);
	if (resources.InitCheck() != B_OK)
		return;

	size_t size;
	const void *data = resources.LoadResource(B_VECTOR_ICON_TYPE,
		"Replicant", &size);
	if (data != NULL) {
	BBitmap *bitmap = new BBitmap(Bounds(), B_RGBA32);
	if (bitmap->InitCheck() == B_OK
		&& BIconUtils::GetVectorIcon((uint8*)data, size, bitmap) == B_OK) {
		fIcon = bitmap;
	} else
		delete bitmap;
	}
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
		case kQuit: {
			BDeskbar deskbar;
			deskbar.RemoveItem(kReplicantName);
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

void DeskbarView::MouseDown(BPoint location)
{
	uint32 buttons;

	GetMouse(&location, &buttons);

	if (buttons & B_PRIMARY_MOUSE_BUTTON) {
		LeftClick(location);
	} else
	if (buttons & B_SECONDARY_MOUSE_BUTTON) {
		RightClick(location);

	}
}

void DeskbarView::LeftClick(BPoint location)
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
		ConvertToScreen(&location);
		popup->Go(location, true, true, true);
}

void DeskbarView::RightClick(BPoint location)
{
		BPopUpMenu *popup = new BPopUpMenu("popup", false, false);
		popup->AddItem(new BMenuItem("Preferences" B_UTF8_ELLIPSIS,
		new BMessage(kSettingsWin)));
		popup->AddItem(new BMenuItem("About" B_UTF8_ELLIPSIS,
		new BMessage(B_ABOUT_REQUESTED)));

		popup->AddSeparatorItem();

		popup->AddItem(new BMenuItem("Quit",
		new BMessage(kQuit)));
		popup->SetAsyncAutoDestruct(true);
		popup->SetTargetForItems(this);
		ConvertToScreen(&location);
		popup->Go(location, true, true, true);
}

extern "C"
BView *instantiate_deskbar_item(float maxWidth, float maxHeight)
{
	return new DeskbarView(BRect(0, 0, maxHeight - 1, maxHeight - 1),
		B_FOLLOW_ALL);
}
