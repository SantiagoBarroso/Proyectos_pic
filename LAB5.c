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
//------------------------------------------------------------------------------
void Leer_temperatura_y_pote();
void Leer_hora();
void Configuracion_entrada_salida();
void Enviar_UART();
//------------------------------------------------------------------------------
unsigned int POTE, estado = 0;
float TEMP;
unsigned char POTENCIOMETRO [4] = {'\0'};
unsigned char TEMPERATURA [4] = {'\0'};
unsigned char DATO [10] = {'\0'};
//------------------------------------------------------------------------------

int main(void) {
    Configuracion_entrada_salida();
    Configuracion_ADC();
    Configuracion_UART();
    InitLCD();
    initRTCC();
    while (1) {
        switch (estado) {
            case 0:
                Leer_temperatura_y_pote();
                sprintf(DATO, "TEMP: %4s", TEMPERATURA);
                Enviar_UART();
                sprintf(DATO, "POTE: %4s", POTENCIOMETRO);
                Enviar_UART();
                break;
            case 1:
                break;
            case 2:
                break;
        }
    }
}
//------------------------------------------------------------------------------

void Configuracion_entrada_salida() {
    TRISA = 0X0000;
    TRISAbits.TRISA7 = 1;
    TRISDbits.TRISD6 = 1;
    TRISDbits.TRISD7 = 1;
    TRISDbits.TRISD13 = 1;
}

void Configuracion_ADC() {
    AD1PCFG = 0b111111111001111;
    AD1CON1 = 0x0000; // and starts converting
    AD1CSSL = 0;
    AD1CON3 = 0x0002; // Manual Sample, Tad = 2 Tcy
    AD1CON2 = 0;
    AD1CON1bits.ADON = 1; // turn ADC ON
}

void Leer_temperatura_y_pote() {
    /************************POTENCIOMETRO**********************************/
    AD1CHS = 5;
    AD1CON1bits.SAMP = 1;
    __delay_ms(10);
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
    __delay_ms(10);
    AD1CON1bits.SAMP = 0; // start Converting
    while (!AD1CON1bits.DONE); // conversion done?
    TEMP = ADC1BUF0; // yes then get ADC value
    TEMP = (TEMP * 0.3248) - 55;
    sprintf(TEMPERATURA, "%f2.2", TEMP);
    SetLCDC(0), putsLCD("TEMP: ");
    SetLCDC(6), putLCD(TEMPERATURA[0]);
    SetLCDC(7), putLCD(TEMPERATURA[1]);
    SetLCDC(8), putLCD(TEMPERATURA[2]);
    SetLCDC(9), putLCD(TEMPERATURA[3]);
}

void Leer_hora() {
    //------------------
    unsigned int valor;
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

void Configuracion_UART() {
    U2BRG = 103; //Set Baudrate
    U2MODEbits.UARTEN = 1;
    U2MODEbits.BRGH = 0;
    U2STAbits.UTXEN = 1;
    IFS1bits.U2RXIF = 0; //Limpio bandera
    IEC1bits.U2RXIE = 0; //Habilito interrupcion
    IPC7bits.U2RXIP = 0b111; //Prioridad 6
}

void Enviar_UART() {
    int i;
    for (i = 0; i < sizeof (DATO); i++) {
        U2TXREG = DATO[i];
        __delay_ms(20);
    }
    U2TXREG = 0x0D;
}
//------------------------------------------------------------------------------

void __attribute__((interrupt, no_auto_psv)) _U2RXInterrupt(void) {
    IFS1bits.U2RXIF = 0;
}