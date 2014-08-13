/*****************************************************************************
* Copyright (c) 2014 Ted John, Duncan Frost
* OpenRCT2, an open source clone of Roller Coaster Tycoon 2.
*
* This file is part of OpenRCT2.
*
* OpenRCT2 is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.

* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.

* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#include "addresses.h"
#include "game.h"
#include "peep.h"
#include "string_ids.h"
#include "sprite.h"
#include "sprites.h"
#include "widget.h"
#include "window.h"
#include "window_dropdown.h"

enum WINDOW_PEEP_PAGE {
	WINDOW_PEEP_OVERVIEW,
	WINDOW_PEEP_STATS,
	WINDOW_PEEP_RIDES,
	WINDOW_PEEP_FINANCE,
	WINDOW_PEEP_THOUGHTS,
	WINDOW_PEEP_INVENTORY
};

enum WINDOW_PEEP_WIDGET_IDX {
	WIDX_BACKGROUND,
	WIDX_TITLE,
	WIDX_CLOSE,
	WIDX_PAGE_BACKGROUND,
	WIDX_TAB_1,
	WIDX_TAB_2,
	WIDX_TAB_3,
	WIDX_TAB_4,
	WIDX_TAB_5,
	WIDX_TAB_6,
};

void window_peep_emptysub(){};

rct_widget window_peep_overview_widgets[] = { 
	{ WWT_FRAME,	0, 0,	191,	0,		156,	0x0FFFFFFFF,	STR_NONE },				// Panel / Background
	{ WWT_CAPTION,	0, 1,	190,	1,		14,		865,			STR_WINDOW_TITLE_TIP }, // Title
	{ WWT_CLOSEBOX, 0, 179, 189,	2,		13,		824,			STR_CLOSE_WINDOW_TIP }, // Close x button
	{ WWT_RESIZE,	1, 1,	191,	43,		156,	0x0FFFFFFFF,	STR_NONE },				// Resize
	{ WWT_TAB,		1, 3,	33,		17,		43,		0x2000144E,		1938 },					// Tab 1
	{ WWT_TAB,		1, 73,	64,		17,		43,		0x2000144E,		1940},					// Tab 2
	{ WWT_TAB,		1, 65,	95,		17,		43,		0x2000144E,		1941},					// Tab 3
	{ WWT_TAB,		1, 96,	126,	17,		43,		0x2000144E,		1942},					// Tab 4
	{ WWT_TAB,		1, 127, 157,	17,		43,		0x2000144E,		1943},					// Tab 5
	{ WWT_TAB,		1, 158, 188,	17,		43,		0x2000144E,		1944},					// Tab 6
	{ WWT_12,		1, 3,	166,	45,		56,		0x0FFFFFFFF,	STR_NONE},				// Label Thought marquee
	{ WWT_VIEWPORT, 1, 3,	166,	57,		143,	0x0FFFFFFFF,	STR_NONE },				// Viewport
	{ WWT_12,		1, 3,	166,	144,	154,	0x0FFFFFFFF,	STR_NONE},				// Label Action
	{ WWT_FLATBTN,	1, 167, 190,	45,		68,		SPR_RENAME,		1706},					// Rename Button
	{ WWT_FLATBTN,	1, 167, 190,	69,		92,		0x1430,			1055},					// Pickup Button
	{ WWT_FLATBTN,	1, 167, 190,	93,		116,	SPR_LOCATE,		STR_LOCATE_SUBJECT_TIP},// Locate Button
	{ WWT_FLATBTN,	1, 167, 190,	117,	140,	SPR_TRACK_PEEP,	1930},					// Track Button
	{ WIDGETS_END },
};

rct_widget *window_peep_page_widgets[] = {
	window_peep_overview_widgets
};

static void* window_peep_overview_events[] = {
	(void*)0x696A75,
	(void*)0x696A06,
	(void*)0x696FBE,
	window_peep_emptysub,
	window_peep_emptysub,
	window_peep_emptysub,
	(void*)0x696F45,
	window_peep_emptysub,
	window_peep_emptysub,
	(void*)0x696A5F,
	(void*)0x696A54,
	window_peep_emptysub,
	window_peep_emptysub,
	(void*)0x696A49,
	window_peep_emptysub,
	window_peep_emptysub,
	window_peep_emptysub,
	window_peep_emptysub,
	window_peep_emptysub,
	(void*)0x696A6A,
	(void*)0x697076,
	window_peep_emptysub,
	window_peep_emptysub,
	window_peep_emptysub,
	window_peep_emptysub,
	(void*)0x696749, //Invalidate
	(void*)0x696887, //Paint
	(void*)0x69707C
};

void* window_peep_page_events[] = {
	window_peep_overview_events
};

uint32 window_peep_page_enabled_widgets[] = {
	(1 << WIDX_CLOSE) |
	(1 << WIDX_TAB_1) |
	(1 << WIDX_TAB_2) |
	(1 << WIDX_TAB_3) |
	(1 << WIDX_TAB_4) |
	(1 << WIDX_TAB_5) |
	(1 << WIDX_TAB_6)
};

/**
 * rct2: 0x006989E9
 *
 */
void window_peep_open(rct_peep* peep){
	
	if (peep->type == PEEP_TYPE_STAFF){
		RCT2_CALLPROC_X(0x006989E9, 0, 0, 0, (int)peep, 0, 0, 0);
	}

	rct_window* window;

	window = window_bring_to_front_by_id(WC_PEEP, peep->sprite_index);
	if (window == NULL){
		window = window_create_auto_pos(192, 157, (uint32*)window_peep_overview_events, WC_PEEP, 0);
		window->widgets = window_peep_overview_widgets;
		window->enabled_widgets = window_peep_page_enabled_widgets[0];
		window->number = peep->sprite_index;
		window->page = 0;
		window->var_482 = 0;
		window->frame_no = 0;
		window->list_information_type = 0;
		window->var_492 = 0;
		window->var_494 = 0;
		RCT2_CALLPROC_X(0x006987A6, 0, 0, 0, 0, (int)window, 0, 0);
		window->min_width = 192;
		window->min_height = 157;
		window->max_width = 500;
		window->max_height = 450;
		window->flags = 8;
		window->no_list_items = 0;
		window->selected_list_item = -1;
		window->colours[0] = 1;
		window->colours[1] = 15;
		window->colours[2] = 15;
		window->var_482 = -1;
	}
	
	window->page = 0;
	RCT2_CALLPROC_X(0x006EB13A, 0, 0, 0, 0, (int)window, 0, 0);
	
	window->widgets = RCT2_GLOBAL(0x981D0C, rct_widget*);
	window->enabled_widgets = RCT2_GLOBAL(0x981D3C,uint32);
	window->var_020 = RCT2_GLOBAL(0x981D54,uint32);
	window->event_handlers = RCT2_GLOBAL(0x981D24,uint32*);
	window->pressed_widgets = 0;
	
	RCT2_CALLPROC_X(0x006987A6, 0, 0, 0, 0, (int)window, 0, 0);
	window_init_scroll_widgets(window);
	RCT2_CALLPROC_X(0x0069883C, 0, 0, 0, 0, (int)window, 0, 0);
}

rct_window* sub_6BEF1B(int eax, int ecx, int edx, rct_peep* peep)
{
	int v3 = eax; // push    eax

	ecx = 262167;		// mov     ecx, 40017h
	int ebx = 0xB400BE; // mov     ebx, offset unk_B400BE
	edx = 0x992AEC;	// mov     edx, offset off_992AEC
	
	int esi, edi;
	RCT2_CALLFUNC_X(0x6EA9B1, &eax, &ebx, &ecx, &edx, &esi, &edi, (int*)peep); // sub_6EA9B1(262167, off_992AEC);

	rct_window* w = (rct_window*)esi;

	w->widgets = RCT2_GLOBAL(0x9AF81C, rct_widget*);
	w->enabled_widgets = RCT2_GLOBAL(0x9929B0, uint32);
	w->number = v3;
	w->page = 0;
	w->var_482 = 0;
	w->frame_no = 0;

	RCT2_GLOBAL(esi + 1174, sint16) = 0; // ??
	
	//RCT2_GLOBAL(esi + 40, uint32) = 0x9AF81C;
	//RCT2_GLOBAL(esi + 8, uint32) = RCT2_GLOBAL(0x9929B0, uint32);
	//RCT2_GLOBAL(esi + 60, uint16) = v3;
	//RCT2_GLOBAL(esi + 1162, sint16) = 0; // page
	//RCT2_GLOBAL(esi + 1154, sint16) = 0;
	//RCT2_GLOBAL(esi + 1166, sint16) = 0; // frame_no
	

	eax = v3; // pop eax

	RCT2_CALLFUNC_X(0x6BED21, &eax, &ebx, &ecx, &edx, &esi, &edi, (int*)peep); // sub_6BED21();

	w->min_width = 190;
	w->min_height = 180;
	w->max_width = 500;
	w->max_height = 450;

	// RCT2_GLOBAL(esi + 52, sint16) = 190; // mix_width
	// RCT2_GLOBAL(esi + 56, sint16) = 180; // min_height
	// RCT2_GLOBAL(esi + 54, sint16) = 500; // max_width
	// RCT2_GLOBAL(esi + 58, sint16) = 450; // max_height

	RCT2_GLOBAL(esi + 62, uint16) = RCT2_GLOBAL(esi + 62, uint16) | 8; // flags????

	w->colours[0] = 1;
	w->colours[1] = 4;
	w->colours[2] = 4;

	// Colours
	// RCT2_GLOBAL(esi + 1210, uint8) = 1;
	// RCT2_GLOBAL(esi + 1211, uint8) = 4;
	// RCT2_GLOBAL(esi + 1212, uint8) = 4;

	return w;
}

/**
*
*  rct2: 0x006BEE98
*/
void window_staff_peep_open(rct_peep* peep)
{
	rct_window* w = window_bring_to_front_by_id(WC_PEEP, peep->sprite_index);
	if (!w) {
		int eax, ebx, ecx, edx, esi, edi;

		eax = peep->sprite_index;
		ecx = WC_PEEP;
		edx = peep->sprite_index;

		//RCT2_CALLFUNC_X(0x006BEF1B, &eax, &ebx, &ecx, &edx, &esi, &edi, (int*)peep);
		w = sub_6BEF1B(eax, ecx, edx, peep);
		//w = (rct_window*)esi;
	}

	int PEEP_BACKGROUND_IDX = 0;
	w->widgets = RCT2_GLOBAL(0x992998, rct_widget*);
	w->enabled_widgets = RCT2_GLOBAL(0x9929B0, uint32);
	w->var_020 = RCT2_GLOBAL(0x9929BC, uint32);
	w->event_handlers = (uint32*)RCT2_GLOBAL(0x9929A4, uint32);
	w->pressed_widgets = 0;
	RCT2_CALLPROC_X(0x006BED21, 0, 0, 0, 0, (int)w, 0, 0);
	window_init_scroll_widgets(w);
	RCT2_CALLPROC_X(0x006BEDA3, 0, 0, 0, 0, (int)w, 0, 0);
	if (g_sprite_list[w->number].peep.state == PEEP_STATE_PICKED) {
		RCT2_CALLPROC_X(w->event_handlers[1], 0, 0, 0, 10, (int)w, 0, 0);
	}
}