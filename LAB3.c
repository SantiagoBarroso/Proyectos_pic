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
#define LED0 _LATA0
#define LED1 _LATA1
#define LED2 _LATA2
#define LED3 _LATA3
#define LED4 _LATA4
#define LED5 _LATA5
#define LED6 _LATA6
#define LED7 _LATA7

#define BA _RD6
#define BB _RD7
#define BC _RA7
#define BD _RD13

void Configuracion_interrupciones();
int contador = 0;

int main(void) {
    AD1PCFG = 0XFFFF;
    TRISA = 0X0000;
    _TRISD7 = 1;
    _TRISD6 = 1;
    _TRISA7 = 1;
    _TRISD13 = 1;
    Configuracion_interrupciones();
    InitLCD();
    while (1) {
        LED6 = 1;
    }
}

void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void) {
    _T3IF = 0;
    LED0 = 1;
    LED6 = 0;
    __delay_ms(25);
    LED0 = 0;
}

void __attribute__((interrupt, no_auto_psv)) _T5Interrupt(void) {
    _T5IF = 0;
    if (contador == 4) {
        contador = 0;
        LED1 = 1;
        LED6 = 0;
        __delay_ms(1000);
        LED1 = 0;
    } else {
        contador++;
    }
}

void __attribute__((interrupt, no_auto_psv)) _CNInterrupt() {
    int a;
    a = PORTD;
    IFS1bits.CNIF = 0; //BAJO BANDERA DE INTERRUPCIONES DE CN
    T3CONbits.TON = 0; //APAGO EL CONTEO DEL TIMER 3
    T5CONbits.TON = 0; //APAGO EL CONTEO DEL TIMER 5
    IEC0bits.T3IE = 0; //DESHABILITO INTERRUPCIONES DE TIMER 3
    IEC1bits.T5IE = 0; //DESHABILITO INTERRUPCIONES DE TIMER 5
    if (BA == 0) {
        putsLCD("a");
        IPC2bits.T3IP = 0B111; //ESTABLEZCO PRIORIDAD DEL TIMER 3
        IPC7bits.T5IP = 0B001; //ESTABLEZCO PRIORIDAD DEL TIMER 5
        SRbits.IPL = 0B000; //ESTABLEZCO PRIORIDAD DEL CPU
    }
    if (BB == 0) {
        putsLCD("b");
        IPC7bits.T5IP = 0B111; //ESTABLEZCO PRIORIDAD DEL TIMER 5
        IPC2bits.T3IP = 0B001; //ESTABLEZCO PRIORIDAD DEL TIMER 3
        SRbits.IPL = 0B000; //ESTABLEZCO PRIORIDAD DEL CPU
    }
    if (BD == 0) {
        putsLCD("c");
        SRbits.IPL = 0B111; //ESTABLEZCO PRIORIDAD DEL CPU
        IPC2bits.T3IP = 0B001; //ESTABLEZCO PRIORIDAD DEL TIMER 3
        IPC7bits.T5IP = 0B000; //ESTABLEZCO PRIORIDAD DEL TIMER 5
    }
    T3CONbits.TON = 1; //APAGO EL CONTEO DEL TIMER 3
    T5CONbits.TON = 1; //APAGO EL CONTEO DEL TIMER 5
    IEC0bits.T3IE = 1; //DESHABILITO INTERRUPCIONES DE TIMER 3
    IEC1bits.T5IE = 1; //DESHABILITO INTERRUPCIONES DE TIMER 5

    __delay_ms(500);
}

void Configuracion_interrupciones() {
    /***********************TIMER 3 CUENTA HASTA 0.5 SEGUNDOS*************************/
    T3CONbits.TCKPS = 0B11; //INICIO EL PREESCALER EN 255
    IPC2bits.T3IP = 0B001; //ESTABLEZCO PRIORIDAD DEL TIMER 3
    IEC0bits.T3IE = 1; //HABILITO INTERRUPCIONES DE TIMER 3
    T3CONbits.TON = 1; //INICIO EL CONTEO DEL TIMER 3
    PR3 = 31250;
    /***********************TIMER 5 CUENTA HASTA 2 SEGUNDOS*************************/
    T5CONbits.TCKPS = 0B11; //INICIO EL PREESCALER EN 255
    IPC7bits.T5IP = 0B001; //ESTABLEZCO PRIORIDAD DEL TIMER 5
    IEC1bits.T5IE = 1; //HABILITO INTERRUPCIONES DE TIMER 5
    T5CONbits.TON = 1; //INICIO EL CONTEO DEL TIMER 5
    PR5 = 31250;
    /***********************CPU CONFIGURACIONES*************************/
    SRbits.IPL = 0B000; //ESTABLEZCO PRIORIDAD DEL CPU    
    /***********************ENTRADAS CONFIGURACIONES*************************/
    _CNIE = 1; //HABILITO LAS INTERRUPCIONES
    IPC4bits.CNIP = 0B111; //ESTABLEZCO PRIORIDAD DE LAS ENTRADAS
    CNEN1bits.CN15IE = 1; // HABILITO LAS DEL CN15
    CNEN2bits.CN16IE = 1; // HABILITO LAS DEL CN16
    CNEN2bits.CN19IE = 1; // HABILITO LAS DEL CN19
}
/*
PRX -> VALOR MAXIMO DE CONTEO DEL TIMER X, CUANDO LLEGA HASTA EL VALOR DEL PRX, DESBORDA Y RETORNA EL TIMERX A 0X0000
LA ECUACION DEL TIEMPO ES:
TIEMPO = [1/(FOSC/2)]*PRX*PREESCALERX
 */

//CN15, CN16, CN19
