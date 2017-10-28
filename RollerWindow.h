/**************************************************************************
 * Copyright 2017 Khaled Berraoui <khallebal@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 **************************************************************************/

#ifndef ROLLERWINDOW_H
#define ROLLERWINDOW_H

#include <Window.h>

class BBox;
class BView;
class BButton;
class BCheckBox;
class BFilePanel;
class BMenu;
class BMenuField;
class BTextControl;

extern "C" _EXPORT BView *instantiate_deskbar_item();

class RollerWindow : public BWindow
{
public:
									RollerWindow();

				void				MessageReceived(BMessage *msg);

				//void				CreateTimerMenu();
				//void				addDirectory();
				//uint32			selectTimer();
				//uint32			viewMode();
	
private:

				BBox 				*box;
				BView				*topview;
				BButton 			*fAboutButton;
				BButton 			*fImagesButton;
				BCheckBox 			*fDeskbarRep;
				BCheckBox			*fRandomButton;
				BFilePanel			*fFilePanel;
				BMenu				*fViewModeMenu;
				BMenu				*fTimerMenu;
				BMenuField 			*fViewModeMenuField;
				BMenuField			*fTimerMenuField;
				BTextControl		*fLocation;

};

class RollerView : public BView
{
public:
									RollerView();
									~RollerView();
									
									RollerView(BMessage *message);

		static		RollerView		*Instantiate(BMessage *message);
		virtual		status_t 		Archive(BMessage *message, bool deep = true) const;
					void			AttachedToWindow();

};

#endif
