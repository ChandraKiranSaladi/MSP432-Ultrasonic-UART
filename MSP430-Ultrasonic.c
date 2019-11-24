//#include <intrinsics.h>
//#include <msp430.h>
//#include <stdint.h>
//
//int miliseconds;
//int distance;
//long sensor;
//
//void main(void)
//{
//  // BCSCTL1 = CALBC1_1MHZ;
//  // DCOCTL = CALDCO_1MHZ;                     // submainclock 1mhz
//  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
//
//  TA0CCTL0 = CCIE;                             // CCR0 interrupt enabled
//  TA0CCR0 = 1000;				    // 1ms at 1mhz
//  TA0CTL = TASSEL_2 + MC_1;                  // SMCLK, upmode
//
//  P1IFG  = 0x00;			    //clear all interrupt flags
//  P1DIR |= 0x01;                            // P1.0 as output for LED
//  P1OUT &= ~0x01;                           // turn LED off
//
//  _BIS_SR(GIE);                 	    // global interrupt enable
//
// while(1){
//	P1IE &= ~0x01;			// disable interupt
//	P1DIR |= 0x02;			// trigger pin as output
//	P1OUT |= 0x02;			// generate pulse
//	__delay_cycles(10);             // for 10us
//	P1OUT &= ~0x02;                 // stop pulse
//  	P1DIR &= ~0x04;			// make pin P1.2 input (ECHO)
//        P1IFG = 0x00;                   // clear flag just in case anything happened before
//	P1IE |= 0x04;			// enable interupt on ECHO pin
//	P1IES &= ~0x04;			// rising edge on ECHO pin
//        __delay_cycles(30000);          // delay for 30ms (after this time echo times out if there is no object detected)
//        distance = sensor/58;           // converting ECHO lenght into cm
//        if(distance < 20 && distance != 0) P1OUT |= 0x01;  //turning LED on if distance is less than 20cm and if distance isn't 0.
//        else P1OUT &= ~0x01;
//
//
//
//
// }
//}
//
//#pragma vector=PORT1_VECTOR
//__interrupt void Port_1(void)
//{
//	if(P1IFG&0x04)  //is there interrupt pending?
//        {
//          if(!(P1IES&0x04)) // is this the rising edge?
//          {
//            TA0CTL|=TA0CLR;   // clears timer A
//            miliseconds = 0;
//            P1IES |= 0x04;  //falling edge
//          }
//          else
//          {
//            sensor = (long)miliseconds*1000 + (long)TAR;	//calculating ECHO lenght
//
//          }
//	P1IFG &= ~0x04;				//clear flag
//	}
//}
//
//#pragma vector=TIMER0_A0_VECTOR
//__interrupt void Timer_A (void)
//{
//  miliseconds++;
//}
//
//
//
///**
// *
// * 16 bit register
// *
// * count => process of counting
// * TAx . x=> the no.of instantiation
// * n -> the compare/capture register associated with the
// *      timer
// *
// * TAxR incremets
// *
// *
// *
// *
// * #include <msp430.h>
//
//int main(void)
//{
//  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
//  P2DIR |= BIT0+BIT1;                       // P2.0 and P2.1 output
//  P2SEL |= BIT0+BIT1;                       // P2.0 and P2.1 options select
//  TA1CCR0 = 10-1;                          // PWM Period
//  TA1CCTL1 = OUTMOD_7;                      // CCR1 reset/set
//  TA1CCR1 = 384;                            // CCR1 PWM duty cycle
//  TA1CCTL2 = OUTMOD_7;                      // CCR2 reset/set
//  TA1CCR2 = 128;                            // CCR2 PWM duty cycle
//  TA1CTL = TASSEL_2 + MC_1 + TACLR;         // SMCLK, up mode, clear TAR
//
//  __bis_SR_register(LPM0_bits);             // Enter LPM0
//  __no_operation();                         // For debugger
//}
//
// * /
// *
