/*******************************************************************
  Company:
    Microchip Technology Inc.
    Memory System Service SMC Initialization File

  File Name:
    plib_smc.c

  Summary:
    Static Memory Controller (SMC).
    This file contains the source code to initialize the SMC controller

  Description:
    SMC configuration interface
    The SMC System Memory interface provides a simple interface to manage 8-bit and 16-bit
    parallel devices.

  *******************************************************************/

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
#include "plib_smc.h"
#include "device.h"


/* Function:
    void SMC_Initialize( void )

  Summary:
    Initializes hardware and data for the given instance of the SMC module.

  Description:
    This function initializes the SMC timings according to the external parralel device requirements.

  Returns:
  None.
 */

void SMC_Initialize( void )
{


    /* Chip Select CS0 Timings */
    /* Setup SMC SETUP register */
    SMC_REGS->SMC_CS_NUMBER[0].SMC_SETUP= SMC_SETUP_NWE_SETUP(2U) | SMC_SETUP_NCS_WR_SETUP(1U) | SMC_SETUP_NRD_SETUP(3U) | SMC_SETUP_NCS_RD_SETUP(1U);

    /* Setup SMC CYCLE register */
    SMC_REGS->SMC_CS_NUMBER[0].SMC_CYCLE= SMC_CYCLE_NWE_CYCLE(10U) | SMC_CYCLE_NRD_CYCLE(8U);

    /* Setup SMC_PULSE register */
    SMC_REGS->SMC_CS_NUMBER[0].SMC_PULSE= SMC_PULSE_NWE_PULSE(6U) | SMC_PULSE_NCS_WR_PULSE(8U) | SMC_PULSE_NRD_PULSE(3U) | SMC_PULSE_NCS_RD_PULSE(6U);

    /* Setup SMC MODE register */
    SMC_REGS->SMC_CS_NUMBER[0].SMC_MODE= SMC_MODE_EXNW_MODE_DISABLED | SMC_MODE_PMEN_Msk | SMC_MODE_PS_4_BYTE   \
                                          | SMC_MODE_WRITE_MODE_Msk | SMC_MODE_READ_MODE_Msk  | SMC_MODE_DBW_8_BIT | SMC_MODE_BAT_BYTE_SELECT;


    /* Chip Select CS1 Timings */
    /* Setup SMC SETUP register */
    SMC_REGS->SMC_CS_NUMBER[1].SMC_SETUP= SMC_SETUP_NWE_SETUP(2U) | SMC_SETUP_NCS_WR_SETUP(2U) | SMC_SETUP_NRD_SETUP(2U) | SMC_SETUP_NCS_RD_SETUP(2U);

    /* Setup SMC CYCLE register */
    SMC_REGS->SMC_CS_NUMBER[1].SMC_CYCLE= SMC_CYCLE_NWE_CYCLE(17U) | SMC_CYCLE_NRD_CYCLE(17U);

    /* Setup SMC_PULSE register */
    SMC_REGS->SMC_CS_NUMBER[1].SMC_PULSE= SMC_PULSE_NWE_PULSE(9U) | SMC_PULSE_NCS_WR_PULSE(9U) | SMC_PULSE_NRD_PULSE(9U) | SMC_PULSE_NCS_RD_PULSE(9U);

    /* Setup SMC MODE register */
    SMC_REGS->SMC_CS_NUMBER[1].SMC_MODE= SMC_MODE_EXNW_MODE_DISABLED    \
                                          | SMC_MODE_WRITE_MODE_Msk | SMC_MODE_READ_MODE_Msk  | SMC_MODE_DBW_8_BIT | SMC_MODE_BAT_BYTE_SELECT;



    /* Chip Select CS3 Timings */
    /* Setup SMC SETUP register */
    SMC_REGS->SMC_CS_NUMBER[3].SMC_SETUP= SMC_SETUP_NWE_SETUP(2U) | SMC_SETUP_NCS_WR_SETUP(2U) | SMC_SETUP_NRD_SETUP(2U) | SMC_SETUP_NCS_RD_SETUP(2U);

    /* Setup SMC CYCLE register */
    SMC_REGS->SMC_CS_NUMBER[3].SMC_CYCLE= SMC_CYCLE_NWE_CYCLE(17U) | SMC_CYCLE_NRD_CYCLE(17U);

    /* Setup SMC_PULSE register */
    SMC_REGS->SMC_CS_NUMBER[3].SMC_PULSE= SMC_PULSE_NWE_PULSE(9U) | SMC_PULSE_NCS_WR_PULSE(9U) | SMC_PULSE_NRD_PULSE(9U) | SMC_PULSE_NCS_RD_PULSE(9U);

    /* Setup SMC MODE register */
    SMC_REGS->SMC_CS_NUMBER[3].SMC_MODE= SMC_MODE_EXNW_MODE_DISABLED    \
                                          | SMC_MODE_WRITE_MODE_Msk | SMC_MODE_READ_MODE_Msk  | SMC_MODE_DBW_8_BIT | SMC_MODE_BAT_BYTE_SELECT;


} /* SMC_Initialize */

/*******************************************************************************
 End of File
*/
