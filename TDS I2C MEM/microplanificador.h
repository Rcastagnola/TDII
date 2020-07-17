#ifndef MICROPLANIFICADOR_H_
#define MICROPLANIFICADOR_H_

void MONITOR_I_Init(void);
int despacharTarea(void (*Tarea)(void), uint32_t bcet, uint32_t wcet, uint32_t *et);

#endif /* MICROPLANIFICADOR_H_ */
