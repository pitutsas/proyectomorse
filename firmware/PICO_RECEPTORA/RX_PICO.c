#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "lcd.h"

// LCD
#define I2C         i2c0
#define SDA_GPIO    0
#define SCL_GPIO    1
#define ADDR        0x27

// UART
#define UART_ID   uart1 //RX
#define BAUD_RATE 115200

#define UART_TX_PIN 4   
#define UART_RX_PIN 5

// oh the weather outside is rizzy but the fire is so skibidi but since ive gyatt to go ohio ohio ohio

// VARIABLES
uint8_t morse[5]; // 1
char texto[16]; // 2 
uint8_t morseindex = 0;
uint8_t textindex = 0;

// 1 = . | 2 = -
// ARRAYS PARA DECODIFICACION
char LARGO1[2]={'T','E'};
char LARGO2[4]={'M','N','A','I'};
char LARGO3[8]={'O','G','K','D','W','R','U','S'};
char LARGO4[16]={'?','?','Q','Z','Y','C','X','B','J','P','?','L','?','F','V','H'};

// FUNCIONES

int powbutmejor(int x,int n)
{
    int i; 
    int number = 1;
    for (i = 0; i < n; ++i)
        number *= x;
    return(number);
}

int reset(int a){
    //printf("RESET\n");
    if (a == 1) {
        morseindex = 0;
    } else {
        for (uint8_t i = 0; i < sizeof(texto); i++) {
            texto[i] = '\0';
        }
        textindex = 0;
        lcd_clear();
    }
}

int add(char a,uint8_t b){
    if (b == 1) {
        morse[morseindex] = a;
        morseindex++;
    } else {
        texto[textindex] = a;
        textindex++;
        lcd_clear();
        lcd_string(texto);    
    }
}

int decode() {
   add(2,1);
   uint8_t pos = 0;
   uint8_t peso = 0;
   for (uint8_t i = 0; i < sizeof(morse); i++) {
    if (morse[i] == 2) {
        pos = i;
    }
   }

   for (uint8_t i = 0; i < sizeof(morse); i++) {
       if (morse[i] != 2) {
        peso += morse[i]*powbutmejor(2,(pos-i)-1);
       } else {
        break;
       }
   }
   printf("Vamos con POS = %d, PESO = %d\n",pos,peso); // print de prueba
   printf("Texto: %s\n",texto);
   switch (pos)
   {
// EMPIEZA 1
   case 1:
   add(LARGO1[peso],2);
   break;
 // EMPIEZA 2
    case 2:
    add(LARGO2[peso],2);
    break;
// EMPIEZA 3
    case 3:
    add(LARGO3[peso],2);
    break;
// EMPIEZA 4
    case 4:
    add(LARGO4[peso],2);
    break;
    default:
    add('?',2);
    break;
}
reset(1); // Limpio el array de morse
}

int main()
{
    stdio_init_all();
    // HABILITO UART | P/ TX Y RX
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    // HABILITO I2C Y LCD | P/ RX
    i2c_init(I2C, 100000);
    gpio_set_function(SDA_GPIO, GPIO_FUNC_I2C);
    gpio_set_function(SCL_GPIO, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_GPIO);
    gpio_pull_up(SCL_GPIO);
    lcd_init(I2C, ADDR);
    lcd_clear();
   // printf("Recibiendo\n");
    while (true) {
        if (uart_is_readable(UART_ID)) {
            while (uart_is_readable(UART_ID)) {
                char uart_rec = uart_getc(UART_ID);
                printf("%d\n",uart_rec);
                switch (uart_rec)
                {
                case '-':
                    add(0,1);
                    printf("Añadiendo 0\n");
                    break;
                case '.':
                    add(1,1);
                    printf("Añadiendo 1\n");
                    break;
                case ' ':
                    decode();
                    printf("Decodificando\n");
                    break;
                case '/':
                    add(' ',2);
                    break;
                case 'R':
                    reset(1);
                    reset(2);
                    break;
                default:
                printf("Añadiendo nada\n");
                    break;
                }
                break;
            }
        }
        sleep_ms(100);
    }
}
