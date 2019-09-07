#define FOSC 32000000        // Frecuencia del clock del micro
#define FCY FOSC/2          // Frecuencia de instrucciones usada por  "__delay_ms()"
#include <p24FJ128GA010.h>   // Header específico deL micro
#include <libpic30.h>       // se necesita para "__delay_ms()"
#include "LCD.h"
#include "RTCC.h"
#include <stdio.h>
//------------------------------------------------------------------------------

_CONFIG1(JTAGEN_OFF // deshabilito interface JTAG
        & GCP_OFF // deshabilito proteción de código general
        & GWRP_OFF // deshabilito protección escritura flash
        & ICS_PGx2 // ICSP interface (2=default)
        & FWDTEN_OFF) // deshabilito watchdog timer
_CONFIG2(IESO_OFF // deshabilito 2 velocidades de start up
        & FCKSM_CSDCMD // deshabilito clock-swithcing/monitor
        & FNOSC_PRIPLL // primary oscillator: habilito PLL
        & POSCMOD_XT) // primary oscillator: modo XT
//------------------------------------------------------------------------------
#define LED0 LATAbits.LATA0
#define LED1 LATAbits.LATA1
#define LED2 LATAbits.LATA2
#define LED3 LATAbits.LATA3
#define LED4 LATAbits.LATA4
#define LED5 LATAbits.LATA5
#define LED6 LATAbits.LATA6
#define LED7 LATAbits.LATA7

#define BA LATDbits.LATD6
#define BB LATDbits.LATD7
#define BC LATAbits.LATA7
#define BD LATTDbits.LATD13
void deco_hora(unsigned char dato);
void leer_hora();

int main(void) {
    AD1PCFG = 0xFFFF;
    initRTCC();
    InitLCD();
    while (1) {
        __delay_ms(100);
        leer_hora();
    }
}

void leer_hora() {
    unsigned char segundos, minutos, hora, dia, mes, anioa, dia_semana;
    unsigned int valor;
    //------------------
    _RTCPTR = 3; // comienza la secuencia
    valor = RTCVAL;
    SetLCDG(0), putLCD(((valor & 0x00F0) >> 4) + 0x30);
    SetLCDG(1), putLCD((valor & 0x000F) + 0x30);
    SetLCDG(2), putLCD('/');
    valor = RTCVAL;
    SetLCDG(3), putLCD(((valor & 0xF000) >> 12) + 0x30);
    SetLCDG(4), putLCD(((valor & 0x0F00) >> 8) + 0x30);
    SetLCDG(5), putLCD('/');
    SetLCDG(6), putLCD(((valor & 0x00F0) >> 4) + 0x30);
    SetLCDG(7), putLCD(((valor & 0x000F)) + 0x30);
    valor = RTCVAL;
    SetLCDC(0), putLCD(((valor & 0x00F0) >> 4) + 0x30);
    SetLCDC(1), putLCD(((valor & 0x000F)) + 0x30);
    SetLCDC(2), putLCD(':');
    valor = RTCVAL;
    SetLCDC(3), putLCD(((valor & 0xF000) >> 12) + 0x30);
    SetLCDC(4), putLCD(((valor & 0x0F00) >> 8) + 0x30);
    SetLCDC(5), putLCD(':');
    SetLCDC(6), putLCD(((valor & 0x00F0) >> 4) + 0x30);
    SetLCDC(7), putLCD(((valor & 0x000F)) + 0x30);
}
