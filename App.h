/**************************************************************************
 * Copyright 2017 All rights reserved. 									  													*	
 * Distributed under the terms of the MIT license.						  											*
 *																		  																			*
 * Author:																  																	*	
 *		Khaled Berraoui <khallebal@gmail.com>							  											*
 *																		  																			*
 **************************************************************************/
 
#ifndef APP_H
#define APP_H

#include <Application.h>

extern const char *M_Roller_Signature;
extern const char *kRollerDeskbarItem;

class App : public BApplication
{
public:
							App(void);
		virtual		void	ReadyToRun();
};

#endif
