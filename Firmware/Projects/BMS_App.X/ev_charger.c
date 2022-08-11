/******************************************************************************
 * Includes
 *******************************************************************************/
#include "ev_charger.h"
#include "SysTick.h"
#include "IO.h"
#include "bolt_CAN.h"
#include "bms_dbc.h"

#include <stdio.h>
#include <string.h>

/******************************************************************************
 * Constants
 *******************************************************************************/

/******************************************************************************
 * Macros
 *******************************************************************************/

#define EV_CHARGER_STATES(state)\
state(idle) /* init state for startup code */ \
state(negotiating)\
state(charging)\
state(stopping)\

/*Creates an enum of states suffixed with _state*/
#define STATE_FORM(WORD) WORD##_state,
#define FUNCTION_FORM(WORD) static void WORD(EV_CHARGER_entry_types_E entry_type);
#define FUNC_PTR_FORM(WORD) WORD,

#define CHARGER_VOLTAGE_SCALING 10.0
#define CHARGER_CURRENT_SCALING 10.0


/******************************************************************************
 * Configuration
 *******************************************************************************/

/******************************************************************************
 * Typedefs
 *******************************************************************************/
typedef enum {
    EV_CHARGER_STATES(STATE_FORM)
    NUMBER_OF_STATES
} EV_CHARGER_states_E;

typedef enum {
    ENTRY,
    EXIT,
    RUN
} EV_CHARGER_entry_types_E;

typedef void(*statePtr)(EV_CHARGER_entry_types_E);

/******************************************************************************
 * Variable Declarations
 *******************************************************************************/
/*State variables*/
/* function-ifies the state list*/
EV_CHARGER_STATES(FUNCTION_FORM)
static statePtr state_functions[] = {EV_CHARGER_STATES(FUNC_PTR_FORM)};

static EV_CHARGER_states_E prevState = 0; /* initialize previous state */
static EV_CHARGER_states_E curState = 0; /* initialize current state */
static EV_CHARGER_states_E nextState = idle_state; /* initialize current state */

SysTick_Timer_new(EV_charger_timer); /*Timer for state machine*/

static uint8_t chargeRequestFromMCU = 0;
static uint8_t chargeRequestFromBMS = 0;
static uint16_t chargeVoltage = 24 * 4.2 * CHARGER_VOLTAGE_SCALING; //24 cells, 4.2v each, 0.1V/bit
static uint16_t chargeCurrent = 5 * CHARGER_CURRENT_SCALING;

/******************************************************************************
 * Function Prototypes
 *******************************************************************************/
uint8_t checkHeartBeat(void);
uint8_t checkChargerErrors(void);

/******************************************************************************
 * Function Definitions
 *******************************************************************************/
void EV_Charger_Init(void) {


}

void EV_CHARGER_Run_10ms(void) {

    chargeRequestFromMCU = CAN_mcu_command_ev_charger_enable_get() && checkHeartBeat();

    /* This only happens during state transition
     * State transitions thus have priority over posting new events
     * State transitions always consist of an exit event to curState and entry event to nextState */
    if (nextState != curState) {
        state_functions[curState](EXIT);
        prevState = curState;
        curState = nextState;
        state_functions[curState](ENTRY);
    } else {
        state_functions[curState](RUN);
    }
}

void idle(EV_CHARGER_entry_types_E entry_type) {
    switch (entry_type) {
        case ENTRY:
            chargeRequestFromBMS = 0;
            IO_SET_EV_CHARGER_EN(LOW);
            break;
        case RUN:
            if (chargeRequestFromMCU) {
                nextState = negotiating_state;
            }

            break;
        case EXIT:
            break;
        default:
            break;
    }

}

void negotiating(EV_CHARGER_entry_types_E entry_type) {
    switch (entry_type) {
        case ENTRY:
            chargeRequestFromBMS = 0;
            IO_SET_EV_CHARGER_EN(HIGH);
            break;
        case RUN:
            if (!chargeRequestFromMCU) {
                nextState = idle_state;
            } else if (!checkChargerErrors()) {
                nextState = charging_state;
            }
            break;
        case EXIT:
            break;
        default:
            break;
    }
}

void charging(EV_CHARGER_entry_types_E entry_type) {
    switch (entry_type) {
        case ENTRY:
            IO_SET_EV_CHARGER_EN(HIGH);
            chargeRequestFromBMS = 1;
            SysTick_TimerStart(1500, EV_charger_timer);
            break;
        case RUN:
            /*Give the charger some time to kick in before flagging errors*/
            if (SysTick_TimeOut(EV_charger_timer)) {
                if (!chargeRequestFromMCU || checkChargerErrors()) {
                    nextState = stopping_state;
                }
            }
            break;
        case EXIT:
            break;
        default:
            break;
    }
}

void stopping(EV_CHARGER_entry_types_E entry_type) {
    switch (entry_type) {
        case ENTRY:
            chargeRequestFromBMS = 0;
            SysTick_TimerStart(1500, EV_charger_timer);
            break;
        case RUN:
            if (SysTick_TimeOut(EV_charger_timer)) {
                IO_SET_EV_CHARGER_EN(LOW);
                nextState = idle_state;
            }
            break;
        case EXIT:
            break;
        default:
            break;
    }
}

void EV_CHARGER_set_charge_voltage(float volts) {
    chargeVoltage = (uint16_t) (volts * CHARGER_VOLTAGE_SCALING);
}

void EV_CHARGER_set_charge_current(float current) {
    chargeCurrent = (uint16_t) (current * CHARGER_CURRENT_SCALING);
}

float EV_CHARGER_get_charge_current() {
    return (float) chargeCurrent / CHARGER_CURRENT_SCALING;
}

float EV_CHARGER_get_charge_voltage() {
    return (float) chargeVoltage / CHARGER_VOLTAGE_SCALING;
}

uint8_t EV_CHARGER_get_bms_request_charge(void) {
    return chargeRequestFromBMS;
}

uint8_t checkHeartBeat(void) {
    static uint8_t lastHeartBeat = 0;
    static uint32_t lastTime = 0;

    /*Check if heartbeat is within interval and updating*/
    uint8_t heartBeat = CAN_mcu_status_heartbeat_get();

    if (heartBeat != lastHeartBeat) {
        lastHeartBeat = heartBeat;
        lastTime = SysTick_Get();
        return 1;
    } else if ((SysTick_Get() - lastTime) < 4 * CAN_mcu_status_interval()) {
        return 1;
    } else {
        return 0;
    }

}

uint8_t checkChargerErrors(void) {
    uint8_t chargerError = 0;
    chargerError |= CAN_charger_status_charger_overtemp_error_get();
    chargerError |= CAN_charger_status_battery_detect_error_get();
    chargerError |= CAN_charger_status_communication_error_get();
    chargerError |= CAN_charger_status_hardware_error_get();
    chargerError |= CAN_charger_status_input_voltage_error_get();
    return chargerError;
}