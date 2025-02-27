/*******************************************************************************
  System Initialization File

  File Name:
    initialization.c

  Summary:
    This file contains source code necessary to initialize the system.

  Description:
    This file contains source code necessary to initialize the system.  It
    implements the "SYS_Initialize" function, defines the configuration bits,
    and allocates any necessary global system resources,
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *******************************************************************************/
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include "configuration.h"
#include "definitions.h"
#include "device.h"



// ****************************************************************************
// ****************************************************************************
// Section: Configuration Bits
// ****************************************************************************
// ****************************************************************************
#pragma config TCM_CONFIGURATION = 0
#pragma config SECURITY_BIT = CLEAR
#pragma config BOOT_MODE = SET




// *****************************************************************************
// *****************************************************************************
// Section: Driver Initialization Data
// *****************************************************************************
// *****************************************************************************
// <editor-fold defaultstate="collapsed" desc="DRV_I2C Instance 0 Initialization Data">

/* I2C Client Objects Pool */
static DRV_I2C_CLIENT_OBJ drvI2C0ClientObjPool[DRV_I2C_CLIENTS_NUMBER_IDX0];

/* I2C PLib Interface Initialization */
const DRV_I2C_PLIB_INTERFACE drvI2C0PLibAPI = {

    /* I2C PLib Transfer Read Add function */
    .read = (DRV_I2C_PLIB_READ)TWIHS2_Read,

    /* I2C PLib Transfer Write Add function */
    .write = (DRV_I2C_PLIB_WRITE)TWIHS2_Write,


    /* I2C PLib Transfer Write Read Add function */
    .writeRead = (DRV_I2C_PLIB_WRITE_READ)TWIHS2_WriteRead,

    /*I2C PLib Transfer Abort function */
    .transferAbort = (DRV_I2C_PLIB_TRANSFER_ABORT)TWIHS2_TransferAbort,

    /* I2C PLib Transfer Status function */
    .errorGet = (DRV_I2C_PLIB_ERROR_GET)TWIHS2_ErrorGet,

    /* I2C PLib Transfer Setup function */
    .transferSetup = (DRV_I2C_PLIB_TRANSFER_SETUP)TWIHS2_TransferSetup,

    /* I2C PLib Callback Register */
    .callbackRegister = (DRV_I2C_PLIB_CALLBACK_REGISTER)TWIHS2_CallbackRegister,
};


/* I2C Driver Initialization Data */
const DRV_I2C_INIT drvI2C0InitData =
{
    /* I2C PLib API */
    .i2cPlib = &drvI2C0PLibAPI,

    /* I2C Number of clients */
    .numClients = DRV_I2C_CLIENTS_NUMBER_IDX0,

    /* I2C Client Objects Pool */
    .clientObjPool = (uintptr_t)&drvI2C0ClientObjPool[0],

    /* I2C Clock Speed */
    .clockSpeed = DRV_I2C_CLOCK_SPEED_IDX0,
};

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="DRV_I2C Instance 1 Initialization Data">

/* I2C Client Objects Pool */
static DRV_I2C_CLIENT_OBJ drvI2C1ClientObjPool[DRV_I2C_CLIENTS_NUMBER_IDX1];

/* I2C PLib Interface Initialization */
const DRV_I2C_PLIB_INTERFACE drvI2C1PLibAPI = {

    /* I2C PLib Transfer Read Add function */
    .read = (DRV_I2C_PLIB_READ)TWIHS1_Read,

    /* I2C PLib Transfer Write Add function */
    .write = (DRV_I2C_PLIB_WRITE)TWIHS1_Write,


    /* I2C PLib Transfer Write Read Add function */
    .writeRead = (DRV_I2C_PLIB_WRITE_READ)TWIHS1_WriteRead,

    /*I2C PLib Transfer Abort function */
    .transferAbort = (DRV_I2C_PLIB_TRANSFER_ABORT)TWIHS1_TransferAbort,

    /* I2C PLib Transfer Status function */
    .errorGet = (DRV_I2C_PLIB_ERROR_GET)TWIHS1_ErrorGet,

    /* I2C PLib Transfer Setup function */
    .transferSetup = (DRV_I2C_PLIB_TRANSFER_SETUP)TWIHS1_TransferSetup,

    /* I2C PLib Callback Register */
    .callbackRegister = (DRV_I2C_PLIB_CALLBACK_REGISTER)TWIHS1_CallbackRegister,
};


/* I2C Driver Initialization Data */
const DRV_I2C_INIT drvI2C1InitData =
{
    /* I2C PLib API */
    .i2cPlib = &drvI2C1PLibAPI,

    /* I2C Number of clients */
    .numClients = DRV_I2C_CLIENTS_NUMBER_IDX1,

    /* I2C Client Objects Pool */
    .clientObjPool = (uintptr_t)&drvI2C1ClientObjPool[0],

    /* I2C Clock Speed */
    .clockSpeed = DRV_I2C_CLOCK_SPEED_IDX1,
};

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="DRV_I2C Instance 2 Initialization Data">

/* I2C Client Objects Pool */
static DRV_I2C_CLIENT_OBJ drvI2C2ClientObjPool[DRV_I2C_CLIENTS_NUMBER_IDX2];

/* I2C PLib Interface Initialization */
const DRV_I2C_PLIB_INTERFACE drvI2C2PLibAPI = {

    /* I2C PLib Transfer Read Add function */
    .read = (DRV_I2C_PLIB_READ)TWIHS0_Read,

    /* I2C PLib Transfer Write Add function */
    .write = (DRV_I2C_PLIB_WRITE)TWIHS0_Write,


    /* I2C PLib Transfer Write Read Add function */
    .writeRead = (DRV_I2C_PLIB_WRITE_READ)TWIHS0_WriteRead,

    /*I2C PLib Transfer Abort function */
    .transferAbort = (DRV_I2C_PLIB_TRANSFER_ABORT)TWIHS0_TransferAbort,

    /* I2C PLib Transfer Status function */
    .errorGet = (DRV_I2C_PLIB_ERROR_GET)TWIHS0_ErrorGet,

    /* I2C PLib Transfer Setup function */
    .transferSetup = (DRV_I2C_PLIB_TRANSFER_SETUP)TWIHS0_TransferSetup,

    /* I2C PLib Callback Register */
    .callbackRegister = (DRV_I2C_PLIB_CALLBACK_REGISTER)TWIHS0_CallbackRegister,
};


/* I2C Driver Initialization Data */
const DRV_I2C_INIT drvI2C2InitData =
{
    /* I2C PLib API */
    .i2cPlib = &drvI2C2PLibAPI,

    /* I2C Number of clients */
    .numClients = DRV_I2C_CLIENTS_NUMBER_IDX2,

    /* I2C Client Objects Pool */
    .clientObjPool = (uintptr_t)&drvI2C2ClientObjPool[0],

    /* I2C Clock Speed */
    .clockSpeed = DRV_I2C_CLOCK_SPEED_IDX2,
};

// </editor-fold>



// *****************************************************************************
// *****************************************************************************
// Section: System Data
// *****************************************************************************
// *****************************************************************************
/* Structure to hold the object handles for the modules in the system. */
SYSTEM_OBJECTS sysObj;

// *****************************************************************************
// *****************************************************************************
// Section: Library/Stack Initialization Data
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
// *****************************************************************************
// Section: System Initialization
// *****************************************************************************
// *****************************************************************************
// <editor-fold defaultstate="collapsed" desc="SYS_TIME Initialization Data">

const SYS_TIME_PLIB_INTERFACE sysTimePlibAPI = {
    .timerCallbackSet = (SYS_TIME_PLIB_CALLBACK_REGISTER)TC0_CH0_TimerCallbackRegister,
    .timerStart = (SYS_TIME_PLIB_START)TC0_CH0_TimerStart,
    .timerStop = (SYS_TIME_PLIB_STOP)TC0_CH0_TimerStop ,
    .timerFrequencyGet = (SYS_TIME_PLIB_FREQUENCY_GET)TC0_CH0_TimerFrequencyGet,
    .timerPeriodSet = (SYS_TIME_PLIB_PERIOD_SET)TC0_CH0_TimerPeriodSet,
};

const SYS_TIME_INIT sysTimeInitData =
{
    .timePlib = &sysTimePlibAPI,
    .hwTimerIntNum = TC0_CH0_IRQn,
};

// </editor-fold>



// *****************************************************************************
// *****************************************************************************
// Section: Local initialization functions
// *****************************************************************************
// *****************************************************************************



/*******************************************************************************
  Function:
    void SYS_Initialize ( void *data )

  Summary:
    Initializes the board, services, drivers, application and other modules.

  Remarks:
 */

void SYS_Initialize ( void* data )
{
    /* MISRAC 2012 deviation block start */
    /* MISRA C-2012 Rule 2.2 deviated in this file.  Deviation record ID -  H3_MISRAC_2012_R_2_2_DR_1 */


    EFC_Initialize();
  
    CLOCK_Initialize();
	PIO_Initialize();



  

 
    TC0_CH0_TimerInitialize(); 
    TC0_CH1_TimerInitialize(); 
    TC0_CH2_TimerInitialize();
	RTC_Initialize();

	UART2_Initialize();

	UART0_Initialize();

    MCAN0_Initialize();

	TWIHS0_Initialize();

    MCAN1_Initialize();

	TWIHS1_Initialize();

	TWIHS2_Initialize();

    USART1_Initialize();

    XDMAC_Initialize();

	RSWDT_REGS->RSWDT_MR = RSWDT_MR_WDDIS_Msk;	// Disable RSWDT 

	WDT_Initialize();


    PWM0_Initialize();

	RSTC_Initialize();

    AFEC0_Initialize();

    SMC_Initialize();


    /* Initialize I2C0 Driver Instance */
    sysObj.drvI2C0 = DRV_I2C_Initialize(DRV_I2C_INDEX_0, (SYS_MODULE_INIT *)&drvI2C0InitData);
    /* Initialize I2C1 Driver Instance */
    sysObj.drvI2C1 = DRV_I2C_Initialize(DRV_I2C_INDEX_1, (SYS_MODULE_INIT *)&drvI2C1InitData);
    /* Initialize I2C2 Driver Instance */
    sysObj.drvI2C2 = DRV_I2C_Initialize(DRV_I2C_INDEX_2, (SYS_MODULE_INIT *)&drvI2C2InitData);

    sysObj.sysTime = SYS_TIME_Initialize(SYS_TIME_INDEX_0, (SYS_MODULE_INIT *)&sysTimeInitData);


    APP_Initialize();


    NVIC_Initialize();

    /* MISRAC 2012 deviation block end */
}


/*******************************************************************************
 End of File
*/
