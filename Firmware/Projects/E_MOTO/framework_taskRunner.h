/* 
 * File:   framework_taskRunner.h
 * Author: Zachary S. Levenberg
 * Created on November 3, 2016, 9:22 AM
 */

#ifndef FRAMEWORK_TASKRUNNER_H
#define	FRAMEWORK_TASKRUNNER_H

/*******************************************************************************
 * Include
 * ****************************************************************************/
#include "framework_configure.h"

/*******************************************************************************
 * Defines and Datatypes
 * ****************************************************************************/

/*******************************************************************************
 * Function Prototypes
 * ****************************************************************************/

/**
 * Function call handle for Task Service, Takes and Empty Event and returns the same empty event
 * @param emptyEvent: Any Event, has no effect
 * @return 
 */
Event FRAMEWORK_TASKRUNNER_run(Event emptyEvent);

#endif	/* FRAMEWORK_TASKRUNNER_H */

