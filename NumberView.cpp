/*
 * Copyright (c) 2015 Markus Himmel
 * This file is distributed under the terms of the MIT license
 */

#include "NumberView.h"
#include <Rect.h>
#include <String.h>
#include <iostream>

NumberView::NumberView(uint32 number)
	:
	BView("noname", B_WILL_DRAW),
	fNumber(number)
{
	SetExplicitMinSize(BSize(100, 100));
	SetExplicitMaxSize(BSize(100, 100));
	SetExplicitPreferredSize(BSize(100, 100));
	BFont *font = new BFont(be_bold_font);
	font->SetSize(20.0);
	SetFont(font);
}

void
NumberView::SetNumber(uint32 number)
{
	fNumber = number;
}

void
NumberView::Draw(BRect r)
{
	if (fNumber < 2)
		return;

	BRect bounds = Bounds();
	BString c;
	c << fNumber;
	float width = StringWidth(c.String());
	SetHighColor(255, 0, 0);
	DrawString(c.String(),
		BPoint(bounds.left + 50 - width / 2, bounds.bottom - 50 + 10));
}

NumberView::~NumberView()
{
}
