/*
 * cctalk.c
 *
 *  Created on: Aug 19, 2014
 *      Author: THINKCHIP
 *      
 */

#include "cctalk.h"

//private functions
uint8_t calculate_crc(uint8_t slave,uint8_t master,uint8_t command,uint8_t numberdata,uint8_t *data);
void SendDataccTalk(uint8_t slave,uint8_t numberdata,uint8_t master,uint8_t command,uint8_t *data);

//private function for coin acceptors
uint8_t CoinAcceptorPoll(void);
uint8_t CoinAcceptorReqManufacturerID(void);
uint8_t CoinAcceptorReqProductCode(void);
uint8_t CoinAcceptorReqSerialNumber(void);
uint8_t CoinAcceptorReqSoftwareVersion(void);
uint8_t CoinAcceptorReqOptionFlags(void);
uint8_t CoinAcceptorModInhibitStatus(uint16_t typecoin);
uint8_t CoinAcceptorModMasterInhibitStatus(uint8_t action);
uint8_t CoinAcceptorReqMasterInhibitStatus(void);
uint8_t CoinAcceptorReqCoinID(uint8_t id,uint8_t *string);
uint8_t CoinAcceptorReadBufferedCreditErrorCodes(void);

//private function for hoppers
uint8_t HopperSimplePoll(uint8_t id);
uint8_t HopperEnable(uint8_t id,uint8_t action);
uint8_t HopperReqSerialNumber(uint8_t id, uint8_t *string);
uint8_t HopperReqProductCode(uint8_t id, uint8_t *string);
uint8_t HopperDispenseCoin(uint8_t id,uint8_t numbercoins,uint8_t *serialnumber);
uint8_t HopperReqStatus(uint8_t id,uint8_t *counter,uint8_t *coinspendent,uint8_t *coinspayout,uint8_t *coinsunpay);
uint8_t HopperReqTestHopper(uint8_t id,uint8_t *data);
uint8_t HopperSendCoins(uint8_t id,uint8_t number_coins);

uint8_t DeviceChangeAddress(uint8_t address_old, uint8_t address_new);
uint8_t DeviceResetSoftware(uint8_t id);

uint8_t BillPoll(void);
uint8_t BillReqSerialNumber(void);
uint8_t BillReqProductCode(void);
uint8_t BillReqManufacturerID(void);
uint8_t BillReqBillTypeID(uint8_t id,uint8_t *string);
uint8_t BillModBillOperatingMode(uint8_t data);
uint8_t BillModMasterInhibitStatus(uint8_t action);
uint8_t BillReadBufferedBillEvents(void);
uint8_t BillRouteBill(uint8_t data,uint8_t *response);
uint8_t BillReqBarcode(uint8_t *string);

//public functions
//void InitccTalk(void);
void TaskccTalk(void);

VARccTalk cctalk;


/*************************************************************
 * 
 *************************************************************/
void CleanBufferInput(void){
	uint8_t i;
	for(i=0;i<255;i++){
		cctalk.BufferInput[i]=0;
	}
	cctalk.PointerInput=0;
	cctalk.SumData=0;
}

/*************************************************************
 * 
 *
 *************************************************************/
uint8_t calculate_crc(uint8_t slave,uint8_t master,uint8_t command,uint8_t numberdata,uint8_t *data){
	
	uint16_t sum_data=0;
	uint16_t sum_paq=0;
	uint16_t valida_suma=0;
	uint8_t checksum=0;
	uint8_t i;
	
	if(numberdata){		
		for(i=0;i<numberdata;++i){
			sum_data += data[i];						
		}		
	}
	sum_paq=slave+master+command+numberdata+sum_data;
	valida_suma=((sum_paq/256)+1)*256-sum_paq;	
	checksum = (uint8_t)valida_suma&0x00ff;
	
	return checksum;
}

/*************************************************************
 * 
 ************************************************************/
void SendDataccTalk(uint8_t slave,uint8_t numberdata,uint8_t master,uint8_t command,uint8_t *data){
	
	uint8_t i;
	uint8_t checksum=0;
	
	cctalk.FlagReceiveData=0;
	cctalk.PointerEcho = 5+numberdata;
	CleanBufferInput();
	
	
	while(AS_CCTALK_SendChar(slave)!=ERR_OK);
	while(AS_CCTALK_SendChar(numberdata)!=ERR_OK);
	while(AS_CCTALK_SendChar(master)!=ERR_OK);
	while(AS_CCTALK_SendChar(command)!=ERR_OK);
		
	
	if(numberdata){
		for(i=0;i<numberdata;i++){
			while(AS_CCTALK_SendChar(data[i])!=ERR_OK);
		}		
	}
	
	checksum=calculate_crc(slave,master,command,numberdata,data);
	while(AS_CCTALK_SendChar(checksum)!=ERR_OK);
	
}
/*************************************************************
 * 
 ************************************************************/
uint8_t CoinAcceptorPoll(void){
	uint8_t err=0;
	uint16_t time=0;
	SendDataccTalk(COIN_ACCEPTOR,0,MASTER,CCTALK_SIMPLE_POLL,cctalk.BufferOutput);
	(void)FC_CCTALK_Reset();
	while(!cctalk.FlagReceiveData && time<100){
		(void)FC_CCTALK_GetTimeMS(&time);	
		(void)WDOG_Clear();
	}
	if(!cctalk.FlagReceiveData && time>=100){
		err=0;
	}else{
		err=1;
		
	}
	CleanBufferInput();
	return err;
}
/*************************************************************
 * 
 ************************************************************/
uint8_t CoinAcceptorReqManufacturerID(void){
	
	uint8_t err=0;
	uint16_t time=0;
	
	SendDataccTalk(COIN_ACCEPTOR,0,MASTER,CCTALK_REQ_MANU_ID,cctalk.BufferOutput);
	(void)FC_CCTALK_Reset();
	while(!cctalk.FlagReceiveData && time<100){
		(void)FC_CCTALK_GetTimeMS(&time);
		(void)WDOG_Clear();
	}
	if(!cctalk.FlagReceiveData && time>=100){
		
		err=0;
	}else{
		
		cctalk.CoinManufacturerID[0]=cctalk.BufferInput[9];
		cctalk.CoinManufacturerID[1]=cctalk.BufferInput[10];
		cctalk.CoinManufacturerID[2]=cctalk.BufferInput[11];
		
		err=1;
	}
	CleanBufferInput();
	return err;

}
/*************************************************************
 * 
 ************************************************************/
uint8_t CoinAcceptorReqProductCode(void){
	
	uint8_t err=0;
	uint16_t time=0;
	uint8_t size=0;
	
	SendDataccTalk(COIN_ACCEPTOR,0,MASTER,CCTALK_REQ_PROD_CODE,cctalk.BufferOutput);
	(void)FC_CCTALK_Reset();
	while(!cctalk.FlagReceiveData && time<100){
		(void)FC_CCTALK_GetTimeMS(&time);
		(void)WDOG_Clear();
	}
	if(!cctalk.FlagReceiveData && time>=100){
		
		err=0;
	}else{
		
		size=cctalk.BufferInput[6];
		MoveStringtoString(cctalk.CoinProductCode,cctalk.BufferInput,0,9,size);
		
		err=1;
	}
	CleanBufferInput();
	return err;
}
/*************************************************************
 * 
 ************************************************************/
uint8_t CoinAcceptorReqSerialNumber(void){
	
	uint8_t err=0;
	uint16_t time=0;
	SendDataccTalk(COIN_ACCEPTOR,0,MASTER,CCTALK_REQ_SERIAL_NUM,cctalk.BufferOutput);
	(void)FC_CCTALK_Reset();
	while(!cctalk.FlagReceiveData && time<100){
		(void)FC_CCTALK_GetTimeMS(&time);	
		(void)WDOG_Clear();
	}
	if(!cctalk.FlagReceiveData && time>=100){
		
		err=0;
	}else{
		
		cctalk.CoinSerialNumber[0]=cctalk.BufferInput[11];
		cctalk.CoinSerialNumber[1]=cctalk.BufferInput[10];
		cctalk.CoinSerialNumber[2]=cctalk.BufferInput[9];
		
		err=1;
	}
	CleanBufferInput();
	return err;
}
/*************************************************************
 * 
 ************************************************************/
uint8_t CoinAcceptorReqSoftwareVersion(void){
	
	uint8_t err=0;
	uint16_t time=0;
	SendDataccTalk(COIN_ACCEPTOR,0,MASTER,CCTALK_REQ_SOFT_VER,cctalk.BufferOutput);
	(void)FC_CCTALK_Reset();
	while(!cctalk.FlagReceiveData && time<100){
		(void)FC_CCTALK_GetTimeMS(&time);	
		(void)WDOG_Clear();
	}
	if(!cctalk.FlagReceiveData && time>=100){
		
		err=0;
	}else{
		
		MoveStringtoString(cctalk.CoinSoftwareVersion,cctalk.BufferInput,0,9,14);
		
		err=1;
	}
	CleanBufferInput();
	return err;
}
/*************************************************************
 * 
 ************************************************************/
uint8_t CoinAcceptorReqOptionFlags(void){
	
	uint8_t err=0;
	uint16_t time=0;
	SendDataccTalk(COIN_ACCEPTOR,0,MASTER,CCTALK_REQ_OPT_FLAGS,cctalk.BufferOutput);
	(void)FC_CCTALK_Reset();
	while(!cctalk.FlagReceiveData && time<100){
		(void)FC_CCTALK_GetTimeMS(&time);
		(void)WDOG_Clear();
	}
	if(!cctalk.FlagReceiveData && time>=100){
		
		err=0;
	}else{
		
		cctalk.CoinOptionFlags=cctalk.BufferInput[9];		
		err=1;
	}
	CleanBufferInput();
	return err;
}
/*************************************************************
 * 
 ************************************************************/
uint8_t CoinAcceptorModInhibitStatus(uint16_t typecoin){
	
	uint8_t err=0;
	uint16_t time=0;
	
	cctalk.BufferOutput[0]=(uint8_t)(typecoin&0x00ff);
	cctalk.BufferOutput[1]=(uint8_t)(typecoin>>8);
	SendDataccTalk(COIN_ACCEPTOR,2,MASTER,CCTALK_MODIFY_INH_STA,cctalk.BufferOutput);
	(void)FC_CCTALK_Reset();
	while(!cctalk.FlagReceiveData && time<100){
		(void)FC_CCTALK_GetTimeMS(&time);	
		(void)WDOG_Clear();
	}
	if(!cctalk.FlagReceiveData && time>=100){
		
		err=0;
	}else{		
				
		err=1;
	}
	CleanBufferInput();
	return err;
}

/*************************************************************
 * 
 ************************************************************/
uint8_t CoinAcceptorModMasterInhibitStatus(uint8_t action){
	
	uint8_t err=0;
	uint16_t time=0;
	
	cctalk.BufferOutput[0]=action;
	SendDataccTalk(COIN_ACCEPTOR,1,MASTER,CCTALK_MOD_MAST_INH_STA,cctalk.BufferOutput);
	(void)FC_CCTALK_Reset();
	while(!cctalk.FlagReceiveData && time<100){
		(void)FC_CCTALK_GetTimeMS(&time);	
		(void)WDOG_Clear();
	}
	if(!cctalk.FlagReceiveData && time>=100){		
		err=0;
	}else{						
		err=1;
	}
	CleanBufferInput();
	return err;
}
/*************************************************************
 * 
 ************************************************************/
uint8_t CoinAcceptorReqMasterInhibitStatus(void){
	
	uint8_t err=0;
	uint16_t time=0;
	
	SendDataccTalk(COIN_ACCEPTOR,0,MASTER,CCTALK_REQ_MAST_INH_STA,cctalk.BufferOutput);
	(void)FC_CCTALK_Reset();
	while(!cctalk.FlagReceiveData && time<100){
		(void)FC_CCTALK_GetTimeMS(&time);	
		(void)WDOG_Clear();
	}
	if(!cctalk.FlagReceiveData && time>=100){		
		err=0;
	}else{						
		err=1;
	}
	CleanBufferInput();
	return err;
}
/*************************************************************
 * 
 ************************************************************/
uint8_t CoinAcceptorReqCoinID(uint8_t id,uint8_t *string){
	
	uint8_t err=0;
	uint16_t time=0;

	cctalk.FlagReceiveData=0;
	time=0;
	cctalk.BufferOutput[0]=id;
	SendDataccTalk(COIN_ACCEPTOR,1,MASTER,CCTALK_REQ_COIN_ID,cctalk.BufferOutput);
	(void)FC_CCTALK_Reset();
	while(!cctalk.FlagReceiveData && time<100){
		(void)FC_CCTALK_GetTimeMS(&time);		
		(void)WDOG_Clear();
	}
	if(!cctalk.FlagReceiveData && time>=100){		
		return 0;
	}else{		
		err=1;
	}
	
	string[0]=cctalk.BufferInput[9];
	string[1]=cctalk.BufferInput[10];
	string[2]=cctalk.BufferInput[11];
	string[3]=cctalk.BufferInput[12];
	string[4]=cctalk.BufferInput[13];
	string[5]=cctalk.BufferInput[14];
	
	CleanBufferInput();	
	return err;
}
/*************************************************************
 * 
 ************************************************************/
uint8_t CoinAcceptorReadBufferedCreditErrorCodes(void){
	
	uint8_t err=0;
	uint16_t time=0;
		
	SendDataccTalk(COIN_ACCEPTOR,0,MASTER,CCTALK_READ_BUFF_CRED,cctalk.BufferOutput);
	(void)FC_CCTALK_Reset();
	while(!cctalk.FlagReceiveData && time<100){
		(void)FC_CCTALK_GetTimeMS(&time);	
		(void)WDOG_Clear();
	}
	if(!cctalk.FlagReceiveData && time>=100){		
		err=0;
	}else{	
		
		MoveStringtoString(cctalk.CoinBufferErrorCredit,cctalk.BufferInput,0,9,11);					
		err=1;
	}
	CleanBufferInput();
	return err;
}
/**************************************************************
 * 
 **************************************************************/
uint8_t HopperSimplePoll(uint8_t id){
	
	uint8_t err=0;
	uint16_t time=0;
	
	
	SendDataccTalk(id,0,MASTER,CCTALK_SIMPLE_POLL,cctalk.BufferOutput);
	(void)FC_CCTALK_Reset();
	while(!cctalk.FlagReceiveData && time<100){
		(void)FC_CCTALK_GetTimeMS(&time);	
		(void)WDOG_Clear();
	}
	if(!cctalk.FlagReceiveData && time>=100){		
		err=0;
	}else{	//response ok					
		err=1;	
	}
	
	
	CleanBufferInput();
	return err;

}

/**************************************************************
 * 
 **************************************************************/
uint8_t HopperEnable(uint8_t id,uint8_t action){
	
	uint8_t err=0;
	uint16_t time=0;
	
	cctalk.BufferOutput[0]=action;
	SendDataccTalk(id,1,MASTER,CCTALK_ENABLE_HOPP,cctalk.BufferOutput);
	(void)FC_CCTALK_Reset();
	while(!cctalk.FlagReceiveData && time<100){
		(void)FC_CCTALK_GetTimeMS(&time);	
		(void)WDOG_Clear();
	}
	if(!cctalk.FlagReceiveData && time>=100){		
		err=0;
	}else{						
		err=1;
	}
	CleanBufferInput();
	return err;
	
}
/**************************************************************
 * 
 **************************************************************/
uint8_t HopperReqSerialNumber(uint8_t id, uint8_t *string){
	
	uint8_t err=0;
	uint16_t time=0;
	
	SendDataccTalk(id,0,MASTER,CCTALK_REQ_SERIAL_NUM,cctalk.BufferOutput);
	(void)FC_CCTALK_Reset();
	while(!cctalk.FlagReceiveData && time<100){
		(void)FC_CCTALK_GetTimeMS(&time);	
		(void)WDOG_Clear();
	}
	if(!cctalk.FlagReceiveData && time>=100){		
		err=0;
	}else{						
		err=1;
		string[0]=cctalk.BufferInput[11];
		string[1]=cctalk.BufferInput[10];
		string[2]=cctalk.BufferInput[9];
	}
	CleanBufferInput();
	return err;
	
	
}
/**************************************************************
 * 
 **************************************************************/
uint8_t HopperReqProductCode(uint8_t id, uint8_t *string){
	
	uint8_t err=0;
	uint16_t time=0;
	
	SendDataccTalk(id,0,MASTER,CCTALK_REQ_PROD_CODE,cctalk.BufferOutput);
	(void)FC_CCTALK_Reset();
	while(!cctalk.FlagReceiveData && time<100){
		(void)FC_CCTALK_GetTimeMS(&time);	
		(void)WDOG_Clear();
	}
	if(!cctalk.FlagReceiveData && time>=100){		
		err=0;
	}else{						
		err=1;
		
		MoveStringtoString(string,cctalk.BufferInput,0,9,6);
	}
	CleanBufferInput();
	return err;
	
}
/**************************************************************
 * 
 **************************************************************/
uint8_t HopperDispenseCoin(uint8_t id,uint8_t numbercoins,uint8_t *serialnumber){
	
	uint8_t err=0;
	uint16_t time=0;
	
	cctalk.BufferOutput[2]=serialnumber[0];
	cctalk.BufferOutput[1]=serialnumber[1];
	cctalk.BufferOutput[0]=serialnumber[2];
	cctalk.BufferOutput[3]=numbercoins;
	SendDataccTalk(id,4,MASTER,CCTALK_DISP_HOPP_COIN,cctalk.BufferOutput);
	(void)FC_CCTALK_Reset();
	while(!cctalk.FlagReceiveData && time<300){
		(void)FC_CCTALK_GetTimeMS(&time);	
		(void)WDOG_Clear();
	}
	if(!cctalk.FlagReceiveData && time>=300){		
		err=0;
	}else{						
		err=1;
		
	}
	CleanBufferInput();
	return err;
	
}
/**************************************************************
 * 
 **************************************************************/
uint8_t HopperReqStatus(uint8_t id,uint8_t *counter,uint8_t *coinspendent,uint8_t *coinspayout,uint8_t *coinsunpay){
	
	uint8_t err=0;
	uint16_t time=0;
	
	SendDataccTalk(id,0,MASTER,CCTALK_REQ_HOPP_STATUS,cctalk.BufferOutput);
	(void)FC_CCTALK_Reset();
	while(!cctalk.FlagReceiveData && time<100){
		(void)FC_CCTALK_GetTimeMS(&time);	
		(void)WDOG_Clear();
	}
	if(!cctalk.FlagReceiveData && time>=100){		
		err=0;
	}else{						
		err=1;
		*counter=cctalk.BufferInput[9];
		*coinspendent = cctalk.BufferInput[10];
		*coinspayout = cctalk.BufferInput[11];
		*coinsunpay = cctalk.BufferInput[12];
	}
	CleanBufferInput();
	return err;

}
/**********************************************************************
 * 
 * 
 *********************************************************************/
uint8_t HopperReqTestHopper(uint8_t id,uint8_t *data){
	
	uint8_t err=0;
	uint16_t time=0;
	
	SendDataccTalk(id,0,MASTER,CCTALK_TEST_HOPP,cctalk.BufferOutput);
	(void)FC_CCTALK_Reset();
	while(!cctalk.FlagReceiveData && time<100){
		(void)FC_CCTALK_GetTimeMS(&time);	
		(void)WDOG_Clear();
	}
	if(!cctalk.FlagReceiveData && time>=100){		
		err=0;
	}else{						
		err=1;
		*data=cctalk.BufferInput[4];
		
	}
	CleanBufferInput();
	return err;
	
}

/**********************************************************************
 * 
 * 
 *********************************************************************/
uint8_t HopperSendCoins(uint8_t id,uint8_t number_coins){
	
	CleanBufferInput();
	
	cctalk.FlagReceiveData=0;
	if(HopperEnable(id,HOPPER_ENABLE)){
		
		CleanBufferInput();
		cctalk.FlagReceiveData=0;
		if(id==cctalk.HopperAddress[0]){
			if(!HopperDispenseCoin(id,number_coins,cctalk.Hopper1SerialNumber)){
				return 0;
			}else{
				cctalk.Hopper1FlagStatus=DEVICE_PAYOUT;
			}
		}else if(id==cctalk.HopperAddress[1]){
			if(!HopperDispenseCoin(id,number_coins,cctalk.Hopper2SerialNumber)){
				return 0;
			}else{
				cctalk.Hopper2FlagStatus=DEVICE_PAYOUT;
			}
		}else{
			if(!HopperDispenseCoin(id,number_coins,cctalk.Hopper3SerialNumber)){
				return 0;
			}else{
				cctalk.Hopper3FlagStatus=DEVICE_PAYOUT;
			}
		}
				
		(void)FC_HOPPER_Reset();
		
		return 1;
	}else{
		return 0;
	}
	
}
/*******************************************************************
 * 
 * 
 ******************************************************************/
void HopperScanID(void){
	
	uint8_t i=0,j=0;
	
	CleanBufferInput();
	cctalk.FlagReceiveData=0;
	for(i=3,j=0;i<20;i++){
		if(HopperSimplePoll(i)){
			cctalk.HopperAddress[j]=i;
			j++;
		}
		Cpu_Delay100US(500);
		cctalk.FlagReceiveData=0;
	}
	Cpu_Delay100US(100);
	//i=0;
	if(cctalk.HopperAddress[0]){		
		
		CleanBufferInput();
		cctalk.FlagReceiveData=0;
		cctalk.Hopper1FlagStatus = DEVICE_OK;
		(void)HopperReqSerialNumber(cctalk.HopperAddress[0],cctalk.Hopper1SerialNumber);
				
	}
	
	if(cctalk.HopperAddress[1]){		
			
		CleanBufferInput();
		cctalk.FlagReceiveData=0;
		cctalk.Hopper2FlagStatus = DEVICE_OK;
		(void)HopperReqSerialNumber(cctalk.HopperAddress[1],cctalk.Hopper2SerialNumber);
				
	}
	
	if(cctalk.HopperAddress[2]){		
				
		CleanBufferInput();
		cctalk.FlagReceiveData=0;
		cctalk.Hopper3FlagStatus = DEVICE_OK;
		(void)HopperReqSerialNumber(cctalk.HopperAddress[2],cctalk.Hopper3SerialNumber);
				
	}
	
}

/***************************************************************
 * 
 ***************************************************************/
uint8_t DeviceResetSoftware(uint8_t id){
	uint8_t err=0;
	uint16_t time=0;
	
	SendDataccTalk(id,0,MASTER,CCTALK_RESET_DEVICE,cctalk.BufferOutput);
	(void)FC_CCTALK_Reset();
	while(!cctalk.FlagReceiveData && time<100){
		(void)FC_CCTALK_GetTimeMS(&time);	
		(void)WDOG_Clear();
	}
	if(!cctalk.FlagReceiveData && time>=100){		
		err=0;
	}else{						
		err=1;
		
	}
	CleanBufferInput();
	return err;
	
}
/**************************************************************
 * 
 **************************************************************/
uint8_t DeviceChangeAddress(uint8_t address_old, uint8_t address_new){
	uint8_t err=0;
	uint16_t time=0;
	cctalk.BufferOutput[0]=address_new;
	SendDataccTalk(address_old,1,MASTER,CCTALK_ADDRESS_CHANGE,cctalk.BufferOutput);
	(void)FC_CCTALK_Reset();
	while(!cctalk.FlagReceiveData && time<100){
		(void)FC_CCTALK_GetTimeMS(&time);	
		(void)WDOG_Clear();
	}
	if(!cctalk.FlagReceiveData && time>=100){		
		err=0;
	}else{						
		err=1;
		
	}
	CleanBufferInput();
	return err;
	
}

/*************************************************************
 * 
 ************************************************************/
uint8_t BillPoll(void){
	uint8_t err=0;
	uint16_t time=0;
	SendDataccTalk(BILL,0,MASTER,CCTALK_SIMPLE_POLL,cctalk.BufferOutput);
	(void)FC_CCTALK_Reset();
	while(!cctalk.FlagReceiveData && time<100){
		(void)FC_CCTALK_GetTimeMS(&time);	
		(void)WDOG_Clear();
	}
	if(!cctalk.FlagReceiveData && time>=100){
		err=0;
	}else{
		err=1;
		
	}
	CleanBufferInput();
	return err;
}
/*************************************************************
 * 
 ************************************************************/
uint8_t BillReqProductCode(void){

	uint8_t err=0;
	uint16_t time=0;
	uint8_t size=0;
	
	SendDataccTalk(BILL,0,MASTER,CCTALK_REQ_PROD_CODE,cctalk.BufferOutput);
	(void)FC_CCTALK_Reset();
	while(!cctalk.FlagReceiveData && time<100){
		(void)FC_CCTALK_GetTimeMS(&time);
		(void)WDOG_Clear();
	}
	if(!cctalk.FlagReceiveData && time>=100){		
		err=0;
	}else{		
		size=cctalk.BufferInput[6];
		MoveStringtoString(cctalk.BillProductCode,cctalk.BufferInput,0,9,size);
		err=1;
	}
	CleanBufferInput();
	return err;
	
}
/**********************************************************************
 * 
 *********************************************************************/
uint8_t BillReqManufacturerID(void){
	
	uint8_t err=0;
	uint16_t time=0;
	
	SendDataccTalk(BILL,0,MASTER,CCTALK_REQ_MANU_ID,cctalk.BufferOutput);
	(void)FC_CCTALK_Reset();
	while(!cctalk.FlagReceiveData && time<100){
		(void)FC_CCTALK_GetTimeMS(&time);
		(void)WDOG_Clear();
	}
	if(!cctalk.FlagReceiveData && time>=100){		
		err=0;
	}else{
		
		cctalk.BillManufacturerID[0]=cctalk.BufferInput[9];
		cctalk.BillManufacturerID[1]=cctalk.BufferInput[10];
		cctalk.BillManufacturerID[2]=cctalk.BufferInput[11];
		
		err=1;
	}
	CleanBufferInput();
	return err;

}

/*************************************************************
 * 
 ************************************************************/
uint8_t BillReqSerialNumber(void){
	
	uint8_t err=0;
	uint16_t time=0;
	SendDataccTalk(BILL,0,MASTER,CCTALK_REQ_SERIAL_NUM,cctalk.BufferOutput);
	(void)FC_CCTALK_Reset();
	while(!cctalk.FlagReceiveData && time<100){
		(void)FC_CCTALK_GetTimeMS(&time);	
		(void)WDOG_Clear();
	}
	if(!cctalk.FlagReceiveData && time>=100){
		
		err=0;
	}else{
		
		cctalk.BillSerialNumber[0]=cctalk.BufferInput[11];
		cctalk.BillSerialNumber[1]=cctalk.BufferInput[10];
		cctalk.BillSerialNumber[2]=cctalk.BufferInput[9];
		
		err=1;
	}
	CleanBufferInput();
	return err;
}
/*************************************************************
 * 
 ************************************************************/
uint8_t BillReqBillTypeID(uint8_t id,uint8_t *string){
	
	uint8_t err=0;
	uint16_t time=0;

	cctalk.FlagReceiveData=0;
	time=0;
	cctalk.BufferOutput[0]=id;
	SendDataccTalk(BILL,1,MASTER,CCTALK_REQ_BILL_ID,cctalk.BufferOutput);
	(void)FC_CCTALK_Reset();
	while(!cctalk.FlagReceiveData && time<100){
		(void)FC_CCTALK_GetTimeMS(&time);		
		(void)WDOG_Clear();
	}
	if(!cctalk.FlagReceiveData && time>=100){		
		return 0;
	}else{		
		err=1;
	}
	
	string[0]=cctalk.BufferInput[9];
	string[1]=cctalk.BufferInput[10];
	string[2]=cctalk.BufferInput[11];
	string[3]=cctalk.BufferInput[12];
	string[4]=cctalk.BufferInput[13];
	string[5]=cctalk.BufferInput[14];
	string[6]=cctalk.BufferInput[15];
	
	CleanBufferInput();	
	return err;
}
/*************************************************************
 * 
 ************************************************************/
uint8_t BillModBillOperatingMode(uint8_t data){
	
	uint8_t err=0;
	uint16_t time=0;
	cctalk.BufferOutput[0]=data;
	SendDataccTalk(BILL,1,MASTER,CCTALK_MOD_BILL_OPE_MO,cctalk.BufferOutput);
	(void)FC_CCTALK_Reset();
	while(!cctalk.FlagReceiveData && time<100){
		(void)FC_CCTALK_GetTimeMS(&time);	
		(void)WDOG_Clear();
	}
	if(!cctalk.FlagReceiveData && time>=100){		
		err=0;
	}else{				
		err=1;
	}
	CleanBufferInput();
	return err;
}
/*************************************************************
 * 
 ************************************************************/
uint8_t BillModMasterInhibitStatus(uint8_t action){
	
	uint8_t err=0;
	uint16_t time=0;
	
	cctalk.BufferOutput[0]=action;
	SendDataccTalk(BILL,1,MASTER,CCTALK_MOD_MAST_INH_STA,cctalk.BufferOutput);
	(void)FC_CCTALK_Reset();
	while(!cctalk.FlagReceiveData && time<100){
		(void)FC_CCTALK_GetTimeMS(&time);	
		(void)WDOG_Clear();
	}
	if(!cctalk.FlagReceiveData && time>=100){		
		err=0;
	}else{						
		err=1;
	}
	CleanBufferInput();
	return err;
}

/*************************************************************
 * 
 ************************************************************/
uint8_t BillReadBufferedBillEvents(void){
	
	uint8_t err=0;
	uint16_t time=0;
		
	SendDataccTalk(BILL,0,MASTER,CCTALK_BUF_BILL_EV,cctalk.BufferOutput);
	(void)FC_CCTALK_Reset();
	while(!cctalk.FlagReceiveData && time<100){
		(void)FC_CCTALK_GetTimeMS(&time);	
		(void)WDOG_Clear();
	}
	if(!cctalk.FlagReceiveData && time>=100){		
		err=0;
	}else{	
		
		MoveStringtoString(cctalk.BillBufferEvents,cctalk.BufferInput,0,9,11);			
		err=1;
	}
	CleanBufferInput();
	return err;
}

/*************************************************************
 * 
 ************************************************************/
uint8_t BillRouteBill(uint8_t data,uint8_t *response){
	
	uint8_t err=0;
	uint16_t time=0;
	cctalk.BufferOutput[0]=data;
	SendDataccTalk(BILL,1,MASTER,CCTALK_ROUTE_BILL,cctalk.BufferOutput);
	(void)FC_CCTALK_Reset();
	while(!cctalk.FlagReceiveData && time<100){
		(void)FC_CCTALK_GetTimeMS(&time);	
		(void)WDOG_Clear();
	}
	if(!cctalk.FlagReceiveData && time>=100){		
		err=0;
	}else{				
		err=1;
		*response = cctalk.BufferInput[9];
	}
	CleanBufferInput();
	return err;
}

/*************************************************************
 * 
 ************************************************************/
uint8_t BillReqBarcode(uint8_t *string){
	
	uint8_t err=0;
	uint16_t time=0;

	cctalk.FlagReceiveData=0;
	time=0;
	SendDataccTalk(BILL,0,MASTER,CCTALK_READ_BARC_DATA,cctalk.BufferOutput);
	(void)FC_CCTALK_Reset();
	while(!cctalk.FlagReceiveData && time<100){
		(void)FC_CCTALK_GetTimeMS(&time);		
		(void)WDOG_Clear();
	}
	if(!cctalk.FlagReceiveData && time>=100){		
		return 0;
	}else{		
		err=1;
	}
	
	MoveStringtoString(string,cctalk.BufferInput,0,9,cctalk.BufferInput[6]);
	
	CleanBufferInput();	
	return err;
}


/**********************************************************************
 * 
 * 
 *********************************************************************/
void TaskccTalk(void){
	
	static uint8_t FlagTurnDevice=0;
	static uint8_t counter_events_coin_saved=0,coin_first_time=0;
	static uint8_t counter_events_bill_saved=0;
	uint8_t num_event;
	uint8_t string[6]={0};
	uint16_t time;
	uint8_t x;
	
	(void)EEPROM_GetPage(EEPROM_AREA_START);
	(void)EEPROM_GetBytePage(ADDR_SYSTEM_MODE,&cctalk.FlagPoll);
	(void)EEPROM_GetBytePage(ADDR_BILL_MODE,&cctalk.BillFlagMode);
	
	
	
	if(cctalk.FlagPoll==1){
		
		if(!coin_first_time){
			
			(void)CoinAcceptorReadBufferedCreditErrorCodes();
			counter_events_coin_saved=cctalk.CoinBufferErrorCredit[0];
			(void)BillReadBufferedBillEvents();
			counter_events_bill_saved=cctalk.BillBufferEvents[0];
			
			coin_first_time=1;			
		
		}else{
			
			//========================================================================//
			if(cctalk.CoinBufferErrorCredit[0]!=counter_events_coin_saved){
				num_event=cctalk.CoinBufferErrorCredit[0]-counter_events_coin_saved;
				if(num_event>=1 && cctalk.CoinBufferErrorCredit[1]){
					(void)CoinAcceptorReqCoinID(cctalk.CoinBufferErrorCredit[1],string);
					dataprintf("CI",2);
					dataprintf(string,6);
					dataprintf("\r\n",2);
				}
				
				if(num_event>=2 && cctalk.CoinBufferErrorCredit[3]){
					(void)CoinAcceptorReqCoinID(cctalk.CoinBufferErrorCredit[3],string);
					dataprintf("CI",2);
					dataprintf(string,6);
					dataprintf("\r\n",2);
				}
				
				if(num_event>=3 && cctalk.CoinBufferErrorCredit[5]){
					(void)CoinAcceptorReqCoinID(cctalk.CoinBufferErrorCredit[5],string);
					dataprintf("CI",2);
					dataprintf(string,6);
					dataprintf("\r\n",2);
				}
				
				if(num_event>=4 && cctalk.CoinBufferErrorCredit[7]){
					(void)CoinAcceptorReqCoinID(cctalk.CoinBufferErrorCredit[7],string);
					dataprintf("CI",2);
					dataprintf(string,6);
					dataprintf("\r\n",2);
				}
				
				if(num_event==5 && cctalk.CoinBufferErrorCredit[9]){
					(void)CoinAcceptorReqCoinID(cctalk.CoinBufferErrorCredit[9],string);
					dataprintf("CI",2);
					dataprintf(string,6);
					dataprintf("\r\n",2);
				}
				
				counter_events_coin_saved = cctalk.CoinBufferErrorCredit[0];
			}
			//===========================================================================//
			if(cctalk.BillBufferEvents[0]!=counter_events_bill_saved){
				
				num_event=cctalk.BillBufferEvents[0]-counter_events_bill_saved;
				
				if(num_event>=1 && cctalk.BillBufferEvents[1]){
									
					if(cctalk.BillFlagMode){
						//stacker automatically
						if(cctalk.BufferInput[2]){
							//send to stacker
							if(cctalk.BillBufferEvents[1]==255 && !cctalk.BillBufferEvents[1+2] && cctalk.BillBufferEvents[1+3]==20){
								(void)BillReqBarcode(cctalk.BillBarcode);								
							}
							(void)BillRouteBill(1,&x);//send to stacker
							
						}else{
							//bill stacked
							if(cctalk.BillBufferEvents[1]==255){
								dataprintf("BC",2);
								dataprintf(cctalk.BillBarcode,18);								
							}else{
								(void)BillReqBillTypeID(cctalk.BillBufferEvents[1],string);
								dataprintf("BI",2);
								dataprintf(string,6);
								dataprintf("ST",2);							
							}
							dataprintf("\r\n",2);
						}											
					}else{
						//escrow mode
						(void)BillReqBillTypeID(cctalk.BillBufferEvents[1],string);
						dataprintf("BI",2);
						dataprintf(string,6);
						if(cctalk.BillBufferEvents[2]){
							//escrow
							dataprintf("ES",2);
						}else{
							dataprintf("ST",2);
						}
						dataprintf("\r\n",2);
					}
				
				}
				
				if(num_event>=2 && cctalk.BillBufferEvents[3]){
													
					if(cctalk.BillFlagMode){
						//stacker automatically
						if(cctalk.BufferInput[4]){//bill in escrow
							//send to stacker
							if(cctalk.BillBufferEvents[3]==255 && !cctalk.BillBufferEvents[3+2] && cctalk.BillBufferEvents[3+3]==20){
								(void)BillReqBarcode(cctalk.BillBarcode);								
							}
							(void)BillRouteBill(1,&x);//send to stacker
							
						}else{
							//bill stacked
							if(cctalk.BillBufferEvents[3]==255){
								dataprintf("BC",2);
								dataprintf(cctalk.BillBarcode,18);								
							}else{
								(void)BillReqBillTypeID(cctalk.BillBufferEvents[3],string);
								dataprintf("BI",2);
								dataprintf(string,6);
								dataprintf("ST",2);							
							}
							dataprintf("\r\n",2);
						}											
					}else{
						//escrow mode
						(void)BillReqBillTypeID(cctalk.BillBufferEvents[3],string);
						dataprintf("BI",2);
						dataprintf(string,6);
						if(cctalk.BillBufferEvents[4]){
							//escrow
							dataprintf("ES",2);
						}else{
							dataprintf("ST",2);
						}
						dataprintf("\r\n",2);
					}
				
				}

				if(num_event>=3 && cctalk.BillBufferEvents[5]){
													
					if(cctalk.BillFlagMode){
						//stacker automatically
						if(cctalk.BufferInput[6]){
							//send to stacker
							if(cctalk.BillBufferEvents[5]==255 && !cctalk.BillBufferEvents[5+2] && cctalk.BillBufferEvents[5+3]==20){
								(void)BillReqBarcode(cctalk.BillBarcode);								
							}
							(void)BillRouteBill(1,&x);//send to stacker
							
						}else{
							//bill stacked
							if(cctalk.BillBufferEvents[5]==255){
								dataprintf("BC",2);
								dataprintf(cctalk.BillBarcode,18);								
							}else{
								(void)BillReqBillTypeID(cctalk.BillBufferEvents[5],string);
								dataprintf("BI",2);
								dataprintf(string,6);
								dataprintf("ST",2);							
							}
							dataprintf("\r\n",2);
						}											
					}else{
						//escrow mode
						(void)BillReqBillTypeID(cctalk.BillBufferEvents[5],string);
						dataprintf("BI",2);
						dataprintf(string,6);
						if(cctalk.BillBufferEvents[6]){
							//escrow
							dataprintf("ES",2);
						}else{
							dataprintf("ST",2);
						}
						dataprintf("\r\n",2);
					}
				
				}
				if(num_event>=4 && cctalk.BillBufferEvents[7]){
													
					if(cctalk.BillFlagMode){
						//stacker automatically
						if(cctalk.BufferInput[8]){
							//send to stacker
							if(cctalk.BillBufferEvents[7]==255 && !cctalk.BillBufferEvents[7+2] && cctalk.BillBufferEvents[7+3]==20){
								(void)BillReqBarcode(cctalk.BillBarcode);								
							}
							(void)BillRouteBill(1,&x);//send to stacker
							
						}else{
							//bill stacked
							if(cctalk.BillBufferEvents[7]==255){
								dataprintf("BC",2);
								dataprintf(cctalk.BillBarcode,18);								
							}else{
								(void)BillReqBillTypeID(cctalk.BillBufferEvents[7],string);
								dataprintf("BI",2);
								dataprintf(string,6);
								dataprintf("ST",2);							
							}
							dataprintf("\r\n",2);
						}											
					}else{
						//escrow mode
						(void)BillReqBillTypeID(cctalk.BillBufferEvents[7],string);
						dataprintf("BI",2);
						dataprintf(string,6);
						if(cctalk.BillBufferEvents[8]){
							//escrow
							dataprintf("ES",2);
						}else{
							dataprintf("ST",2);
						}
						dataprintf("\r\n",2);
					}
				
				}
				
				if(num_event>=5 && cctalk.BillBufferEvents[9]){
									
					if(cctalk.BillFlagMode){
						//stacker automatically
						if(cctalk.BufferInput[10]){
							//send to stacker
							if(cctalk.BillBufferEvents[9]==255){
								(void)BillReqBarcode(cctalk.BillBarcode);								
							}
							(void)BillRouteBill(1,&x);//send to stacker
							
						}else{
							//bill stacked
							if(cctalk.BillBufferEvents[9]==255){
								dataprintf("BC",2);
								dataprintf(cctalk.BillBarcode,18);								
							}else{
								(void)BillReqBillTypeID(cctalk.BillBufferEvents[9],string);
								dataprintf("BI",2);
								dataprintf(string,6);
								dataprintf("ST",2);							
							}
							dataprintf("\r\n",2);
						}											
					}else{
						//escrow mode
						(void)BillReqBillTypeID(cctalk.BillBufferEvents[9],string);
						dataprintf("BI",2);
						dataprintf(string,6);
						if(cctalk.BillBufferEvents[10]){
							//escrow
							dataprintf("ES",2);
						}else{
							dataprintf("ST",2);
						}
						dataprintf("\r\n",2);
					}
				
				}
				
				counter_events_coin_saved = cctalk.CoinBufferErrorCredit[0];
				
			}
			
		}
		
		
	}
	

	if(cctalk.FlagStatus){
		
		cctalk.FlagStatus=0;		
		
			switch(FlagTurnDevice){
			
			/////////////////////////////////////////////////////////
			
			case 0:
			
				if(cctalk.CoinFlagStatus!=DEVICE_NO_RESPONSE){
					
					cctalk.CoinFlagThere=1;
					
					(void)CoinAcceptorReadBufferedCreditErrorCodes();
					
					
				}else{
					
				//init coin acceptor
					
					cctalk.CoinFlagThere=0;
					
						if(CoinAcceptorPoll()){
							
							cctalk.CoinFlagStatus = DEVICE_OK;
							Cpu_Delay100US(100);
							//request manufacturer
							CleanBufferInput();
							cctalk.FlagReceiveData=0;
							(void)CoinAcceptorReqManufacturerID();
							
							Cpu_Delay100US(100);
							//request product code
							CleanBufferInput();
							cctalk.FlagReceiveData=0;
							
							(void)CoinAcceptorReqProductCode();
							Cpu_Delay100US(100);
							//request serial number
							CleanBufferInput();
							cctalk.FlagReceiveData=0;
							
							(void)CoinAcceptorReqSerialNumber();
							Cpu_Delay100US(100);
							//request software revision
							CleanBufferInput();
							cctalk.FlagReceiveData=0;
							
							(void)CoinAcceptorReqSoftwareVersion();
							Cpu_Delay100US(100);
							//request option flags
							CleanBufferInput();
							cctalk.FlagReceiveData=0;
							(void)CoinAcceptorReqOptionFlags();
							Cpu_Delay100US(100);
							//modify inhibit status
							CleanBufferInput();
							cctalk.FlagReceiveData=0;
							(void)CoinAcceptorModInhibitStatus(0xffff);
							Cpu_Delay100US(100);
							//modify master inhibit status
							CleanBufferInput();
							cctalk.FlagReceiveData=0;
							(void)CoinAcceptorModMasterInhibitStatus(1);
							Cpu_Delay100US(100);
																					
						}
					
				}
				
				FlagTurnDevice=1;
				
			break;
			////////////////////////////////////////////////////////
				
			case 1:
				
				if(cctalk.Hopper1FlagStatus!=DEVICE_NO_RESPONSE){
					
					cctalk.Hopper1FlagThere=1;
					
					if(cctalk.Hopper1FlagStatus == DEVICE_OK){
						
						(void)HopperSimplePoll(3);
					}
					
					if(cctalk.Hopper1FlagStatus == DEVICE_PAYOUT){
						
						if(HopperReqStatus(3,&cctalk.Hopper1CounterPay,&cctalk.Hopper1CoinsPendent,&cctalk.Hopper1CoinsPayOut,&cctalk.Hopper1UnPay)){
							
							(void)FC_HOPPER_GetTimeMS(&time);
							
							if((cctalk.Hopper1CoinsPayOut == cctalk.HopperCoinsToPay) && time>8500){
								
								UTIL_Num8uToStr(string,3,cctalk.Hopper1CounterPay);
								dataprintf("CTP",3);
								dataprintf(string,3);
								UTIL_Num8uToStr(string,3,cctalk.Hopper1CoinsPendent);
								dataprintf("CPD",3);
								dataprintf(string,3);
								UTIL_Num8uToStr(string,3,cctalk.Hopper1CoinsPayOut);
								dataprintf("CPY",3);
								dataprintf(string,3);
								UTIL_Num8uToStr(string,3,cctalk.Hopper1UnPay);
								dataprintf("CUP",3);
								dataprintf(string,3);
								dataprintf("\r\n",2);
								
							}
							
						}
						(void)HopperReqTestHopper(3,&cctalk.Hopper1Test);
					}
					
					
					
				}else{
					
					cctalk.Hopper1FlagThere=0;
					
					if(HopperSimplePoll(3)){
						
						cctalk.HopperAddress[0]=3;
						CleanBufferInput();
						cctalk.FlagReceiveData=0;
						cctalk.Hopper1FlagStatus = DEVICE_OK;
						(void)HopperReqSerialNumber(3,cctalk.Hopper1SerialNumber);
						
					}
					
				}
				
				FlagTurnDevice=2;
			
			break;
			/////////////////////////////////////////////////////////
			
			case 2:
				
				if(cctalk.Hopper2FlagStatus!=DEVICE_NO_RESPONSE){
							
					cctalk.Hopper2FlagThere=1;
					
					if(cctalk.Hopper2FlagStatus == DEVICE_OK){
						
						(void)HopperSimplePoll(4);		
					}
					
					if(cctalk.Hopper2FlagStatus == DEVICE_PAYOUT){
						
						if(HopperReqStatus(4,&cctalk.Hopper2CounterPay,&cctalk.Hopper2CoinsPendent,&cctalk.Hopper2CoinsPayOut,&cctalk.Hopper2UnPay)){
							
							(void)FC_HOPPER_GetTimeMS(&time);
														
							if((cctalk.Hopper2CoinsPayOut == cctalk.HopperCoinsToPay) && time>8500){
								
								UTIL_Num8uToStr(string,3,cctalk.Hopper2CounterPay);
								dataprintf("CTP",3);
								dataprintf(string,3);
								UTIL_Num8uToStr(string,3,cctalk.Hopper2CoinsPendent);
								dataprintf("CPD",3);
								dataprintf(string,3);
								UTIL_Num8uToStr(string,3,cctalk.Hopper2CoinsPayOut);
								dataprintf("CPY",3);
								dataprintf(string,3);
								UTIL_Num8uToStr(string,3,cctalk.Hopper2UnPay);
								dataprintf("CUP",3);
								dataprintf(string,3);
								dataprintf("\r\n",2);
								
							}
							
						}	
						
						(void)HopperReqTestHopper(4,&cctalk.Hopper2Test);
						
					}
					
					
					
				}else{
					
					cctalk.Hopper2FlagThere=0;
					
					if(HopperSimplePoll(4)){
									
						cctalk.HopperAddress[1]=4;
						CleanBufferInput();
						cctalk.FlagReceiveData=0;
						cctalk.Hopper2FlagStatus = DEVICE_OK;
						(void)HopperReqSerialNumber(4,cctalk.Hopper2SerialNumber);
						
					}
					
				}
				
				FlagTurnDevice=3;
			
			break;
			////////////////////////////////////////////////////////
			
			case 3:
			
				if(cctalk.Hopper3FlagStatus!=DEVICE_NO_RESPONSE){
					
					cctalk.Hopper3FlagThere=1;
					
					if(cctalk.Hopper3FlagStatus == DEVICE_OK){
						
						(void)HopperSimplePoll(5);
						
					}
					
					if(cctalk.Hopper3FlagStatus == DEVICE_PAYOUT){
					
						if(HopperReqStatus(5,&cctalk.Hopper3CounterPay,&cctalk.Hopper3CoinsPendent,&cctalk.Hopper3CoinsPayOut,&cctalk.Hopper3UnPay)){
							
							(void)FC_HOPPER_GetTimeMS(&time);
														
							if((cctalk.Hopper3CoinsPayOut == cctalk.HopperCoinsToPay) && time>8500){
								
								UTIL_Num8uToStr(string,3,cctalk.Hopper3CounterPay);
								dataprintf("CTP",3);
								dataprintf(string,3);
								UTIL_Num8uToStr(string,3,cctalk.Hopper3CoinsPendent);
								dataprintf("CPD",3);
								dataprintf(string,3);
								UTIL_Num8uToStr(string,3,cctalk.Hopper3CoinsPayOut);
								dataprintf("CPY",3);
								dataprintf(string,3);
								UTIL_Num8uToStr(string,3,cctalk.Hopper3UnPay);
								dataprintf("CUP",3);
								dataprintf(string,3);
								dataprintf("\r\n",2);
								
							}
							
						}	
						
						(void)HopperReqTestHopper(5,&cctalk.Hopper3Test);
		
					}
								
				}else{
					
					cctalk.Hopper3FlagThere=0;
					
					if(HopperSimplePoll(5)){
									
						cctalk.HopperAddress[2]=5;
						CleanBufferInput();
						cctalk.FlagReceiveData=0;
						cctalk.Hopper3FlagStatus = DEVICE_OK;
						(void)HopperReqSerialNumber(5,cctalk.Hopper3SerialNumber);
						
					}
				}
				
				FlagTurnDevice=4;
			break;
			///////////////////////////////////////////////////
			
			case 4:
				
				if(cctalk.BillFlagStatus!=DEVICE_NO_RESPONSE){
					
					cctalk.BillFlagThere=1;
					
					(void)BillReadBufferedBillEvents();
					
				}else{
					
					cctalk.BillFlagThere=0;
					
					if(BillPoll()){
						
						cctalk.BillFlagStatus = DEVICE_OK;
						
						CleanBufferInput();
						cctalk.FlagReceiveData=0;
						(void)BillReqProductCode();
						
						Cpu_Delay100US(100);
						CleanBufferInput();
						cctalk.FlagReceiveData=0;
						(void)BillReqManufacturerID();
						
						Cpu_Delay100US(100);
						CleanBufferInput();
						cctalk.FlagReceiveData=0;
						(void)BillReqSerialNumber();
						
						Cpu_Delay100US(100);
						CleanBufferInput();
						cctalk.FlagReceiveData=0;
						(void)BillModBillOperatingMode(3);
						
						Cpu_Delay100US(100);
						CleanBufferInput();
						cctalk.FlagReceiveData=0;
						(void)BillModMasterInhibitStatus(1);
						
					}
					
				}
				
				
				FlagTurnDevice=0;
				
			break;
			
		}
			

	}
	
	
}


































