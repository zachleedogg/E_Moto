#ifndef mcu_DBC_H
#define mcu_DBC_H

#include <stdint.h>
#include "bolt_CAN.h"
typedef enum{
    dash,
    mcu,
    bms,
    motorcontroller,
    charger,
    boot_host,
} CAN_nodes_E;

/**********************************************************
 * dash NODE MESSAGES
 */
uint8_t CAN_dash_status_checkDataIsFresh(void);
uint16_t CAN_dash_status_heartBeat_get(void);
uint16_t CAN_dash_status_state_get(void);
uint16_t CAN_dash_status_killButton_get(void);
uint16_t CAN_dash_status_ignButton_get(void);
uint16_t CAN_dash_status_modeButton_get(void);
uint16_t CAN_dash_status_selectButton_get(void);
uint16_t CAN_dash_status_driveMode_get(void);

uint8_t CAN_dash_command_checkDataIsFresh(void);
uint16_t CAN_dash_command_ignitionRequest_get(void);
uint16_t CAN_dash_command_killRequest_get(void);
uint16_t CAN_dash_command_batteryEjectRequest_get(void);
uint16_t CAN_dash_command_lightsRequest_get(void);
uint16_t CAN_dash_command_hornRequest_get(void);

uint8_t CAN_dash_data1_checkDataIsFresh(void);
uint16_t CAN_dash_data1_speed_get(void);
uint16_t CAN_dash_data1_odometer_get(void);
uint16_t CAN_dash_data1_tripA_get(void);
uint16_t CAN_dash_data1_tripB_get(void);

uint8_t CAN_dash_data2_checkDataIsFresh(void);
uint16_t CAN_dash_data2_runningTime_get(void);
uint16_t CAN_dash_data2_odometer_get(void);
uint16_t CAN_dash_data2_tripA_get(void);
uint16_t CAN_dash_data2_tripB_get(void);

/**********************************************************
 * mcu NODE MESSAGES
 */
void CAN_mcu_status_state_set(uint16_t state);
void CAN_mcu_status_throttleMode_set(uint16_t throttleMode);
void CAN_mcu_status_highBeam_set(uint16_t highBeam);
void CAN_mcu_status_lowBeam_set(uint16_t lowBeam);
void CAN_mcu_status_brakeLight_set(uint16_t brakeLight);
void CAN_mcu_status_tailLight_set(uint16_t tailLight);
void CAN_mcu_status_horn_set(uint16_t horn);
void CAN_mcu_status_turnSignalFR_set(uint16_t turnSignalFR);
void CAN_mcu_status_turnSignalFL_set(uint16_t turnSignalFL);
void CAN_mcu_status_turnSignalRR_set(uint16_t turnSignalRR);
void CAN_mcu_status_turnSignalRL_set(uint16_t turnSignalRL);
void CAN_mcu_status_GBSALL_set(uint16_t GBSALL);
void CAN_mcu_status_contactor_set(uint16_t contactor);
void CAN_mcu_status_chargePort_set(uint16_t chargePort);
void CAN_mcu_status_brakeSwitchFront_set(uint16_t brakeSwitchFront);
void CAN_mcu_status_brakeSwitchRear_set(uint16_t brakeSwitchRear);
void CAN_mcu_status_throttleVal_set(uint16_t throttleVal);
void CAN_mcu_status_send(void);


void CAN_mcu_command_DCDC_enable_set(uint16_t DCDC_enable);
void CAN_mcu_command_ev_charger_enable_set(uint16_t ev_charger_enable);
void CAN_mcu_command_motor_controller_enable_set(uint16_t motor_controller_enable);
void CAN_mcu_command_send(void);


void CAN_mcu_motorControllerRequest_requestType_set(uint16_t requestType);
void CAN_mcu_motorControllerRequest_send(void);


/**********************************************************
 * bms NODE MESSAGES
 */
uint8_t CAN_bms_status_checkDataIsFresh(void);
uint16_t CAN_bms_status_state_get(void);
uint16_t CAN_bms_status_SOC_get(void);
uint16_t CAN_bms_status_packVoltage_get(void);
uint16_t CAN_bms_status_packCurrent_get(void);
uint16_t CAN_bms_status_minTemp_get(void);
uint16_t CAN_bms_status_maxTemp_get(void);

uint8_t CAN_bms_status_2_checkDataIsFresh(void);
uint16_t CAN_bms_status_2_DCDC_state_get(void);
uint16_t CAN_bms_status_2_DCDC_fault_get(void);
uint16_t CAN_bms_status_2_DCDC_voltage_get(void);
uint16_t CAN_bms_status_2_DCDC_current_get(void);
uint16_t CAN_bms_status_2_EV_charger_state_get(void);
uint16_t CAN_bms_status_2_EV_charger_fault_get(void);
uint16_t CAN_bms_status_2_EV_charger_voltage_get(void);
uint16_t CAN_bms_status_2_EV_charger_current_get(void);
uint16_t CAN_bms_status_2_HV_precharge_state_get(void);
uint16_t CAN_bms_status_2_HV_contactor_state_get(void);
uint16_t CAN_bms_status_2_HV_bus_voltage_get(void);

/**********************************************************
 * motorcontroller NODE MESSAGES
 */
uint8_t CAN_motorcontroller_motorStatus_checkDataIsFresh(void);
uint16_t CAN_motorcontroller_motorStatus_motorSpeed_get(void);
uint16_t CAN_motorcontroller_motorStatus_motorCurrent_get(void);
uint16_t CAN_motorcontroller_motorStatus_IphaseA_get(void);
uint16_t CAN_motorcontroller_motorStatus_IphaseB_get(void);
uint16_t CAN_motorcontroller_motorStatus_IphaseC_get(void);
uint16_t CAN_motorcontroller_motorStatus_VphaseA_get(void);
uint16_t CAN_motorcontroller_motorStatus_VphaseB_get(void);
uint16_t CAN_motorcontroller_motorStatus_VphaseC_get(void);

uint8_t CAN_motorcontroller_response_checkDataIsFresh(void);
uint16_t CAN_motorcontroller_response_byte1_get(void);
uint16_t CAN_motorcontroller_response_byte2_get(void);
uint16_t CAN_motorcontroller_response_byte3_get(void);
uint16_t CAN_motorcontroller_response_byte4_get(void);
uint16_t CAN_motorcontroller_response_byte5_get(void);
uint16_t CAN_motorcontroller_response_byte6_get(void);
uint16_t CAN_motorcontroller_response_byte7_get(void);
uint16_t CAN_motorcontroller_response_byte8_get(void);

/**********************************************************
 * charger NODE MESSAGES
 */
/**********************************************************
 * boot_host NODE MESSAGES
 */
void CAN_DBC_init();

void CAN_send_10ms(void);
void CAN_send_1ms(void);


#endif /*mcu_DBC_H*/
