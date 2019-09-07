#define FOSC 32000000        // Frecuencia del clock del micro
#define FCY FOSC/2          // Frecuencia de instrucciones usada por  "__delay_ms()"
#include <p24FJ128GA010.h>   // Header específico deL micro
#include <libpic30.h>       // se necesita para "__delay_ms()"
#include "LCD.h"
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
#define BD LATDbits.LATD13
void Configuracion_inicial();
unsigned int POTE;
float TEMP;
unsigned char POTENCIOMETRO [6] = {'\0'};
unsigned char TEMPERATURA [6] = {'\0'};

int main(void) {
    Configuracion_inicial();
    InitLCD();
    while (1) {
        /************************POTENCIOMETRO**********************************/
        AD1CHS = 5;
        AD1CON1bits.SAMP = 1;
        __delay_ms(100);
        AD1CON1bits.SAMP = 0; // start Converting
        while (!AD1CON1bits.DONE); // conversion done?
        POTE = ADC1BUF0; // yes then get ADC value
        sprintf(POTENCIOMETRO, "%4i", POTE);
        SetLCDG(0), putsLCD("POTE: ");
        SetLCDG(6), putLCD(POTENCIOMETRO[0]);
        SetLCDG(7), putLCD(POTENCIOMETRO[1]);
        SetLCDG(8), putLCD(POTENCIOMETRO[2]);
        SetLCDG(9), putLCD(POTENCIOMETRO[3]);
        /**************************TEMPERATURA**********************************/
        AD1CHS = 4;
        AD1CON1bits.SAMP = 1;
        __delay_ms(100);
        AD1CON1bits.SAMP = 0; // start Converting
        while (!AD1CON1bits.DONE); // conversion done?
        TEMP = ADC1BUF0; // yes then get ADC value
        TEMP = (TEMP *0.3248)-55 ;
        sprintf(TEMPERATURA, "%f2.1", TEMP);
        SetLCDC(0), putsLCD("TEMP: ");
        SetLCDC(6), putLCD(TEMPERATURA[0]);
        SetLCDC(7), putLCD(TEMPERATURA[1]);
        SetLCDC(8), putLCD(TEMPERATURA[2]);
        SetLCDC(9), putLCD(TEMPERATURA[3]);
    }
}

void Configuracion_inicial() {

    ///////////////////////////////////////////////////////////////////////////
    AD1PCFG = 0b111111111001111;
    AD1CON1 = 0x0000; // and starts converting
    AD1CSSL = 0;
    AD1CON3 = 0x0002; // Manual Sample, Tad = 2 Tcy
    AD1CON2 = 0;
    AD1CON1bits.ADON = 1; // turn ADC ON
    ///////////////////////////////////////////////////////////////////////////
    TRISA = 0X0000;
    TRISAbits.TRISA7 = 1;
    TRISDbits.TRISD6 = 1;
    TRISDbits.TRISD7 = 1;
    TRISDbits.TRISD13 = 1;

}
