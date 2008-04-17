// Test RTI

#include "includes.h"

// ����RTI
// ע��:RTI ʹ�þ���Ƶ�� �� PLL�޹�
// RTICTL = 0x1B ʱ ����ԼΪ1ms
void TestRTI(void) {
    StartTimeBase();
    PORTB = 0xAA;
    DDRB = 0xFF;
    for (;;) {

        Wait(1);
        PORTB = ~PORTB;
    }
}

void Test_WaitError(void) {
    StartTimeBase();
    StartTimeBase();

    DDRB = 0xff;
    for (;;) {
        Wait(1000);
        PORTB = 0xAA;

        Wait(1000);
        PORTB = 0x55;

    }

}

