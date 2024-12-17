/******************************************************************************
* File Name:   main.c
*
* Description: This is the source code for Class-B safety Test for DAC
*              IP code example for ModusToolbox.
*
* Related Document: See README.md
*
*
*******************************************************************************
* Copyright 2021-2024, Cypress Semiconductor Corporation (an Infineon company) or
* an affiliate of Cypress Semiconductor Corporation.  All rights reserved.
*
* This software, including source code, documentation and related
* materials ("Software") is owned by Cypress Semiconductor Corporation
* or one of its affiliates ("Cypress") and is protected by and subject to
* worldwide patent protection (United States and foreign),
* United States copyright laws and international treaty provisions.
* Therefore, you may use this Software only as provided in the license
* agreement accompanying the software package from which you
* obtained this Software ("EULA").
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
* non-transferable license to copy, modify, and compile the Software
* source code solely for use in connection with Cypress's
* integrated circuit products.  Any reproduction, modification, translation,
* compilation, or representation of this Software except as specified
* above is prohibited without the express written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
* reserves the right to make changes to the Software without notice. Cypress
* does not assume any liability arising out of the application or use of the
* Software or any product or circuit described in the Software. Cypress does
* not authorize its products for use in any products where a malfunction or
* failure of the Cypress product may reasonably be expected to result in
* significant property damage, injury or death ("High Risk Product"). By
* including Cypress's product in a High Risk Product, the manufacturer
* of such system or application assumes all risk of such use and in doing
* so agrees to indemnify Cypress against all liability.
*******************************************************************************/

#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "cy_retarget_io.h"
#include "SelfTest.h"

/*******************************************************************************
* Macros
********************************************************************************/
#define DAC_TEST_DELAY     (500u)

/*******************************************************************************
* Global Variables
********************************************************************************/


/*******************************************************************************
* Function Prototypes
********************************************************************************/


/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
* This is the main function for CM4 CPU. It does Class-B safety Test for DAC IP.
*
*
* Parameters:
*  void
*
* Return:
*  int
*
*******************************************************************************/
int main(void)
{
    cy_rslt_t result;
    cy_en_ctdac_status_t status;

    /* Initialize the device and board peripherals */
    result = cybsp_init() ;
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    /* Enable global interrupts */
    __enable_irq();

    /* Initialize retarget-io to use the debug UART port */
    cy_retarget_io_init(CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX, CY_RETARGET_IO_BAUDRATE);

    /* Init AREF */
    result = Cy_SysAnalog_Init(&self_test_aref_0_config);
    if (CY_SAR_SUCCESS != result)
    {
        CY_ASSERT(0);
    }
    Cy_SysAnalog_Enable();

    /* Initialize SAR ADC */
    result = Cy_SAR_Init(CYBSP_DUT_SAR_ADC_HW, &CYBSP_DUT_SAR_ADC_config);
    if (CY_SAR_SUCCESS != result)
    {
        CY_ASSERT(0);
    }

    /*Initialize VDAC block*/
    Cy_SAR_Enable(CYBSP_DUT_SAR_ADC_HW);

    status = Cy_CTDAC_Init(VDAC_HW, &VDAC_config);
    if (CY_CTDAC_SUCCESS == status)
    {
        /* Turn on the VDAC hardware block. */
        Cy_CTDAC_Enable(VDAC_HW);
    }

    /* \x1b[2J\x1b[;H - ANSI ESC sequence for clear screen */
    printf("\x1b[2J\x1b[;H");

    printf("****************** "
           "Class-B: DAC IP Safety Test "
           "****************** \r\n\n");

    for (;;)
    {

        if (OK_STATUS != SelfTests_DAC(VDAC_HW, CYBSP_DUT_SAR_ADC_HW, 0))
        {
            /* Process error */
            printf("ERROR: DAC SelfTest failed.\r\n");
        }
        else
        {
            printf("SUCCESS: DAC SelfTest passed.\r\n");
        }


        /* Delay between DAC Selftests */
        cyhal_system_delay_ms(DAC_TEST_DELAY);
    }
}


/* [] END OF FILE */