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
 *	@brief Handles wiimote events.
 *
 *	The file includes functions that handle the events
 *	that are sent from the wiimote to us.
 */

#include <stdio.h>

#ifndef WIN32
	#include <sys/time.h>
	#include <unistd.h>
#else
	#include <winsock2.h>
#endif

#include <sys/types.h>
#include <malloc.h>
#include <math.h>

#include "definitions.h"
#include "io.h"
#include "wiiuse_internal.h"
#include "dynamics.h"
#include "ir.h"
#include "nunchuk.h"
#include "classic.h"
#include "events.h"

static void propagate_event(struct wiimote_t* wm, byte event, byte* msg);
static void event_data_read(struct wiimote_t* wm, byte* msg);
static void event_status(struct wiimote_t* wm, byte* msg);
static void handle_expansion(struct wiimote_t* wm, byte* msg);

static void save_state(struct wiimote_t* wm);
static int state_changed(struct wiimote_t* wm);

/**
 *	@brief Poll the wiimotes for any events.
 *
 *	@param wm		An array of pointers to wiimote_t structures.
 *	@param wiimotes	The number of wiimote_t structures in the \a wm array.
 *
 *	It is necessary to poll the wiimote devices for events
 *	that occur.  If an event occurs on a particular wiimote,
 *	the registered callback function is invoked.
 */
DLLEXPORT void wiiuse_poll(struct wiimote_t** wm, int wiimotes) {
	#ifndef WIN32
		/*
		 *	*nix
		 */
		struct timeval tv;
		fd_set fds;
		int r;
		int i;
		int highest_fd = 0;

		if (!wm) return;

		/* block select() for 1/2000th of a second */
		tv.tv_sec = 0;
		tv.tv_usec = 500;

		FD_ZERO(&fds);

		for (i = 0; i < wiimotes; ++i) {
			/* only poll it if it is connected */
			if (WIIMOTE_IS_SET(wm[i], WIIMOTE_STATE_CONNECTED)) {
				FD_SET(wm[i]->in_sock, &fds);

				/* find the highest fd of the connected wiimotes */
				if (wm[i]->in_sock > highest_fd)
					highest_fd = wm[i]->in_sock;
			}
		}

		if (select(highest_fd + 1, &fds, NULL, NULL, &tv) == -1) {
    		ERROR("Unable to select() the wiimote interrupt socket(s).");
    		return;
		}

		/* check each socket for an event */
		for (i = 0; i < wiimotes; ++i) {
			/* if this wiimote is not connected, skip it */
			if (!WIIMOTE_IS_CONNECTED(wm[i]))
				continue;

			if (!FD_ISSET(wm[i]->in_sock, &fds))
				/* no event */
				continue;

			/* clear out the event buffer */
			memset(wm[i]->event, 0, sizeof(wm[i]->event));

			/* read the pending message into the buffer */
			r = read(wm[i]->in_sock, wm[i]->event, sizeof(wm[i]->event));
			if (r == -1) {
				/* error reading data */
				ERROR("Receiving wiimote data (id %i).", wm[i]->unid);
				continue;
			}
			if (!r) {
				/* remote disconnect */
				wiiuse_disconnected(wm[i]);
				continue;
			}

			/* propagate the event */
			propagate_event(wm[i], wm[i]->event[1], wm[i]->event+2);
		}
	#else
		/*
		 *	Windows
		 */
		int i;

		if (!wm) return;

		for (i = 0; i < wiimotes; ++i) {
			if (wiiuse_io_read(wm[i])) {
				/* propagate the event */
				propagate_event(wm[i], wm[i]->event[0], wm[i]->event+1);

				/* clear out the event buffer */
				memset(wm[i]->event, 0, sizeof(wm[i]->event));
			}
		}
	#endif
}


/**
 *	@brief Analyze the event that occured on a wiimote.
 *
 *	@param wm		An array of pointers to wiimote_t structures.
 *	@param event	The event that occured.
 *	@param msg		The message specified in the event packet.
 *
 *	Pass the event to the registered event callback.
 */
static void propagate_event(struct wiimote_t* wm, byte event, byte* msg) {
	save_state(wm);

	switch (event) {
		case WM_RPT_BTN:
		{
			/* button */
			wiiuse_pressed_buttons(wm, msg);
			break;
		}
		case WM_RPT_BTN_ACC:
		{
			/* button - motion */
			wiiuse_pressed_buttons(wm, msg);

			wm->accel.x = msg[2];
			wm->accel.y = msg[3];
			wm->accel.z = msg[4];

			/* calculate the remote orientation */
			calculate_orientation(&wm->accel_calib, &wm->accel, &wm->orient, WIIMOTE_IS_FLAG_SET(wm, WIIUSE_SMOOTHING));

			/* calculate the gforces on each axis */
			calculate_gforce(&wm->accel_calib, &wm->accel, &wm->gforce);

			break;
		}
		case WM_RPT_READ:
		{
			/* data read */
			event_data_read(wm, msg);

			/* yeah buttons may be pressed, but this wasn't an "event" */
			return;
		}
		case WM_RPT_CTRL_STATUS:
		{
			/* controller status */
			event_status(wm, msg);

			/* don't execute the event callback */
			return;
		}
		case WM_RPT_BTN_EXP:
		{
			/* button - expansion */
			wiiuse_pressed_buttons(wm, msg);
			handle_expansion(wm, msg+2);

			break;
		}
		case WM_RPT_BTN_ACC_EXP:
		{
			/* button - motion - expansion */
			wiiuse_pressed_buttons(wm, msg);

			wm->accel.x = msg[2];
			wm->accel.y = msg[3];
			wm->accel.z = msg[4];

			calculate_orientation(&wm->accel_calib, &wm->accel, &wm->orient, WIIMOTE_IS_FLAG_SET(wm, WIIUSE_SMOOTHING));
			calculate_gforce(&wm->accel_calib, &wm->accel, &wm->gforce);

			handle_expansion(wm, msg+5);

			break;
		}
		case WM_RPT_BTN_ACC_IR:
		{
			/* button - motion - ir */
			wiiuse_pressed_buttons(wm, msg);

			wm->accel.x = msg[2];
			wm->accel.y = msg[3];
			wm->accel.z = msg[4];

			calculate_orientation(&wm->accel_calib, &wm->accel, &wm->orient, WIIMOTE_IS_FLAG_SET(wm, WIIUSE_SMOOTHING));
			calculate_gforce(&wm->accel_calib, &wm->accel, &wm->gforce);

			/* ir */
			calculate_extended_ir(wm, msg+5);

			break;
		}
		case WM_RPT_BTN_IR_EXP:
		{
			/* button - ir - expansion */
			wiiuse_pressed_buttons(wm, msg);
			handle_expansion(wm, msg+12);

			/* ir */
			calculate_basic_ir(wm, msg+2);

			break;
		}
		case WM_RPT_BTN_ACC_IR_EXP:
		{
			/* button - motion - ir - expansion */
			wiiuse_pressed_buttons(wm, msg);

			wm->accel.x = msg[2];
			wm->accel.y = msg[3];
			wm->accel.z = msg[4];

			calculate_orientation(&wm->accel_calib, &wm->accel, &wm->orient, WIIMOTE_IS_FLAG_SET(wm, WIIUSE_SMOOTHING));
			calculate_gforce(&wm->accel_calib, &wm->accel, &wm->gforce);

			handle_expansion(wm, msg+15);

			/* ir */
			calculate_basic_ir(wm, msg+5);

			break;
		}
		case WM_RPT_WRITE:
		{
			/* write feedback - safe to skip */
			break;
		}
		default:
		{
			WARNING("Unknown event, can not handle it [Code 0x%x].", event);
			return;
		}
	}

	/* invoke the callback function */
	if (state_changed(wm) && (wm->event_cb))
		wm->event_cb(wm);
}


/**
 *	@brief Find what buttons are pressed.
 *
 *	@param wm		Pointer to a wiimote_t structure.
 *	@param msg		The message specified in the event packet.
 */
void wiiuse_pressed_buttons(struct wiimote_t* wm, byte* msg) {
	short now;

	/* convert to big endian */
	now = BIG_ENDIAN_SHORT(*(short*)msg) & WIIMOTE_BUTTON_ALL;

	/* pressed now & were pressed, then held */
	wm->btns_held = (now & wm->btns);

	/* were pressed or were held & not pressed now, then released */
	wm->btns_released = ((wm->btns | wm->btns_held) & ~now);

	/* buttons pressed now */
	wm->btns = now;
}


/**
 *	@brief Received a data packet from a read request.
 *
 *	@param wm		Pointer to a wiimote_t structure.
 *	@param msg		The message specified in the event packet.
 *
 *	Data from the wiimote comes in packets.  If the requested
 *	data segment size is bigger than one packet can hold then
 *	several packets will be received.  These packets are first
 *	reassembled into one, then the registered callback function
 *	that handles data reads is invoked.
 */
static void event_data_read(struct wiimote_t* wm, byte* msg) {
	/* we must always assume the packet received is from the most recent request */
	byte err;
	byte len;
	unsigned short offset;
	struct read_req_t* req = wm->read_req;

	wiiuse_pressed_buttons(wm, msg);

	/* if we don't have a request out then we didn't ask for this packet */
	if (!req) {
		WARNING("Received data packet when no request was made.");
		return;
	}

	err = msg[2] & 0x0F;

	if (err == 0x08) {
		WARNING("Unable to read data - address does not exist.");
		return;
	} else if (err == 0x07) {
		WARNING("Unable to read data - address is for write-only registers.");
		return;
	} else if (err) {
		WARNING("Unable to read data - unknown error code %x.", err);
		return;
	}

	len = ((msg[2] & 0xF0) >> 4) + 1;
	offset = BIG_ENDIAN_SHORT(*(unsigned short*)(msg + 3));
	req->addr = (req->addr & 0xFFFF);

	req->wait -= len;
	if (req->wait >= req->size)
		/* this should never happen */
		req->wait = 0;

	DEBUG("Received read packet:");
	DEBUG("    Packet read offset:   %i bytes", offset);
	DEBUG("    Request read offset:  %i bytes", req->addr);
	DEBUG("    Read offset into buf: %i bytes", offset - req->addr);
	DEBUG("    Read data size:       %i bytes", len);
	DEBUG("    Still need:           %i bytes", req->wait);

	/* reconstruct this part of the data */
	memcpy((req->buf + offset - req->addr), (msg + 5), len);

	/* if all data has been received, execute the read event callback */
	if (!req->wait) {
		req->cb(wm, req->buf, req->size);

		/* delete this request */
		wm->read_req = req->next;
		free(req);

		/* if another request exists send it to the wiimote */
		if (wm->read_req)
			wiiuse_send_next_pending_read_request(wm);
	}
}


/**
 *	@brief Read the controller status.
 *
 *	@param wm		Pointer to a wiimote_t structure.
 *	@param msg		The message specified in the event packet.
 *
 *	Read the controller status and execute the registered status callback.
 */
static void event_status(struct wiimote_t* wm, byte* msg) {
	float battery_level;
	int led[4] = {0};
	int attachment = 0;
	int speaker = 0;
	int ir = 0;
	int exp_changed = 0;

	wiiuse_pressed_buttons(wm, msg);

	/* find what LEDs are lit */
	if (msg[2] & WM_CTRL_STATUS_BYTE1_LED_1)	led[0] = 1;
	if (msg[2] & WM_CTRL_STATUS_BYTE1_LED_2)	led[1] = 1;
	if (msg[2] & WM_CTRL_STATUS_BYTE1_LED_3)	led[2] = 1;
	if (msg[2] & WM_CTRL_STATUS_BYTE1_LED_4)	led[3] = 1;

	/* is an attachment connected to the expansion port? */
	if ((msg[2] & WM_CTRL_STATUS_BYTE1_ATTACHMENT) == WM_CTRL_STATUS_BYTE1_ATTACHMENT)
		attachment = 1;

	/* is the speaker enabled? */
	if ((msg[2] & WM_CTRL_STATUS_BYTE1_SPEAKER_ENABLED) == WM_CTRL_STATUS_BYTE1_SPEAKER_ENABLED)
		speaker = 1;

	/* is IR sensing enabled? */
	if ((msg[2] & WM_CTRL_STATUS_BYTE1_IR_ENABLED) == WM_CTRL_STATUS_BYTE1_IR_ENABLED)
		ir = 1;

	/* find the battery level and normalize between 0 and 1 */
	battery_level = (msg[5] / (float)WM_MAX_BATTERY_CODE);

	/* expansion port */
	if (attachment && !WIIMOTE_IS_SET(wm, WIIMOTE_STATE_EXP)) {
		/* send the initialization code for the attachment */
		byte* handshake_buf;
		byte buf = 0x00;
		exp_changed = 1;

		wiiuse_write_data(wm, WM_EXP_MEM_ENABLE, &buf, 1);

		/* get the calibration data */
		handshake_buf = malloc(EXP_HANDSHAKE_LEN * sizeof(byte));
		wiiuse_read_data(wm, handshake_expansion, handshake_buf, WM_EXP_MEM_CALIBR, EXP_HANDSHAKE_LEN);

		/* tell the wiimote to send expansion data */
		WIIMOTE_ENABLE_STATE(wm, WIIMOTE_STATE_EXP);

	} else if (!attachment && WIIMOTE_IS_SET(wm, WIIMOTE_STATE_EXP)) {
		/* attachment removed */
		WIIMOTE_DISABLE_STATE(wm, WIIMOTE_STATE_EXP);
		wm->exp.type = EXP_NONE;
		exp_changed = 1;

		/* tell the assoicated module the expansion was removed */
		switch (wm->exp.type) {
			case EXP_NUNCHUK:
				nunchuk_disconnected(&wm->exp.nunchuk);
				break;
			case EXP_CLASSIC:
				classic_ctrl_disconnected(&wm->exp.classic);
				break;
			default:
				break;
		}
	}

	/*
	 *	From now on the remote will only send status packets.
	 *	We need to send a WIIMOTE_CMD_REPORT_TYPE packet to
	 *	reenable other incoming reports.
	 */
	if (exp_changed && WIIMOTE_IS_SET(wm, WIIMOTE_STATE_IR)) {
		/*
		 *	Since the expansion status changed IR needs to
		 *	be reset for the new IR report mode.
		 */
		WIIMOTE_DISABLE_STATE(wm, WIIMOTE_STATE_IR);
		wiiuse_set_ir(wm, 1);
	} else
		wiiuse_set_report_type(wm);

	/* execute the callback */
	if (wm->stat_cb)
		wm->stat_cb(wm, attachment, speaker, ir, led, battery_level);
}


/**
 *	@brief Handle data from the expansion.
 *
 *	@param wm		A pointer to a wiimote_t structure.
 *	@param msg		The message specified in the event packet for the expansion.
 */
static void handle_expansion(struct wiimote_t* wm, byte* msg) {
	switch (wm->exp.type) {
		case EXP_NUNCHUK:
			nunchuk_event(&wm->exp.nunchuk, msg);
			break;
		case EXP_CLASSIC:
			classic_ctrl_event(&wm->exp.classic, msg);
			break;
		default:
			break;
	}
}


/**
 *	@brief Handle the handshake data from the expansion device.
 *
 *	@param wm		A pointer to a wiimote_t structure.
 *	@param data		The data read in from the device.
 *	@param len		The length of the data block, in bytes.
 */
void handshake_expansion(struct wiimote_t* wm, byte* data, unsigned short len) {
	int id = BIG_ENDIAN_LONG(*(int*)(data + 220));

	/* call the corresponding handshake function for this expansion */
	switch (id) {
		case EXP_ID_CODE_NUNCHUK:
			nunchuk_handshake(wm, &wm->exp.nunchuk, data, len);
			break;
		case EXP_ID_CODE_CLASSIC_CONTROLLER:
			classic_ctrl_handshake(wm, &wm->exp.classic, data, len);
			break;
		default:
			WARNING("Unknown expansion type. Code: 0x%x", id);
			break;
	}

	free(data);
}


/**
 *	@brief Save important state data.
 *	@param wm	A pointer to a wiimote_t structure.
 */
static void save_state(struct wiimote_t* wm) {
	/* expansion */
	switch (wm->exp.type) {
		case EXP_NUNCHUK:
			wm->lstate.exp_ljs_ang = wm->exp.nunchuk.js.ang;
			wm->lstate.exp_ljs_mag = wm->exp.nunchuk.js.mag;
			wm->lstate.exp_btns = wm->exp.nunchuk.btns;

		case EXP_CLASSIC:
			wm->lstate.exp_ljs_ang = wm->exp.classic.ljs.ang;
			wm->lstate.exp_ljs_mag = wm->exp.classic.ljs.mag;
			wm->lstate.exp_rjs_ang = wm->exp.classic.rjs.ang;
			wm->lstate.exp_rjs_mag = wm->exp.classic.rjs.mag;
			wm->lstate.exp_r_shoulder = wm->exp.classic.r_shoulder;
			wm->lstate.exp_l_shoulder = wm->exp.classic.l_shoulder;

		case EXP_NONE:
			break;
	}

	/* ir */
	if (WIIUSE_USING_IR(wm)) {
		wm->lstate.ir_ax = wm->ir.ax;
		wm->lstate.ir_ay = wm->ir.ay;
		wm->lstate.ir_distance = wm->ir.distance;
	}

	/* wiimote */
	wm->lstate.btns = wm->btns;
}


/**
 *	@brief Determine if the current state differs significantly from the previous.
 *	@param wm	A pointer to a wiimote_t structure.
 *	@return	1 if a significant change occured, 0 if not.
 */
static int state_changed(struct wiimote_t* wm) {
	#define STATE_CHANGED(a, b)		if (a != b)				return 1

	#define CROSS_THRESH(last, now, thresh)										\
				do {															\
					if (WIIMOTE_IS_FLAG_SET(wm, WIIUSE_ORIENT_THRESH)) {		\
						if ((diff_f(last.roll, now.roll) >= thresh) ||			\
							(diff_f(last.pitch, now.pitch) >= thresh) ||		\
							(diff_f(last.yaw, now.yaw) >= thresh))				\
						{														\
							last = now;											\
							return 1;											\
						}														\
					} else {													\
						if (last.roll != now.roll)		return 1;				\
						if (last.pitch != now.pitch)	return 1;				\
						if (last.yaw != now.yaw)		return 1;				\
					}															\
				} while (0)

	/* ir */
	if (WIIUSE_USING_IR(wm)) {
		STATE_CHANGED(wm->lstate.ir_ax, wm->ir.ax);
		STATE_CHANGED(wm->lstate.ir_ay, wm->ir.ay);
		STATE_CHANGED(wm->lstate.ir_distance, wm->ir.distance);
	}

	/* orientation */
	if (WIIUSE_USING_ACC(wm))
		CROSS_THRESH(wm->lstate.orient, wm->orient, wm->orient_threshold);

	/* expansion */
	switch (wm->exp.type) {
		case EXP_NUNCHUK:
			STATE_CHANGED(wm->lstate.exp_ljs_ang, wm->exp.nunchuk.js.ang);
			STATE_CHANGED(wm->lstate.exp_ljs_mag, wm->exp.nunchuk.js.mag);
			STATE_CHANGED(wm->lstate.exp_btns, wm->exp.nunchuk.btns);

			CROSS_THRESH(wm->lstate.exp_orient, wm->exp.nunchuk.orient, wm->orient_threshold);

		case EXP_CLASSIC:
			STATE_CHANGED(wm->lstate.exp_ljs_ang, wm->exp.classic.ljs.ang);
			STATE_CHANGED(wm->lstate.exp_ljs_mag, wm->exp.classic.ljs.mag);
			STATE_CHANGED(wm->lstate.exp_rjs_ang, wm->exp.classic.rjs.ang);
			STATE_CHANGED(wm->lstate.exp_rjs_mag, wm->exp.classic.rjs.mag);
			STATE_CHANGED(wm->lstate.exp_r_shoulder, wm->exp.classic.r_shoulder);
			STATE_CHANGED(wm->lstate.exp_l_shoulder, wm->exp.classic.l_shoulder);

		case EXP_NONE:
			break;
	}

	STATE_CHANGED(wm->lstate.btns, wm->btns);

	return 0;
}
