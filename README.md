# STM32F103_USART1_RingBuffer
This program demonstrates the working of UART RingBuffer on STM32F103C8 Bluepill Development Board.</br>
Below is the illustration of the Concept of Ring Buffer:
![Alt Text](https://upload.wikimedia.org/wikipedia/commons/f/fd/Circular_Buffer_Animation.gif)

-------------------------------------------------------------------
The UART1 peripheral is initialized and a loopback mechanism is implemented such as whatever UART1 RX receives it sends the data back by writing it to UART1 TX.
This Software Code uses Direct Register Level Programming to keep the code size less.  </br>
You can read more about the concept of Ring Buffer [here](https://en.wikipedia.org/wiki/Circular_buffer).

-------------------------------------------------------------------
UART Specifications :</br>
UART1_TX -> PA9</br>
UART1-RX -> PA10</br>
UART_Baud Rate -> 115200</br>
System Clock -> Default Clock setting of STM32F103C8</br>

-------------------------------------------------------------------
This program is written in KEil µVision5.17.00 IDE
