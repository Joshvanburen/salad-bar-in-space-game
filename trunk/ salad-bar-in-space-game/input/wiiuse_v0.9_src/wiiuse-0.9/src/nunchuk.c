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
 *	@brief Nunchuk expansion device.
 */

#include <stdio.h>
#include <malloc.h>
#include <math.h>

#include "definitions.h"
#include "wiiuse_internal.h"
#include "dynamics.h"
#include "events.h"
#include "nunchuk.h"

static void nunchuk_pressed_buttons(struct nunchuk_t* nc, byte now);

/**
 *	@brief Handle the handshake data from the nunchuk.
 *
 *	@param nc		A pointer to a nunchuk_t structure.
 *	@param data		The data read in from the device.
 *	@param len		The length of the data block, in bytes.
 */
void nunchuk_handshake(struct wiimote_t* wm, struct nunchuk_t* nc, byte* data, unsigned short len) {
	int i;

	nc->btns = 0;
	nc->btns_held = 0;
	nc->btns_released = 0;

	/* set the smoothing to the same as the wiimote */
	nc->flags = &wm->flags;
	nc->accel_calib.st_alpha = wm->accel_calib.st_alpha;

	/* decrypt data */
	for (i = 0; i < len; ++i)
		data[i] = (data[i] ^ 0x17) + 0x17;

	nc->accel_calib.cal_zero.x = data[0];
	nc->accel_calib.cal_zero.y = data[1];
	nc->accel_calib.cal_zero.z = data[2];
	nc->accel_calib.cal_g.x = data[4];
	nc->accel_calib.cal_g.y = data[5];
	nc->accel_calib.cal_g.z = data[6];
	nc->js.max.x = data[8];
	nc->js.min.x = data[9];
	nc->js.center.x = data[10];
	nc->js.max.y = data[11];
	nc->js.min.y = data[12];
	nc->js.center.y = data[13];

	/*
	 *	Sometimes the data returned here is not correct.
	 *	This might happen because the wiimote is lagging
	 *	behind our initialization sequence.
	 *	To fix this just request the handshake again.
	 */
	if (data[0] == 0xff) {
		/* get the calibration data */
		byte* handshake_buf = malloc(EXP_HANDSHAKE_LEN * sizeof(byte));
		wiiuse_read_data(wm, handshake_expansion, handshake_buf, WM_EXP_MEM_CALIBR, EXP_HANDSHAKE_LEN);
	} else
		/* handshake done */
		wm->exp.type = EXP_NUNCHUK;
}


/**
 *	@brief The nunchuk disconnected.
 *
 *	@param nc		A pointer to a nunchuk_t structure.
 */
void nunchuk_disconnected(struct nunchuk_t* nc) {
	memset(nc, 0, sizeof(struct nunchuk_t));
}



/**
 *	@brief Handle nunchuk event.
 *
 *	@param nc		A pointer to a nunchuk_t structure.
 *	@param msg		The message specified in the event packet.
 */
void nunchuk_event(struct nunchuk_t* nc, byte* msg) {
	int i;

	/* decrypt data */
	for (i = 0; i < 6; ++i)
		msg[i] = (msg[i] ^ 0x17) + 0x17;

	/* get button states */
	nunchuk_pressed_buttons(nc, msg[5]);

	/* calculate joystick state */
	calc_joystick_state(&nc->js, msg[0], msg[1]);

	/* calculate orientation */
	nc->accel.x = msg[2];
	nc->accel.y = msg[3];
	nc->accel.z = msg[4];

	calculate_orientation(&nc->accel_calib, &nc->accel, &nc->orient, NUNCHUK_IS_FLAG_SET(nc, WIIUSE_SMOOTHING));
	calculate_gforce(&nc->accel_calib, &nc->accel, &nc->gforce);
}


/**
 *	@brief Find what buttons are pressed.
 *
 *	@param nc		Pointer to a nunchuk_t structure.
 *	@param msg		The message byte specified in the event packet.
 */
static void nunchuk_pressed_buttons(struct nunchuk_t* nc, byte now) {
	/* message is inverted (0 is active, 1 is inactive) */
	now = ~now;

	/* pressed now & were pressed, then held */
	nc->btns_held = (now & nc->btns);

	/* were pressed or were held & not pressed now, then released */
	nc->btns_released = ((nc->btns | nc->btns_held) & ~now);

	/* buttons pressed now */
	nc->btns = now;
}
