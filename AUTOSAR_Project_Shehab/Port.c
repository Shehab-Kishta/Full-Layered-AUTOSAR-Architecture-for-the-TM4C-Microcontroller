 /******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port.c
 *
 * Description: Source file for TM4C123GH6PM Microcontroller - Port Driver.
 *
 * Author: Shehab Kishta
 ******************************************************************************/

#include "Port.h"
#include "Port_Regs.h"
#include "Det.h"

STATIC const Port_ConfigChannel * PORT_PortChannels = NULL_PTR;
STATIC uint8 Port_Status = PORT_NOT_INITIALIZED;

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
void Port_Init(const Port_ConfigType* ConfigPtr)
{
#if (PORT_DEV_ERROR_DETECT == STD_ON)
	/* check if the input configuration pointer is not a NULL_PTR */
	if (NULL_PTR == ConfigPtr)
	{
		Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_INIT_SID,
		     PORT_E_PARAM_CONFIG);
	}
	else
#endif
	{
	/*
	* Set the module state to initialized and point to the PB configuration structure using a global pointer.
	* This global pointer is global to be used by other functions to read the PB configuration structures
	*/
            Port_Status       = PORT_INITIALIZED;
            PORT_PortChannels = ConfigPtr->Channels; /* address of the first Channels structure --> Channels[0] */
            uint8 Counter;
            for(Counter = 0; Counter < PORT_CONFIGURED_CHANNELS; Counter++)
            {
                volatile uint32 * PortGpio_Ptr = NULL_PTR; /* point to the required Port Registers base address */
                volatile uint32 delay = 0;
    
                switch(PORT_PortChannels[Counter].Port)
                {
                    case  0: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
                          break;
                    case  1: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
                          break;
                    case  2: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
                          break;
                    case  3: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
                          break;
                    case  4: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
                          break;
                    case  5: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
                          break;
                  }
                  /* Enable clock for PORT and allow time for clock to start*/
                  SYSCTL_REGCGC2_REG |= (1<<PORT_PortChannels[Counter].Pin);
                  delay = SYSCTL_REGCGC2_REG;
                  if(PORT_PortChannels[Counter].Ch_Mode == PORT_PIN_INITIAL_MODE)
                  {
                     CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) ,PORT_PortChannels[Counter].Pin);             /* Disable Alternative function for this pin by clear the corresponding bit */
                     *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (PORT_PortChannels[Counter].Pin * 4));     /* Clear the bits for this pin */
                  }
                  else
                  {
                    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) ,PORT_PortChannels[Counter].Pin);             /* Enable Alternative function for this pin by clear the corresponding bit */
                    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (PORT_PortChannels[Counter].Pin * 4));     /* Clear the bits for this pin */
                    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (PORT_PortChannels[Counter].Ch_Mode << (PORT_PortChannels[Counter].Pin * 4));
                  }
                  if(PORT_PortChannels[Counter].Ch_Direction == PORT_PIN_IN)
                  {
                    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) ,PORT_PortChannels[Counter].Pin);                /* Set the corresponding bit to configure it as output pin */
                    if(PORT_PortChannels[Counter].PORT_PIN_INTERNAL_PULL_UP == STD_ON)
                    {
                      SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_UP_REG_OFFSET) , PORT_PortChannels[Counter].Pin);       /* Set the corresponding bit to enable the internal pull up pin */
                    }
                    else if(PORT_PortChannels[Counter].PORT_PIN_INTERNAL_PULL_DOWN == STD_ON)
                    {
                      SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_DOWN_REG_OFFSET) , PORT_PortChannels[Counter].Pin);     /* Set the corresponding bit to enable the internal pull down pin */
                    }
                    else
                    {
                      CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_UP_REG_OFFSET) ,PORT_PortChannels[Counter].Pin);     /* Clear the corresponding bit to disable the internal pull up pin */
                      CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_DOWN_REG_OFFSET) , PORT_PortChannels[Counter].Pin);   /* Clear the corresponding bit to disable the internal pull down pin */
                    }
                  }
                  else if(PORT_PortChannels[Counter].Ch_Direction == PORT_PIN_OUT)
                  {
                    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) , PORT_PortChannels[Counter].Pin);             /* Clear the corresponding bit to configure it as input pin */
                    if(PORT_PortChannels[Counter].Ch_Init_Value == STD_HIGH)
                    {
                      SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DATA_REG_OFFSET) , PORT_PortChannels[Counter].Pin);          /* Set the corresponding bit to provide initial value 1 */
                    }
                    else
                    {
                      CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DATA_REG_OFFSET) , PORT_PortChannels[Counter].Pin);        /* Clear the corresponding bit to configure it as input pin */
                    }
                  }
                  else
                  {
                      /* Do Nothing */
                  }
                  if(PORT_PortChannels[Counter].PORT_PIN_ANALOG_ENABLE == STD_ON)
                  {
                     SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , PORT_PortChannels[Counter].Pin);          /* Clear the corresponding bit to disable analog functionality on this pin */
                  }
                  else if(PORT_PortChannels[Counter].PORT_PIN_ANALOG_ENABLE == STD_OFF)
                  {
                    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , PORT_PortChannels[Counter].Pin);         /* Clear the corresponding bit */
                  }
                  else
                  {
                    /* Do Nothing */
                  }
                  if(PORT_PortChannels[Counter].PORT_PIN_DIGITAL_ENABLE == STD_ON)
                  {
                    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PORT_PortChannels[Counter].Pin);            /* Set the corresponding bit */
                  }
                  else if(PORT_PortChannels[Counter].PORT_PIN_DIGITAL_ENABLE == STD_OFF)
                  {
                    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PORT_PortChannels[Counter].Pin);          /* Clear the corresponding bit */
                  }
                   else
                  {
                    /* Do Nothing */
                  }
            }
                
	}
}

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
#if (PORT_SET_PIN_DIRECTION_API == STD_ON)
void Port_SetPinDirection(Port_PinType Pin, Port_PinDirectionType Direction)
{
  volatile uint32 * PortGpio_Ptr = NULL_PTR; /* point to the required Port Registers base address */
  boolean error = FALSE;
  #if (PORT_DEV_ERROR_DETECT == STD_ON)
	/* Check if the Driver is initialized before using this function */
	if (PORT_NOT_INITIALIZED == Port_Status)
	{
		Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,
				PORT_SET_PIN_DIRECTION_SID, PORT_E_UNINIT);
		error = TRUE;
	}
	else
	{
		/* No Action Required */
	}
        /* Check if the used channel is within the valid range */
	if (Pin >= PORT_CONFIGURED_CHANNELS)
	{

		Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,
				PORT_SET_PIN_DIRECTION_SID, PORT_E_PARAM_PIN);
		error = TRUE;
	}
	else
	{
		/* No Action Required */
	}
        if (PORT_PortChannels[Pin].PORT_PIN_DIRECTION_CHANGEABLE == STD_OFF)
        {
          Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,
				PORT_SET_PIN_DIRECTION_SID, PORT_E_DIRECTION_UNCHANGEABLE);
		error = TRUE;
        }
        else
        {
          /* No Action Required */
        }
  #endif
        if(error == FALSE)
        {
  switch(PORT_PortChannels[Pin].Port)
  {
     case  0: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
        break;
     case  1: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
        break;
     case  2: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
        break;
     case  3: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
        break;
     case  4: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
        break;
     case  5: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
        break;
  }
  if(Direction == PORT_PIN_IN)
  {
    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) ,PORT_PortChannels[Pin].Pin);                /* Set the corresponding bit to configure it as output pin */
  }
  else if(PORT_PortChannels[Pin].Ch_Direction == PORT_PIN_OUT)
  {
     SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) , PORT_PortChannels[Pin].Pin);             /* Clear the corresponding bit to configure it as input pin */
  }
  else
  {
     /* Do Nothing */
  }
        }
}
#endif

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
void Port_RefreshPortDirection(void)
{
  volatile uint32 * PortGpio_Ptr = NULL_PTR; /* point to the required Port Registers base address */
  uint8 Counter;
  #if (PORT_DEV_ERROR_DETECT == STD_ON)
	/* Check if the Driver is initialized before using this function */
	if (PORT_NOT_INITIALIZED == Port_Status)
	{
		Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,
				PORT_REFRESH_DIRECTION_SID, PORT_E_UNINIT);
	}
	else
  #endif
        {
          for(Counter = 0; Counter < PORT_CONFIGURED_CHANNELS; Counter++)
          {
            switch(PORT_PortChannels[Counter].Port)
            {
              case  0: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
                  break;
              case  1: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
                  break;
              case  2: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
                  break;
              case  3: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
                  break;
              case  4: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
                  break;
              case  5: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
                  break;
            }
            if(PORT_PortChannels[Counter].PORT_PIN_DIRECTION_CHANGEABLE == STD_ON)
            {
              /* Do Nothing */
            }
            else
            {
              if(PORT_PortChannels[Counter].Ch_Direction == PORT_PIN_IN)
              {
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) ,PORT_PortChannels[Counter].Pin);          /* Clear the corresponding bit to configure it as input port */
              }
              else
              {
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) ,PORT_PortChannels[Counter].Pin);            /* Set the corresponding bit */
              }
            }
          }
        }
        
}

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
#if (PORT_VERSION_INFO_API == STD_ON)
void Port_GetVersionInfo(Std_VersionInfoType* versioninfo)
{
  boolean error = FALSE;
#if (PORT_DEV_ERROR_DETECT == STD_ON)
	/* Check if input pointer is not Null pointer */
	if(NULL_PTR == versioninfo)
	{
		/* Report to DET  */
		Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,
				PORT_GET_VERSION_INFO_SID, PORT_E_PARAM_POINTER);
                error = TRUE;
	}
	else
        {
          /* Do Nothing */
        }
        if(PORT_NOT_INITIALIZED == Port_Status)
        {
          Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,
				PORT_GET_VERSION_INFO_SID, PORT_E_UNINIT);
          error = TRUE;
        }
        else
        {
          /* Do Nothing */
        }
#endif /* (PORT_DEV_ERROR_DETECT == STD_ON) */
	if(error == FALSE)
        {
		/* Copy the vendor Id */
		versioninfo->vendorID = (uint16)PORT_VENDOR_ID;
		/* Copy the module Id */
		versioninfo->moduleID = (uint16)PORT_MODULE_ID;
		/* Copy Software Major Version */
		versioninfo->sw_major_version = (uint8)PORT_SW_MAJOR_VERSION;
		/* Copy Software Minor Version */
		versioninfo->sw_minor_version = (uint8)PORT_SW_MINOR_VERSION;
		/* Copy Software Patch Version */
		versioninfo->sw_patch_version = (uint8)PORT_SW_PATCH_VERSION;
	}
        else
        {
          /* Do Nothing */
        }
}
#endif

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
#if (PORT_SET_PIN_MODE_API == STD_ON)
void Port_SetPinMode(Port_PinType Pin, Port_PinModeType Mode)
{
  boolean error = FALSE;
  
  #if (PORT_DEV_ERROR_DETECT == STD_ON)
	/* Check if the Driver is initialized before using this function */
	if (PORT_NOT_INITIALIZED == Port_Status)
	{
		Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,
				Port_SET_PIN_MODE_SID, PORT_E_UNINIT);
                error = TRUE;
	}
	else
        {
          /* Do Nothing */
        }
        /* Check if Incorrect Port Pin ID passed */
        if (Pin >= PORT_CONFIGURED_CHANNELS)
	{

		Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,
				PORT_SET_PIN_DIRECTION_SID, PORT_E_PARAM_PIN);
		error = TRUE;
	}
	else
	{
		/* No Action Required */
	}
        /* Check if Incorrect Port Pin mode valid */
        if (Mode > PORT_MODES_NUM)
        {
          Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,
				Port_SET_PIN_MODE_SID, PORT_E_PARAM_INVALID_MODE);
		error = TRUE;
        }
        else
	{
		/* No Action Required */
	}
        if (PORT_PortChannels[Pin].PORT_PIN_MODE_CHANGEABLE == STD_OFF)
        {
          Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,
				Port_SET_PIN_MODE_SID, PORT_E_MODE_UNCHANGEABLE);
		error = TRUE;
        }
        else
	{
		/* No Action Required */
	}
  #endif
        if(error == FALSE)
        {
          volatile uint32 * PortGpio_Ptr = NULL_PTR; /* point to the required Port Registers base address */
          switch(PORT_PortChannels[Pin].Port)
            {
              case  0: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
                  break;
              case  1: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
                  break;
              case  2: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
                  break;
              case  3: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
                  break;
              case  4: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
                  break;
              case  5: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
                  break;
            }
          if(Mode == PORT_PIN_INITIAL_MODE)
          {
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) ,PORT_PortChannels[Pin].Pin);             /* Disable Alternative function for this pin by clear the corresponding bit */
            *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (PORT_PortChannels[Pin].Pin * 4));     /* Clear the PMCx bits for this pin */               
          }
          else
          {
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) ,PORT_PortChannels[Pin].Pin);             /* Enable Alternative function for this pin by clear the corresponding bit */
            *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (PORT_PortChannels[Pin].Pin * 4));     /* Clear bits for this pin */
            *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (PORT_PortChannels[Pin].Ch_Mode << (PORT_PortChannels[Pin].Pin * 4));
          }
        }
  
}
#endif