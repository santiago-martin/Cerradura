/* 
 * File:   CerraduraElectronica.c
 * Author: santi
 *
 * Created on 17 de agosto de 2019, 10:22
 */
#include <p24FJ128GA010.h>   // Header específico de un micro

//******************************************************************************
#define FOSC 32000000        // Frecuencia del clock del micro
#define FCY FOSC/2          // Frecuencia de instrucciones usada por  "__delay_ms()"
//------------------------------------------------------------------------------
//  ARCHIVOS HEADER
//------------------------------------------------------------------------------

#include "LCD.h"

#include <libpic30.h>       // se necesita para "__delay_ms()"

//------------------------------------------------------------------------------
//   CONFIGURACION DE BITS
//------------------------------------------------------------------------------

_CONFIG1( JTAGEN_OFF        // deshabilito interface JTAG
        & GCP_OFF           // deshabilito proteción de código general
        & GWRP_OFF          // deshabilito protección escritura flash
        & ICS_PGx2         // ICSP interface (2=default)
        & FWDTEN_OFF)       // deshabilito watchdog timer
_CONFIG2( IESO_OFF          // deshabilito 2 velocidades de start up
        & FCKSM_CSDCMD      // deshabilito clock-swithcing/monitor
        & FNOSC_PRIPLL      // primary oscillator: habilito PLL
        & POSCMOD_XT)       // primary oscillator: modo XT

//------------------------------------------------------------------------------
//  Constantes de Programa
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  Definición de Tipos de variable
//------------------------------------------------------------------------------
        unsigned char aux=0,abrir=0,entrar=0,cambiar=0,i=0,j=0,clabe=0,l=0,abierto=0,a=0,c=1;
 unsigned char clave[5]={0};
 unsigned char claven[5]={0};
//------------------------------------------------------------------------------
//  Declaración de variables Globales
//------------------------------------------------------------------------------

//

//------------------------------------------------------------------------------
//  Prototipos de funciones
//------------------------------------------------------------------------------

void InitLCD(void);
char ReadLCD( int addr);
void WriteLCD( int addr, char c);
void putsLCD( char *s);
void ConfigIni(void);
void escribirclave(void);
void msjini(void);
    //funcion interrupcion
    void __attribute__((interrupt, no_auto_psv))_CNInterrupt () { 
       IFS1bits.CNIF=0;
        if (aux==0){
        if(PORTDbits.RD6==0){
            __delay_ms(200);
         abrir=1;
            }
          if(PORTDbits.RD13==0){
            __delay_ms(200);
            
            cambiar=1;
            }
        }
        else{
            if(PORTDbits.RD6==0){
                while (PORTDbits.RD6==0){}
                if(i==0){
                    i=9;
                    claven[j]=i;
                    }
                else{
                i=i-1;
                claven[j]=i;
                }
                }
            if(PORTDbits.RD7==0){
                while (PORTDbits.RD7==0){}
            if(i==9){
                i=0;
                claven[j]=i;
            }
            else{
                i=i+1;
                claven[j]=i;
            }
            }
            if(PORTDbits.RD13==0){
  
                __delay_ms(200);
                j=j+1;
                i=0;
                if(j==4){
                  clabe=1;
                   j=0;
                   a=1;
                }
            }
       }
    }
    
    void main (void) {
        unsigned char result = 1;
        ConfigIni();
    InitLCD();
      msjini();
    while(1){
        LATAbits.LATA0=1;
            LATAbits.LATA1=0;
            LATAbits.LATA2=0;
   if(abrir==1){
       ClrLCD() ;
        SetLCDG(0);
        putsLCD("Ingrese Su Clave");
        SetLCDC(0);
        putsLCD("0000");
        aux=1;
        escribirclave();
        result = 1;
        for (l=0;l<4;l++){
                if(clave[l]!=claven[l]){
                    result = 0;
                 }      
                }
        if(result==1){
            ClrLCD() ;
            SetLCDG(0);
            putsLCD("Estado:Abierto");
           LATAbits.LATA0=0;
            LATAbits.LATA1=1;
            LATAbits.LATA2=0;
            while(PORTAbits.RA7==1){}
            msjini();
        }
        else{
                ClrLCD() ;
                SetLCDG(0);
            putsLCD("clave errada");
            while(PORTAbits.RA7==1){}
            msjini();
        }
        aux=0;
        abrir=0;
        entrar=0;
        result=1;
        clabe=0;
        j=0;
    }
      if(cambiar==1){
        ClrLCD() ;
        SetLCDG(0);
        putsLCD("Ingrese ClaveA");
        SetLCDC(0);
        putsLCD("0000");
        aux=1;
        escribirclave();
        result = 1;
        for (l=0;l<4;l++){
                if(clave[l]!=claven[l]){
                    result = 0;
                 }      
                }
        if(result==1){
            clabe=0;
            j=0;
            LATAbits.LATA0=0;
            LATAbits.LATA1=0;
            LATAbits.LATA2=1;
            
            ClrLCD() ;
            SetLCDG(0);
            putsLCD("clave correcta");
            while(PORTAbits.RA7==1){}
            ClrLCD() ;
            SetLCDG(0);
            putsLCD("nueva clave");
            SetLCDC(0);
            putsLCD("0000");
            escribirclave();
            for(l=0;l<=3;l++){
                clave[l]=claven[l];
                ClrLCD();
                SetLCDG(0);
        }
            ClrLCD() ;
            SetLCDG(0);
            putsLCD("clave definida");
            while(PORTAbits.RA7==1){}
            LATAbits.LATA0=0;
            LATAbits.LATA1=0;
            LATAbits.LATA2=1;
            msjini();
          }
        else{
            ClrLCD() ;
            SetLCDG(0);
            putsLCD("clave incorrecta");
            while(PORTAbits.RA7==1){}
            LATAbits.LATA0=1;
            LATAbits.LATA1=0;
            LATAbits.LATA2=0;
            msjini();
        }
        }
      aux=0;
      cambiar=0;
      entrar=0;
      clabe=0;
      a=0;
      for(l=0;l<=3;l++){
       claven[l]=0;
    } 
}
      }
    //funciones
    void ConfigIni(void){
        TRISAbits.TRISA7=1;
        TRISDbits.TRISD6=1;
        TRISDbits.TRISD7=1;
        TRISDbits.TRISD13=1;
        TRISAbits.TRISA0=0;
        TRISAbits.TRISA1=0;
        TRISAbits.TRISA2=0;
        PORTAbits.RA0=1;
         PORTAbits.RA1=0;
          PORTAbits.RA2=0;
         IFS1bits.CNIF=0;
         CNEN1bits.CN15IE=1;
         CNEN2bits.CN16IE=1;
         CNEN2bits.CN19IE=1;
        IEC1bits.CNIE=1;
    }
    
    void escribirclave(void){
        aux=1;
         while(clabe==0){
             SetLCDC (0);
            for(l=0;l<=3;l++){
                putLCD(claven[l] + 0x30);
            }
         }
        while(a==0){}
        l=0;
        a=0;
        }
    void msjini(void){
    ClrLCD() ;
    SetLCDG(2);
    putsLCD("Estado:cerrado");    
    SetLCDC(0);
      putsLCD("b1:Abrir b4:new");
    }