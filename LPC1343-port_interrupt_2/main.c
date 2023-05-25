#include <lpc13xx.h>

void delay(void);
  
int main(void)
{       
        volatile unsigned int ButtonP3_2,Button;
        // Kesme
        LPC_GPIO3->DIR &= ~(0x04); // P3.2  direction is input - button
        LPC_GPIO3->IS &= ~(0x00); // 0 = PIOn_x pinindeki kesme kenara duyarli olarak yapilandirilir.
        LPC_GPIO3->IBE &= ~(0x00); // controlled by register IEV
        LPC_GPIO3->IEV |= (0x04); // 1 = GPIOIS kaydindaki ayara bagli olarak,
        // PIOn_x pimindeki yükselen kenarlar veya YÜKSEK seviye bir kesmeyi tetikler. 
        /*LPC_GPIO3->IEV &= ~(0x00);//0 = GPIOIS kaydindaki ayara bagli olarak,
        // PIOn_x pinindeki düsen kenarlar veya DÜSÜK seviye bir kesmeyi tetikler.*/
        while(!(((Button=LPC_GPIO3->RIS)&0x04)==0x04)){};// Kesme hazirligi tamamdir
        LPC_GPIO3->IE |= 0x04; // P3.2 Kesmesi aktif
        // Kesme
	LPC_GPIO0->DIR |=  (1<<7); //Config PIO0_7 as Output
        LPC_GPIO0->DATA |= (1<<7); //drive PIO0_7 Led ON
        
        /*
        LPC_GPIO1->DIR |= (1<<7); //Config PIO0_7 as Output
        LPC_GPIO1->DATA |= (1<<7); //drive PIO0_7 Led ON
        */
           while(1)
	{
          LPC_GPIO0->DATA &= ~(1<<7); //Drive output HIGH to turn LED OFF
            // Better way would be LPC_GPIO0->DATA |= (1<<7);
          while(((ButtonP3_2=LPC_GPIO3->MIS) & 0x04)==0x04)
          {

            LPC_GPIO0->DATA |= (1<<7); //Drive output HIGH to turn LED ON
            // Better way would be LPC_GPIO0->DATA |= (1<<7);
            LPC_GPIO3->IC |= (0x04);
            /*LPC_GPIO3->IC &= ~(0x04);//Yanlis kullanim*/
            //LPC_GPIO3->IE |= 0x04;/* Gerekli degil*/
          }
	}
	return 0; //normally this wont execute
}	

void delay(void) //Hard-coded delay function
{
	int count,i=0;
	for(count=0; count < 4500000; count++) //You can edit this as per your needs
	{
		i++; //something needs to be here else compiler will remove the for loop!
	}
}
