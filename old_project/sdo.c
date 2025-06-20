#include "application_data.h"


extern __IO uint32_t tx_buf;

extern AppHandle_t *locHandle;


uint8_t SDO_MsgHandler(uint32_t MsgID, void *Data, int8_t DataSize)
{
	CAN_TxMsgTypeDef TxMsg;
	
	data_SDO_un rx;
	data_SDO_un tx;
	
	memcpy(rx.data, Data, 8);

	if (MsgID == ID_SDO_MASTER)
	{
		TxMsg.ID 			= ID_SDO_SLAVE;
		TxMsg.IDE     = CAN_ID_EXT;
		TxMsg.DLC     = 8;
		TxMsg.PRIOR_0 = DISABLE;
		
		uint8_t subindex =(uint8_t)((rx.sdoMsg.subindex >> 8) + rx.sdoMsg.subindex);
		uint8_t index = rx.sdoMsg.index;
		
		tx.sdoMsg.index = index;
		tx.sdoMsg.subindex = subindex << 8;
		
		uint32_t data = rx.sdoMsg.data;
		
		switch(rx.sdoMsg.css)
		{
			case CSS_WRITE_FALSH:
			{
			 tx.sdoMsg.css = diag_parameter_set (index, subindex, data);
				
			 if (tx.sdoMsg.css != CSS_OK)
       tx.sdoMsg.css = CSS_ERROR;
				 
			 tx.sdoMsg.data = 0;
			}
			case CSS_READ_FALSH:
			{
				tx.sdoMsg.n = 0;
				int32_t error_flag = 0;
				tx.sdoMsg.data = diag_parameter_get( index, subindex,  &error_flag);
				
				if(error_flag == 0)
					tx.sdoMsg.css = CSS_OK;
  			else
					tx.sdoMsg.css = CSS_ERROR;
					
				 for (int i = 0; i < 4; i += 1)
				{
				 if ((tx.sdoMsg.data >> 8*i) == 0)
					tx.sdoMsg.n++;
				} 
			}	
			case CSS_ALL_WRITE_FALSH:
			{
				if (write_to_eeprom() == 0)
					tx.sdoMsg.css = CSS_OK;
				else
					tx.sdoMsg.css = CSS_ERROR;	
			}				
		}		
		memcpy(TxMsg.Data,tx.data, 8);
		CAN_Transmit(pCAN_2, tx_buf, &TxMsg);
		
		return 0;
	}	
  return 1;
}

//typedef struct {
//	uint8_t UnitIndex;
//	commCAN_Ports_e CANportNum;
//} DiagData_t;

//static DiagData_t _Diagnostic;

//typedef enum
//{
//	can_cs_req_write = 0x20,
//	can_cs_req_read = 0x40,
//	can_cs_resp_write = 0x60,
//	can_cs_resp_read = 0x40,
//	can_cs_resp_fault = 0x80,
//	can_cs_req_fault = 0x80,
//} CAN_msg_types;

//typedef struct {
//	uint32_t
//		CS			: 8,
//		Index		: 16,
//		SubIndex	: 8;

//	int32_t Value;
//} displayTxMsg_t;


//static int diagnostic_can_handler(int32_t msg_id, void *Data, int8_t DataSize) {
//	if(msg_id == 0x600 + _Diagnostic.UnitIndex) {
//		int msg_tx_id = 0x580 + _Diagnostic.UnitIndex;
//		uint8_t data_tx[8] = {0, 0, 0, 0, 0, 0, 0, 0};
//		displayTxMsg_t *d = (displayTxMsg_t*)Data;
//		displayTxMsg_t *d_tx = (displayTxMsg_t*)data_tx;

//		if(d->CS == can_cs_req_read) {
//			d_tx->Index = d->Index;
//			d_tx->SubIndex = d->SubIndex;

//			int32_t error_flag = 0;
//			d_tx->Value = diag_parameter_get(d->Index, d->SubIndex, &error_flag);
//			if(error_flag == 0)
//				d_tx->CS = can_cs_resp_read;
//			else
//				d_tx->CS = can_cs_resp_fault;

//		}
//		else if(d->CS == can_cs_req_write) {
//			d_tx->Index = d->Index;
//			d_tx->SubIndex = d->SubIndex;

//			int32_t error = diag_parameter_set(d->Index, d->SubIndex, d->Value);
//			if(error == 0) {
//				d_tx->CS = can_cs_resp_write;
//				d_tx->Value = diag_parameter_get(d->Index, d->SubIndex, &error);
//			}
//			else {
//				d_tx->CS = can_cs_resp_fault;
//				d_tx->Value = 0;
//			}
//		}
//		else
//		{
//			d_tx->CS = can_cs_resp_fault;
//			d_tx->Index = d->Index;
//			d_tx->SubIndex = d->SubIndex;
//			d_tx->Value = 0;
//		}

//		commCAN_AddTxMsg(_Diagnostic.CANportNum, msg_tx_id, (void*)data_tx, sizeof(data_tx));

//		return 0;
//	}

//	return 1;

//}
