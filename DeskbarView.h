/**************************************************************************
 * Copyright 2017 All rights reserved. 									  													*	
 * Distributed under the terms of the MIT license.						  											*
 *																		  																			*
 * Author:																  																	*	
 *		Khaled Berraoui <khallebal@gmail.com>							  											*
 *																		  																			*
 **************************************************************************/

#ifndef DESKBARVIEW_H
#define DESKBARVIEW_H


#include <Window.h>

class BView;
class BBitmap;
class BPopUpMenu;

class _EXPORT DeskbarView;
extern "C" _EXPORT BView *instantiate_deskbar_item();


class DeskbarView : public BView
{
public:
									DeskbarView(bool inDeskbar = false);
									DeskbarView(BMessage *message);
		virtual						~DeskbarView();
		
					void			MessageReceived(BMessage *message);
		static 		DeskbarView		*Instantiate(BMessage *message);
		virtual		status_t 		Archive(BMessage *message, bool deep = true) const;
					void			AttachedToWindow();
					void			Draw(BRect rect);
					void			MouseDown(BPoint where);
					void			Quit();

private:
					void			Init();
					void			RightClick(BPoint where);
					void			LeftClick(BPoint where);

private:
					BBitmap			*fIcon;
					bool			fInDeskbar;


};


#endif // DESKBAR_H
