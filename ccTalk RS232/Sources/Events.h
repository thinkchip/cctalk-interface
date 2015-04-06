/* ###################################################################
**     Filename    : Events.h
**     Project     : ccTalk RS232
**     Processor   : MC9S08PA16VLC
**     Component   : Events
**     Version     : Driver 01.02
**     Compiler    : CodeWarrior HCS08 C Compiler
**     Date/Time   : 2015-02-01, 21:11, # CodeGen: 0
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file Events.h
** @version 01.02
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup Events_module Events module documentation
**  @{
*/         

#ifndef __Events_H
#define __Events_H
/* MODULE Events */

#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "PE_Timer.h"
#include "AS_CCTALK.h"
#include "FC_CCTALK.h"
#include "TI_CCTALK.h"
#include "WDOG.h"
#include "AS_DATABUS.h"
#include "LED_RUN.h"
#include "LED_CCTALK.h"
#include "FC_DATABUS.h"
#include "TI_LEDS.h"
#include "EEPROM.h"
#include "UTIL.h"
#include "FC_HOPPER.h"


void TI_LEDS_OnInterrupt(void);
/*
** ===================================================================
**     Event       :  TI_LEDS_OnInterrupt (module Events)
**
**     Component   :  TI_LEDS [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void AS_DATABUS_OnError(void);
/*
** ===================================================================
**     Event       :  AS_DATABUS_OnError (module Events)
**
**     Component   :  AS_DATABUS [AsynchroSerial]
**     Description :
**         This event is called when a channel error (not the error
**         returned by a given method) occurs. The errors can be read
**         using <GetError> method.
**         The event is available only when the <Interrupt
**         service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void AS_DATABUS_OnRxChar(void);
/*
** ===================================================================
**     Event       :  AS_DATABUS_OnRxChar (module Events)
**
**     Component   :  AS_DATABUS [AsynchroSerial]
**     Description :
**         This event is called after a correct character is received.
**         The event is available only when the <Interrupt
**         service/event> property is enabled and either the <Receiver>
**         property is enabled or the <SCI output mode> property (if
**         supported) is set to Single-wire mode.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void AS_DATABUS_OnTxChar(void);
/*
** ===================================================================
**     Event       :  AS_DATABUS_OnTxChar (module Events)
**
**     Component   :  AS_DATABUS [AsynchroSerial]
**     Description :
**         This event is called after a character is transmitted.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void TI_CCTALK_OnInterrupt(void);
/*
** ===================================================================
**     Event       :  TI_CCTALK_OnInterrupt (module Events)
**
**     Component   :  TI_CCTALK [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void AS_CCTALK_OnError(void);
/*
** ===================================================================
**     Event       :  AS_CCTALK_OnError (module Events)
**
**     Component   :  AS_CCTALK [AsynchroSerial]
**     Description :
**         This event is called when a channel error (not the error
**         returned by a given method) occurs. The errors can be read
**         using <GetError> method.
**         The event is available only when the <Interrupt
**         service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void AS_CCTALK_OnRxChar(void);
/*
** ===================================================================
**     Event       :  AS_CCTALK_OnRxChar (module Events)
**
**     Component   :  AS_CCTALK [AsynchroSerial]
**     Description :
**         This event is called after a correct character is received.
**         The event is available only when the <Interrupt
**         service/event> property is enabled and either the <Receiver>
**         property is enabled or the <SCI output mode> property (if
**         supported) is set to Single-wire mode.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void AS_CCTALK_OnTxChar(void);
/*
** ===================================================================
**     Event       :  AS_CCTALK_OnTxChar (module Events)
**
**     Component   :  AS_CCTALK [AsynchroSerial]
**     Description :
**         This event is called after a character is transmitted.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

/* END Events */
#endif /* __Events_H*/

/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.3 [05.09]
**     for the Freescale HCS08 series of microcontrollers.
**
** ###################################################################
*/
