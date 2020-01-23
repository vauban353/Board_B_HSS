/*******************************************************************************
 * Copyright 2019 Microchip Corporation.
 *
 * SPDX-License-Identifier: MIT
 * 
 * MPFS HSS Embedded Software
 *
 */

/*!
 * \file DDR Retraining Driver State Machine
 * \brief DDR Retraining
 */

#include "config.h"
#include "hss_types.h"
#include "hss_state_machine.h"
#include "hss_debug.h"

#include "ssmb_ipi.h"


/* Timeouts */
#define DDR_IDLE_PERIODIC_TIMEOUT (ONE_SEC * 60u * 10u)

/* Module Prototypes (states) */
static void ddr_init_handler(struct StateMachine * const pMyMachine);
static void ddr_idle_handler(struct StateMachine * const pMyMachine);
static void ddr_retrain_handler(struct StateMachine * const pMyMachine);

/*!
 * \brief DDR Driver States
 */
enum DDRStatesEnum {
    DDR_INITIALIZATION,
    DDR_IDLE,
    DDR_RETRAIN,
    DDR_NUM_STATES = DDR_IDLE+1
};

/*!
 * \brief DDR Driver State Descriptors
 */
static const struct StateDesc ddr_state_descs[] = {
    { (const stateType_t)DDR_INITIALIZATION, (const char *)"Init",    NULL, NULL, &ddr_init_handler },
    { (const stateType_t)DDR_IDLE,           (const char *)"Idle",    NULL, NULL, &ddr_idle_handler },
    { (const stateType_t)DDR_RETRAIN,        (const char *)"Retrain", NULL, NULL, &ddr_retrain_handler }
};

/*!
 * \brief DDR Driver State Machine
 */
struct StateMachine ddr_service = {
    (stateType_t)DDR_INITIALIZATION, (stateType_t)SM_INVALID_STATE, (const uint32_t)DDR_NUM_STATES, (const char *)"ddr_service", 0u, 0u, 0u, ddr_state_descs, false, 0u, NULL
};

// --------------------------------------------------------------------------------------------------
// Handlers for each state in the state machine
//
static void ddr_init_handler(struct StateMachine * const pMyMachine)
{
    (void) pMyMachine;

    pMyMachine->state = DDR_IDLE;
}

/////////////////

static void ddr_idle_handler(struct StateMachine * const pMyMachine)
{
    (void) pMyMachine;

    if (HSS_Timer_IsElapsed(pMyMachine->startTime, (HSSTicks_t)DDR_IDLE_PERIODIC_TIMEOUT)) {
        pMyMachine->state = DDR_RETRAIN;
    }
}

/////////////////

void HSS_DDR_Train(void); // TODO: clean up
static void ddr_retrain_handler(struct StateMachine * const pMyMachine)
{
    (void) pMyMachine;

    HSS_DDR_Train();  
    pMyMachine->state = DDR_IDLE;
}

