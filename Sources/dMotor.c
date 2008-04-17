// ������

#include "includes.h"

#define FRONT_RUN 1
#define STOP 0
#define BACK_RUN -1

INT8S lastMotorState = STOP;

// ����MC33886��ʼ��
void InitMotor(void) {

    InitPWM45();
    InitPWM23();

    DDRM_DDRM4 = 1;
    DDRM_DDRM5 = 1;

    StopPWM23();
    StopPWM45();

    PTM_PTM4 = 0;
    PTM_PTM5 = 1;

}


// ----------------------------------------------------------------------------
// �������ƺ���

// ����ǰ��
void FrontRun(INT16U acc) {
    if (lastMotorState != FRONT_RUN) {
        StopPWM23();
        StartPWM45();
    }
    if (acc > 300) return;
    SetPWM45(300,acc);
    lastMotorState = FRONT_RUN;

}

// �������
void BackRun(INT16U acc) {
    if (lastMotorState != BACK_RUN) {
        StopPWM45();
        StartPWM23();
    }
    SetPWM23(300,acc);
    lastMotorState = BACK_RUN;
}

void StopRun(void) {
    StopPWM23();
    StopPWM45();
    lastMotorState = STOP;
}

// ----------------------------------------------------------------------------
// �߼����ƺ���

// ����
// TODO

// ɲ��
// TODO
