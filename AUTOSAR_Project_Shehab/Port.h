 /******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port.h
 *
 * Description: Header file for TM4C123GH6PM Microcontroller - Port Driver.
 *
 * Author: Shehab Kishta
 ******************************************************************************/

#ifndef PORT_H
#define PORT_H

/* Id for the company in the AUTOSAR
 * for example Mohamed Tarek's ID = 1000 :) */
#define PORT_VENDOR_ID    (1000U)

/* Port Module Id */
#define PORT_MODULE_ID    (124U)

/* Port Instance Id */
#define PORT_INSTANCE_ID  (0U)

/*
 * Module Version 1.0.0
 */
#define PORT_SW_MAJOR_VERSION           (1U)
#define PORT_SW_MINOR_VERSION           (0U)
#define PORT_SW_PATCH_VERSION           (0U)

/*
 * AUTOSAR Version 4.0.3
 */
#define PORT_AR_RELEASE_MAJOR_VERSION   (4U)
#define PORT_AR_RELEASE_MINOR_VERSION   (0U)
#define PORT_AR_RELEASE_PATCH_VERSION   (3U)

/*
 * Macros for Port Status
 */
#define PORT_INITIALIZED                (1U)
#define PORT_NOT_INITIALIZED            (0U)

/* Standard AUTOSAR types */
#include "Std_Types.h"

/* AUTOSAR checking between Std Types and Port Modules */
#if ((STD_TYPES_AR_RELEASE_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
 ||  (STD_TYPES_AR_RELEASE_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
 ||  (STD_TYPES_AR_RELEASE_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of Std_Types.h does not match the expected version"
#endif

/* Port Pre-Compile Configuration Header file */
#include "Port_Cfg.h"

/* AUTOSAR Version checking between Port_Cfg.h and Port.h files */
#if ((PORT_CFG_AR_RELEASE_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
 ||  (PORT_CFG_AR_RELEASE_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
 ||  (PORT_CFG_AR_RELEASE_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of Port_Cfg.h does not match the expected version"
#endif

/* Software Version checking between Port_Cfg.h and Port.h files */
#if ((PORT_CFG_SW_MAJOR_VERSION != PORT_SW_MAJOR_VERSION)\
 ||  (PORT_CFG_SW_MINOR_VERSION != PORT_SW_MINOR_VERSION)\
 ||  (PORT_CFG_SW_PATCH_VERSION != PORT_SW_PATCH_VERSION))
  #error "The SW version of Port_Cfg.h does not match the expected version"
#endif

/* Non AUTOSAR files */
#include "Common_Macros.h"
/******************************************************************************
 *                      API Service Id Macros                                 *
 ******************************************************************************/
/* Service ID for Initializes the Port Driver module */
#define PORT_INIT_SID                           (uint8)0x00

/* Service ID for Sets the port pin direction */
#define PORT_SET_PIN_DIRECTION_SID              (uint8)0x01

/* Service ID for Refreshes port direction */
#define PORT_REFRESH_DIRECTION_SID              (uint8)0x02

/* Service ID for Returns the version information of this module */
#define PORT_GET_VERSION_INFO_SID               (uint8)0x03

/* Service ID for Sets the port pin mode */
#define Port_SET_PIN_MODE_SID                   (uint8)0x04
 
/*******************************************************************************
 *                      DET Error Codes                                        *
 *******************************************************************************/
/* DET code to report Invalid Port Pin ID requested */
#define PORT_E_PARAM_PIN                        (uint8)0x0A

/* DET code to report Port Pin not configured as changeable */
#define PORT_E_DIRECTION_UNCHANGEABLE           (uint8)0x0B

/* DET code to report API Port_Init service called with wrong parameter */
#define PORT_E_PARAM_CONFIG                     (uint8)0x0C

/* DET code to report API Port_SetPinMode service called when mode is unchangeable */
#define PORT_E_PARAM_INVALID_MODE               (uint8)0x0D
#define PORT_E_MODE_UNCHANGEABLE                (uint8)0x0E

/* DET code to report API service called without module initialization */
#define PORT_E_UNINIT                           (uint8)0x0F

/* DET code to report APIs called with a Null Pointer */
#define PORT_E_PARAM_POINTER                   (uint8)0x10
   
/*******************************************************************************
 *                              Module Data Types                              *
 *******************************************************************************/
   
/* Type definition for Port_PinType */
typedef uint8 Port_PinType;
   
/* Description: Enum to hold PIN direction */
typedef enum
{
    PORT_PIN_IN, PORT_PIN_OUT
}Port_PinDirectionType;

/* Type definition for Port_PinModeType */
typedef uint8 Port_PinModeType;

/* Description: Enum to hold internal resistor type for PIN */
typedef enum
{
  PORT_PIN_LEVEL_LOW,
  PORT_PIN_LEVEL_HIGH
}PORT_PIN_LEVEL_VALUE;

typedef uint8 Port_Num;
typedef uint8 Pin_Num;

typedef struct 
{
  /* Descripes the port number of the pin */
  Port_Num Port;
  
  /* Descripes the pin number in the port */
  Pin_Num Pin;
  
  /* Pin mode (e.g. DIO, ADC, SPI …) */
  Port_PinModeType Ch_Mode;
  
  /* Pin direction (input, output) */
  Port_PinDirectionType Ch_Direction;
  
  /* Pin level init value */
  PORT_PIN_LEVEL_VALUE Ch_Init_Value;
  
  /* Pin direction changeable during runtime */
  boolean PORT_PIN_DIRECTION_CHANGEABLE;
  
  /* Pin mode changeable during runtime */  
  boolean PORT_PIN_MODE_CHANGEABLE;
  
  /* Boolean to set Analog */        
  boolean PORT_PIN_ANALOG_ENABLE;
  
  /* Boolean to set Digital */        
  boolean PORT_PIN_DIGITAL_ENABLE;
  
  /* Boolean to set the internal pull up */        
  boolean PORT_PIN_INTERNAL_PULL_UP;
  
  /* Boolean to set the internal pull down */          
  boolean PORT_PIN_INTERNAL_PULL_DOWN;
  
}Port_ConfigChannel;

/* Description: Structure to configure */
typedef struct 
{
  Port_ConfigChannel Channels[PORT_CONFIGURED_CHANNELS];
}Port_ConfigType;

/*******************************************************************************
 *                      Function Prototypes                                    *
 *******************************************************************************/
/************************************************************************************
* Service Name: Port_Init
* Sync/Async: Synchronous
* Reentrancy: Non Reentrant
* Parameters (in): ConfigPtr - Pointer to configuration set
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Function to Initializes the Port Driver module
************************************************************************************/
void Port_Init(const Port_ConfigType* ConfigPtr);

/************************************************************************************
* Service Name: Port_SetPinDirection
* Sync/Async: Synchronous
* Reentrancy: Reentrant
* Parameters (in): Pin - Port Pin ID number and Direction - Port Pin Direction
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Function to Set the port pin direction
************************************************************************************/
void Port_SetPinDirection(Port_PinType Pin, Port_PinDirectionType Direction);

/************************************************************************************
* Service Name: Port_RefreshPortDirection
* Sync/Async: Synchronous
* Reentrancy: Non Reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Function to Refreshe port direction
************************************************************************************/
void Port_RefreshPortDirection(void);

/************************************************************************************
* Service Name: Port_GetVersionInfo
* Sync/Async: Synchronous
* Reentrancy: Non Reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): versioninfo - Pointer to where to store the version information of this module
* Return value: None
* Description: Function to Return the version information of this module
************************************************************************************/
void Port_GetVersionInfo(Std_VersionInfoType* versioninfo);

/************************************************************************************
* Service Name: Port_SetupGpioPin
* Sync/Async: Synchronous
* Reentrancy: Reentrant
* Parameters (in): Pin - Port Pin ID number and Mode - New Port Pin mode to be set on port pin
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Function to Sets the port pin mode
************************************************************************************/
void Port_SetPinMode(Port_PinType Pin, Port_PinModeType Mode);

/*******************************************************************************
 *                       External Variables                                    *
 *******************************************************************************/

/* Extern PB structures to be used by Dio and other modules */
extern const Port_ConfigType Port_Configuration;

#endif /* PORT_H */
