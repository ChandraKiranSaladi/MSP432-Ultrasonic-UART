#include "ti/devices/msp432p4xx/inc/msp.h"
#include <stdint.h>

int miliseconds = 0;
int distance = 0;
long sensor = 0;

//#include "msp430dna.h"
//#include <intrinsics.h>
void initiateTrigger(void){


}

static void Delay(uint32_t loop)
{
    volatile uint32_t i;

    for (i = 0 ; i < loop ; i++);
}

int main(void)
{
    uint32_t i;
    uint32_t error = 0;

    WDT_A->CTL = WDT_A_CTL_PW |             // Stop watchdog timer
            WDT_A_CTL_HOLD;

    // Configure GPIO
    P1->DIR |= BIT0;                        // Set P1.0 as output
    P1->OUT |= BIT0;                        // P1.0 high
    P1->OUT &= ~BIT0;

    P2->DIR &= ~BIT7;            // P2.4 as o
    P2->REN |= BIT7;            // P2.4 pull down enabled
    P2->OUT &= ~BIT7;            // P2.4 initial output Low

    P2->SEL0 = 0;
    P2->SEL1 = 0;

    P2->IFG = 0;
    P2->IE |= BIT7;
    P2->IES &= ~BIT7;

        TIMER_A0->CCTL[0]= TIMER_A_CCTLN_CCIE;                             // CCR0 interrupt enabled
        TIMER_A0->CCR[0] = 1000-1;                   // 1ms at 1mhz
        TIMER_A0->CTL = TIMER_A_CTL_TASSEL_2 | TIMER_A_CTL_MC__UP | TIMER_A_CTL_CLR;                  // SMCLK, upmode



    //    P2->SEL0 |= BIT5;                       // TA0.CCI2A input capture pin, second function
    //    P2->DIR &= ~BIT5;
    //
    //    P4->SEL0 |= BIT2;                       // Set as ACLK pin, second function
    //    P4->DIR |= BIT2;
    //
    //    CS->KEY = CS_KEY_VAL;                   // Unlock CS module for register access
    //    // Select ACLK = REFO, SMCLK = MCLK = DCO
    //    CS->CTL1 = CS_CTL1_SELA_2 |
    //            CS_CTL1_SELS_3 |
    //            CS_CTL1_SELM_3;
    //    CS->KEY = 0;                            // Lock CS module from unintended accesses
    //
    //    // Timer0_A3 Setup
    //    TIMER_A0->CCTL[2] = TIMER_A_CCTLN_CM_1 | // Capture rising edge,
    //            TIMER_A_CCTLN_CCIS_0 |          // Use CCI2A=ACLK,
    //            TIMER_A_CCTLN_CCIE |            // Enable capture interrupt
    //            TIMER_A_CCTLN_CAP |             // Enable capture mode,
    //            TIMER_A_CCTLN_SCS;              // Synchronous capture
    //
    //    TIMER_A0->CTL |= TIMER_A_CTL_TASSEL_2 | // Use SMCLK as clock source,
    //            TIMER_A_CTL_MC__CONTINUOUS |    // Start timer in continuous mode
    //            TIMER_A_CTL_CLR;                // clear TA0R
    //
    //    SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;    // Enable sleep on exit from ISR
    //
    //    // Ensures SLEEPONEXIT takes effect immediately
    //    __DSB();
    //
    //    // Enable global interrupt
        __enable_irq();
    NVIC->ISER[1] = 1 << ((PORT2_IRQn) & 31);
          NVIC->ISER[0] = 1 << ((TA0_0_IRQn) & 31);
    //    __low_power_mode_3();
    //    __no_operation();

    while(1){

        //        P2->IE &= ~BIT7;          // disable interupt
        P2->DIR |= BIT6;          // trigger pin as output
        P2->OUT |= BIT6;          // generate pulse
        Delay(100);             // for 10us
        P2->OUT &= ~BIT6;                 // stop pulse
        //        P2->IE |= BIT7;          // disable interupt
        P2->IFG = 0;                   // clear flag just in case anything happened before
        P2->IES &= ~BIT7;         // rising edge on ECHO pin
        Delay(30000);          // delay for 30ms (after this time echo times out if there is no object detected)
        distance = sensor/58;           // converting ECHO lenght into cm
                if(distance < 100 && distance != 0)
                    P1->OUT |= BIT0;  //turning LED on if distance is less than 20cm and if distance isn't 0.
                else
                    P1->OUT &= ~BIT0;

    }
    // Validate the captured clocks
    //    for (i = 0; i < (NUMBER_TIMER_CAPTURES - 1); i++)
    //    {
    //        timerAcaptureCalcVal[i] = timerAcaptureValues[i + 1] - timerAcaptureValues[i];
    //
    //        if ((timerAcaptureCalcVal[i] > (REFO_CYCLES_TO_MCLK + 10)) ||
    //                (timerAcaptureCalcVal[i] < (REFO_CYCLES_TO_MCLK - 10)))
    //        {
    //            // Set an error flag
    //            error = 1;
    //        }
    //    }

}

// Timer A0 interrupt service routine
void PORT2_IRQHandler(void)
{

    if(P2->IFG & BIT7)  //is there interrupt pending?
    {
        if(!(P2->IES & BIT7)) // is this the rising edge?
        {

            TIMER_A0->CTL |= TIMER_A_CTL_CLR;   // clears timer A
            miliseconds = 0;
            P2->IES |=  BIT7;  //falling edge
        }
        else
        {
            sensor = (long)miliseconds*1000 + (long)TIMER_A0->R;    //calculating ECHO lenght
//            P1->OUT ^= BIT0;
            P2->IES &=  ~BIT7;  //falling edge
        }
        P2->IFG &= ~BIT7;             //clear flag
    }
}

void TA0_0_IRQHandler(void)
{
//    P1->OUT ^= BIT0;
    miliseconds++;
    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;
//    printf("milliseconds: "+miliseconds);
}

