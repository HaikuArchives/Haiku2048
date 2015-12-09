/*
 * Copyright (c) 2015 Markus Himmel
 * This file is distributed under the terms of the MIT license
 */

#include "NumberView.h"
#include <Rect.h>
#include <iostream>

NumberView::NumberView(uint32 number, float x, float y)
	:
	BView(BRect(x, y, x + 10, y + 10), "noname", B_FOLLOW_NONE, B_WILL_DRAW),
	fNumber(number)
{
	std::cout << "Hallo" << std::endl;
	std::cout << Bounds().right << std::endl;
//	SetViewColor(255, 0, 0);
//	ResizeTo(10, 10);
//	Invalidate();
}

void
NumberView::Draw(BRect r)
{
	std::cout << "Hallo2" << std::endl;
	std::cout << r.right << std::endl;
	StrokeLine(r.LeftTop(), r.RightBottom());
}

NumberView::~NumberView()
{
}
