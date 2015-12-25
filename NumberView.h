/*
 * Copyright (c) 2015 Markus Himmel
 * This file is distributed under the terms of the MIT license
 */

#ifndef NUMBER_VIEW_H
#define NUMBER_VIEW_H

#include <View.h>

class BSize;

class NumberView : public BView
{
public:
						NumberView(uint32 number);
						~NumberView();

			void		SetNumber(uint32 number);

			void		Draw(BRect r);

private:
	uint32				fNumber;
};

#endif // NUMBER_VIEW_H
