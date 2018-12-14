/*
 * Copyright 2017 All rights reserved.
 * Distributed under the terms of the MIT license.
 *
 * Author:
 *		Khaled Berraoui <khallebal@gmail.com>
 *
 */

#ifndef DESKBARVIEW_H
#define DESKBARVIEW_H


#include <Window.h>

class BView;
class BBitmap;
class BPopUpMenu;


class DeskbarView : public BView
{
public:
						DeskbarView(BRect frame, uint32 resizingMode);
						DeskbarView(BMessage *message);
	virtual				~DeskbarView();

			void		MessageReceived(BMessage *message);
	static 	DeskbarView	*Instantiate(BMessage *message);
virtual	status_t 		Archive(BMessage *message, bool deep = true) const;
			void		AttachedToWindow();
			void		Draw(BRect rect);
			void		MouseDown(BPoint location);

private:
			void		Init();
			void		RightClick(BPoint location);
			void		LeftClick(BPoint location);

private:
			BBitmap		*fIcon;


};


#endif // DESKBAR_H
