/*******************************************************************
* This Program is to demonstrate the UART Peripheral driver with 
* Ring Buffer implementation .
*	This Program initializes the UART1 Peripheral of STM32F103C8 
* microcontroller and this software demonstrates a feedback loop
* using the Ring Buffer Technique. Whatever the UART1 RX receives
* it sends the data back by writing it to UART1 TX.
*	This Software Code uses Direct Register Level Programming to keep
* the binary file size low.
*
*	Author : Sumedh S. Burbure     
* Email ID: sumedh2670@gmail.com
********************************************************************/

#include "main.h"

int main(void){
	Comm_UART_Init();
	while(1){
		if(Is_Comm_rx_Available()){
			char data = Comm_ReadFrom_rx_Buff();
			Comm_StoreIn_tx_Buff(data);
		}
	}
}
