/*
 *	wiiuse
 *
 *	Written By:
 *		Michael Laforest	< para >
 *		Email: < thepara (--AT--) g m a i l [--DOT--] com >
 *
 *	Copyright 2006-2007
 *
 *	This file is part of wiiuse.
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *	$Header$
 *
 */

/**
 *	@file
 *	@brief Module entry point.
 *
 *	The file includes functions that should be called
 *	by third party programs using wiiuse as a library.
 */

#include "wiiuse_internal.h"
#include "io.h"
#include "events.h"

/**
 *	@brief API data
 *
 *	This is what the \a api struct passed to wiiuse_main()
 *	will point to when it is called.
 */
struct wiiuse_api_t wiiuse_api = {
	/* version info */
	WIIUSE_VERSION,
	WIIUSE_API_VERSION,

	/* functions */
	wiiuse_init,
	wiiuse_disconnected,
	wiiuse_rumble,
	wiiuse_toggle_rumble,
	wiiuse_set_leds,
	wiiuse_motion_sensing,
	wiiuse_read_data,
	wiiuse_write_data,
	wiiuse_status,
	wiiuse_get_by_id,
	wiiuse_set_flags,
	wiiuse_set_smooth_alpha,
	wiiuse_set_ir,
	wiiuse_set_ir_vres,
	wiiuse_set_ir_position,
	wiiuse_set_aspect_ratio,
	wiiuse_set_bluetooth_stack,
	wiiuse_set_orient_threshold,

	wiiuse_find,
	wiiuse_connect,
	wiiuse_disconnect,

	wiiuse_poll
};


/**
 *	@brief Module entry point.
 *
 *	@param api Double pointer to an empty api struct.
 *
 *	@return 1 on success, 0 on failure
 *
 *	Load the function pointers into the \a api struct passed.
 */
DLLEXPORT int wiiuse_main(struct wiiuse_api_t** api) {
	if (!api)
		return 0;

	*api = &wiiuse_api;

	return 1;
}
