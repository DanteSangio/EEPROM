/*
===============================================================================
 Name        : ProyI2C.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#include "chip.h"
#include "stdint.h"

#include <cr_section_macros.h>

#define PUERTO_0  	 		0

#define I2C1				1

#define PIN_SDA1 	   	   19
#define PIN_SCL1  	   	   20

#define NEITHER				2
#define I2C_FUNC     		3

#define SLAVE_ADDRESS		0x50
#define W_ADDRESS			0x00B0
#define DATO				'A'


unsigned char Datos_Tx [] = { (W_ADDRESS & 0xFF00) >> 8 , W_ADDRESS & 0x00FF, DATO};
//En caso que quisieramos escribir en la memoria y no solo posicionarnos, debemos escribir los datos luego de la posicion.
unsigned char Datos_Rx [] = {0};


/**
 * @brief	I2C Interrupt Handler
 * @return	None
 */
void I2C1_IRQHandler(void)
{
	Chip_I2C_MasterStateHandler (I2C1);
}


void Configuracion_uC ( void )
{
	Chip_GPIO_Init(LPC_GPIO);
	Chip_IOCON_Init(LPC_IOCON);

	Chip_IOCON_PinMux (LPC_IOCON, PUERTO_0, PIN_SDA1 , NEITHER , I2C_FUNC );
	Chip_IOCON_PinMux (LPC_IOCON, PUERTO_0, PIN_SCL1 , NEITHER , I2C_FUNC );
	Chip_IOCON_EnableOD (LPC_IOCON, PUERTO_0, PIN_SDA1);
	Chip_IOCON_EnableOD (LPC_IOCON, PUERTO_0, PIN_SCL1);

	Chip_I2C_Init (I2C1);
	Chip_I2C_SetClockRate (I2C1, 100000);

	Chip_I2C_SetMasterEventHandler (I2C1, Chip_I2C_EventHandler);
	NVIC_EnableIRQ(I2C1_IRQn);
}


int main(void)
{
#if defined (__USE_LPCOPEN)
    SystemCoreClockUpdate();
#endif

    Configuracion_uC ();

    Chip_I2C_MasterSend (I2C1, SLAVE_ADDRESS, Datos_Tx, 2); // Nos posicionamos en la Posicion a leer.
    Chip_I2C_MasterRead (I2C1, SLAVE_ADDRESS, Datos_Rx, 5); // Se leen 05 bytes a partir de la Posición.

    while (1);

    return 0 ;
}
