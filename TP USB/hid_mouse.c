/*
 * @brief This file contains USB HID Mouse example using USB ROM Drivers.
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2013
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */

#include "board.h"
#include <stdint.h>
#include <string.h>
#include "usbd_rom_api.h"
#include "hid_mouse.h"

#define NO_KEY 		  0
#define VALIDACION    20

uint32_t permisoA = 1;
uint32_t permisoB = 1;
uint32_t permisoC = 1;
uint32_t permisoD = 1;

void TeclaUP(void);
void TeclaDOWN(void);
void TeclaLEFT(void);
void TeclaRIGHT(void);

extern uint8_t getmousepos(void);

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/**
 * @brief Structure to hold mouse data
 */
typedef struct {
	USBD_HANDLE_T hUsb;	/*!< Handle to USB stack. */
	uint8_t report[MOUSE_REPORT_SIZE];	/*!< Last report data  */
	uint8_t tx_busy;	/*!< Flag indicating whether a report is pending in endpoint queue. */
} Mouse_Ctrl_T;

/** Singleton instance of mouse control */
static Mouse_Ctrl_T g_mouse;

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

extern const uint8_t Mouse_ReportDescriptor[];
extern const uint16_t Mouse_ReportDescSize;

/*****************************************************************************
 * Private functions
 ****************************************************************************/

static void setXYMouseReport(uint8_t *rep, int8_t xVal, int8_t yVal)
{
	rep[1] = (uint8_t) xVal;
	rep[2] = (uint8_t) yVal;
}

/* Routine to update mouse state report */
uint8_t joystick_status = 0;

static void Mouse_UpdateReport(void)
{

	switch (joystick_status) {

	case JOY_LEFT:
		setXYMouseReport(g_mouse.report, -127, 0);
		break;

	case JOY_RIGHT:
		setXYMouseReport(g_mouse.report, 127, 0);
		break;

	case JOY_UP:
		setXYMouseReport(g_mouse.report, 0, -127);
		break;

	case JOY_DOWN:
		setXYMouseReport(g_mouse.report, 0, 127);
		break;
	}
}

uint8_t tecla_actualA = NO_KEY;
void TeclaUP (void)
{

	/*Declaracion de Variables*/

	static uint8_t tecla_anteriorA = NO_KEY;
	static uint8_t contA = VALIDACION;

	/*Guardo la Tecla devuleta por Barrido_Teclado*/
	tecla_actualA = Chip_GPIO_GetPinState(LPC_GPIO, 2, 12);


	/*Proceso la Tecla*/
	if( tecla_actualA == NO_KEY )
	{
		permisoA = 1;
		tecla_anteriorA = NO_KEY;
		return; //Pongo return aca para no seguir con la ejecucion
	}

	/*Si la tecla actual es IGUAL a la anterior*/
	if( tecla_actualA == tecla_anteriorA )
	{
		contA--;		// Decremento el Contador
		/*Si el contador llego a 0 valido la tecla*/
		if( contA == 0 )
		{
			contA = VALIDACION;

			if(tecla_actualA != NO_KEY)
				{
				if(permisoA == 1)
					{
					permisoA = 0;
					joystick_status = 1;
					}
				}
		}
		return;
	}
	/*Si la tecla actual es DISTINTA a la anterior*/
	else
	{
		tecla_anteriorA = tecla_actualA;
		contA = VALIDACION;
		return;
	}
}

uint8_t tecla_actualB = NO_KEY;
void TeclaDOWN (void)
{

	/*Declaracion de Variables*/

	static uint8_t tecla_anteriorB = NO_KEY;
	static uint8_t contB = VALIDACION;

	/*Guardo la Tecla devuleta por Barrido_Teclado*/
	tecla_actualB = Chip_GPIO_GetPinState(LPC_GPIO, 2, 11);


	/*Proceso la Tecla*/
	if( tecla_actualB == NO_KEY )
	{
		permisoB = 1;
		tecla_anteriorB = NO_KEY;
		return; //Pongo return aca para no seguir con la ejecucion
	}

	/*Si la tecla actual es IGUAL a la anterior*/
	if( tecla_actualB == tecla_anteriorB )
	{
		contB--;		// Decremento el Contador
		/*Si el contador llego a 0 valido la tecla*/
		if( contB == 0 )
		{
			contB = VALIDACION;

			if(tecla_actualB != NO_KEY)
				{
				if(permisoB == 1)
					{
					permisoB = 0;
					joystick_status = 2;
					}
				}
		}
		return;
	}
	/*Si la tecla actual es DISTINTA a la anterior*/
	else
	{
		tecla_anteriorB = tecla_actualB;
		contB = VALIDACION;
		return;
	}
}

uint8_t tecla_actualC = NO_KEY;
void TeclaLEFT (void)
{

	/*Declaracion de Variables*/

	static uint8_t tecla_anteriorC = NO_KEY;
	static uint8_t contC = VALIDACION;

	/*Guardo la Tecla devuleta por Barrido_Teclado*/
	tecla_actualC = Chip_GPIO_GetPinState(LPC_GPIO, 2, 10);


	/*Proceso la Tecla*/
	if( tecla_actualC == NO_KEY )
	{
		permisoC = 1;
		tecla_anteriorC = NO_KEY;
		return; //Pongo return aca para no seguir con la ejecucion
	}

	/*Si la tecla actual es IGUAL a la anterior*/
	if( tecla_actualC == tecla_anteriorC )
	{
		contC--;		// Decremento el Contador
		/*Si el contador llego a 0 valido la tecla*/
		if( contC == 0 )
		{
			contC = VALIDACION;

			if(tecla_actualC != NO_KEY)
				{
				if(permisoC == 1)
					{
					permisoC = 0;
					joystick_status = 4;
					}
				}
		}
		return;
	}
	/*Si la tecla actual es DISTINTA a la anterior*/
	else
	{
		tecla_anteriorC = tecla_actualC;
		contC = VALIDACION;
		return;
	}
}

uint8_t tecla_actualD = NO_KEY;
void TeclaRIGHT (void)
{

	/*Declaracion de Variables*/

	static uint8_t tecla_anteriorD = NO_KEY;
	static uint8_t contD = VALIDACION;

	/*Guardo la Tecla devuleta por Barrido_Teclado*/
	tecla_actualD = Chip_GPIO_GetPinState(LPC_GPIO, 2, 8);


	/*Proceso la Tecla*/
	if( tecla_actualD == NO_KEY )
	{
		permisoD = 1;
		tecla_anteriorD = NO_KEY;
		return; //Pongo return aca para no seguir con la ejecucion
	}

	/*Si la tecla actual es IGUAL a la anterior*/
	if( tecla_actualD == tecla_anteriorD )
	{
		contD--;		// Decremento el Contador
		/*Si el contador llego a 0 valido la tecla*/
		if( contD == 0 )
		{
			contD = VALIDACION;

			if(tecla_actualD != NO_KEY)
				{
				if(permisoD == 1)
					{
					permisoD = 0;
					joystick_status = 8;
					}
				}
		}
		return;
	}
	/*Si la tecla actual es DISTINTA a la anterior*/
	else
	{
		tecla_anteriorD = tecla_actualD;
		contD = VALIDACION;
		return;
	}
}

/* HID Get Report Request Callback. Called automatically on HID Get Report Request */
static ErrorCode_t Mouse_GetReport(USBD_HANDLE_T hHid, USB_SETUP_PACKET *pSetup, uint8_t * *pBuffer, uint16_t *plength)
{
	/* ReportID = SetupPacket.wValue.WB.L; */
	switch (pSetup->wValue.WB.H) {
	case HID_REPORT_INPUT:
		Mouse_UpdateReport();
		*pBuffer = &g_mouse.report[0];
		*plength = MOUSE_REPORT_SIZE;
		break;

	case HID_REPORT_OUTPUT:				/* Not Supported */
	case HID_REPORT_FEATURE:			/* Not Supported */
		return ERR_USBD_STALL;
	}
	return LPC_OK;
}

/* HID Set Report Request Callback. Called automatically on HID Set Report Request */
static ErrorCode_t Mouse_SetReport(USBD_HANDLE_T hHid, USB_SETUP_PACKET *pSetup, uint8_t * *pBuffer, uint16_t length)
{
	/* we will reuse standard EP0Buf */
	if (length == 0) {
		return LPC_OK;
	}
	/* ReportID = SetupPacket.wValue.WB.L; */
	switch (pSetup->wValue.WB.H) {
	case HID_REPORT_INPUT:				/* Not Supported */
	case HID_REPORT_OUTPUT:				/* Not Supported */
	case HID_REPORT_FEATURE:			/* Not Supported */
		return ERR_USBD_STALL;
	}
	return LPC_OK;
}

/* HID interrupt IN endpoint handler */
static ErrorCode_t Mouse_EpIN_Hdlr(USBD_HANDLE_T hUsb, void *data, uint32_t event)
{
	switch (event) {
	case USB_EVT_IN:
		/* USB_EVT_IN occurs when HW completes sending IN packet. So clear the
		    busy flag for main loop to queue next packet.
		 */
		g_mouse.tx_busy = 0;
		break;
	}
	return LPC_OK;
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/* HID mouse interface init routine */
ErrorCode_t Mouse_Init(USBD_HANDLE_T hUsb,
					   USB_INTERFACE_DESCRIPTOR *pIntfDesc,
					   uint32_t *mem_base,
					   uint32_t *mem_size)
{
	USBD_HID_INIT_PARAM_T hid_param;
	USB_HID_REPORT_T reports_data[1];
	ErrorCode_t ret = LPC_OK;

	/* Do a quick check of if the interface descriptor passed is the right one. */
	if ((pIntfDesc == 0) || (pIntfDesc->bInterfaceClass != USB_DEVICE_CLASS_HUMAN_INTERFACE)) {
		return ERR_FAILED;
	}

	/* init joystick control */
	//Board_Joystick_Init();

	/* Init HID params */
	memset((void *) &hid_param, 0, sizeof(USBD_HID_INIT_PARAM_T));
	hid_param.max_reports = 1;
	hid_param.mem_base = *mem_base;
	hid_param.mem_size = *mem_size;
	hid_param.intf_desc = (uint8_t *) pIntfDesc;
	/* user defined functions */
	hid_param.HID_GetReport = Mouse_GetReport;
	hid_param.HID_SetReport = Mouse_SetReport;
	hid_param.HID_EpIn_Hdlr  = Mouse_EpIN_Hdlr;
	/* Init reports_data */
	reports_data[0].len = Mouse_ReportDescSize;
	reports_data[0].idle_time = 0;
	reports_data[0].desc = (uint8_t *) &Mouse_ReportDescriptor[0];
	hid_param.report_data  = reports_data;

	ret = USBD_API->hid->init(hUsb, &hid_param);

	/* update memory variables */
	*mem_base = hid_param.mem_base;
	*mem_size = hid_param.mem_size;
	/* store stack handle for later use. */
	g_mouse.hUsb = hUsb;

	return ret;
}

/* Mouse tasks */
void Mouse_Tasks(void)
{
	/* check device is configured before sending report. */
	if ( USB_IsConfigured(g_mouse.hUsb)) {
		if (g_mouse.tx_busy == 0) {
			/* update report based on board state */
			TeclaUP();
			TeclaDOWN();
			TeclaLEFT();
			TeclaRIGHT();
			Mouse_UpdateReport();
			/* send report data */
			g_mouse.tx_busy = 1;
			USBD_API->hw->WriteEP(g_mouse.hUsb, HID_EP_IN, &g_mouse.report[0], MOUSE_REPORT_SIZE);
		}
	}
	else {
		/* reset busy flag if we get disconnected. */
		g_mouse.tx_busy = 0;
	}

}
