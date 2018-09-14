/**************************************************************************
 * Copyright 2017 All rights reserved. 									  													*	
 * Distributed under the terms of the MIT license.						  											*
 *																		  																			*
 * Author:																  																	*	
 *		Khaled Berraoui <khallebal@gmail.com>							  											*
 *																		  																			*
 **************************************************************************/

#ifndef ROLLERWINDOW_H
#define ROLLERWINDOW_H

#include <Window.h>

class BBox;
class BBitmap;
class BView;
class BButton;
class BCheckBox;
class BFilePanel;
class BMenu;
class BMenuField;
class BTextControl;


class RollerWindow : public BWindow
{
public:
									RollerWindow();
				void				MessageReceived(BMessage *msg);

				void				AddToDeskbar();
				void				RemoveFromDeskbar();

private:

				BBox 				*box;
				BView				*topview;
				BButton 			*fApplyButton;
				BButton 			*fRevertButton;				
				BButton 			*fAboutButton;
				BButton 			*fImagesButton;
				BCheckBox 			*fDeskbarControl;
				BCheckBox			*fRandomControl;
				BCheckBox			*fEraseTextControl;
				BFilePanel			*fFilePanel;
				BMenu				*fWorkSpacesMenu;
				BMenu				*fViewModeMenu;
				BMenu				*fTimerMenu;
				BMenuField 			*fViewModeMenuField;
				BMenuField			*fTimerMenuField;
				BMenuField			*fWorkSpacesMenuField;
				BTextControl		*fPathText;

};

#endif
