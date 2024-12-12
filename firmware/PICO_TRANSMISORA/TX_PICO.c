#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"

// UART
#define UART_ID   uart0
#define BAUD_RATE 115200

#define UART_TX_PIN 12
#define UART_RX_PIN 13

// oh the weather outside is rizzy but the fire is so skibidi but since ive gyatt to go ohio ohio ohio

// VARIABLES
uint8_t morse[5]; // 1
char texto[16]; // 2 
uint8_t morseindex = 0;
uint8_t textindex = 0;

// 1 = . | 0 = -
// ARRAYS PARA DECODIFICACION
char LARGO1[2]={'T','E'};
char LARGO2[4]={'M','N','A','I'};
char LARGO3[8]={'O','G','K','D','W','R','U','S'};
char LARGO4[16]={'?','?','Q','Z','Y','C','X','B','J','P','?','L','?','F','V','H'};

// FUNCIONES

int send(char a[]) { // Función send para separar y enviar un string mediante chars (con la función uart_putc)
    uint8_t i = 0;
    while (a[i] != '\0') {
        uart_putc(UART_ID,a[i]);
        i++;
        sleep_ms(200);
    }
    sleep_ms(200);
    uart_putc(UART_ID,' '); // Llegue al final, envio un espacio para decodificar la ultima letra
}

int del() { // Funcion del(ete) para borrar limpiar todo de manera rapida
    uart_putc(UART_ID,'R');
}

int main()
{
    stdio_init_all();
    // HABILITO UART | P/ TX Y RX
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    sleep_ms(1000); // espero 1s para estar chill de cojones
    del();
    send("R-.. . ... .- .--. .-. --- -... .- .-. . -- --- ..."); // Poner una 'R' es otro modo de hacer reset/del

    while (true) {
        sleep_ms(100);
    }
}
