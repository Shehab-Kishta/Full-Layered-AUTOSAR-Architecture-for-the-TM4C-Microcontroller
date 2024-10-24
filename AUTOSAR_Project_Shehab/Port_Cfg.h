/******************************************************************************
 *
 * Module: PORT
 *
 * File Name: PORT_Cfg.h
 *
 * Description: Pre-Compile Configuration Header file for TM4C123GH6PM Microcontroller - PORT Driver
 *
 * Author: Shehab Kishta
 ******************************************************************************/

#ifndef PORT_CFG_H
#define PORT_CFG_H

/*
 * Module Version 1.0.0
 */
#define PORT_CFG_SW_MAJOR_VERSION               (1U)
#define PORT_CFG_SW_MINOR_VERSION               (0U)
#define PORT_CFG_SW_PATCH_VERSION               (0U)

/*
 * AUTOSAR Version 4.0.3
 */
#define PORT_CFG_AR_RELEASE_MAJOR_VERSION       (4U)
#define PORT_CFG_AR_RELEASE_MINOR_VERSION       (0U)
#define PORT_CFG_AR_RELEASE_PATCH_VERSION       (3U)

/* Pre-compile option for Switches the Development Error Detection and Notification on or off */
#define PORT_DEV_ERROR_DETECT                   (STD_ON)

/* Pre-processor switch to enable / disable the use of the function Port_SetPinDirection() */
#define PORT_SET_PIN_DIRECTION_API              (STD_ON)

/* Pre-processor switch to enable / disable the use of the function Port_SetPinMode() */
#define PORT_SET_PIN_MODE_API                   (STD_ON)
   
/* Pre-processor switch to enable / disable the API to read out the modules version information */
#define PORT_VERSION_INFO_API                   (STD_ON)
   
/* The initial direction of the pin (IN or OUT) */
#define PORT_PIN_DIRECTION                      (PORT_PIN_IN)
   
#define PORT_PIN_DIRECTION_LED                  (PORT_PIN_OUT)
   
/* Parameter to indicate if the direction is changeable on a port pin during runtime */
#define PORT_PIN_DIRECTION_CHANGEABLE_DEFAULT   (STD_ON)

/* Port pin mode from mode list for use with Port_Init() function */
#define PORT_PIN_INITIAL_MODE                   (0U)
   
/* Port Pin Level value from Port pin list */
#define PORT_PIN_INIT_VALUE                     (PORT_PIN_LEVEL_LOW)

/* Parameter to indicate if the mode is changeable on a port pin during runtime */
#define PORT_PIN_MODE_CHANGEABLE_DEFAULT        (STD_ON)

/* Enable or disable the analog */
#define DEFAULT_PORT_ANALOG_ENABLE              (STD_OFF)
   
/* Enable or disable the digital */
#define DEFAULT_PORT_DIGITAL_ENABLE             (STD_ON)
   
/* Enable or disable the pull up */
#define DEFAULT_PORT_PULL_UP_ENABLE             (STD_OFF)

/* Enable or disable the pull down */
#define DEFAULT_PORT_PULL_DOWN_ENABLE           (STD_OFF)
   
/* Defines port A */   
#define PORTA_NUM                               (0U)

/* Defines port B */   
#define PORTB_NUM                               (1U)

/* Defines port C */  
#define PORTC_NUM                               (2U)

/* Defines port D */  
#define PORTD_NUM                               (3U)
 
/* Defines port E */  
#define PORTE_NUM                               (4U)
   
/* Defines port F */  
#define PORTF_NUM                               (5U)

/* Defines the pins */
#define PIN0_NUM                               (0U)
#define PIN1_NUM                               (1U)
#define PIN2_NUM                               (2U)
#define PIN3_NUM                               (3U)
#define PIN4_NUM                               (4U)
#define PIN5_NUM                               (5U)
#define PIN6_NUM                               (6U)
#define PIN7_NUM                               (7U)

#define PORT_CONFIGURED_CHANNELS               (43U)
   
#define PORT_MODES_NUM                         (14U)
#endif