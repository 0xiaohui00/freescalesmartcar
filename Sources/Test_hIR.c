// Test IR
#include "includes.h"


// ��������Ϊ2�ķ���
void TestIR() {
    StartTimeBase();
//	DDRB = 0xFF;
//	PORTB = 0;
    DDRP_DDRP1 = 1;
    for (;;) {
        Wait(1000);
        PTP_PTP1 = 1;
        Wait(1000);
        PTP_PTP1 = 0;
    }
}
