	#define FOSC 8000000        // Frecuencia del clock del micro
#define FCY FOSC/2          // Frecuencia de instrucciones usada por  "__delay_ms()"
#include <p24FJ128GA010.h>   // Header específico deL micro
#include <libpic30.h>       // se necesita para "__delay_ms()"
#include "LCD.h"
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
#define LED0 PORTAbits.RA0
#define LED1 PORTAbits.RA1
#define LED2 PORTAbits.RA2
#define LED3 PORTAbits.RA3
#define LED4 PORTAbits.RA4
#define LED5 PORTAbits.RA5
#define LED6 PORTAbits.RA6
#define LED7 PORTAbits.RA7

#define BA PORTDbits.RD6
#define BB PORTDbits.RD7
#define BC PORTAbits.RA7
#define BD PORTDbits.RD13
#define TIEMPO __delay_ms(1000)
//------------------------------------------------------------------------------

const unsigned char CERRADURA_OFF[] = {"CERRADURA OFF"};
const unsigned char INGRESE_CODIGO[] = {"INGRESE CODIGO"};
const unsigned char CODIGO_INCORRECTO[] = {"CODIGO INCORRECTO"};
const unsigned char CODIGO_CORRECTO[] = {"CODIGO CORRECTO"};
const unsigned char DESBLOQUEADO[] = {"DESBLOQUEADO"};
const unsigned char INTENTOS[] = {"INTENTOS"};
const unsigned char BLOQUEADO[] = {"BLOQUEADO"};
const unsigned char REINICIE[] = {"REINICIE"};
const unsigned char CAMBIO_EXITOSO[] = {"CAMBIO EXITOSO"};
const unsigned char REINICIANDO[] = {"REINICIANDO"};
const unsigned char NUEVO_CODIGO[] = {"NUEVO CODIGO"};

unsigned char passp[4];
unsigned char passi[4];
char AUX[0];
char CARACTER;
int estado = 0, Error = 0, i, intentos = 3, CAMBIAR_CLAVE = 0;
//------------------------------------------------------------------------------
unsigned char Ingresar_pass();

int main(void) {
    TRISA = 0x0000;
    AD1PCFG = 0xFFFF;
    TRISDbits.TRISD6 = 1;
    TRISDbits.TRISD7 = 1;
    TRISAbits.TRISA7 = 1;
    TRISDbits.TRISD13 = 1;
    InitLCD();
    passp[0] = 'A';
    passp[1] = 'B';
    passp[2] = 'C';
    passp[3] = 'D';
    while (1) {
        switch (estado) {
            case 0:
                SetLCDG(0), putsLCD(CERRADURA_OFF);
                SetLCDC(0), putsLCD(INGRESE_CODIGO);
                //----------
                if (BA == 0 && BD == 0) {
                    estado++;
                    TIEMPO;
                    ClrLCD();
                }
                if (BB == 0 && BC == 0) {
                    estado++;
                    TIEMPO;
                    ClrLCD();
                    CAMBIAR_CLAVE = 1;
                }
                break;
            case 1:
                if (CAMBIAR_CLAVE == 2) {
                    SetLCDG(0), putsLCD(NUEVO_CODIGO);
                } else {
                    SetLCDG(0), putsLCD(INGRESE_CODIGO);
                }
                CARACTER = Ingresar_pass();
                if (CAMBIAR_CLAVE == 0 || CAMBIAR_CLAVE == 1)passi[0] = CARACTER;
                if (CAMBIAR_CLAVE == 2)passp[0] = CARACTER;
                AUX[0] = CARACTER;
                SetLCDC(0), putsLCD(AUX);
                estado++;
                break;
            case 2:
                CARACTER = Ingresar_pass();
                if (CAMBIAR_CLAVE == 0 || CAMBIAR_CLAVE == 1)passi[1] = CARACTER;
                if (CAMBIAR_CLAVE == 2)passp[1] = CARACTER;
                AUX[0] = CARACTER;
                SetLCDC(2), putsLCD(AUX);
                estado++;
                break;
            case 3:
                CARACTER = Ingresar_pass();
                if (CAMBIAR_CLAVE == 0 || CAMBIAR_CLAVE == 1)passi[2] = CARACTER;
                if (CAMBIAR_CLAVE == 2)passp[2] = CARACTER;
                AUX[0] = CARACTER;
                SetLCDC(4), putsLCD(AUX);
                estado++;
                break;
            case 4:
                CARACTER = Ingresar_pass();
                if (CAMBIAR_CLAVE == 0 || CAMBIAR_CLAVE == 1)passi[3] = CARACTER;
                if (CAMBIAR_CLAVE == 2)passp[3] = CARACTER, CAMBIAR_CLAVE = 3;
                AUX[0] = CARACTER;
                SetLCDC(6), putsLCD(AUX);
                estado++;
                break;
            case 5:
                if (CAMBIAR_CLAVE == 0 || CAMBIAR_CLAVE == 1) {
                    for (i = 0; i <= 3; i++)if (passi[i] != passp[i])Error = 1;
                    if (Error == 0) {
                        SetLCDG(0), putsLCD(CODIGO_CORRECTO);
                        for (i = 0; i <= 3; i++)TIEMPO;
                        ClrLCD();
                        if (CAMBIAR_CLAVE == 0)SetLCDG(0), putsLCD(DESBLOQUEADO);
                        for (i = 0; i <= 3; i++)TIEMPO;
                        intentos = 3;
                        if (CAMBIAR_CLAVE == 0) estado = 0;
                        if (CAMBIAR_CLAVE == 1) estado = 1, CAMBIAR_CLAVE = 2;
                        ClrLCD();
                    }
                    if (Error == 1) {
                        SetLCDG(0), putsLCD(CODIGO_INCORRECTO);
                        for (i = 0; i <= 3; i++)TIEMPO;
                        ClrLCD(), SetLCDG(0), putsLCD(INTENTOS);
                        for (i = 0; i <= 3; i++)TIEMPO;
                        ClrLCD();
                        if (intentos > 0) intentos--, estado = 0;
                        if (intentos == 0) estado = 6;
                        ClrLCD();
                        Error = 0;
                    }
                }
                if (CAMBIAR_CLAVE == 3) {
                    SetLCDG(0), putsLCD(CAMBIO_EXITOSO);
                    for (i = 0; i <= 3; i++)TIEMPO;
                    SetLCDC(0), putsLCD(REINICIANDO);
                    for (i = 0; i <= 3; i++)TIEMPO;
                    estado = 0;
                    CAMBIAR_CLAVE = 0;
                    ClrLCD();
                }
                break;
            case 6:
                ClrLCD();
                while (1) {
                    SetLCDG(0), putsLCD(BLOQUEADO);
                    SetLCDC(0), putsLCD(REINICIE);
                }
                break;
        }
    }
}

//------------------------------------------------------------------------------

unsigned char Ingresar_pass() {
    unsigned char letra = 'j';
    while (letra == 'j') {
        if (BA == 0)letra = 'A';
        if (BB == 0)letra = 'B';
        if (BC == 0)letra = 'C';
        if (BD == 0)letra = 'D';
    }
    __delay_ms(1000);
    return letra;
}