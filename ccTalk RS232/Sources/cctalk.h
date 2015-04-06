/*
 * cctalk.h
 *
 *  Created on: Aug 19, 2014
 *      Author: THINKCHIP
 */

#ifndef CCTALK_H_
#define CCTALK_H_

#include "Cpu.h"
#include "AS_CCTALK.h"
#include "FC_CCTALK.h"
#include "WDOG.h"
#include "EEPROM.h"
#include "FC_HOPPER.h"
#include "UTIL.h"

#include "Utility.h"
#include "EEpromAddress.h"
#include "Databus.h"

#define CCTALK_FACTORY_SET  	255
#define CCTALK_SIMPLE_POLL 		254
#define CCTALK_ADDRESS_POLL 	253
#define CCTALK_ADDRESS_CLASH 	252
#define CCTALK_ADDRESS_CHANGE   251
#define CCTALK_ADDRESS_RANDOM   250
#define CCTALK_REQ_POLL_PRIO    249
#define CCTALK_REQ_STATUS       248
#define CCTALK_REQ_VAR_SET      247
#define CCTALK_REQ_MANU_ID      246
#define CCTALK_REQ_EQUIP_CAT_ID 245
#define CCTALK_REQ_PROD_CODE    244
#define CCTALK_REQ_DATA_VER     243
#define CCTALK_REQ_SERIAL_NUM   242
#define CCTALK_REQ_SOFT_VER     241
#define CCTALK_TEST_SOLENOID    240
#define CCTALK_OPERATE_MOTOR    239
#define CCTALK_READ_OUT_LINE    238
#define CCTALK_READ_INP_LINE    237
#define CCTALK_READ_OPTO_STATE  236
#define CCTALK_READ_LAST_CRED   235
#define CCTALK_ISSUE_CODE       234
#define CCTALK_LATCH_OUT_LIN    233
#define CCTALK_PERFORM_SELF     232
#define CCTALK_MODIFY_INH_STA   231
#define CCTALK_REQ_INH_STA      230
#define CCTALK_READ_BUFF_CRED   229
#define CCTALK_MOD_MAST_INH_STA 228
#define CCTALK_REQ_MAST_INH_STA 227
#define CCTALK_REQ_INSER_COUNT  226
#define CCTALK_REQ_ACCEP_COUNT  225
#define CCTALK_DISP_COIN        224
#define CCTALK_DISP_CHANGE      223
#define CCTALK_MODI_SORT_OV_ST  222
#define CCTALK_REQ_SOR_OV_ST    221
#define CCTALK_ONESHOT_CREDIT   220
#define CCTALK_ENT_NEW_PIN      219
#define CCTALK_ENT_PIN          218
#define CCTALK_REQ_PAYOUT       217
#define CCTALK_REQ_DAT_STOR_AVA 216
#define CCTALK_READ_DATA_BLOCK  215
#define CCTALK_WRITE_DAT_BLOCK  214
#define CCTALK_REQ_OPT_FLAGS    213
#define CCTALK_REQ_COIN_POS     212
#define CCTALK_POWER_MAN_CON    211
#define CCTALK_MOD_SORT_PAT     210
#define CCTALK_REQ_SORT_PAT     209
#define CCTALK_MOD_PAY_ABS_COU  208
#define CCTALK_REQ_PAY_ABS_COU  207
#define CCTALK_EMPTY_PAYOUT     206
#define CCTALK_REQ_AUDIT_INF_B  205
#define CCTALK_METER_CONTROL    204
#define CCTALK_DISP_CONTROL     203
#define CCTALK_TEACH_MODE_CON   202
#define CCTALK_REQ_TEACH_MODE   201
#define CCTALK_UPLOAD_COIN_DATA 200
#define CCTALK_CONFIG_EEPROM    199
#define CCTALK_COUN_EEPROM      198
#define CCTALK_CALC_ROM_CHE     197
#define CCTALK_REQ_CREAT_DATE   196
#define CCTALK_REQ_LAST_MOD_DA  195
#define CCTALK_REQ_REJECT_COUN  194
#define CCTALK_FRAUD_COUN       193
#define CCTALK_BUILD_CODE       192
#define CCTALK_KEYPAD_CONTROL   191
#define CCTALK_REQ_PAY_STATUS   190
#define CCTALK_MOD_DEF_SORT_PAT 189
#define CCTALK_REQ_DEF_SORT_PAT 188
#define CCTALK_MOD_PAY_CAP      187
#define CCTALK_REQ_PAY_CAP      186
#define CCTALK_MOD_COIN_ID      185
#define CCTALK_REQ_COIN_ID      184
#define CCTALK_UPL_WIND_DATA    183
#define CCTALK_DOWN_CAL_INFO    182
#define CCTALK_MOD_SEC_SETT     181
#define CCTALK_REQ_SECU_SETT    180
#define CCTALK_MOD_BANK_SEL     179
#define CCTALK_REQ_BANK_SEL     178
#define CCTALK_HAND_FUN         177
#define CCTALK_REQ_ALA_COUN     176
#define CCTALK_MOD_PAY_FLOAT    175
#define CCTALK_REQ_PAY_FLOAT    174
#define CCTALK_REQ_THER_READ    173
#define CCTALK_EMERG_STOP       172
#define CCTALK_REQ_HOPP_COIN    171
#define CCTALK_REQ_BASE_YEAR    170
#define CCTALK_REQ_ADDR_MODE    169
#define CCTALK_REQ_HOPP_DISP_M  168
#define CCTALK_DISP_HOPP_COIN   167
#define CCTALK_REQ_HOPP_STATUS  166
#define CCTALK_MOD_VAR_SET      165
#define CCTALK_ENABLE_HOPP      164
#define CCTALK_TEST_HOPP        163
#define CCTALK_MOD_INH_OVER_REG 162
#define CCTALK_PUMP_RNG         161
#define CCTALK_REQ_CIP_KEY		160
#define CCTALK_BUF_BILL_EV      159
#define CCTALK_MOD_BILL_ID      158
#define CCTALK_REQ_BILL_ID      157
#define CCTALK_REQ_COUNT_SCALE  156
#define CCTALK_REQ_BILL_POS     155
#define CCTALK_ROUTE_BILL       154
#define CCTALK_MOD_BILL_OPE_MO  153
#define CCTALK_REQ_BILL_OPE_MO  152
#define CCTALK_TEST_LAMPS       151
#define CCTALK_REQ_IND_ACC_COU  150
#define CCTALK_REQ_IND_ERR_COU  149
#define CCTALK_READ_OPTO_VOL    148
#define CCTALK_PERF_STA_CY      147
#define CCTALK_OPE_BID_MOTOR    146
#define CCTALK_REQ_CURR_REV     145
#define CCTALK_UPL_BILL_TAB     144
#define CCTALK_BEG_BILL_TAB_UP  143
#define CCTALK_FIN_BILL_TAB_UP  142
#define CCTALK_REQ_FIRM_UP_CAP  141
#define CCTALK_UPLOAD_FIRM      140
#define CCTALK_BEG_FIRM_UP      139
#define CCTALK_FIN_FIRM_UP      138
#define CCTALK_SWI_ENC_CODE     137
#define CCTALK_STO_ENC_CODE     136
#define CCTALK_SET_ACCEPT_LIM   135
#define CCTALK_DISP_HOPP_VAL    134
#define CCTALK_REQ_HOPP_POLL_VA 133
#define CCTALK_EME_STOP_VAL     132
#define CCTALK_REQ_HOPP_COIN_V  131
#define CCTALK_REQ_IND_HOPP_DI  130
#define CCTALK_READ_BARC_DATA   129
#define CCTALK_REQ_MONEY_IN     128
#define CCTALK_REQ_MONEY_OUT    127
#define CCTALK_CLEAR_MON_COUN   126
#define CCTALK_PAY_MONEY_OUT    125
#define CCTALK_VERI_MONEY_OUT   124
#define CCTALK_REQ_ACT_REG      123
#define CCTALK_REQ_ERR_STATUS   122
#define CCTALK_PURGE_HOPP       121
#define CCTALK_MOD_HOPP_BAL     120
#define CCTALK_REQ_HOPP_BAL     119
#define CCTALK_MOD_CASH_VAL     118
#define CCTALK_REQ_CASH_VAL     117
#define CCTALK_MOD_REAL_TIM_CL  116
#define CCTALK_REQ_REAL_TIM_CL  115
#define CCTALK_REQ_USB_ID       114
#define CCTALK_SWITCH_BAUD_RATE 113
#define CCTALK_READ_ENC_EVE     112
#define CCTALK_REQ_ENC_EVE      111
#define CCTALK_SWITCH_ENC_KEY   110
#define CCTALK_REQ_ENC_HOPP_ST  109
#define CCTALK_REQ_ENC_MON_ID   108
#define CCTALK_REQ_COMMS_REV         4
#define CCTALK_CLEAR_COMMS_ST_VAR    3
#define CCTALK_REQ_COMMS_ST_VAR      2
#define CCTALK_RESET_DEVICE          1
#define CCTALK_REPLY                 0

#define MASTER 1
#define COIN_ACCEPTOR 2
#define HOPPER1       3
#define HOPPER2       4  
#define HOPPER3		  5
#define BILL          40

#define INIT 						0x10
#define INIT_REQ_MANUFACTURER_CODE 	0x11
#define INIT_REQ_PROD_CODE  		0x12
#define INIT_REQ_SER_NUM    		0x13
#define INIT_OPT_FLAGS   			0x14
#define INIT_MOD_INH_STATUS 		0x15
#define INIT_MOD_MAST_INH_STATUS	0x16
#define INIT_REQ_MAST_INH_STATUS	0x17
#define INIT_COIN_ID     			0x18
#define INIT_REQ_INH_STATUS			0x19
#define INIT_READ_BUFF_CREDIT		0x1a

#define HOPPER_ENABLE  0xa5
#define HOPPER_DISABLE 0x00

#define DEVICE_NO_RESPONSE 0
#define DEVICE_OK          1

#define DEVICE_PAYOUT  2
#define DEVICE_ERROR_PAYOUT  3


typedef struct{
	
	uint8_t BufferInput[255];
	uint8_t PointerInput;
	uint8_t BufferOutput[35];
	uint8_t FlagReceiveData;
	uint8_t FlagStatus;
	uint8_t PointerEcho;
	uint16_t SumData;
	uint8_t FlagPoll;
	
	uint8_t CoinFlagThere;
	uint8_t Hopper1FlagThere;
	uint8_t Hopper2FlagThere;
	uint8_t Hopper3FlagThere;
	uint8_t BillFlagThere;
	
	uint8_t CoinFlagStatus;
	uint8_t CoinManufacturerID[3];
	uint8_t CoinProductCode[6];
	uint8_t CoinSerialNumber[3];
	uint8_t CoinSoftwareVersion[14];
	uint8_t CoinOptionFlags;
	uint16_t CoinCreditInserted;
	uint8_t CoinErrorCodes[5];
	uint8_t CoinValueAccepted[10];
	uint8_t CoinFlagError;
	uint8_t CoinBufferErrorCredit[11];
	uint8_t CoinFlagEnabled;	
	
	uint8_t HopperAddress[3];
	uint8_t HopperCoinsToPay;
	uint8_t Hopper1SerialNumber[3];
	uint8_t Hopper2SerialNumber[3];
	uint8_t Hopper3SerialNumber[3];
	uint8_t Hopper1ProductCode[6];
	uint8_t Hopper2ProductCode[6];
	uint8_t Hopper3ProductCode[6];
	uint8_t Hopper1CounterPay;
	uint8_t Hopper2CounterPay;
	uint8_t Hopper3CounterPay;
	uint8_t Hopper1CoinsPendent;
	uint8_t Hopper2CoinsPendent;
	uint8_t Hopper3CoinsPendent;
	uint8_t Hopper1CoinsPayOut;
	uint8_t Hopper2CoinsPayOut;
	uint8_t Hopper3CoinsPayOut;
	uint8_t Hopper1UnPay;
	uint8_t Hopper2UnPay;
	uint8_t Hopper3UnPay;
	uint8_t Hopper1Test;
	uint8_t Hopper2Test;
	uint8_t Hopper3Test;	
	uint8_t Hopper1FlagStatus;
	uint8_t Hopper2FlagStatus;
	uint8_t Hopper3FlagStatus;
	
	uint8_t BillFlagStatus;
	uint8_t BillFlagMode;
	uint8_t BillSerialNumber[3];
	uint8_t BillProductCode[6];
	uint8_t BillManufacturerID[3];
	uint16_t BillScalingFactor;
	uint8_t BillDecimalPlaces;
	uint8_t BillBufferEvents[11];
	uint8_t BillBarcode[18];
	
	
}VARccTalk;


extern VARccTalk cctalk;

//public functions
//void InitccTalk(void);
void TaskccTalk(void);

uint8_t CoinAcceptorReqSerialNumber(void);
uint8_t CoinAcceptorReqProductCode(void);
uint8_t CoinAcceptorModMasterInhibitStatus(uint8_t action);
uint8_t CoinAcceptorReqCoinID(uint8_t id,uint8_t *string);
uint8_t HopperSimplePoll(uint8_t id);
uint8_t DeviceChangeAddress(uint8_t address_old, uint8_t address_new);
void HopperScanID(void);
uint8_t HopperSendCoins(uint8_t id,uint8_t number_coins);

uint8_t BillReqBillTypeID(uint8_t id,uint8_t *string);
uint8_t BillModBillOperatingMode(uint8_t data);
uint8_t BillModMasterInhibitStatus(uint8_t action);
uint8_t BillReadBufferedBillEvents(void);
uint8_t BillRouteBill(uint8_t data,uint8_t *response);
uint8_t BillReqBarcode(uint8_t *string);

#endif /* CCTALK_H_ */
