/* ###################################################################
**     Filename    : Events.c
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
** @file Events.c
** @version 01.02
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup Events_module Events module documentation
**  @{
*/         
/* MODULE Events */


#include "Cpu.h"
#include "Events.h"

/* User includes (#include below this line is not maintained by Processor Expert) */


#include "cctalk.h"
#include "Databus.h"





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
void TI_LEDS_OnInterrupt(void)
{
  /* Write your code here ... */
	
	LED_RUN_NegVal();
	
	

}

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
void  AS_DATABUS_OnError(void)
{
  /* Write your code here ... */
}

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
void  AS_DATABUS_OnRxChar(void)
{
  /* Write your code here ... */
	uint8_t car;
	
	if(AS_DATABUS_RecvChar(&car)==ERR_OK){	
		
		data.BufferInput[data.pointer++]=car;	
		data.FlagData=1;
		
	}
	
}

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
void  AS_DATABUS_OnTxChar(void)
{
  /* Write your code here ... */
}

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
void TI_CCTALK_OnInterrupt(void)
{
  /* Write your code here ... */
	static uint8_t ticks_poll=0;
	
	ticks_poll++;
	
	if(ticks_poll>=2){
		ticks_poll=0;
		cctalk.FlagStatus=1;
		
	}
}

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
void  AS_CCTALK_OnError(void)
{
  /* Write your code here ... */
}

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
void  AS_CCTALK_OnRxChar(void)
{
  /* Write your code here ... */
	uint16_t sum_data_resp=0;
	uint16_t valsum=0;
	uint8_t car=0;
	
	
	if(AS_CCTALK_RecvChar(&car)==ERR_OK){
		(void)FC_CCTALK_Reset();	
		cctalk.BufferInput[cctalk.PointerInput++]=car;
		if(cctalk.PointerInput>cctalk.PointerEcho){//start process checksum 
			sum_data_resp=cctalk.SumData;
			cctalk.SumData+=car;
			valsum=((sum_data_resp/256)+1)*256-sum_data_resp;
			if(valsum==car){
				cctalk.FlagReceiveData=1;
				cctalk.PointerInput=0;
				cctalk.SumData=0;
			}
		}		
	}
}

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
void  AS_CCTALK_OnTxChar(void)
{
  /* Write your code here ... */
}

/* END Events */

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
