// Test PA;
#include "includes.h"

INT8U look1 = 0, look2 = 0;



void Test_PAI_FUNC() {
    // ���õ�ֵ����PACN2 & PACN3 ��
    PORTB = PACN2;
}

void Test_PAOVI_FUNC() {
    PORTB = 0xAA;

}

// ����pa
void TestPA(void) {
    StartTimeBase();

    DDRB = 0xFF;
    PORTB = 0xA5;

    PAI_FUNC = Test_PAI_FUNC;
    //PAOVI_FUNC = Test_PAOVI_FUNC;

    InitPA();

    ClearPA();

    StartPA();


}
