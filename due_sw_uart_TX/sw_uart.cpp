#include "sw_uart.h"
#pragma GCC optimize ("-O3")

void sw_uart_setup(due_sw_uart *uart, int tx, int stopbits, int databits, int paritybit) {
	
	uart->pin_tx     = tx;
	uart->stopbits   = stopbits;
	uart->paritybit  = paritybit;
  uart->databits   = databits;
  pinMode(tx, OUTPUT);
  
}




int calc_even_parity(char data) {
  int ones = 0;

  for(int i = 0; i < 8; i++) {
    ones += (data >> i) & 0x01;
  }

  return ones % 2;
}

int calc_parity(due_sw_uart *uart, char data){
  if (uart->paritybit == SW_UART_EVEN_PARITY){
    return calc_even_parity(data);
  } else {
    return !calc_even_parity(data);
  }
}


void sw_uart_write_byte(due_sw_uart *uart, char data) {
  // calcula paridade
  int parity = calc_parity(uart, data);

  // start bit
  digitalWrite(uart->pin_tx, LOW);
  _sw_uart_wait_T(uart);
  Serial.print("0");

  // envia bits do char
  for (int bits = 0; bits < 8; bits++) {
    digitalWrite(uart->pin_tx, (data >> bits) & 0x01);
    _sw_uart_wait_T(uart);
    Serial.print((data >> bits) & 0x01);
  }

  // bit de paridade
  digitalWrite(uart->pin_tx, parity);
  _sw_uart_wait_T(uart);
  Serial.print(parity);

  // stop bits
  for (int bits = 0; bits < uart->stopbits; bits++){
    digitalWrite(uart->pin_tx, HIGH);
    _sw_uart_wait_T(uart);
    Serial.print("1");
  }
}


// MCK 21MHz
void _sw_uart_wait_half_T(due_sw_uart *uart) {
  for(int i = 0; i < 1093; i++)
    asm("NOP");
}

void _sw_uart_wait_T(due_sw_uart *uart) {
  _sw_uart_wait_half_T(uart);
  _sw_uart_wait_half_T(uart);
}
