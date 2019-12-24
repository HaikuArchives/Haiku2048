/*
 * Copyright (c) 2015 Markus Himmel
 * This file is distributed under the terms of the MIT license
 */

#include "NumberView.h"
#include <Rect.h>
#include <String.h>
#include <iostream>

float squareSize = 100;

void ScaleBy (float scale)
{
	squareSize *= scale;
}

NumberView::NumberView(uint32 number)
	:
	BView("noname", B_WILL_DRAW),
	fNumber(number)
{
	UpdateTiles();
}

void NumberView::UpdateTiles(){
	SetExplicitMinSize(BSize(squareSize, squareSize));
	SetExplicitMaxSize(BSize(squareSize, squareSize));
	SetExplicitPreferredSize(BSize(squareSize, squareSize));
	BFont *font = new BFont(be_bold_font);
	font->SetSize(squareSize/2);
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
	UpdateTiles();
	BRect bounds = Bounds();
	if (fNumber < 2) {
		SetHighColor(205,193,180);
		FillRect(bounds);
		return;
	}

	BString c;
	c << fNumber;
	float width = StringWidth(c.String());
	rgb_color color_foreground = {255,255,255,0}, color_background;
	const static rgb_color cell2_bg = {238,228,218,0}, cell4_bg = {237,224,200,0},
		cell8_bg = {242,177,121,0}, cell16_bg = {245,149,99,0}, cell32_bg = {246,124,95,0},
		cell64_bg = {246,94,59,0}, cell128_bg = {237,207,114,0}, cell256_bg = {237,204,97,0},
		cell512_bg = {237,200,80,0}, cell1024_bg = {237,197,63,0}, cell2048P_bg = {237,194,46,0},
		cell_dark_fg = {119,110,101};
	
	switch (fNumber) {
	case 2: color_background = cell2_bg; color_foreground = cell_dark_fg; break;
	case 4: color_background = cell4_bg; color_foreground = cell_dark_fg; break;
	case 8: color_background = cell8_bg; break;
	case 16: color_background = cell16_bg; break;
	case 32: color_background = cell32_bg; break;
	case 64: color_background = cell64_bg; break;
	case 128: color_background = cell128_bg; break;
	case 256: color_background = cell256_bg; break;
	case 512: color_background = cell512_bg; break;
	case 1024: color_background = cell1024_bg; break;
	default: color_background = cell2048P_bg; break;
	}
	
	SetHighColor(color_background);
	FillRect(bounds);
	SetHighColor(color_foreground);
	SetFontSize(squareSize/2);
	DrawString(c.String(),
		BPoint(bounds.left + (squareSize/2) - width / 2, bounds.bottom - (squareSize/2) + (squareSize*0.2)));
}

NumberView::~NumberView()
{
}
