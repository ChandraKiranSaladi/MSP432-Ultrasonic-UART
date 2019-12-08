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

    /*         UART                      */
    CS->KEY = CS_KEY_VAL;                   // Unlock CS module for register access
    CS->CTL0 = 0;                           // Reset tuning parameters
    CS->CTL0 = CS_CTL0_DCORSEL_3;           // Set DCO to 12MHz (nominal, center of 8-16MHz range)
    CS->CTL1 = CS_CTL1_SELA_2 |             // Select ACLK = REFO
            CS_CTL1_SELS_3 |                // SMCLK = DCO
            CS_CTL1_SELM_3;                 // MCLK = DCO
    CS->KEY = 0;                            // Lock CS module from unintended accesses


    // Configure GPIO
    P1->DIR |= BIT0;                        // Set P1.0 as output
    P1->OUT |= BIT0;                        // P1.0 high
    P1->OUT &= ~BIT0;

    // Configure UART pins
        P1->SEL0 |= BIT2 | BIT3;                // set 2-UART pin as secondary function


    P2->DIR &= ~BIT7;            // P2.4 as o
    P2->REN |= BIT7;            // P2.4 pull down enabled
    P2->OUT &= ~BIT7;            // P2.4 initial output Low

    P2->SEL0 = 0;
    P2->SEL1 = 0;

    P2->IFG = 0;
    P2->IE |= BIT7;
    P2->IES &= ~BIT7;



    // Configure UART
    EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SWRST; // Put eUSCI in reset
    EUSCI_A0->CTLW0 = EUSCI_A_CTLW0_SWRST | // Remain eUSCI in reset
            EUSCI_B_CTLW0_SSEL__SMCLK;      // Configure eUSCI clock source for SMCLK
    // Baud Rate calculation
    // 12000000/(16*9600) = 78.125
    // Fractional portion = 0.125
    // User's Guide Table 21-4: UCBRSx = 0x10
    // UCBRFx = int ( (78.125-78)*16) = 2
    EUSCI_A0->BRW = 78;                     // 12000000/16/9600
    EUSCI_A0->MCTLW = (2 << EUSCI_A_MCTLW_BRF_OFS) |
            EUSCI_A_MCTLW_OS16;

    EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_SWRST; // Initialize eUSCI
    EUSCI_A0->IFG &= ~EUSCI_A_IFG_RXIFG;    // Clear eUSCI RX interrupt flag
    EUSCI_A0->IE &= ~EUSCI_A_IE_RXIE;        // Disable USCI_A0 RX interrupt


    /*              TIMER A0            */
    TIMER_A0->CCTL[0]= TIMER_A_CCTLN_CCIE;       // CCR0 interrupt enabled
    TIMER_A0->CCR[0] = 1000-1;                   // 1ms at 1mhz
    TIMER_A0->CTL = TIMER_A_CTL_TASSEL_2 | TIMER_A_CTL_MC__UP | TIMER_A_CTL_CLR;                  // SMCLK, upmode

    __enable_irq();             // Enables interrupts to the system

    NVIC->ISER[1] = 1 << ((PORT2_IRQn) & 31);       // Very important to assign interrupts to the NVIC vector otherwise they would not
                                                    // considered
    NVIC->ISER[0] = 1 << ((TA0_0_IRQn) & 31);
    //    __low_power_mode_3();
    //    __no_operation();

    while(1){

        //        P2->IE &= ~BIT7;          // disable interupt
        P2->DIR |= BIT6;          // trigger pin as output
        P2->OUT |= BIT6;          // generate pulse
        Delay(100);               // for 10us
        P2->OUT &= ~BIT6;         // stop pulse
        //        P2->IE |= BIT7;          // disable interupt
        P2->IFG = 0;              // clear P2 interrupt just in case anything happened before
        P2->IES &= ~BIT7;         // rising edge on ECHO pin
        Delay(30000);             // delay for 30ms (after this time echo times out if there is no object detected)
        distance = sensor/58;     // converting ECHO lenght into cm
        char buffer[50];
        sprintf(buffer,"distance is %d\n",distance);
        uart_puts(buffer);
        if(distance < 300 && distance != 0)
            P1->OUT |= BIT0;  //turning LED on if distance is less than 20cm and if distance isn't 0.
        else
            P1->OUT &= ~BIT0;

    }

}

int uart_puts(const char *str)
{
    int status = -1;

    if (str != '\0') {
        status = 0;

        while (*str != '\0') {
            /* Wait for the transmit buffer to be ready */
            while (!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));

            /* Transmit data */
            EUSCI_A0->TXBUF  = *str;

            /* If there is a line-feed, add a carriage return */
            if (*str == '\n') {
                /* Wait for the transmit buffer to be ready */
                while (!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
                EUSCI_A0->TXBUF = '\r';
            }

            str++;
        }
    }

    return status;
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
            sensor = (long)miliseconds*1000 + (long)TIMER_A0->R;    //calculating ECHO length
            //            P1->OUT ^= BIT0;
            P2->IES &=  ~BIT7;  //falling edge

        }
        P2->IFG &= ~BIT7;             //clear flag
    }
}

void TA0_0_IRQHandler(void)
{
    //    Interrupt gets triggered for every clock cycle in SMCLK Mode counting number of pulses
    miliseconds++;
    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;
    //    printf("milliseconds: "+miliseconds);
}

