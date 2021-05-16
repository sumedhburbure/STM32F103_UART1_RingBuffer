#include "COMM_UART.h"

comm_uart comm_rx;
comm_uart comm_tx;
/*************************************
*	USART 1 For Communication and Debug 
* PA9  -> TX
* PA10 -> RX
* Baud Rate -> 115200
**************************************/
void Comm_UART_Init(void){
	//----USART 1 Initialization----//
	//Enable clock to Port A
	RCC->APB2ENR |= ((0x01)<< 2) | ((0x01)<<14);  //Enable GPIO Port A clock and usart1
	//Set PA9 as output (bcoz uart tx)
	GPIOA->CRH |= (0x30) | ((0x01)<<7); //configure as output mode(pushpull) and output mode 2MHz speed
	//Set PA10 as input(Because uart rx)
	//PA10 already by default set to input mode and floating input so no need to cofigure it
	USART1->BRR = 0x271;		//BAUD RATE FOR 8MHZ APB2 BUS CLOCK Baud rate set to 115200
	//USART1->BRR = 0x1D4C;
	USART1->CR1 |= ((0x01)<<2) | ((0x01)<<3) | ((0x01)<<13) | ((0x01<<5)) | ((0x01)<<7); //TX RX AND USART ENABLE
	NVIC_EnableIRQ(USART1_IRQn);
}

/*******************************************
* Comm_UART_Flush function is used for 
* flushing the uart rx buffer
********************************************/
void Comm_UART_Flush(void){
	memset(comm_rx.buffer,'\0',COMM_BUFFER_SIZE);
	comm_rx.head = 0;
	comm_rx.tail = 0;
}

/*******************************************
* Comm_StoreInBuff function is used for 
* storing rx buffer from UART DATA Register
********************************************/
void Comm_StoreIn_rx_Buff(char data){
	int i = (comm_rx.head + 1) % COMM_BUFFER_SIZE;
	comm_rx.buffer[comm_rx.head] = data;
	comm_rx.head = i;
}

/*******************************************
* Comm_StoreIn_tx_Buff function is used for 
* storing tx ring buffer to send it to UART 
* DATA Register
********************************************/
void Comm_StoreIn_tx_Buff(char data){
	int i = (comm_tx.head + 1) % COMM_BUFFER_SIZE;
	comm_tx.buffer[comm_tx.head] = data;
	comm_tx.head = i;
	USART1->CR1 |= (USART_CR1_TXEIE); // Enable TX Data Register Empty Interrupt
}

/*******************************************
* Comm_ReadFrom_rx_Buff function is used for 
* reading rx ring buffer to send it to UART 
* DATA Register
********************************************/
char Comm_ReadFrom_rx_Buff(void){
	int i = (comm_rx.tail + 1) % COMM_BUFFER_SIZE;
	char data = comm_rx.buffer[comm_rx.tail];
	comm_rx.tail = i;
	return data;
}
/*******************************************
* Comm_SendStringIn_tx_Buff function is used 
* for storing string in tx buffer to send it
* to UART DATA Register
********************************************/
void Comm_SendStringIn_tx_Buff(char *data){
	while(* data){
		Comm_StoreIn_tx_Buff(*data++);
	}
}

/*******************************************
* Is_Comm_rx_Available function is used for 
* checking if anything is left to read.
********************************************/
uint16_t Is_Comm_rx_Available(void){
	return (uint16_t) ((COMM_BUFFER_SIZE -  (comm_rx.head - comm_rx.tail)) % COMM_BUFFER_SIZE);
}



/*******************************************
* USART1_IRQHandler function is an interrupt
* handler for usart1.
********************************************/
void USART1_IRQHandler(void){
	if(USART1->SR & USART_SR_RXNE){
		char c = USART1->DR;
		Comm_StoreIn_rx_Buff(c);
	}
	else if(USART1->SR & (USART_SR_TXE)){
		if(comm_tx.head == comm_tx.tail){
			USART1->CR1 &= ~(USART_CR1_TXEIE); //Disable TX Data Register Empty Interrupt
		}
		else{
			volatile int i = (comm_tx.tail +1) % COMM_BUFFER_SIZE;
			USART1->DR = comm_tx.buffer[comm_tx.tail];
			comm_tx.tail = i;
		}
	}
}
