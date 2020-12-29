/*
 * Core.c
 *
 *  Created on: 23 oct. 2020
 *      Author: Luis
 */


#include "Core.h"

extern uint8_t Estado_pantalla;
extern uint16_t teclasApretadas;
extern uint32_t valor_promediado;
extern tipo_configuracion configuracion;
extern uint32_t corrienteMedida;
extern int32_t presionMedida;

uint8_t CORE_STATUS=0;
uint8_t teclaPresionada;
uint8_t accionCore=IND_BORR_LCD;
uint16_t refrescarPantalla=0;
uint8_t control=0;
uint32_t variable_temp=0;
uint32_t tiempo_de_comienzo = 0; //aca almaceno cuanto tiempo llevo de ejecucion para implementar el safe start

//#define tecla_presionada	teclasApretadas != 0

uint8_t estado_ejecucion = START_UP;
uint8_t flag_debug = 0;

void tarea_sistema(void *p)
{
	ProcesarTeclado();

	switch(accionCore)
	{

		///MENU INDICACION///
		case IND_BORR_LCD:
			flag_debug = 0;
			control=BorrarPantalla();
			if(control == WRITING_OK) accionCore=IND_ACT_LCD;
			break;
		case IND_ACT_LCD:
			control=PantallaIndicacion(presionMedida);
			if(control == WRITING_OK) accionCore=IND_LEE_TECL;
			break;
		case IND_ACT_VAL_LCD:
			control=PantallaActualizarIndicacion(presionMedida);
			if(control == WRITING_OK) accionCore=IND_LEE_TECL;
			break;
		case IND_LEE_TECL:
			switch(teclaPresionada)
			{
				case TECLA_A:
					accionCore=DBG_BORR_LCD;
					break;
				case TECLA_B:
					accionCore=CFG_BORR_LCD;
					break;
				case TECLA_C:
					accionCore=IND_BORR_LCD;
					break;
				case TECLA_D:
					break;
				case NO_KEY:
					break;
			}
			refrescarPantalla++;
			if(refrescarPantalla==500)
			{
				accionCore=IND_ACT_VAL_LCD;
				refrescarPantalla=0;
			}
			break;


		////MENU CONFIG///

			case CFG_BORR_LCD:
				control=BorrarPantalla();
				if(control == WRITING_OK) accionCore=CFG_ACT_LCD;
				break;
			case CFG_ACT_LCD:
				control=PantallaConfig();
				if(control == WRITING_OK) accionCore=CFG_LEE_TECL;
				break;
			case CFG_LEE_TECL:
				switch(teclaPresionada)
				{
					case TECLA_A:
						accionCore=IND_BORR_LCD;
						break;
					case TECLA_B:
						accionCore=DBG_BORR_LCD;
						break;
					case TECLA_C:
						accionCore=IND_BORR_LCD;
						break;
					case TECLA_D:
						accionCore=SUBM_CFG_ALARM_BORR;
						break;
					case NO_KEY:
						break;
				}
				break;

			/// COMIENZO SUBMENU ALARMAS ///

			case SUBM_CFG_ALARM_BORR:
				control=BorrarPantalla();
				if(control == WRITING_OK) accionCore=SUBM_CFG_ALARM;
				break;
			case SUBM_CFG_ALARM:
				control=PantallaAlarm();
				if(control == WRITING_OK) accionCore=SUBM_CFG_ALARM_LEE_TECL;
				break;
			case SUBM_CFG_ALARM_LEE_TECL:
				switch(teclaPresionada)
				{
					case TECLA_A:
						accionCore=SUBM_CFG_VAR_BORR;
						break;
					case TECLA_B:
						accionCore=SUBM_CFG_VAR_BORR;
						break;
					case TECLA_C:
						accionCore=CFG_BORR_LCD;
						break;
					case TECLA_D:
						accionCore=SUBM_CFG_ALARM_MAX_BORR;
						break;
					case NO_KEY:
						break;
				}
				break;

		/// COMIENZO SUBMENU ALARMAS MAXIMO///

			case SUBM_CFG_ALARM_MAX_BORR:
				control=BorrarPantalla();
				if(control == WRITING_OK) accionCore=SUBM_CFG_ALARM_MAX;
				break;
			case SUBM_CFG_ALARM_MAX:
				control=PantallaAltaPresion(configuracion.alarma_alta_presion);
				if(control == WRITING_OK) accionCore=SUBM_CFG_ALARM_MAX_LEE_TECL;
				break;
			case SUBM_CFG_ALARM_MAX_ACT:
				control=PantallaActualizarAltaPresion(variable_temp);
				if(control == WRITING_OK) accionCore=SUBM_CFG_ALARM_MAX_LEE_TECL;
				break;
			case SUBM_CFG_ALARM_MAX_LEE_TECL:
				switch(teclaPresionada)
				{
					case TECLA_A:
						accionCore=SUBM_CFG_ALARM_MIN_BORR;
						variable_temp=0;
						break;
					case TECLA_B:
						accionCore=SUBM_CFG_ALARM_MIN_BORR;
						variable_temp=0;
						break;
					case TECLA_C:
						accionCore=SUBM_CFG_ALARM_BORR;
						variable_temp=0;
						break;
					case TECLA_D:
						if((variable_temp>configuracion.compresor_alta_presion)&&(variable_temp<=10000))
						{
							configuracion.alarma_alta_presion=variable_temp;
							Actualizar_EEPROM(OFFSET_ALARMA_ALTA);
							variable_temp=0;
						}
						else
						{
							accionCore=SUBM_CFG_ALARM_MAX_BORR;
							variable_temp=0;
						}
						break;
					case NO_KEY:
						break;
					default:
						ModificarVariable(teclaPresionada);
						accionCore=SUBM_CFG_ALARM_MAX_ACT;
						break;
				}
				break;

	/// COMIENZO SUBMENU ALARMAS MINIMO///

			case SUBM_CFG_ALARM_MIN_BORR:
				control=BorrarPantalla();
				if(control == WRITING_OK) accionCore=SUBM_CFG_ALARM_MIN;
				break;
			case SUBM_CFG_ALARM_MIN:
				control=PantallaBajaPresion(configuracion.alarma_baja_presion);
				if(control == WRITING_OK) accionCore=SUBM_CFG_ALARM_MIN_LEE_TECL;
				break;
			case SUBM_CFG_ALARM_MIN_ACT:
				control=PantallaActualizarBajaPresion(variable_temp);
				if(control == WRITING_OK) accionCore=SUBM_CFG_ALARM_MIN_LEE_TECL;
				break;
			case SUBM_CFG_ALARM_MIN_LEE_TECL:
				switch(teclaPresionada)
				{
					case TECLA_A:
						accionCore=SUBM_CFG_ALARM_MAX_BORR;
						variable_temp=0;
						break;
					case TECLA_B:
						accionCore=SUBM_CFG_ALARM_MAX_BORR;
						variable_temp=0;
						break;
					case TECLA_C:
						accionCore=SUBM_CFG_ALARM_BORR;
						variable_temp=0;
						break;
					case TECLA_D:
						if((variable_temp<configuracion.compresor_baja_presion)&&(variable_temp>=0))
						{
							configuracion.alarma_baja_presion=variable_temp;
							Actualizar_EEPROM(OFFSET_ALARMA_BAJA);
							variable_temp=0;
						}
						else
						{
							accionCore=SUBM_CFG_ALARM_MIN_BORR;
							variable_temp=0;
						}
						break;
					case NO_KEY:
						break;
					default:
						ModificarVariable(teclaPresionada);
						accionCore=SUBM_CFG_ALARM_MIN_ACT;
						break;
				}
				break;

		/// COMIENZO MENU VARIABLES DE OPERACION ///

			case SUBM_CFG_VAR_BORR:
				control=BorrarPantalla();
				if(control == WRITING_OK) accionCore=SUBM_CFG_VAR;
				break;
			case SUBM_CFG_VAR:
				control=PantallaVarOp();
				if(control == WRITING_OK) accionCore=SUBM_CFG_VAR_LEE_TECL;
				break;
			case SUBM_CFG_VAR_LEE_TECL:
				switch(teclaPresionada)
				{
					case TECLA_A:
						accionCore=SUBM_CFG_ALARM_BORR;
						break;
					case TECLA_B:
						accionCore=SUBM_CFG_ALARM_BORR;
						break;
					case TECLA_C:
						accionCore=CFG_BORR_LCD;
						break;
					case TECLA_D:
						accionCore=SUBM_CFG_COMPR_MAX_BORR;
						break;
					case NO_KEY:
						break;
				}
				break;

			/// COMIENZO SUBMENU COMPRESOR MAXIMO///

			case SUBM_CFG_COMPR_MAX_BORR:
				control=BorrarPantalla();
				if(control == WRITING_OK) accionCore=SUBM_CFG_COMPR_MAX;
				break;
			case SUBM_CFG_COMPR_MAX:
				control=PantallaRangoMax(configuracion.compresor_alta_presion);
				if(control == WRITING_OK) accionCore=SUBM_CFG_COMPR_MAX_LEE_TECL;
				break;
			case SUBM_CFG_COMPR_MAX_ACT:
				control=PantallaActualizarRangoMax(variable_temp);
				if(control == WRITING_OK) accionCore=SUBM_CFG_COMPR_MAX_LEE_TECL;
				break;
			case SUBM_CFG_COMPR_MAX_LEE_TECL:
				switch(teclaPresionada)
				{
					case TECLA_A:
						accionCore=SUBM_CFG_COMPR_MIN_BORR;
						variable_temp=0;
						break;
					case TECLA_B:
						accionCore=SUBM_CFG_COMPR_MIN_BORR;
						variable_temp=0;
						break;
					case TECLA_C:
						accionCore=SUBM_CFG_VAR_BORR;
						variable_temp=0;
						break;
					case TECLA_D:
						if((variable_temp>configuracion.compresor_baja_presion)&&(variable_temp<configuracion.alarma_alta_presion))
						{
							configuracion.compresor_alta_presion=variable_temp;
							Actualizar_EEPROM(OFFSET_COMPRESOR_ALTA);
							variable_temp=0;
						}
						else
						{
							accionCore=SUBM_CFG_COMPR_MAX_BORR;
							variable_temp=0;
						}
						break;
					case NO_KEY:
						break;
					default:
						ModificarVariable(teclaPresionada);
						accionCore=SUBM_CFG_COMPR_MAX_ACT;
						break;
				}
				break;

			/// COMIENZO SUBMENU COMPRESOR MINIMO///

			case SUBM_CFG_COMPR_MIN_BORR:
				control=BorrarPantalla();
				if(control == WRITING_OK) accionCore=SUBM_CFG_COMPR_MIN;
				break;
			case SUBM_CFG_COMPR_MIN:
				control=PantallaRangoMin(configuracion.compresor_baja_presion);
				if(control == WRITING_OK) accionCore=SUBM_CFG_COMPR_MIN_LEE_TECL;
				break;
			case SUBM_CFG_COMPR_MIN_ACT:
				control=PantallaActualizarRangoMin(variable_temp);
				if(control == WRITING_OK) accionCore=SUBM_CFG_COMPR_MIN_LEE_TECL;
				break;
			case SUBM_CFG_COMPR_MIN_LEE_TECL:
				switch(teclaPresionada)
				{
					case TECLA_A:
						accionCore=SUBM_CFG_COMPR_MAX_BORR;
						break;
					case TECLA_B:
						accionCore=SUBM_CFG_COMPR_MAX_BORR;
						break;
					case TECLA_C:
						accionCore=SUBM_CFG_VAR_BORR;
						break;
					case TECLA_D:
						if((variable_temp<configuracion.compresor_alta_presion)&&(variable_temp>configuracion.alarma_baja_presion))
						{
							configuracion.compresor_baja_presion=variable_temp;
							Actualizar_EEPROM(OFFSET_COMPRESOR_BAJA);
							variable_temp=0;
						}
						else
						{
							accionCore=SUBM_CFG_COMPR_MIN_BORR;
							variable_temp=0;
						}
						break;
					case NO_KEY:
						break;
					default:
						ModificarVariable(teclaPresionada);
						accionCore=SUBM_CFG_COMPR_MIN_ACT;
						break;
				}
				break;



		///MENU DEBUG///
		case DBG_BORR_LCD:
			control=BorrarPantalla();
			if(control == WRITING_OK) accionCore=DBG_ACT_LCD;
			break;
		case DBG_ACT_LCD:
			control=PantallaDebug();
			if(control == WRITING_OK) accionCore=DBG_LEE_TECL;
			break;
		case DBG_LEE_TECL:
			switch(teclaPresionada)
			{
				case TECLA_A:
					accionCore=CFG_BORR_LCD;
					break;
				case TECLA_B:
					accionCore=IND_BORR_LCD;
					break;
				case TECLA_C:
					accionCore=IND_BORR_LCD;
					break;
				case TECLA_D:
					accionCore=SUBM_DBG_SAL_BORR;
					flag_debug = 1;
					break;
				case NO_KEY:
					break;
			}
			break;

		/*************SUBMENU DEBUG ***********************/
		case SUBM_DBG_SAL_BORR:
			control=BorrarPantalla();
			if(control == WRITING_OK) accionCore=SUBM_DBG_SAL;
			break;
		case SUBM_DBG_SAL:
			control=PantallaSalidas();
			if(control == WRITING_OK) accionCore=SUBM_DBG_SAL_LEE_TECL;
			break;
		case SUBM_DBG_SAL_LEE_TECL:
			switch(teclaPresionada)
				{
					case TECLA_A:
						accionCore=SUBM_DBG_TEC_BORR;
						break;
					case TECLA_B:
						accionCore=SUBM_DBG_ADC_BORR;
						break;
					case TECLA_C:
						accionCore=IND_BORR_LCD;
						break;
					case TECLA_D:
						accionCore=SUBM_SAL_TCOMP_BORR;
						break;
					case NO_KEY:
						break;
				}
			break;
		case SUBM_DBG_ADC_BORR:
			control=BorrarPantalla();
			if(control == WRITING_OK) accionCore=SUBM_DBG_ADC;
			break;
		case SUBM_DBG_ADC:
			control=PantallaTestLazo(0);
			if(control == WRITING_OK) accionCore=SUBM_DBG_ADC_LEE_TECL;
			break;
		case SUBM_DBG_ADC_ACT_LCD:
			control=PantallaActualizarTestLazo(corrienteMedida);
			if(control == WRITING_OK) accionCore=SUBM_DBG_ADC_LEE_TECL;
			break;
		case SUBM_DBG_ADC_LEE_TECL:
			switch(teclaPresionada)
				{
					case TECLA_A:
						accionCore=SUBM_DBG_SAL_BORR;
						break;
					case TECLA_B:
						accionCore=SUBM_DBG_TEC_BORR;
						break;
					case TECLA_C:
						accionCore=IND_BORR_LCD;
						break;
					case TECLA_D:
						break;
					case NO_KEY:
						break;
				}
			refrescarPantalla++;
			if(refrescarPantalla==500)
			{
				accionCore=SUBM_DBG_ADC_ACT_LCD;
				refrescarPantalla=0;
			}
			break;
		case SUBM_DBG_TEC_BORR:
			control=BorrarPantalla();
			if(control == WRITING_OK) accionCore=SUBM_DBG_TEC;
			break;
		case SUBM_DBG_TEC:
			control=PantallaTestTeclado(0);
			if(control == WRITING_OK) accionCore=SUBM_DBG_TEC_LEE_TECL;
			break;
		case SUBM_DBG_TEC_LEE_TECL:
			switch(teclaPresionada)
				{
					case TECLA_A:
						accionCore=SUBM_DBG_ADC_BORR;
						break;
					case TECLA_B:
						accionCore=SUBM_DBG_SAL_BORR;
						break;
					case TECLA_C:
						accionCore=IND_BORR_LCD;
						break;
					case TECLA_D:
						break;
					case NO_KEY:
						break;
					default:
						PantallaActualizarTecla_TEST_TECLADO(teclaPresionada);
						break;
				}
			break;

		/************* FIN SUBMENU DEBUG ********************/

		/************** SUBMENU TEST SALIDAS ***************/
			case SUBM_SAL_TCOMP_BORR:
				control=BorrarPantalla();
				if(control == WRITING_OK) accionCore=SUBM_SAL_TCOMP;
				break;
			case SUBM_SAL_TCOMP:
				control=PantallaTestCompr();
				if(control == WRITING_OK) accionCore=SUBM_SAL_TCOMP_LEE_TECL;
				break;
			case SUBM_SAL_TCOMP_LEE_TECL:
				switch(teclaPresionada)
				{
					case TECLA_A:
						accionCore=SUBM_SAL_TDESC_BORR;
						break;
					case TECLA_B:
						accionCore=SUBM_SAL_TALARM_BORR;
						break;
					case TECLA_C:
						accionCore=SUBM_DBG_SAL_BORR;
						break;
					case TECLA_D:
						break;
					case NO_KEY:
						break;
					case TECLA_1:
						//Prender COMPRESOR
						CambiarEstadoCompresor(activo);
						break;
					case TECLA_2:
						//Apagar COMPRESOR
						CambiarEstadoCompresor(inactivo);
						break;
				}
				break;

			case SUBM_SAL_TALARM_BORR:
				control=BorrarPantalla();
				if(control == WRITING_OK) accionCore=SUBM_SAL_TALARM;
				break;
			case SUBM_SAL_TALARM:
				control=PantallaTestAlarm();
				if(control == WRITING_OK) accionCore=SUBM_SAL_TALARM_LEE_TECL;
				break;
			case SUBM_SAL_TALARM_LEE_TECL:
				switch(teclaPresionada)
				{
					case TECLA_A:
						accionCore=SUBM_SAL_TCOMP_BORR;
						break;
					case TECLA_B:
						accionCore=SUBM_SAL_TDESC_BORR;
						break;
					case TECLA_C:
						accionCore=SUBM_DBG_SAL_BORR;
						break;
					case TECLA_D:
						break;
					case NO_KEY:
						break;
					case TECLA_1:
						//Prender ALARMA
						CambiarEstadoAlarma(activo);
						break;
					case TECLA_2:
						//Apagar ALARMA
						CambiarEstadoAlarma(inactivo);
						break;
				}
				break;

			case SUBM_SAL_TDESC_BORR:
				control=BorrarPantalla();
				if(control == WRITING_OK) accionCore=SUBM_SAL_TDESC;
				break;
			case SUBM_SAL_TDESC:
				control=PantallaTestDesc();
				if(control == WRITING_OK) accionCore=SUBM_SAL_TDESC_LEE_TECL;
				break;
			case SUBM_SAL_TDESC_LEE_TECL:
				switch(teclaPresionada)
				{
					case TECLA_A:
						accionCore=SUBM_SAL_TALARM_BORR;
						break;
					case TECLA_B:
						accionCore=SUBM_SAL_TCOMP_BORR;
						break;
					case TECLA_C:
						accionCore=SUBM_DBG_SAL_BORR;
						break;
					case TECLA_D:
						break;
					case NO_KEY:
						break;
					case TECLA_1:
						//Prender Valvula de Descarga
						CambiarEstadoDescarga(activo);
						break;
					case TECLA_2:
						//Apagar Valvula de Descarga
						CambiarEstadoDescarga(inactivo);
						break;
				}
				break;

				//Pantallas de visualizacion de error
			case PANTALLA_ALERTA_ALTA_PRESION_BORR:
				control=BorrarPantalla();
				if(control == WRITING_OK) accionCore=PANTALLA_ALERTA_ALTA_PRESION;
				break;
			case PANTALLA_ALERTA_ALTA_PRESION:
				control=PantallaAlertaAltaPresion();
				if(control == WRITING_OK) accionCore=PANTALLA_ALERTA_LEE_TECL;
				break;
			case PANTALLA_ALERTA_BAJA_PRESION_BORR:
				control=BorrarPantalla();
				if(control == WRITING_OK) accionCore=PANTALLA_ALERTA_BAJA_PRESION;
				break;
			case PANTALLA_ALERTA_BAJA_PRESION:
				control=PantallaAlertaBajaPresion();
				if(control == WRITING_OK) accionCore=PANTALLA_ALERTA_LEE_TECL;
				break;
			case PANTALLA_ALERTA_ERROR_LAZO_BORR:
				control=BorrarPantalla();
				if(control == WRITING_OK) accionCore=PANTALLA_ALERTA_ERROR_LAZO;
				break;
			case PANTALLA_ALERTA_ERROR_LAZO:
				control=PantallaAlertaLazo();
				if(control == WRITING_OK) accionCore=PANTALLA_ALERTA_LEE_TECL;
				break;
			case PANTALLA_ALERTA_LEE_TECL:
				switch(teclaPresionada)
				{
					case TECLA_A:
						break;
					case TECLA_B:
						break;
					case TECLA_C:
						//Si apreto la tecla C salgo del error y vuelvo a controlar la presion
						accionCore=IND_BORR_LCD;
						volverAlControlDePresion();
						break;
					case TECLA_D:
						break;
					case NO_KEY:
						break;
				}
				break;
	}
}


void ProcesarTeclado(void){

	if(teclasApretadas != 0)
	{
		for(uint8_t tecla=0; tecla<CANTIDAD_TECLAS; tecla++){
			if(teclasApretadas & (1<<tecla)){
				teclaPresionada = tecla;
				teclasApretadas &= ~(1<<tecla);
				break;
			}
		}
	}
	else
	{
		teclaPresionada=NO_KEY;
	}

}

void ModificarVariable(uint8_t tecla)
{
	switch(tecla){
		case TECLA_0:
			variable_temp=variable_temp*10;
			break;
		case TECLA_1:
			variable_temp=variable_temp*10+1;
			break;
		case TECLA_2:
			variable_temp=variable_temp*10+2;
			break;
		case TECLA_3:
			variable_temp=variable_temp*10+3;
			break;
		case TECLA_4:
			variable_temp=variable_temp*10+4;
			break;
		case TECLA_5:
			variable_temp=variable_temp*10+5;
			break;
		case TECLA_6:
			variable_temp=variable_temp*10+6;
			break;
		case TECLA_7:
			variable_temp=variable_temp*10+7;
			break;
		case TECLA_8:
			variable_temp=variable_temp*10+8;
			break;
		case TECLA_9:
			variable_temp=variable_temp*10+9;
			break;
	}
}
/****** Tarea que controla que la presion este dentro de los limites permitidos ***********/

void tarea_control_presion(void *p){

	static uint32_t tiempo_en_alta_presion = 0;
	static uint32_t tiempo_en_baja_presion = 0;

	//Si estoy en debug no controlo nada..
	if(flag_debug) return;

	//Incremento el contador de ciclos de ejecucion hasta que llegue a START_UP_TIME
	if(tiempo_de_comienzo < START_UP_TIME)tiempo_de_comienzo++;

	switch(estado_ejecucion){
	case START_UP:
		if(configuracion.flags & estado_lazo_sensor){
			/* Agregar aca control del lazo de 4-20mA */
			if(flag_datos_completos && ((corrienteMedida > configuracion.sensor_maximo_adc )||(corrienteMedida < configuracion.sensor_minimo_adc))){
				//Espero que el flag se ponga en 1 para tener una corriente medida valida.
				estado_ejecucion = ERROR_LAZO;
				break;
			}
		}
		CambiarEstadoAlarma(inactivo);
		CambiarEstadoDescarga(inactivo);

		if(presionMedida >= configuracion.compresor_alta_presion){
			CambiarEstadoCompresor(inactivo);
		}
		if(presionMedida <= configuracion.compresor_baja_presion){
			CambiarEstadoCompresor(activo);
		}
		if(presionMedida >= configuracion.alarma_alta_presion){
			estado_ejecucion = SOBRE_PRESION;
			CambiarEstadoCompresor(inactivo);
			CambiarEstadoAlarma(activo);
			CambiarEstadoDescarga(activo);
			tiempo_en_alta_presion = 0;
		}

		//Si cumpli el tiempo de START_UP me paso a full mode
		if(tiempo_de_comienzo >= START_UP_TIME) estado_ejecucion=FULL_MODE;
		break;
	case FULL_MODE:
		if(configuracion.flags & estado_lazo_sensor){
			/* Agregar aca control del lazo de 4-20mA */
			if(flag_datos_completos && ((corrienteMedida > configuracion.sensor_maximo_adc )||(corrienteMedida < configuracion.sensor_minimo_adc))){
				//Espero que el flag se ponga en 1 para tener una corriente medida valida.
				estado_ejecucion = ERROR_LAZO;
				break;
			}
		}

		CambiarEstadoAlarma(inactivo);
		CambiarEstadoDescarga(inactivo);

		if(presionMedida >= configuracion.compresor_alta_presion){
			CambiarEstadoCompresor(inactivo);
		}
		if(presionMedida <= configuracion.compresor_baja_presion){
			CambiarEstadoCompresor(activo);
		}

		if(presionMedida >= configuracion.alarma_alta_presion){
			estado_ejecucion = SOBRE_PRESION;
			CambiarEstadoCompresor(inactivo);
			CambiarEstadoAlarma(activo);
			CambiarEstadoDescarga(activo);
			tiempo_en_alta_presion = 0;
		}

		if(presionMedida <= configuracion.alarma_baja_presion){
			estado_ejecucion = BAJA_PRESION;
			CambiarEstadoCompresor(activo);
			CambiarEstadoAlarma(activo);
			CambiarEstadoDescarga(inactivo);
			tiempo_en_baja_presion = 0;
		}

		break;
	case SOBRE_PRESION:
		if(configuracion.flags & estado_lazo_sensor){
			/* Agregar aca control del lazo de 4-20mA */
			if(flag_datos_completos && ((corrienteMedida > configuracion.sensor_maximo_adc )||(corrienteMedida < configuracion.sensor_minimo_adc))){
				//Espero que el flag se ponga en 1 para tener una corriente medida valida.
				estado_ejecucion = ERROR_LAZO;
				break;
			}
		}

		if(tiempo_en_alta_presion++ > TIEMPO_ESPERA_ALTA_PRESION){
			CambiarEstadoAlarma(activo);
			CambiarEstadoDescarga(activo);
			CambiarEstadoCompresor(inactivo);
			estado_ejecucion=SOBRE_PRESION_CRITICA;
		}
		if(presionMedida <= configuracion.compresor_alta_presion){
			CambiarEstadoAlarma(inactivo);
			CambiarEstadoDescarga(inactivo);
			if(tiempo_de_comienzo < START_UP_TIME)
				estado_ejecucion=START_UP;
			else
				estado_ejecucion=FULL_MODE;
		}
		break;
	case SOBRE_PRESION_CRITICA:
		//De aca no me muevo.. exploto todo.. msddd
		accionCore=PANTALLA_ALERTA_ALTA_PRESION_BORR;
		estado_ejecucion=ERROR_PERMANENTE;
		break;
	case BAJA_PRESION:
		if(configuracion.flags & estado_lazo_sensor){
			/* Agregar aca control del lazo de 4-20mA */
			if(flag_datos_completos && ((corrienteMedida > configuracion.sensor_maximo_adc )||(corrienteMedida < configuracion.sensor_minimo_adc))){
				//Espero que el flag se ponga en 1 para tener una corriente medida valida.
				estado_ejecucion = ERROR_LAZO;
				break;
			}
		}

		if(tiempo_en_baja_presion++ > TIEMPO_ESPERA_BAJA_PRESION){
			CambiarEstadoAlarma(activo);
			CambiarEstadoDescarga(inactivo);
			CambiarEstadoCompresor(inactivo);
			estado_ejecucion=BAJA_PRESION_CRITICA;
		}
		if(presionMedida > configuracion.compresor_baja_presion){
			CambiarEstadoAlarma(inactivo);
			CambiarEstadoDescarga(inactivo);
			CambiarEstadoCompresor(activo);
			if(tiempo_de_comienzo < START_UP_TIME)
				estado_ejecucion=START_UP;
			else
				estado_ejecucion=FULL_MODE;
		}
		break;
	case BAJA_PRESION_CRITICA:
		//De aca no me muevo.. exploto todo.. msddd
		accionCore=PANTALLA_ALERTA_BAJA_PRESION_BORR;
		estado_ejecucion=ERROR_PERMANENTE;
		break;
	case ERROR_LAZO:
		//Murio el sensor :(
		CambiarEstadoAlarma(activo);
		CambiarEstadoDescarga(inactivo);
		CambiarEstadoCompresor(inactivo);
		accionCore=PANTALLA_ALERTA_ERROR_LAZO_BORR;
		estado_ejecucion=ERROR_PERMANENTE;
		break;
	case ERROR_PERMANENTE:
		//Si llego aca la unica manera de salir es reseteando el dispositivo
		break;
	}

}

//Con este metodo vuelvo al funcionamiento normal de la tarea de control de presion
void volverAlControlDePresion()
{
	if((START_UP_TIME-30000)<0){
		tiempo_de_comienzo = 0;
	}else{
		tiempo_de_comienzo = START_UP_TIME-30000;
	}
	estado_ejecucion = START_UP;
}





