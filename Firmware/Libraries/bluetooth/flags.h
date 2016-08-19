/* 
 * File:   flags.h
 * Author: abartels
 *
 * Created on April 19, 2016, 9:17 PM
 */

#ifndef FLAGS_H
#define	FLAGS_H

typedef enum{
    BT_BUTTON,
    BT_SPEED,
    BT_BATTERY,
    //DASH
    BT_TRIP = 0x10,
    BT_ODOMETER,
    BT_MODEBTN,
    BT_SELBTN,
    BT_IGNITON,
    BT_KILLSWITCH,
    BT_THROTTLE,
    //Lights + Horn
    BT_RUNNINGLIGHTS,
    BT_BRAKELIGHTS,
    BT_HIGHLIGHTS,
    BT_HORN,
    //Motor
    BT_POWER = 0x20,
    BT_MOTORVOLTAGE,
    BT_MOTORCURRENT,
    //Drive modes
    BT_SPORTMODE,
    BT_ECONMODE,
    BT_DEMO1MODE,
    BT_DEMO2MODE,            
      //Battery
    BT_BATTERYVOLTAGE = 0x30,
    BT_BATTERYPERCENT,
    //Battery 1
    BT_BATT1CELL1,
    BT_BATT1CELL2,
    BT_BATT1CELL3,
    BT_BATT1CELL4,
    BT_BATT1CELL5,
    BT_BATT1TEMP1,
    BT_BATT1TEMP2,
    //Battery 2    
    BT_BATT2CELL1,
    BT_BATT2CELL2,
    BT_BATT2CELL3,
    BT_BATT2CELL4,
    BT_BATT2CELL5,
    BT_BATT2TEMP1,
    BT_BATT2TEMP2,
      //State machine flags start at 0x50
    //CANerrors start at 0x70
}BTflag;

#endif	/* FLAGS_H */

