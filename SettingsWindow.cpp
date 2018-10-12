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

#include <Box.h>
#include <Button.h>
#include <Catalog.h>
#include <CheckBox.h>
#include <Deskbar.h>
#include <Entry.h>
#include <FilePanel.h>
#include <LayoutBuilder.h>
#include <Locale.h>
#include <Menu.h>
#include <MenuItem.h>
#include <MenuField.h>
#include <Path.h>
#include <PopUpMenu.h>
#include <Roster.h>
#include <SeparatorView.h>
#include <TextControl.h>

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
static const uint32 kAutoStart = 'astt';
static const uint32 kScaleMode = 'scmd';
static const uint32 kCenterMode = 'ctmd';
static const uint32 kTileMode = 'tlmd';
static const uint32 kApply = 'aply';
static const uint32 kRevert = 'rvrt';


SettingsWindow::SettingsWindow(void)
	:	BWindow(BRect(0, 0, 0, 0), "Roller", B_TITLED_WINDOW,
	B_ASYNCHRONOUS_CONTROLS | B_NOT_ZOOMABLE | B_NOT_RESIZABLE
		| B_AUTO_UPDATE_SIZE_LIMITS)

{
struct timerOptions {
	const char *name;
	time_t	seconds;
};


timerOptions kSelections[] = {
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
	{"24 Hours", 24 * 60 * 60 },
	{"1 Week", 7 * 24 * 60 * 60 }
};
const int32 kOptions = sizeof(kSelections) / sizeof(timerOptions);


	fPrefsView = new BSeparatorView(B_HORIZONTAL, B_FANCY_BORDER);
	fPrefsView->SetLabel(B_TRANSLATE("Preferences"));
	fPrefsView->SetFont(be_bold_font);

	fApply = new BButton("revert", B_TRANSLATE("Apply"),
		new BMessage(kApply));
	fApply->SetExplicitAlignment(BAlignment(B_ALIGN_RIGHT,
		B_ALIGN_NO_VERTICAL));

	fRevert = new BButton("revert", B_TRANSLATE("Revert"),
		new BMessage(kRevert));
	fRevert->SetExplicitAlignment(BAlignment(B_ALIGN_RIGHT,
		B_ALIGN_NO_VERTICAL));


	fButton = new BButton("fButton",B_TRANSLATE("Images" B_UTF8_ELLIPSIS),
		new BMessage(kAddImages));

	fLocationText = new BTextControl("fLocationText",
		B_TRANSLATE("Path:"), NULL,
		new BMessage(B_REFS_RECEIVED));

	fWorkSpacesMenu = new BPopUpMenu(B_TRANSLATE("WorkSpaces"));
	fWorkSpacesMenu->AddItem(new BMenuItem("All",
		new BMessage(kAllWorkspaces)));
	fWorkSpacesMenu->AddItem(new BMenuItem("Current",
		new BMessage(kCurrentWorkspace)));
	fWorkSpacesMenu->SetTargetForItems(this);
	fWorkSpacesMenu->SetRadioMode(true);

	fWorkSpacesMenuField = new BMenuField("fWorkSpacesMenuField",
		B_TRANSLATE("Workspaces:"), fWorkSpacesMenu);
	fWorkSpacesMenuField->SetAlignment(B_ALIGN_RIGHT);

	fModeMenu = new BPopUpMenu(B_TRANSLATE("Mode"));
	fModeMenu->AddItem(new BMenuItem("Scale",
		new BMessage(kScaleMode)));
	fModeMenu->AddItem(new BMenuItem("Center",
		new BMessage(kCenterMode)));
	fModeMenu->AddItem(new BMenuItem("Tile",
		new BMessage(kTileMode)));
	fModeMenu->SetTargetForItems(this);
	fModeMenu->SetRadioMode(true);

	fModeMenuField = new BMenuField("fModeMenuField",
		B_TRANSLATE("View Mode:"), fModeMenu);
	fModeMenuField->SetAlignment(B_ALIGN_RIGHT);

	fTimerMenu = new BPopUpMenu(B_TRANSLATE("Timer"));
		for (int32 i = 0; i < kOptions; i++) {
			BMessage *choiceMessage = new BMessage(kSelectTimer);
			choiceMessage->AddInt32("seconds", kSelections[i].seconds);
	fTimerMenu->AddItem(new BMenuItem(kSelections[i].name,
			choiceMessage));
	}
	fTimerMenu->SetTargetForItems(this);
	fTimerMenu->SetRadioMode(true);

	fTimerMenuField = new BMenuField("fTimerMenuField",
		B_TRANSLATE("Change Every:"), fTimerMenu);
	fTimerMenuField->SetAlignment(B_ALIGN_RIGHT);


	fEraseText = new BCheckBox("fEraseText", B_TRANSLATE("Erase Text"),
		new BMessage(kEraseText));

	fRandom = new BCheckBox("fRandom",B_TRANSLATE("Random"),
		new BMessage(kRandomMode));

	fDeskbarControl = new BCheckBox("fDeskbarControl",B_TRANSLATE("DeskBar"),
		new BMessage(kAddReplicant));
	fDeskbarControl->SetValue(isInDeskbar());

	fAutoStart = new BCheckBox("fAutoStart",B_TRANSLATE("Auto Start"),
		new BMessage(kAutoStart));

	box = new BBox(B_FANCY_BORDER,
	BLayoutBuilder::Grid<>(B_USE_DEFAULT_SPACING, 15)
			.Add(BSpaceLayoutItem::CreateGlue(), 1, 3, 4, 0)
			.Add(fEraseText, 2, 0)
			.Add(fRandom, 2, 1)
			.Add(fDeskbarControl, 2, 2)
			.Add(BSpaceLayoutItem::CreateGlue(), 3, 3, 6, 0)
			.Add(new BSeparatorView(B_VERTICAL), 8, 0, 1, 3)
			.AddMenuField(fWorkSpacesMenuField, 10, 0)
			.AddMenuField(fModeMenuField, 10, 1)
			.AddMenuField(fTimerMenuField, 10, 2)
			.Add(BSpaceLayoutItem::CreateGlue(), 12, 3, 2, 0)
			.SetInsets(0,B_USE_DEFAULT_SPACING)
			.View());

	BLayoutBuilder::Group<>(this, B_VERTICAL)
			.SetInsets(B_USE_DEFAULT_SPACING)
			.AddGroup(B_HORIZONTAL)
			.Add(fPrefsView)
			.End()
			.AddGroup(B_HORIZONTAL)
			.Add(fLocationText, B_USE_DEFAULT_SPACING)
			.AddGlue()
			.Add(fButton)
			.AddGlue()
			.End()
			.AddGroup(B_VERTICAL)
			.Add(box)
			.AddGroup(B_HORIZONTAL)
			.Add(fAutoStart)
			.AddGlue()
			.Add(fRevert)
			.Add(fApply)
			.SetInsets(B_USE_WINDOW_SPACING, 0)
			.End();

	CenterOnScreen();
}

SettingsWindow::~SettingsWindow()
{
	if (fFilePanel)
	delete fFilePanel;
	fFilePanel = NULL;
}

void SettingsWindow::MessageReceived(BMessage *msg) {
	msg->PrintToStream();
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
			BMessenger msgr(this);
			fFilePanel = new BFilePanel(B_OPEN_PANEL, &msgr, NULL,
			B_FILE_NODE | B_DIRECTORY_NODE, true, NULL, NULL, false, true);
			fFilePanel->Window()->SetTitle(B_TRANSLATE(
				"Roller : " "Select a Folder"));
			fFilePanel->SetButtonLabel(B_DEFAULT_BUTTON,
				B_TRANSLATE("Add"));
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
