/**************************************************************************
 * Copyright 2017 Khaled Berraoui <khallebal@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 **************************************************************************/

#include "RollerWindow.h"

#include <Alert.h>
#include <Application.h>
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
#include <String.h>
#include <TextControl.h>
#include <View.h>

#include <stdio.h>


#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "RollerWindow"

const char* kRollerAppSig = "application/x-vnd.kb.roller";
//static const char* kDeskbarSignature = "application/x-vnd.Be-TSKB";

static const uint32 kSelectImages = 'simg';
static const uint32 kSelectTimer  ='stmr';
static const uint32 kRandomMode  ='rdmd';
static const uint32 kRollerReplicant  ='rrep';
static const uint32 kScaleMode = 'scmd';
static const uint32 kImageLocation = 'imgl';
static const uint32 kCenterMode = 'ctmd';
static const uint32 kTileMode = 'tlmd';

static const rgb_color kBoxColor = {144, 176, 160, 255};

BString aboutRoller =
		"\t\t\t\tROLLER-1.0\n"
		"\n"
		"\t\tCopyright 2017 Khaled Berraoui\n"
		"\t\tAll rights reserved.\n"
		"\n"
	 	"\t\tA native wallpaper changer app for Haiku\n"
	 	"\n"
	 	"\t\t**My first c++ app ever.**";


RollerWindow::RollerWindow(void)
	:	BWindow(BRect(100,100, 550, 470), "window", B_TITLED_WINDOW,
	B_ASYNCHRONOUS_CONTROLS | B_QUIT_ON_WINDOW_CLOSE)
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


	fAboutButton = new BButton(BRect(10,10,10,10), "fAboutButton",
	"Roller", new BMessage(B_ABOUT_REQUESTED));
	fAboutButton->ResizeToPreferred();
	topview->AddChild(fAboutButton);
	

	box = new BBox(BRect(10,50,440,320), "box",
	B_FOLLOW_ALL, B_WILL_DRAW, B_FANCY_BORDER);
	box->SetLabel(B_TRANSLATE("Settings"));
	topview->AddChild(box);


	fImagesButton = new BButton(BRect(310,30,10,10), "fImagesButton",
	B_TRANSLATE("Add Images"),
	new BMessage(kSelectImages), B_FOLLOW_RIGHT | B_FOLLOW_TOP);
	fImagesButton->ResizeToPreferred();
	box->AddChild(fImagesButton);
	
	fLocation = new BTextControl(BRect(10, 35, 250, 10), "fLocation",
	B_TRANSLATE("Path:"), NULL, new BMessage(kImageLocation),
	B_FOLLOW_RIGHT | B_FOLLOW_TOP);
	fLocation->SetDivider(40);
	box->AddChild(fLocation);


	fViewModeMenu = new BPopUpMenu(B_TRANSLATE("Mode"));
	fViewModeMenu->AddItem(new BMenuItem("Scale",new BMessage(kScaleMode)));
	fViewModeMenu->AddItem(new BMenuItem("Center",new BMessage(kCenterMode)));
	fViewModeMenu->AddItem(new BMenuItem("Tile",new BMessage(kTileMode)));
	
	fViewModeMenu->SetTargetForItems(this);
	fViewModeMenuField = new BMenuField(BRect(10,90,200,10), "fViewModeMenuField",
	"Mode:", fViewModeMenu, B_FOLLOW_LEFT | B_FOLLOW_TOP);
	fViewModeMenuField->SetDivider(50);
	//fViewModeMenuField->ResizeToPreferred();
	box->AddChild(fViewModeMenuField);
	
	fTimerMenu = new BPopUpMenu(B_TRANSLATE("Timer"));
	for (int32 i = 0; i < kOptions; i++) {
		BMessage *choiceMessage = new BMessage(kSelectTimer);
		choiceMessage->AddInt32("seconds", kSelections[i].seconds);
		fTimerMenu->AddItem(new BMenuItem(kSelections[i].name,
			choiceMessage));
	}
	fTimerMenu->SetTargetForItems(this);
	fTimerMenuField = new BMenuField(BRect(10,140,200,10),
	"Timer", "Timer:", fTimerMenu, B_FOLLOW_LEFT | B_FOLLOW_BOTTOM);
	fTimerMenuField->SetDivider(50);
	//fTimerMenuField->ResizeToPreferred();
	box->AddChild(fTimerMenuField);


	fRandomButton = new BCheckBox(BRect(10,230,10,10), "fRandomButton",
	B_TRANSLATE("Random"), new BMessage(kRandomMode),
	B_FOLLOW_LEFT | B_FOLLOW_BOTTOM);
	fRandomButton->ResizeToPreferred();
	box->AddChild(fRandomButton);


	fDeskbarRep = new BCheckBox(BRect(20,330,10,10),"fDeskbarRep",
	B_TRANSLATE("Live in DeskBar"), new BMessage(kRollerReplicant),
	B_FOLLOW_LEFT | B_FOLLOW_BOTTOM);
	fDeskbarRep->ResizeToPreferred();
	topview->AddChild(fDeskbarRep);


}	


void RollerWindow::MessageReceived(BMessage *msg) {
	switch (msg->what) {
		case B_ABOUT_REQUESTED: {
			BAlert *about = new BAlert("Roller", aboutRoller, "OK");
			about->SetType(B_EMPTY_ALERT);
			about->SetFlags(about->Flags() | B_CLOSE_ON_ESCAPE);
			about->Go();
			break;
		}
		case kRollerReplicant: {
			BDeskbar deskbar;
			entry_ref appref;
			status_t status;
			
			if (!deskbar.HasItem("rollerView"))
			status = be_roster->FindApp(kRollerAppSig, &appref);
			//if(status == B_OK)
			status = deskbar.AddItem(&appref);
			
			if(status != B_OK) {
				fprintf(stderr, B_TRANSLATE(
				"Adding Roller to Deskbar failed: %s\n"),
				strerror(status));
			}
			break;
		}
		case kSelectTimer: {
			return;
			break;
		}
		case kSelectImages: {
			fFilePanel = new BFilePanel(B_OPEN_PANEL, NULL, NULL,
			B_FILE_NODE | B_DIRECTORY_NODE
			| B_SYMLINK_NODE, true, NULL, NULL, false, true);
			fFilePanel->SetTarget(this);

			fFilePanel->Window()->SetTitle(B_TRANSLATE(
			"Roller : " "Choose a Folder"));
			fFilePanel->SetButtonLabel(B_DEFAULT_BUTTON,
			B_TRANSLATE("Select"));
			fFilePanel->Show();
			break;
		}
		default: {
			BWindow::MessageReceived(msg);
			break;
		}
	}
}

BView *instantiate_deskbar_item()
{
	return new RollerView();
}

RollerView::RollerView()
	:	BView(BRect(0, 0, 15, 15), "RollerView", 
		B_FOLLOW_ALL, B_WILL_DRAW)
{

}

RollerView::~RollerView()
{
}

RollerView::RollerView(BMessage *message)
	:
		BView(message)
{
}

RollerView *RollerView::Instantiate(BMessage *message)
{
	if (validate_instantiation(message, "RollerView"))
	return new RollerView(message);
}

	
status_t RollerView::Archive(BMessage *message, bool deep) const
{
	BView::Archive(message, deep);
	message->AddString("add_on",kRollerAppSig);
	message->AddString("class","RollerView");
	return B_OK;
}

void RollerView::AttachedToWindow()
{
	SetViewColor(Parent()->ViewColor());

	BView::AttachedToWindow();
}
