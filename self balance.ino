#include <avr/io.h>
#include <util/delay.h>

int main()
{  
   int ocra, ocrb, adc_value;
   float angle=60;  

   ocra = 535 - (angle/180)*438;   //angle for upper arm 
   ocrb = 97+(angle/180)*438;      //angle for lower arm
  
 
   //Configure TIMER1
   TCCR1A|=(1<<COM1A1)|(1<<COM1B1)|(1<<WGM11);        //NON Inverted PWM
   TCCR1B|=(1<<WGM13)|(1<<WGM12)|(1<<CS11)|(1<<CS10); //PRESCALER=64 MODE 14(FAST PWM)

   ICR1=4999;  //fPWM=50Hz (Period = 20ms Standard).

   DDRB|=(1<<PB1)|(1<<PB2);   //PWM Pins as Out

   //initialise ADC
   ADMUX = 1<<REFS0 ;         //internal reference voltage   
   ADCSRA = 1<<ADEN | 1<<ADPS1 | 1<<ADPS2 ;   

   OCR1A = ocra;     //initial
   OCR1B = ocrb;     //angles
   _delay_ms(2000);
   
   while(1)
   {    
        ADMUX &= ~((1<<MUX0)|(1<<MUX1)|(1<<MUX2)); //analog pin 0
        ADCSRA |= (1<<ADSC);                       //adc start conversion   
        while(!(ADCSRA & (1<<ADIF)));              //wait till interrupt flag is set 
        ADCSRA &= ~(1<<ADIF);                      //clear interrupt flag
        adc_value=ADC;
        
        if(adc_value<300)
        {
           OCR1B = ocrb++;             
        }
        else if(adc_value>355)
        {
           OCR1B = ocrb--;     
        }
        /*else if(adc_value>280 && adc_value<350)
        {
           _delay_ms(100);  
        }*/
        _delay_ms(5);
   }
}

