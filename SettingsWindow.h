/*
 * Copyright 2017 All rights reserved.
 * Distributed under the terms of the MIT license.
 *
 * Author:
 *		Khaled Berraoui <khallebal@gmail.com>
 *
 */

#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <Window.h>

class BBox;
class BBitmap;
class BView;
class BButton;
class BCheckBox;
class BFilePanel;
class BMenu;
class BMenuField;
class BSeparatorView;
class BTextControl;


class SettingsWindow : public BWindow
{
public:
								SettingsWindow();
	virtual						~SettingsWindow();
				void			MessageReceived(BMessage *msg);
	virtual		bool			QuitRequested();
				bool			isInDeskbar();
private:
				void			_BuildUserInterface();

				BBox 			*box;
				BButton 		*fApply;
				BButton 		*fRevert;
				BButton 		*fButton;
				BCheckBox		*fAutoStart;
				BCheckBox 		*fDeskbarControl;
				BCheckBox		*fRandom;
				BCheckBox		*fEraseText;
				BFilePanel		*fFilePanel;
				BMenu			*fWorkSpacesMenu;
				BMenu			*fModeMenu;
				BMenu			*fTimerMenu;
				BMenuField 		*fModeMenuField;
				BMenuField		*fTimerMenuField;
				BMenuField		*fWorkSpacesMenuField;
				BSeparatorView	*fPrefsView;
				BTextControl	*fLocationText;

};

#endif
