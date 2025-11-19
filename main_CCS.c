
#include <msp430.h>
#include <stdint.h>

#define DATA_SIZE  5  
#define FRAM_ADDR  0xF800

#define DELAY_MS(ms)   __delay_cycles((uint32_t)(1000UL * (ms)))

static void P1_0_Switch(uint8_t value){
    if(value==1)
        P1OUT |= BIT0;// 把 P1.0 設成 1 → 腳位輸出 High
    else
        P1OUT &=~BIT0;// 把 P1.0 設成 0 → 腳位輸出 Low
}
static void P2_0_Switch(uint8_t value){
    if(value==1)
        P2OUT |= BIT0;// 把 P2.0 設成 1 → 腳位輸出 High
    else
        P2OUT &=~BIT0;// 把 P2.0 設成 0 → 腳位輸出 Low
}


static void gpio_output_init(void)
{
    
    P1DIR |= BIT0;          // P1.0 output 
    P1OUT &= ~BIT0;         // 初始 Low

    P2DIR |= BIT0;          // P2.0 output 
    P2OUT &= ~BIT0;         // 初始 Low
    
    P1IFG = 0;              // 清 Port IFG（保險）
    P2IFG = 0;
}





uint8_t flash_data[DATA_SIZE] = {0};

void FRAMWrite(void) {

    SYSCFG0 = FRWPPW;
    uint8_t i;
    for (i=0; i<DATA_SIZE; i++) {
        *((volatile uint8_t*)(FRAM_ADDR + i)) = flash_data[i];
    }

    SYSCFG0 = FRWPPW | PFWP;
}


void FRAMRead(void) {
    uint8_t i;
    for (i=0; i<DATA_SIZE; i++) {
        flash_data[i] = *((volatile uint8_t*)(FRAM_ADDR + i));
    }
}


int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer

    PM5CTL0 &= ~LOCKLPM5;                   // 解除高阻態，讓 GPIO 可用（即使本例沒用到腳位，建議保留）

    gpio_output_init();
    FRAMRead();    //從 FRAM 讀回
    DELAY_MS(100); // 延遲 100 ms
    if(flash_data[0]==1 && flash_data[1]==2 && flash_data[2]==3 && flash_data[3]==4 && flash_data[4]==5){
        P1_0_Switch(1);
        P2_0_Switch(1);
    }else{
        flash_data[0] = 1;
        flash_data[1] = 2;
        flash_data[2] = 3;
        flash_data[3] = 4;
        flash_data[4] = 5;   
        FRAMWrite();   // 寫入 FRAM
        DELAY_MS(100); // 延遲 100 ms
        P1_0_Switch(0); 
        P2_0_Switch(1);
    }

    while(1){
        __no_operation();
    }
}

