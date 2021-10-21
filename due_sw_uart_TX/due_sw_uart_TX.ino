#include "sw_uart.h"

due_sw_uart uart;

void setup() {
  Serial.begin(9600);
  sw_uart_setup(&uart, 4, 1, 8, SW_UART_EVEN_PARITY); // uart, tx, 
}

void loop() {
 send_byte(); // Função para enviar um char
 Serial.println(" Terminei de enviar!");
 delay(10000);
}

void send_byte() {
  char data = 'a'; // char a ser enviado
  sw_uart_write_byte(&uart, data);
}
