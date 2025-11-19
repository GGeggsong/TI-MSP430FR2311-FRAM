
#include <msp430.h>
#include <stdint.h>

#define DATA_SIZE  5  
#define FRAM_ADDR  0xF260


#define DELAY_MS(ms)   __delay_cycles((uint32_t)(1000UL * (ms)))

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

    FRAMRead();    //從 FRAM 讀回
    DELAY_MS(100); // 延遲 100 ms
    
    flash_data[0] = 1;
    flash_data[1] = 2;
    flash_data[2] = 3;
    flash_data[3] = 4;
    flash_data[4] = 5;
      
    FRAMWrite();   // 寫入 FRAM
    DELAY_MS(100); // 延遲 100 ms
    FRAMRead();    //從 FRAM 讀回
    DELAY_MS(100); // 延遲 100 ms


    while(1){
        __no_operation();
    }
}
