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
#include "DeskbarView.h"

#include <Alert.h>
#include <Application.h>
#include <Bitmap.h>
#include <Box.h>
#include <Button.h>
#include <Catalog.h>
#include <CheckBox.h>
#include <Deskbar.h>
#include <Entry.h>
#include <FilePanel.h>
#include <Locale.h>
#include <Menu.h>
#include <MenuItem.h>
#include <MenuField.h>
#include <Path.h>
#include <PopUpMenu.h>
#include <Roster.h>
#include <TextControl.h>
#include <View.h>

#include <stdio.h>


#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "SettingsWindow"

static const uint32 kAddImages = 'aimg';
static const uint32 kAllWorkspaces = 'alwk';
static const uint32 kCurrentWorkspace = 'crwk';
static const uint32 kSelectTimer  ='stmr';
static const uint32 kEraseText = 'etxt';
static const uint32 kRandomMode  ='rdmd';
static const uint32 kAddReplicant  ='arep';
static const uint32 kScaleMode = 'scmd';
static const uint32 kCenterMode = 'ctmd';
static const uint32 kTileMode = 'tlmd';
static const uint32 kApply = 'aply';
static const uint32 kRevert = 'rvrt';


SettingsWindow::SettingsWindow(void)
	:	BWindow(BRect(100,100, 580, 460), "Roller", B_TITLED_WINDOW,
	B_ASYNCHRONOUS_CONTROLS | B_NOT_ZOOMABLE | B_NOT_RESIZABLE)
	
{
	struct timerOptions {
	const char *name;
	time_t	seconds;
};


timerOptions kSelections[] = {
	{"15 Seonds", 15 },
	{"30 Seonds", 30 },
	{"1 Minute", 60 },
	{"3 Minutes", 3 * 60 },
	{"5 Minutes", 5 * 60 },
	{"10 Minutes", 10 * 60 },
	{"20 Minutes", 20 * 60 },
	{"30 Minutes", 30 * 60 },
	{"1 Hour", 60 * 60 },
	{"4 Hours", 4 * 60 * 60 },
	{"8 Hours", 8 * 60 * 60 },
	{"12 Hours", 12 * 60 * 60 },
	{"24 Hours", 24 * 60 * 60 }
};
const int32 kOptions = sizeof(kSelections) / sizeof(timerOptions);


	topview = new BView(Bounds(), "roller", B_FOLLOW_ALL, B_WILL_DRAW);
	topview->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	AddChild(topview);

	fApplyButton = new BButton(BRect(395, 310, 0, 0),"revert", B_TRANSLATE("Apply"),
					new BMessage(kApply), B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM);
	fApplyButton->ResizeToPreferred();
	topview->AddChild(fApplyButton);
		

	fRevertButton = new BButton(BRect(10,310,0,0), "revert", B_TRANSLATE("Revert"),
			new BMessage(kRevert), B_FOLLOW_LEFT | B_FOLLOW_BOTTOM);
	fRevertButton->ResizeToPreferred();
	topview->AddChild(fRevertButton);


	box = new BBox(BRect(10,10,470,295), "box",
	B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW, B_FANCY_BORDER);
	box->SetLabel(B_TRANSLATE("Settings"));
	topview->AddChild(box);


	fImagesButton = new BButton(BRect(350,30,20,10), "fImagesButton",
	B_TRANSLATE("Folder" B_UTF8_ELLIPSIS),
	new BMessage(kAddImages), B_FOLLOW_RIGHT | B_FOLLOW_TOP);
	fImagesButton->ResizeToPreferred();
	box->AddChild(fImagesButton);
	
	fLocationText = new BTextControl(BRect(10, 35, 300, 10), "fLocationText",
	B_TRANSLATE("Path:"), NULL, new BMessage(B_REFS_RECEIVED),
	B_FOLLOW_RIGHT | B_FOLLOW_TOP);
	fLocationText->SetDivider(40);
	box->AddChild(fLocationText);

	fWorkSpacesMenu = new BPopUpMenu(B_TRANSLATE("WorkSpaces"));
	fWorkSpacesMenu->AddItem(new BMenuItem("All workspaces",new BMessage(kAllWorkspaces)));
	fWorkSpacesMenu->AddItem(new BMenuItem("Current workspace",new BMessage(kCurrentWorkspace)));

	fWorkSpacesMenu->SetTargetForItems(this);
	fWorkSpacesMenuField = new BMenuField(BRect(10,100,300,10), "fWorkSpacesMenuField",
	NULL, fWorkSpacesMenu, B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP);
	box->AddChild(fWorkSpacesMenuField);

	
	fModeMenu = new BPopUpMenu(B_TRANSLATE("Mode"));
	fModeMenu->AddItem(new BMenuItem("Scale",new BMessage(kScaleMode)));
	fModeMenu->AddItem(new BMenuItem("Center",new BMessage(kCenterMode)));
	fModeMenu->AddItem(new BMenuItem("Tile",new BMessage(kTileMode)));
	
	fModeMenu->SetTargetForItems(this);
	fModeMenuField = new BMenuField(BRect(10,150,300,10), "fModeMenuField",
	NULL, fModeMenu, B_FOLLOW_LEFT | B_FOLLOW_TOP);
	box->AddChild(fModeMenuField);
	
	fTimerMenu = new BPopUpMenu(B_TRANSLATE("Timer"));
	for (int32 i = 0; i < kOptions; i++) {
		BMessage *choiceMessage = new BMessage(kSelectTimer);
		choiceMessage->AddInt32("seconds", kSelections[i].seconds);
		fTimerMenu->AddItem(new BMenuItem(kSelections[i].name,
			choiceMessage));
	}
	fTimerMenu->SetTargetForItems(this);
	fTimerMenuField = new BMenuField(BRect(10,200,300,10),
	"Timer", NULL, fTimerMenu, B_FOLLOW_LEFT | B_FOLLOW_TOP);
	box->AddChild(fTimerMenuField);


	fEraseTextControl = new BCheckBox(BRect(350,100,10,10), "fEraseTextControl",
	B_TRANSLATE("Erase Text"), new BMessage(kEraseText),
	B_FOLLOW_RIGHT | B_FOLLOW_TOP);
	fEraseTextControl->ResizeToPreferred();
	box->AddChild(fEraseTextControl);

	fRandomControl = new BCheckBox(BRect(350,150,10,10), "fRandomControl",
	B_TRANSLATE("Random"), new BMessage(kRandomMode),
	B_FOLLOW_RIGHT | B_FOLLOW_TOP);
	fRandomControl->ResizeToPreferred();
	box->AddChild(fRandomControl);


	fDeskbarControl = new BCheckBox(BRect(350,200,10,10),"fDeskbarControl",
	B_TRANSLATE("DeskBar"), new BMessage(kAddReplicant),
	B_FOLLOW_RIGHT | B_FOLLOW_TOP);
	fDeskbarControl->ResizeToPreferred();
	box->AddChild(fDeskbarControl);
	fDeskbarControl->SetValue(isInDeskbar());

}	


void SettingsWindow::MessageReceived(BMessage *msg) {
	switch (msg->what) {
		case kAddReplicant: {
			BDeskbar deskbar;
			entry_ref appref;
			if (!deskbar.HasItem(kRollerDeskbarName)) {
				entry_ref appref;
				be_roster->FindApp(kRollerSignature, &appref);
				deskbar.AddItem(&appref);

			} else if (fDeskbarControl->Value() == B_CONTROL_OFF) {
				deskbar.RemoveItem(kRollerDeskbarName);
			}
			break;
		}
		case kSelectTimer: {
			return;
			break;
		}
		case kAddImages: {
			entry_ref ref;
			if (!fFilePanel) {
				BMessenger msgr(this);
				fFilePanel = new BFilePanel(B_OPEN_PANEL, &msgr, NULL,
				B_FILE_NODE | B_DIRECTORY_NODE,
				true, NULL, NULL, false, true);
			fFilePanel->Window()->SetTitle(B_TRANSLATE(
				"Roller : " "Select a Folder"));
			fFilePanel->SetButtonLabel(B_DEFAULT_BUTTON,
				B_TRANSLATE("Add"));
			} else
				fFilePanel->SetPanelDirectory(&ref);
				fFilePanel->Show();
			break;
		}
		case B_REFS_RECEIVED: {
			entry_ref ref;
			if (msg->FindRef("refs", 0, &ref) == B_OK) {
				BEntry entry(&ref, true);
				BPath path;
				entry.GetPath(&path);
				fLocationText->SetText(path.Path());
			}
			break;
		}
		default: {
			BWindow::MessageReceived(msg);
			break;
		}
	}
}

bool SettingsWindow::isInDeskbar()
{
	BDeskbar deskbar;
	return deskbar.HasItem(kRollerDeskbarName);
}

bool SettingsWindow::QuitRequested()
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}
