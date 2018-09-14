/**************************************************************************
 * Copyright 2017 All rights reserved. 									  *	
 * Distributed under the terms of the MIT license.						  *
 *																		  *
 * Author:																  *	
 *		Khaled Berraoui <khallebal@gmail.com>							  *
 *																		  *
 **************************************************************************/

#ifndef DESKBAR_H
#define DESKBAR_H


#include <Window.h>

class BView;
class BBitmap;
class BPopUpMenu;

class _EXPORT Deskbar;
extern "C" _EXPORT BView *instantiate_deskbar_item();


class Deskbar : public BView
{
public:
									Deskbar();
									Deskbar(BMessage *message);
		virtual						~Deskbar();
		
					void			MessageReceived(BMessage *message);
		static 		Deskbar			*Instantiate(BMessage *message);
		virtual		status_t 		Archive(BMessage *message, bool deep = true) const;
					void			AttachedToWindow();
					void			Draw(BRect rect);
					void			MouseDown(BPoint where);
		virtual 	bool 			QuitRequested();

private:
					void			Init();
					void			RightClick(BPoint where);
					void			LeftClick(BPoint where);

private:
					BBitmap			*fIcon;

};


#endif // DESKBAR_H
