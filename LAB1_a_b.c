#define FOSC 8000000        // Frecuencia del clock del micro
#define FCY FOSC/2          // Frecuencia de instrucciones usada por  "__delay_ms()"
#include <p24FJ128GA010.h>   // Header específico deL micro
#include <libpic30.h>       // se necesita para "__delay_ms()"

//Configuración de bits para Explorer-16

_CONFIG1(JTAGEN_OFF // deshabilito interface JTAG
        & GCP_OFF // deshabilito proteción de código general
        & GWRP_OFF // deshabilito protección escritura flash
        & ICS_PGx2 // ICSP interface (2=default)
        & FWDTEN_OFF) // deshabilito watchdog timer
_CONFIG2(IESO_OFF // deshabilito 2 velocidades de start up
        & FCKSM_CSDCMD // deshabilito clock-swithcing/monitor
        & FNOSC_PRIPLL // primary oscillator: habilito PLL
        & POSCMOD_XT) // primary oscillator: modo XT
//Definiciones
#define PI 3.1416
#define LED0 PORTAbits.RA0
#define LED1 PORTAbits.RA1
#define LED2 PORTAbits.RA2
#define LED3 PORTAbits.RA3
#define LED4 PORTAbits.RA4
#define LED5 PORTAbits.RA5
#define LED6 PORTAbits.RA6
#define LED7 PORTAbits.RA7

#define TIEMPO __delay_ms(10)
//Declaracion de variables
/*
int Palabra[7][19] = {
    {1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0},
    {1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0},
    {1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0},
    {1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0},
    {1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0},
    {1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0},
    {1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0},
    {1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0}
};*/
int Palabra[7][19] = {
    {0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0},
    {0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0},
    {0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0},
    {0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0},
    {0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0}
};
//Prototipos de funciones
void Recorrer_Matriz();
//Prototipos de atención a las interrupciónes

//Programa principal

int main(void) {
    TRISA = 0x0000;
    AD1PCFG = 0xFFFF;
    PORTAbits.RA0 = 1;
    while (1) {
        Recorrer_Matriz();
    }
}

void Recorrer_Matriz() {
    int a, i;
    for (a = 0; a <= 7; a++) {
        for (i = 0; i <= 19; i++) {
            if (Palabra[0][i] == 1)LED0 = 1;
            if (Palabra[1][i] == 1)LED1 = 1;
            if (Palabra[2][i] == 1)LED2 = 1;
            if (Palabra[3][i] == 1)LED3 = 1;
            if (Palabra[4][i] == 1)LED4 = 1;
            if (Palabra[5][i] == 1)LED5 = 1;
            if (Palabra[6][i] == 1)LED6 = 1;
            if (Palabra[7][i] == 1)LED7 = 1;
            ////////////////////////
            if (Palabra[0][i] == 0)LED0 = 0;
            if (Palabra[1][i] == 0)LED1 = 0;
            if (Palabra[2][i] == 0)LED2 = 0;
            if (Palabra[3][i] == 0)LED3 = 0;
            if (Palabra[4][i] == 0)LED4 = 0;
            if (Palabra[5][i] == 0)LED5 = 0;
            if (Palabra[6][i] == 0)LED6 = 0;
            if (Palabra[7][i] == 0)LED7 = 0;
            TIEMPO;
        }
    }
}