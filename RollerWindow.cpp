/**************************************************************************
 * Copyright 2017 All rights reserved. 									  													*	
 * Distributed under the terms of the MIT license.						  											*
 *																		  																			*
 * Author:																  																	*	
 *		Khaled Berraoui <khallebal@gmail.com>							  											*
 *																		  																			*
 **************************************************************************/

#include "App.h"
#include "RollerWindow.h"
#include "Deskbar.h"

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
#include <PopUpMenu.h>
#include <Roster.h>
#include <TextControl.h>
#include <View.h>

#include <stdio.h>


#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "RollerWindow"

const char* M_Roller_Signature = "application/x-vnd.kb-roller";

const char *locationText(NULL);
static const uint32 M_SelectImages = 'simg';
static const uint32 M_AllWorkspaces = 'alwk';
static const uint32 M_CurrentWorkspace = 'crwk';
static const uint32 M_SelectTimer  ='stmr';
static const uint32 M_EraseText = 'etxt';
static const uint32 M_RandomMode  ='rdmd';
static const uint32 M_AddReplicant  ='rrep';
static const uint32 M_ScaleMode = 'scmd';
static const uint32 M_ImagesPath = 'ipth';
static const uint32 M_CenterMode = 'ctmd';
static const uint32 M_TileMode = 'tlmd';
static const uint32 M_Apply = 'aply';
static const uint32 M_Revert = 'rvrt';


RollerWindow::RollerWindow(void)
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
					new BMessage(M_Apply), B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM);
	fApplyButton->ResizeToPreferred();
	topview->AddChild(fApplyButton);
		

	fRevertButton = new BButton(BRect(10,310,0,0), "revert", B_TRANSLATE("Revert"),
			new BMessage(M_Revert), B_FOLLOW_LEFT | B_FOLLOW_BOTTOM);
	fRevertButton->ResizeToPreferred();
	topview->AddChild(fRevertButton);


	box = new BBox(BRect(10,10,470,295), "box",
	B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW, B_FANCY_BORDER);
	box->SetLabel(B_TRANSLATE("Settings"));
	topview->AddChild(box);


	fImagesButton = new BButton(BRect(350,30,20,10), "fImagesButton",
	B_TRANSLATE("Add Images"),
	new BMessage(M_SelectImages), B_FOLLOW_RIGHT | B_FOLLOW_TOP);
	fImagesButton->ResizeToPreferred();
	box->AddChild(fImagesButton);
	
	fPathText = new BTextControl(BRect(10, 35, 300, 10), "fPathText",
	B_TRANSLATE("Path:"), NULL, new BMessage(M_ImagesPath),
	B_FOLLOW_RIGHT | B_FOLLOW_TOP);
	fPathText->SetDivider(40);
	box->AddChild(fPathText);

	fWorkSpacesMenu = new BPopUpMenu(B_TRANSLATE("WorkSpaces"));
	fWorkSpacesMenu->AddItem(new BMenuItem("All workspaces",new BMessage(M_AllWorkspaces)));
	fWorkSpacesMenu->AddItem(new BMenuItem("Current workspace",new BMessage(M_CurrentWorkspace)));

	fWorkSpacesMenu->SetTargetForItems(this);
	fWorkSpacesMenuField = new BMenuField(BRect(10,100,300,10), "fWorkSpacesMenuField",
	NULL, fWorkSpacesMenu, B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP);
	box->AddChild(fWorkSpacesMenuField);

	
	fViewModeMenu = new BPopUpMenu(B_TRANSLATE("Mode"));
	fViewModeMenu->AddItem(new BMenuItem("Scale",new BMessage(M_ScaleMode)));
	fViewModeMenu->AddItem(new BMenuItem("Center",new BMessage(M_CenterMode)));
	fViewModeMenu->AddItem(new BMenuItem("Tile",new BMessage(M_TileMode)));
	
	fViewModeMenu->SetTargetForItems(this);
	fViewModeMenuField = new BMenuField(BRect(10,150,300,10), "fViewModeMenuField",
	NULL, fViewModeMenu, B_FOLLOW_LEFT | B_FOLLOW_TOP);
	box->AddChild(fViewModeMenuField);
	
	fTimerMenu = new BPopUpMenu(B_TRANSLATE("Timer"));
	for (int32 i = 0; i < kOptions; i++) {
		BMessage *choiceMessage = new BMessage(M_SelectTimer);
		choiceMessage->AddInt32("seconds", kSelections[i].seconds);
		fTimerMenu->AddItem(new BMenuItem(kSelections[i].name,
			choiceMessage));
	}
	fTimerMenu->SetTargetForItems(this);
	fTimerMenuField = new BMenuField(BRect(10,200,300,10),
	"Timer", NULL, fTimerMenu, B_FOLLOW_LEFT | B_FOLLOW_TOP);
	box->AddChild(fTimerMenuField);


	fEraseTextControl = new BCheckBox(BRect(350,100,10,10), "fEraseTextControl",
	B_TRANSLATE("Erase Text"), new BMessage(M_EraseText),
	B_FOLLOW_RIGHT | B_FOLLOW_TOP);
	fEraseTextControl->ResizeToPreferred();
	box->AddChild(fEraseTextControl);

	fRandomControl = new BCheckBox(BRect(350,150,10,10), "fRandomControl",
	B_TRANSLATE("Random"), new BMessage(M_RandomMode),
	B_FOLLOW_RIGHT | B_FOLLOW_TOP);
	fRandomControl->ResizeToPreferred();
	box->AddChild(fRandomControl);


	fDeskbarControl = new BCheckBox(BRect(350,200,10,10),"fDeskbarControl",
	B_TRANSLATE("DeskBar"), new BMessage(M_AddReplicant),
	B_FOLLOW_RIGHT | B_FOLLOW_TOP);
	fDeskbarControl->ResizeToPreferred();
	box->AddChild(fDeskbarControl);

}	


void RollerWindow::MessageReceived(BMessage *msg) {
	switch (msg->what) {
		case M_AddReplicant: {
			BDeskbar deskbar;
			if (deskbar.HasItem("RollerDeskbarView"))
				deskbar.RemoveItem("RollerDeskbarView");
			else {
				entry_ref appref;
				status_t status;
				status = be_roster->FindApp(M_Roller_Signature, &appref),
				deskbar.AddItem(&appref);

			if(status != B_OK)
				fprintf(stderr, B_TRANSLATE(
				"Adding Roller to Deskbar failed: %s\n"),
				strerror(status));
			}
			
			break;
		}
		case M_SelectTimer: {
			return;
			break;
		}
		case M_SelectImages: {
			BEntry entry(fPathText->Text(), true);
			entry_ref ref;
			if (entry.Exists() && entry.IsDirectory())
				entry.GetRef(&ref);
			if (!fFilePanel) {
				BMessenger msgr(this);
			fFilePanel = new BFilePanel(B_OPEN_PANEL, &msgr, NULL,
			B_FILE_NODE | B_DIRECTORY_NODE
			| B_SYMLINK_NODE, true, NULL, NULL, false, true);
			fFilePanel->SetTarget(this);

			fFilePanel->Window()->SetTitle(B_TRANSLATE(
			"Roller : " "Select a Folder"));
			fFilePanel->SetButtonLabel(B_DEFAULT_BUTTON,
			B_TRANSLATE("Select"));
			} else
				fFilePanel->SetPanelDirectory(&ref);
			fFilePanel->Show();
			break;
		}
		case M_ImagesPath: {
			BEntry entry(fPathText->Text(), true);
			if (!entry.Exists())
			return;
			entry_ref ref;
			entry.GetRef(&ref);
			break;
		}
		default: {
			BWindow::MessageReceived(msg);
			break;
		}
	}
}

