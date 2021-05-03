/*
 * File:   LAB9.c
 * Author: gelp0
 *
 * Created on 27 de abril de 2021, 11:56 AM
 * Laboratorio 09
 * Archivo:      lab9.c
 * Dispositivo:	PIC16F887
 * Autor:	Guillermo Lam
 * Compilador:	pic-as (v2.30), MPLABX V5.45
 * 
 * Programa:	controlador de servomotores
 * Hardware:	servomotores y potenciometros
 *
 * Creado: 27 abr, 2021
 * Ultima modificacion: 27 abr, 2021
 */


//CONFIG 1
#pragma config FOSC = INTRC_NOCLKOUT        // Oscillator Selection bits (LP oscillator: Low-power crystal on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF          // Code Protection bit (Program memory code protection is enabled)
#pragma config CPD = OFF         // Data Code Protection bit (Data memory code protection is enabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 2.1V)
#pragma config WRT = OFF       // Flash Program Memory Self Write Enable bits (0000h to 0FFFh write protected, 1000h to 1FFFh may be modified by EECON control)

#define _XTAL_FREQ 8000000
#include <xc.h>
#include <stdint.h>


void config(void); 


void __interrupt() isr(void){ 
    
    if(PIR1bits.ADIF == 1){
        if(ADCON0bits.CHS == 0){
            CCPR1L = (ADRESH >> 1) + 124;
        }    
        else if(ADCON0bits.CHS == 1){ 
            CCPR2L = (ADRESH >> 1) + 124;
        }
        PIR1bits.ADIF = 0;
    }
}

void main(void) {   
    config();       
    while(1){
        
        if (ADCON0bits.GO == 0){
            if(ADCON0bits.CHS == 1){
              ADCON0bits.CHS = 0;
            }
            else{
              ADCON0bits.CHS = 1;}
            __delay_us(50);
            ADCON0bits.GO =1;}}}

void config(void){
    ANSEL = 0b00000011;
    ANSELH = 0;
    
    TRISA = 0b00000011; 
    TRISC = 0x00; 
    
    PORTA = 0x00;
    PORTB = 0x00;
    PORTC = 0x00;
    PORTD = 0x00;
    PORTE = 0x00;
   
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 1;   
    OSCCONbits.SCS = 1;
    
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    PIE1bits.ADIE = 1;
    PIR1bits.ADIF = 0;
    
    ADCON1bits.ADFM = 0;  
    ADCON1bits.VCFG0 = 0; 
    ADCON1bits.VCFG1 = 0; 
    ADCON0bits.ADCS = 0b10;
    ADCON0bits.CHS = 0; 
    ADCON0bits.ADON = 1;    
    __delay_us(50);
   
    
    TRISCbits.TRISC2 = 1;  
    TRISCbits.TRISC1 = 1;
    PR2 = 255;
    
    CCP1CONbits.P1M = 0;   
    CCP1CONbits.CCP1M = 0b1100; 
    CCPR1L = 0x0f; 
    CCP2CONbits.CCP2M = 0b1100;
    CCPR2L = 0x0f;
  
    CCP1CONbits.DC1B = 0;
    CCP2CONbits.DC2B0 = 0;
    CCP2CONbits.DC2B1 = 0;
    
    PIR1bits.TMR2IF = 0;     
    T2CONbits.T2CKPS = 0b11; 
    T2CONbits.TMR2ON = 1;   
    while(PIR1bits.TMR2IF == 0);    
    PIR1bits.TMR2IF = 0;
    TRISCbits.TRISC2 = 0;          
    TRISCbits.TRISC1 = 0;     
    
    return;
}
