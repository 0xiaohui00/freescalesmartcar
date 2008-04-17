// Test Servo.c

#include "includes.h"

void TestServo(void) {
    INT8U i;

    StartTimeBase();

    InitServo();
    SetServo(90);
    StartServo();

    for (i = 0;i < 180;i++) {
        Wait(50);
        SetServo(i);
    }

}

//
// ���Խ��:��90 + 43 ����ƽ�ȵ�ת��50cm�����ʰ뾶 max 58
void TestServo_Angle(void) {
    INT8U i;

    DDRB = 0xFF;
    PORTB = 0xFF;
    InitServo();
    SetServo(90);
    StartServo();

    for (i = 0;i < 90;i++) {
        WaitSmallButtonPress();
        PORTB = ~i;
        SetServo(90 + i);
        Wait(100);
    }
}

// �������ź�ʱ����ķ�Ӧ����

void TestServo_Action(void) {

    unsigned int i, k, j;

    DDRB = 0xFF;
    PORTB = 0xFF;

    DDRP_DDRP1 = 1;

    WaitDisable();
    StartTimeBase();

    InitServo();
    SetServo(120);


    for (i = 0;i < 32000;i++) {
        PORTB = 0xFF;
        StartServo();

        for (k = 0;k < 0xffff;k++) {
            for (j = 0;j < 40;j++);

        }
        PORTB = 0xAA;
        StopServo();
        PTP_PTP1 = 0;

        for (k = 0;k < 0xffff;k++) {
            for (j = 0;j < 40;j++);

        }

    }


}

void TestServo_Turn(void) {
    INT16U i,k,j;

    DDRB = 0xFF;
    PORTB = 0xFF;



    DDRP_DDRP0 = 0;
    PPSP_PPSP0 = 0;
    PERP_PERP0 = 1;


    InitServo();
    SetServo(90);
    StartServo();
    DisableButton();

    WaitDisable();
    StartTimeBase();


    for (i = 0;i < 32000;i++) {
        PORTB = 0xFF;

        SetServo(140);
        for (k = 0;k < 0xffff;k++) {
            for (j = 0;j < 40;j++);

        }

        PORTB = 0xAA;
        SetServo(40);

        if (!PTIP_PTIP0) {
            Wait(20);
            if (!PTIP_PTIP0) {
                DDRP_DDRP1 = 1;
                PTP_PTP1 = 0;
                Wait(2000);
                for (;;) {
                    if (!PTIP_PTIP0) {
                        Wait(20);
                        if (!PTIP_PTIP0) {
                            break;
                        } else {
                            Wait(20);
                        }
                    }
                }

                DDRP_DDRP1 = 0;
                PTP_PTP1 = 0;
            }
        }

        for (k = 0;k < 0xffff;k++) {
            for (j = 0;j < 40;j++);

        }

    }


}


//extern INT16U motorV;
void Test_ServoWheelAngle(void) {
    INT8U tSrvAgl = 90;
    WaitEnable();

    InitMotor();

    InitServo();
    StartServo();

    //FrontRun(motorV);

    DDRP_DDRP0 = 0;
    PPSP_PPSP0 = 0;
    PERP_PERP0 = 1;

    FOREVER() {
        SetServo(tSrvAgl);
        // �ж�С�����Ƿ���, �������˵�
        if (!PTIP_PTIP0) {
            Wait(20);
            if (!PTIP_PTIP0) {
                StopRun();
                StartMenu();
                Test_ShowDistance();
                WaitSmallButtonPress();
                Wait(1500);
                //FrontRun(motorV);
            }
        }
    }
}

// ���Զ���ٶ�
void Test_ServoSpeed(void) {
    InitPWM01();
    StartPWM01();

    WaitEnable();

    for (;;) {
        Wait(2000);
        SetPWM01(60000, 1500);
        Wait(2000);
        SetPWM01(60000, 7500);

    }
}
