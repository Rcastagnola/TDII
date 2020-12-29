/*
 * Core.h
 *
 *  Created on: 23 oct. 2020
 *      Author: Luis
 */

#ifndef SRC_APLICACION_CORE_CORE_H_
#define SRC_APLICACION_CORE_CORE_H_

#include "main.h"
void tarea_sistema(void *p);
void ProcesarTeclado(void);
void ModificarVariable(uint8_t);

enum{
	IND_ACT_LCD=0,
	IND_ACT_VAL_LCD,
	IND_BORR_LCD,
	IND_LEE_TECL,
	CFG_ACT_LCD,
	CFG_BORR_LCD,
	CFG_LEE_TECL,
	SUBM_CFG_ALARM_BORR,
	SUBM_CFG_ALARM,
	SUBM_CFG_ALARM_LEE_TECL,
	SUBM_CFG_ALARM_MAX_BORR,
	SUBM_CFG_ALARM_MAX,
	SUBM_CFG_ALARM_MAX_ACT,
	SUBM_CFG_ALARM_MAX_LEE_TECL,
	SUBM_CFG_ALARM_MIN_BORR,
	SUBM_CFG_ALARM_MIN,
	SUBM_CFG_ALARM_MIN_ACT,
	SUBM_CFG_ALARM_MIN_LEE_TECL,

	SUBM_CFG_VAR_BORR,
	SUBM_CFG_VAR,
	SUBM_CFG_VAR_LEE_TECL,

	SUBM_CFG_COMPR_MAX_BORR,
	SUBM_CFG_COMPR_MAX,
	SUBM_CFG_COMPR_MAX_ACT,
	SUBM_CFG_COMPR_MAX_LEE_TECL,

	SUBM_CFG_COMPR_MIN_BORR,
	SUBM_CFG_COMPR_MIN,
	SUBM_CFG_COMPR_MIN_ACT,
	SUBM_CFG_COMPR_MIN_LEE_TECL,

	DBG_ACT_LCD,
	DBG_BORR_LCD,
	DBG_LEE_TECL,
	SUBM_DBG_SAL_BORR,
	SUBM_DBG_SAL,
	SUBM_DBG_SAL_LEE_TECL,
	SUBM_DBG_ADC_BORR,
	SUBM_DBG_ADC,
	SUBM_DBG_ADC_ACT_LCD,
	SUBM_DBG_ADC_LEE_TECL,
	SUBM_DBG_TEC_BORR,
	SUBM_DBG_TEC,
	SUBM_DBG_TEC_LEE_TECL,

	SUBM_SAL_TCOMP_BORR,
	SUBM_SAL_TCOMP,
	SUBM_SAL_TCOMP_LEE_TECL,

	SUBM_SAL_TALARM_BORR,
	SUBM_SAL_TALARM,
	SUBM_SAL_TALARM_LEE_TECL,

	SUBM_SAL_TDESC_BORR,
	SUBM_SAL_TDESC,
	SUBM_SAL_TDESC_LEE_TECL,

	PANTALLA_ALERTA_ALTA_PRESION_BORR,
	PANTALLA_ALERTA_ALTA_PRESION,
	PANTALLA_ALERTA_BAJA_PRESION_BORR,
	PANTALLA_ALERTA_BAJA_PRESION,
	PANTALLA_ALERTA_ERROR_LAZO_BORR,
	PANTALLA_ALERTA_ERROR_LAZO,
	PANTALLA_ALERTA_LEE_TECL,

	PANTALLA_ERROR_PERMANENTE
};

#define CORE_IDLE		0
#define CORE_BUSY		1

#define ESCRIBIENDO_LCD		0
#define LEERTECLADO			1
#define BORRANDO_LCD		2

#define NO_KEY			16


/***** DEFINES y PROTOTIIPOS de la tarea de control de presion  ***********/
void tarea_control_presion(void *p);
void volverAlControlDePresion();


#define START_UP_TIME 90000  //Tiempo de inicio en milisegundos. durante este tiempo se prevee el inicio del sistema y por lo tanto no se controla la baja presion de la linea
#define TIEMPO_ESPERA_ALTA_PRESION 1000 //Tiempo entre que detecta una alta presion y si no controla la situacion muere.
#define TIEMPO_ESPERA_BAJA_PRESION 1000 //Tiempo entre que detecta una baja presion y si no controla la situacion muere.

extern int32_t presionMedida; //Variable que almacena la presion medida calculada por el adc
extern uint32_t corrienteMedida; //corriente del sensor calculada en el ADC
extern uint8_t flag_datos_completos; //Este flag se pone en 1 cuando el buffer del ADC esta completo y hay presion valida


enum{
	START_UP,
	FULL_MODE,
	SOBRE_PRESION,
	SOBRE_PRESION_CRITICA,
	BAJA_PRESION,
	BAJA_PRESION_CRITICA,
	ERROR_LAZO,
	ERROR_PERMANENTE
};


#endif /* SRC_APLICACION_CORE_CORE_H_ */
