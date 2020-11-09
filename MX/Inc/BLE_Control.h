#ifndef BLE_CONTROL_H
#define BLE_VONTROL_H

#define BLE_ON  1
#define BLE_OFF 0

#define BLE_ATMode      0
#define BLE_NormalMode  1

void BLE_Power(uint8_t Power_CMD);
void BLE_Mode_Select(uint8_t Mode_CMD);

#endif