#ifndef _TMP_CONTROL_H
#define _TMP_CONTROL_H

void TMP_StartControl();
void TMP_StopControl();
void TMP_ControlOutput();

extern double TMP_Set;
extern double TMP_Now;
extern uint8_t TMP_StartFlg;
extern uint8_t TMP_ShowFlg;
extern uint8_t TMP_ControlCnt;
extern uint32_t ADCTmpData;
#endif