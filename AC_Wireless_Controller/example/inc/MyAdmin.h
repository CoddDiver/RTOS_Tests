/*
 * MyAdmin.h
 *
 *  Created on: 31 Jan 2018
 *      Author: codda
 */

#ifndef MYADMIN_H_
#define MYADMIN_H_


// Log structure supports, scope, housekeeping and logic flow
struct MyAdmin {
    int Real_Time_ms;
    bool Print;
    int TICK_RATE_HZ_div;
    float Loop_Time_ms;
    int Master_Counter;
    int Master_Sub_Counter;
    int Soak_Required;
    int Soak_Done;
    int Debug;
} Admin, newadmin;

// MyAdmin Admin;

#endif /* MYADMIN_H_ */
