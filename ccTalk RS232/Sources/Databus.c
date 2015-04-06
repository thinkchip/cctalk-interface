/*
 * Databus.c
 *
 *  Created on: Feb 7, 2015
 *      Author: THINKCHIP
 */

/*********************************************************************
 * Como funciona el protocolo terminal
 * comandos en ascii
 * 
 * funciones disponibles
 * 
 * habilitar monedas (general) 
 * AC1+\r\n habilita
 * AC1-\r\n deshabilita
 *   
 * recepcion de monedas (no poll)
 * AC2+XX\r\n
 * 
 * recepcion de monedas (con poll y envia los ultimos 5 eventos)
 * AC2?\r\n
 * 
 * modelo
 * AC3?\r\n
 * 
 * numero de serie 
 * AC4?\r\n
 * 
 * tipo de moneda que acepta (16 tipos son 16 cadenas de 6 digitos)
 * AC5?XX\r\n
 * 
 * hoppers
 *  
 * modelo 
 * HX2?
 * 
 * numero de serie
 * HX3?
 * 
 * dispensar moneda
 * HX1+x x x\r\n
 * 
 * 
 * 
 */
#include "Databus.h"


VarDatabus data;

/*******************************************************************
 * 
 ******************************************************************/
void dataprintf(uint8_t *string,uint8_t number){
	
	uint8_t i;
	
	if(number){
			for(i=0;i<number;i++){
				while(AS_DATABUS_SendChar(string[i])!=ERR_OK);
			}		
	}
		
}
/*******************************************************************
 * 
 ******************************************************************/
void DataCleanBuffer(void){
	
	uint8_t i;
	
	for(i=0;i<=15;i++){
		data.BufferInput[i]=0;
		
	}
	data.pointer=0;
}

/*******************************************************************
 * 
 *******************************************************************/
void TaskDatabus(void){
	
	uint8_t x,y;
	uint8_t uni,dec,cen;
	uint8_t string[10];
	uint32_t serialnumber;
	////aceptador de monedas
	//habilitar monedas
	uint16_t time=0;
	
	if(data.FlagData){		
		
		if(data.pointer && data.BufferInput[data.pointer-2]=='\r' && data.BufferInput[data.pointer-1]=='\n'){
			
			data.FlagEndData=1;
			
		}
		
		data.FlagTimeUp=1;
		(void)FC_DATABUS_Reset();
		(void)FC_DATABUS_Enable();
		data.FlagData=0;
		
	}
	
	if(data.FlagEndData){
		
		data.FlagEndData=0;
		data.FlagTimeUp=0;
		(void)FC_DATABUS_Reset();
		
		if(data.BufferInput[0]=='A' && data.BufferInput[1]=='C' && data.BufferInput[2]=='1'){
			if(cctalk.CoinFlagThere){
				x=data.BufferInput[4]-0x30;
				if(CoinAcceptorModMasterInhibitStatus(x)){			
					dataprintf("OK\r\n",4);			
				}else{			
					dataprintf("ERROR\r\n",7);			
				}
			}else{
				dataprintf("NO CONNECTED\r\n",14);
			}
		}else if(data.BufferInput[0]=='A' && data.BufferInput[1]=='C' && data.BufferInput[2]=='2'){		
		//peticion de status
			//coinbuffererrorcredits
			//counter events
			if(cctalk.CoinFlagThere){
			
				cen = cctalk.CoinBufferErrorCredit[0]/100;
				dec = cctalk.CoinBufferErrorCredit[0]%100;
				uni = dec%10;
				dec /= 10;
				
				while(AS_DATABUS_SendChar('C')!=ERR_OK);
				while(AS_DATABUS_SendChar('E')!=ERR_OK);
				while(AS_DATABUS_SendChar(cen+0x30)!=ERR_OK);
				while(AS_DATABUS_SendChar(dec+0x30)!=ERR_OK);
				while(AS_DATABUS_SendChar(uni+0x30)!=ERR_OK);
				
				while(AS_DATABUS_SendChar('E')!=ERR_OK);
				while(AS_DATABUS_SendChar('V')!=ERR_OK);
				while(AS_DATABUS_SendChar('1')!=ERR_OK);
				if(!cctalk.CoinBufferErrorCredit[1]){
					//error
					while(AS_DATABUS_SendChar('E')!=ERR_OK);
					while(AS_DATABUS_SendChar('R')!=ERR_OK);
					while(AS_DATABUS_SendChar('R')!=ERR_OK);
					cen = cctalk.CoinBufferErrorCredit[2]/100;
					dec = cctalk.CoinBufferErrorCredit[2]%100;
					uni = dec%10;
					dec /= 10;
					while(AS_DATABUS_SendChar(cen+0x30)!=ERR_OK);
					while(AS_DATABUS_SendChar(dec+0x30)!=ERR_OK);
					while(AS_DATABUS_SendChar(uni+0x30)!=ERR_OK);				
				}else{
					//coin accepted
					while(AS_DATABUS_SendChar('C')!=ERR_OK);
					while(AS_DATABUS_SendChar('I')!=ERR_OK);
					dec = cctalk.CoinBufferErrorCredit[1]%100;
					uni = dec%10;
					dec /= 10;
					while(AS_DATABUS_SendChar(dec+0x30)!=ERR_OK);
					while(AS_DATABUS_SendChar(uni+0x30)!=ERR_OK);	
					while(AS_DATABUS_SendChar('R')!=ERR_OK);
					while(AS_DATABUS_SendChar('C')!=ERR_OK);
					while(AS_DATABUS_SendChar(cctalk.CoinBufferErrorCredit[2]+0x30)!=ERR_OK);
				}
				
				while(AS_DATABUS_SendChar('E')!=ERR_OK);
				while(AS_DATABUS_SendChar('V')!=ERR_OK);
				while(AS_DATABUS_SendChar('2')!=ERR_OK);
				if(!cctalk.CoinBufferErrorCredit[3]){
					//error
					while(AS_DATABUS_SendChar('E')!=ERR_OK);
					while(AS_DATABUS_SendChar('R')!=ERR_OK);
					while(AS_DATABUS_SendChar('R')!=ERR_OK);
					cen = cctalk.CoinBufferErrorCredit[4]/100;
					dec = cctalk.CoinBufferErrorCredit[4]%100;
					uni = dec%10;
					dec /= 10;
					while(AS_DATABUS_SendChar(cen+0x30)!=ERR_OK);
					while(AS_DATABUS_SendChar(dec+0x30)!=ERR_OK);
					while(AS_DATABUS_SendChar(uni+0x30)!=ERR_OK);				
				}else{
					//coin accepted
					while(AS_DATABUS_SendChar('C')!=ERR_OK);
					while(AS_DATABUS_SendChar('I')!=ERR_OK);
					dec = cctalk.CoinBufferErrorCredit[3]%100;
					uni = dec%10;
					dec /= 10;
					while(AS_DATABUS_SendChar(dec+0x30)!=ERR_OK);
					while(AS_DATABUS_SendChar(uni+0x30)!=ERR_OK);	
					while(AS_DATABUS_SendChar('R')!=ERR_OK);
					while(AS_DATABUS_SendChar('C')!=ERR_OK);
					while(AS_DATABUS_SendChar(cctalk.CoinBufferErrorCredit[4]+0x30)!=ERR_OK);
				}
				
				while(AS_DATABUS_SendChar('E')!=ERR_OK);
				while(AS_DATABUS_SendChar('V')!=ERR_OK);
				while(AS_DATABUS_SendChar('3')!=ERR_OK);
				if(!cctalk.CoinBufferErrorCredit[5]){
					//error
					while(AS_DATABUS_SendChar('E')!=ERR_OK);
					while(AS_DATABUS_SendChar('R')!=ERR_OK);
					while(AS_DATABUS_SendChar('R')!=ERR_OK);
					cen = cctalk.CoinBufferErrorCredit[6]/100;
					dec = cctalk.CoinBufferErrorCredit[6]%100;
					uni = dec%10;
					dec /= 10;
					while(AS_DATABUS_SendChar(cen+0x30)!=ERR_OK);
					while(AS_DATABUS_SendChar(dec+0x30)!=ERR_OK);
					while(AS_DATABUS_SendChar(uni+0x30)!=ERR_OK);				
				}else{
					//coin accepted
					while(AS_DATABUS_SendChar('C')!=ERR_OK);
					while(AS_DATABUS_SendChar('I')!=ERR_OK);
					dec = cctalk.CoinBufferErrorCredit[5]%100;
					uni = dec%10;
					dec /= 10;
					while(AS_DATABUS_SendChar(dec+0x30)!=ERR_OK);
					while(AS_DATABUS_SendChar(uni+0x30)!=ERR_OK);	
					while(AS_DATABUS_SendChar('R')!=ERR_OK);
					while(AS_DATABUS_SendChar('C')!=ERR_OK);
					while(AS_DATABUS_SendChar(cctalk.CoinBufferErrorCredit[6]+0x30)!=ERR_OK);
				}
				
				while(AS_DATABUS_SendChar('E')!=ERR_OK);
				while(AS_DATABUS_SendChar('V')!=ERR_OK);
				while(AS_DATABUS_SendChar('4')!=ERR_OK);
				if(!cctalk.CoinBufferErrorCredit[7]){
					//error
					while(AS_DATABUS_SendChar('E')!=ERR_OK);
					while(AS_DATABUS_SendChar('R')!=ERR_OK);
					while(AS_DATABUS_SendChar('R')!=ERR_OK);
					cen = cctalk.CoinBufferErrorCredit[8]/100;
					dec = cctalk.CoinBufferErrorCredit[8]%100;
					uni = dec%10;
					dec /= 10;
					while(AS_DATABUS_SendChar(cen+0x30)!=ERR_OK);
					while(AS_DATABUS_SendChar(dec+0x30)!=ERR_OK);
					while(AS_DATABUS_SendChar(uni+0x30)!=ERR_OK);				
				}else{
					//coin accepted
					while(AS_DATABUS_SendChar('C')!=ERR_OK);
					while(AS_DATABUS_SendChar('I')!=ERR_OK);
					dec = cctalk.CoinBufferErrorCredit[7]%100;
					uni = dec%10;
					dec /= 10;
					while(AS_DATABUS_SendChar(dec+0x30)!=ERR_OK);
					while(AS_DATABUS_SendChar(uni+0x30)!=ERR_OK);	
					while(AS_DATABUS_SendChar('R')!=ERR_OK);
					while(AS_DATABUS_SendChar('C')!=ERR_OK);
					while(AS_DATABUS_SendChar(cctalk.CoinBufferErrorCredit[8]+0x30)!=ERR_OK);
				}
				
				while(AS_DATABUS_SendChar('E')!=ERR_OK);
				while(AS_DATABUS_SendChar('V')!=ERR_OK);
				while(AS_DATABUS_SendChar('5')!=ERR_OK);
				if(!cctalk.CoinBufferErrorCredit[9]){
					//error
					while(AS_DATABUS_SendChar('E')!=ERR_OK);
					while(AS_DATABUS_SendChar('R')!=ERR_OK);
					while(AS_DATABUS_SendChar('R')!=ERR_OK);
					cen = cctalk.CoinBufferErrorCredit[10]/100;
					dec = cctalk.CoinBufferErrorCredit[10]%100;
					uni = dec%10;
					dec /= 10;
					while(AS_DATABUS_SendChar(cen+0x30)!=ERR_OK);
					while(AS_DATABUS_SendChar(dec+0x30)!=ERR_OK);
					while(AS_DATABUS_SendChar(uni+0x30)!=ERR_OK);				
				}else{
					//coin accepted
					while(AS_DATABUS_SendChar('C')!=ERR_OK);
					while(AS_DATABUS_SendChar('I')!=ERR_OK);
					dec = cctalk.CoinBufferErrorCredit[9]%100;
					uni = dec%10;
					dec /= 10;
					while(AS_DATABUS_SendChar(dec+0x30)!=ERR_OK);
					while(AS_DATABUS_SendChar(uni+0x30)!=ERR_OK);	
					while(AS_DATABUS_SendChar('R')!=ERR_OK);
					while(AS_DATABUS_SendChar('C')!=ERR_OK);
					while(AS_DATABUS_SendChar(cctalk.CoinBufferErrorCredit[10]+0x30)!=ERR_OK);
				}
	
				while(AS_DATABUS_SendChar('\r')!=ERR_OK);
				while(AS_DATABUS_SendChar('\n')!=ERR_OK);
			}else{
				dataprintf("NO CONNECTED\r\n",14);
			}
			
			
		}else if(data.BufferInput[0]=='A' && data.BufferInput[1]=='C' && data.BufferInput[2]=='3'){
				//modelo
			if(cctalk.CoinFlagThere){
				dataprintf(cctalk.CoinManufacturerID,3);
				dataprintf(cctalk.CoinProductCode,6);
				dataprintf(cctalk.CoinSoftwareVersion,14);
				while(AS_DATABUS_SendChar('\r')!=ERR_OK);
				while(AS_DATABUS_SendChar('\n')!=ERR_OK);
			}else{
				dataprintf("NO CONNECTED\r\n",14);			
			}
			
		}else if(data.BufferInput[0]=='A' && data.BufferInput[1]=='C' && data.BufferInput[2]=='4'){
			//numero de serie		
			
			if(cctalk.CoinFlagThere){
				while(AS_DATABUS_SendChar('S')!=ERR_OK);
				while(AS_DATABUS_SendChar('N')!=ERR_OK);
				serialnumber=cctalk.CoinSerialNumber[0];
				serialnumber<<=8;
				serialnumber|=cctalk.CoinSerialNumber[1];
				serialnumber<<=8;
				serialnumber|=cctalk.CoinSerialNumber[2];
				UTIL_Num32uToStr(string,7,serialnumber);
				dataprintf(string,7);			
				while(AS_DATABUS_SendChar('\r')!=ERR_OK);
				while(AS_DATABUS_SendChar('\n')!=ERR_OK);
			}else{
				dataprintf("NO CONNECTED\r\n",14);
			}
			
		}else if(data.BufferInput[0]=='A' && data.BufferInput[1]=='C' && data.BufferInput[2]=='5' && data.BufferInput[3]=='?'){
			//tipo de monedas
			if(cctalk.CoinFlagThere){
				x=(data.BufferInput[4]-0x30);
				x*=10;
				x+=(data.BufferInput[5]-0x30);
				
				if(CoinAcceptorReqCoinID(x,string)){
					dataprintf(string,6);
					
				}else{
					dataprintf("ERROR",5);	
				}
				
				while(AS_DATABUS_SendChar('\r')!=ERR_OK);
				while(AS_DATABUS_SendChar('\n')!=ERR_OK);
			}else{
				dataprintf("NO CONNECTED\r\n",14);
			}
			
		}else if(data.BufferInput[0]=='H' && data.BufferInput[1]=='1'){
		//dispense coins
			x= data.BufferInput[2]-0x30;
			cen = data.BufferInput[4]-0x30;
			dec = data.BufferInput[5]-0x30;
			uni = data.BufferInput[6]-0x30;
			
			y= (cen*100)+(dec*10)+uni;
			
			if(!HopperSendCoins(x,y)){
				dataprintf("ERROR\r\n",7);
			}
			
			
		}else if(data.BufferInput[0]=='H' && data.BufferInput[1]=='2'){	
			//modelo hopper
					
			dataprintf(cctalk.Hopper1ProductCode,6);
			dataprintf(cctalk.Hopper2ProductCode,6);
			dataprintf(cctalk.Hopper3ProductCode,6);
			
			while(AS_DATABUS_SendChar('\r')!=ERR_OK);
			while(AS_DATABUS_SendChar('\n')!=ERR_OK);
			
		}else if(data.BufferInput[0]=='B' && data.BufferInput[1]=='A' && data.BufferInput[2]=='1' && data.BufferInput[3]!='?'){
			//enable disable acceptance of bills
			if(cctalk.BillFlagThere){
				if(BillModMasterInhibitStatus((data.BufferInput[3]-0x30))){
					dataprintf("OK\r\n",4);
				}else{
					dataprintf("ERROR\r\n",7);
				}
			}else{
				dataprintf("NO CONNECTED\r\n",14);
			}
			
		}else if(data.BufferInput[0]=='B' && data.BufferInput[1]=='A' && data.BufferInput[2]=='2'){
			//request buffer events
			if(cctalk.BillFlagThere){
					
				cen = cctalk.BillBufferEvents[0]/100;
				dec = cctalk.BillBufferEvents[0]%100;
				uni = dec%10;
				dec /= 10;
				
				while(AS_DATABUS_SendChar('B')!=ERR_OK);
				while(AS_DATABUS_SendChar('E')!=ERR_OK);
				while(AS_DATABUS_SendChar(cen+0x30)!=ERR_OK);
				while(AS_DATABUS_SendChar(dec+0x30)!=ERR_OK);
				while(AS_DATABUS_SendChar(uni+0x30)!=ERR_OK);
				
				while(AS_DATABUS_SendChar('E')!=ERR_OK);
				while(AS_DATABUS_SendChar('V')!=ERR_OK);
				while(AS_DATABUS_SendChar('1')!=ERR_OK);
				if(!cctalk.BillBufferEvents[1]){
					//error
					while(AS_DATABUS_SendChar('E')!=ERR_OK);
					while(AS_DATABUS_SendChar('R')!=ERR_OK);
					while(AS_DATABUS_SendChar('R')!=ERR_OK);
					cen = cctalk.BillBufferEvents[2]/100;
					dec = cctalk.BillBufferEvents[2]%100;
					uni = dec%10;
					dec /= 10;
					while(AS_DATABUS_SendChar(cen+0x30)!=ERR_OK);
					while(AS_DATABUS_SendChar(dec+0x30)!=ERR_OK);
					while(AS_DATABUS_SendChar(uni+0x30)!=ERR_OK);				
				}else{
					//coin accepted
					while(AS_DATABUS_SendChar('B')!=ERR_OK);
					while(AS_DATABUS_SendChar('I')!=ERR_OK);
					dec = cctalk.BillBufferEvents[1]%100;
					uni = dec%10;
					dec /= 10;
					while(AS_DATABUS_SendChar(dec+0x30)!=ERR_OK);
					while(AS_DATABUS_SendChar(uni+0x30)!=ERR_OK);	
					while(AS_DATABUS_SendChar('R')!=ERR_OK);
					while(AS_DATABUS_SendChar('B')!=ERR_OK);
					while(AS_DATABUS_SendChar(cctalk.BillBufferEvents[2]+0x30)!=ERR_OK);
				}
				
				while(AS_DATABUS_SendChar('E')!=ERR_OK);
				while(AS_DATABUS_SendChar('V')!=ERR_OK);
				while(AS_DATABUS_SendChar('2')!=ERR_OK);
				if(!cctalk.BillBufferEvents[3]){
					//error
					while(AS_DATABUS_SendChar('E')!=ERR_OK);
					while(AS_DATABUS_SendChar('R')!=ERR_OK);
					while(AS_DATABUS_SendChar('R')!=ERR_OK);
					cen = cctalk.BillBufferEvents[4]/100;
					dec = cctalk.BillBufferEvents[4]%100;
					uni = dec%10;
					dec /= 10;
					while(AS_DATABUS_SendChar(cen+0x30)!=ERR_OK);
					while(AS_DATABUS_SendChar(dec+0x30)!=ERR_OK);
					while(AS_DATABUS_SendChar(uni+0x30)!=ERR_OK);				
				}else{
					//coin accepted
					while(AS_DATABUS_SendChar('B')!=ERR_OK);
					while(AS_DATABUS_SendChar('I')!=ERR_OK);
					dec = cctalk.BillBufferEvents[3]%100;
					uni = dec%10;
					dec /= 10;
					while(AS_DATABUS_SendChar(dec+0x30)!=ERR_OK);
					while(AS_DATABUS_SendChar(uni+0x30)!=ERR_OK);	
					while(AS_DATABUS_SendChar('R')!=ERR_OK);
					while(AS_DATABUS_SendChar('B')!=ERR_OK);
					while(AS_DATABUS_SendChar(cctalk.BillBufferEvents[4]+0x30)!=ERR_OK);
				}
				
				while(AS_DATABUS_SendChar('E')!=ERR_OK);
				while(AS_DATABUS_SendChar('V')!=ERR_OK);
				while(AS_DATABUS_SendChar('3')!=ERR_OK);
				if(!cctalk.BillBufferEvents[5]){
					//error
					while(AS_DATABUS_SendChar('E')!=ERR_OK);
					while(AS_DATABUS_SendChar('R')!=ERR_OK);
					while(AS_DATABUS_SendChar('R')!=ERR_OK);
					cen = cctalk.BillBufferEvents[6]/100;
					dec = cctalk.BillBufferEvents[6]%100;
					uni = dec%10;
					dec /= 10;
					while(AS_DATABUS_SendChar(cen+0x30)!=ERR_OK);
					while(AS_DATABUS_SendChar(dec+0x30)!=ERR_OK);
					while(AS_DATABUS_SendChar(uni+0x30)!=ERR_OK);				
				}else{
					//coin accepted
					while(AS_DATABUS_SendChar('B')!=ERR_OK);
					while(AS_DATABUS_SendChar('I')!=ERR_OK);
					dec = cctalk.BillBufferEvents[5]%100;
					uni = dec%10;
					dec /= 10;
					while(AS_DATABUS_SendChar(dec+0x30)!=ERR_OK);
					while(AS_DATABUS_SendChar(uni+0x30)!=ERR_OK);	
					while(AS_DATABUS_SendChar('R')!=ERR_OK);
					while(AS_DATABUS_SendChar('B')!=ERR_OK);
					while(AS_DATABUS_SendChar(cctalk.BillBufferEvents[6]+0x30)!=ERR_OK);
				}
				
				while(AS_DATABUS_SendChar('E')!=ERR_OK);
				while(AS_DATABUS_SendChar('V')!=ERR_OK);
				while(AS_DATABUS_SendChar('4')!=ERR_OK);
				if(!cctalk.BillBufferEvents[7]){
					//error
					while(AS_DATABUS_SendChar('E')!=ERR_OK);
					while(AS_DATABUS_SendChar('R')!=ERR_OK);
					while(AS_DATABUS_SendChar('R')!=ERR_OK);
					cen = cctalk.BillBufferEvents[8]/100;
					dec = cctalk.BillBufferEvents[8]%100;
					uni = dec%10;
					dec /= 10;
					while(AS_DATABUS_SendChar(cen+0x30)!=ERR_OK);
					while(AS_DATABUS_SendChar(dec+0x30)!=ERR_OK);
					while(AS_DATABUS_SendChar(uni+0x30)!=ERR_OK);				
				}else{
					//coin accepted
					while(AS_DATABUS_SendChar('B')!=ERR_OK);
					while(AS_DATABUS_SendChar('I')!=ERR_OK);
					dec = cctalk.BillBufferEvents[7]%100;
					uni = dec%10;
					dec /= 10;
					while(AS_DATABUS_SendChar(dec+0x30)!=ERR_OK);
					while(AS_DATABUS_SendChar(uni+0x30)!=ERR_OK);	
					while(AS_DATABUS_SendChar('R')!=ERR_OK);
					while(AS_DATABUS_SendChar('B')!=ERR_OK);
					while(AS_DATABUS_SendChar(cctalk.BillBufferEvents[8]+0x30)!=ERR_OK);
				}
				
				while(AS_DATABUS_SendChar('E')!=ERR_OK);
				while(AS_DATABUS_SendChar('V')!=ERR_OK);
				while(AS_DATABUS_SendChar('5')!=ERR_OK);
				if(!cctalk.BillBufferEvents[9]){
					//error
					while(AS_DATABUS_SendChar('E')!=ERR_OK);
					while(AS_DATABUS_SendChar('R')!=ERR_OK);
					while(AS_DATABUS_SendChar('R')!=ERR_OK);
					cen = cctalk.BillBufferEvents[10]/100;
					dec = cctalk.BillBufferEvents[10]%100;
					uni = dec%10;
					dec /= 10;
					while(AS_DATABUS_SendChar(cen+0x30)!=ERR_OK);
					while(AS_DATABUS_SendChar(dec+0x30)!=ERR_OK);
					while(AS_DATABUS_SendChar(uni+0x30)!=ERR_OK);				
				}else{
					//coin accepted
					while(AS_DATABUS_SendChar('B')!=ERR_OK);
					while(AS_DATABUS_SendChar('I')!=ERR_OK);
					dec = cctalk.BillBufferEvents[9]%100;
					uni = dec%10;
					dec /= 10;
					while(AS_DATABUS_SendChar(dec+0x30)!=ERR_OK);
					while(AS_DATABUS_SendChar(uni+0x30)!=ERR_OK);	
					while(AS_DATABUS_SendChar('R')!=ERR_OK);
					while(AS_DATABUS_SendChar('B')!=ERR_OK);
					while(AS_DATABUS_SendChar(cctalk.BillBufferEvents[10]+0x30)!=ERR_OK);
				}
	
				while(AS_DATABUS_SendChar('\r')!=ERR_OK);
				while(AS_DATABUS_SendChar('\n')!=ERR_OK);
			}else{
				dataprintf("NO CONNECTED\r\n",14);
			}
			
		}else if(data.BufferInput[0]=='B' && data.BufferInput[1]=='A' && data.BufferInput[2]=='3'){
			//route bill
			if(cctalk.BillFlagThere){
				if(BillRouteBill((data.BufferInput[3]-0x30),&x)){
					if(!x){
						dataprintf("OK\r\n",4);
					}else{
						dataprintf("ESCROW FAIL\r\n",13);	
					}
				}else{
					dataprintf("ERROR\r\n",7);
				}
			}else{
				dataprintf("NO CONNECTED\r\n",14);
			}
			
		}else if(data.BufferInput[0]=='B' && data.BufferInput[1]=='A' && data.BufferInput[2]=='4'){
			//model and product code
			if(cctalk.BillFlagThere){
				dataprintf(cctalk.BillManufacturerID,3);
				dataprintf(cctalk.BillProductCode,6);
				while(AS_DATABUS_SendChar('\r')!=ERR_OK);
				while(AS_DATABUS_SendChar('\n')!=ERR_OK);
			}else{
				dataprintf("NO CONNECTED\r\n",14);			
			}
			
		}else if(data.BufferInput[0]=='B' && data.BufferInput[1]=='A' && data.BufferInput[2]=='5'){
			//serial number
			if(cctalk.BillFlagThere){
				while(AS_DATABUS_SendChar('S')!=ERR_OK);
				while(AS_DATABUS_SendChar('N')!=ERR_OK);
				serialnumber=cctalk.BillSerialNumber[0];
				serialnumber<<=8;
				serialnumber|=cctalk.BillSerialNumber[1];
				serialnumber<<=8;
				serialnumber|=cctalk.BillSerialNumber[2];
				UTIL_Num32uToStr(string,7,serialnumber);
				dataprintf(string,7);			
				while(AS_DATABUS_SendChar('\r')!=ERR_OK);
				while(AS_DATABUS_SendChar('\n')!=ERR_OK);
			}else{
				dataprintf("NO CONNECTED\r\n",14);
			}
		}else if(data.BufferInput[0]=='S' && data.BufferInput[1]=='M' && data.BufferInput[2]=='1' && data.BufferInput[3]=='?'){
		//request mode operation
			(void)EEPROM_GetPage(EEPROM_AREA_START);
			(void)EEPROM_GetBytePage(ADDR_SYSTEM_MODE,&x);
			if(x){
				dataprintf("EVENT\r\n",7);
			}else{
				dataprintf("POLL\r\n",6);
			}
			
		}else if(data.BufferInput[0]=='S' && data.BufferInput[1]=='M' && data.BufferInput[2]=='1' && data.BufferInput[3]!='?'){
		//save mode operation
			(void)EEPROM_GetPage(EEPROM_AREA_START);
			(void)EEPROM_SetBytePage(ADDR_SYSTEM_MODE,(data.BufferInput[3]-0x30));
			(void)EEPROM_SetPage(EEPROM_AREA_START);
			dataprintf("OK\r\n",4);
			
		}else if(data.BufferInput[0]=='S' && data.BufferInput[1]=='M' && data.BufferInput[2]=='2'){
		//poll general
			cen = data.BufferInput[4]-0x30;
			cen *=100;
			dec = data.BufferInput[5]-0x30;
			dec *= 10;
			uni = data.BufferInput[6]-0x30;
			
			x= cen+dec+uni;
			
			if(HopperSimplePoll(x)){
				dataprintf("OK ID\r\n",7);
			}else{
				dataprintf("NO ID ERROR\r\n",13);
			}
			
		}else if(data.BufferInput[0]=='S' && data.BufferInput[1]=='M' && data.BufferInput[2]=='3'){
			//change id address
			cen = data.BufferInput[3]-0x30;
			cen *=100;
			dec = data.BufferInput[4]-0x30;
			dec *= 10;
			uni = data.BufferInput[5]-0x30;
			
			x= cen+dec+uni;
			
			cen = data.BufferInput[7]-0x30;
			cen *=100;
			dec = data.BufferInput[8]-0x30;
			dec *= 10;
			uni = data.BufferInput[9]-0x30;
			
			y= cen+dec+uni;
			
			if(DeviceChangeAddress(x,y)){
				dataprintf("OK\r\n",4);
			}else{
				dataprintf("ERROR\r\n",7);
			}
			
			
		}else if(data.BufferInput[0]=='S' && data.BufferInput[1]=='M' && data.BufferInput[2]=='4' && data.BufferInput[3]!='?'){
			//mode bill acceptor
			(void)EEPROM_GetPage(EEPROM_AREA_START);
			(void)EEPROM_SetBytePage(ADDR_BILL_MODE,(data.BufferInput[3]-0x30));
			(void)EEPROM_SetPage(EEPROM_AREA_START);
			dataprintf("OK\r\n",4);			
			
		}else if(data.BufferInput[0]=='S' && data.BufferInput[1]=='M' && data.BufferInput[2]=='4' && data.BufferInput[3]=='?'){
			//request mode bill acceptor
			(void)EEPROM_GetPage(EEPROM_AREA_START);
			(void)EEPROM_GetBytePage(ADDR_BILL_MODE,&x);
			if(x){
				dataprintf("STACKER\r\n",9);
			}else{
				dataprintf("ESCROW\r\n",8);
			}
			
		}else if(data.BufferInput[0]=='S' && data.BufferInput[1]=='M' && data.BufferInput[2]=='5'){
		
		
		
			
		}else{
			
			dataprintf("ERROR\r\n",7);	
			
		}
		
		DataCleanBuffer();
		
		
	}
	
	if(data.FlagTimeUp){
		(void)FC_DATABUS_GetTimeSec(&time);
		
		if(time>5){
			
			DataCleanBuffer();
			(void)FC_DATABUS_Disable();
			dataprintf("ERROR\r\n",7);
			data.FlagTimeUp=0;
			
		}
	}
	
	
}

