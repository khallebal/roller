/*
 * Copyright 2017 All rights reserved.
 * Distributed under the terms of the MIT license.
 *
 * Author:
 *		Khaled Berraoui <khallebal@gmail.com>
 *
 */

#ifndef APP_H
#define APP_H

#include <Application.h>
#include <image.h>

extern const char *kRollerSignature;
extern const char *kDeskbarSignature;
extern const char *kReplicantName;

status_t our_image(image_info& image);


class App : public BApplication
{
public:
					App(void);
	virtual			~App();
	virtual	void	ReadyToRun();
};

#endif
