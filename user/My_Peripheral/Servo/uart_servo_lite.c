/**
JOHO UART�������_���������
��SDK
***/

#include "uart_servo_lite.h"

uint8_t usart1SendBuf[USART_SEND_BUF_SIZE+1];
uint8_t usart1RecvBuf[USART_RECV_BUF_SIZE+1];
RingBufferTypeDef usart1SendRingBuf;
RingBufferTypeDef usart1RecvRingBuf;
Usart_DataTypeDef usart1;

Usart_DataTypeDef* servoUsart = &usart1;


// �����ڷ��ͻ�����������ȫ������ȥ
void Usart_SendAll(Usart_DataTypeDef *usart){
	uint8_t value;
	while(RingBuffer_GetByteUsed(usart->sendBuf)){
		value = RingBuffer_Pop(usart->sendBuf);
		// printf("Usart_SendAll pop: %d", value);
        HAL_UART_Transmit(usart->pUSARTx,&value, sizeof(value),HAL_MAX_DELAY);
	}
}


//���첢������Ҫ������֡ ����ͷ	ID��	���ݳ���	ָ������	����	����У�顿
void JOHO_PackageBuild_Send(Usart_DataTypeDef *usart, uint8_t usId, uint8_t size,uint8_t cmdType, uint8_t *content){
  // ����ͨ�������ڴ棬��ö�̬���ݰ���С���˴����ݰ���С�̶�
    PackageTypeDef pkg;
	
    // ����֡ͷ
    pkg.header = JOHO_PACK_REQUEST_HEADER;
    // ����ID��
    pkg.usId = usId;
    // ���ݳ���		
    pkg.size = size;
	
	  //ָ������
		pkg.sstat = cmdType;
		// ��һ�����������������
		for(int i=0; i<size-2; i++){
			pkg.content[i] = content[i];
		}
    // ��pkg���͵����ͻ�����sendBuf����
    JOHO_Package2RingBuffer(&pkg, usart->sendBuf);
		// ͨ�����ڽ����ݷ��ͳ�ȥ
    Usart_SendAll(usart);
}




// ��������֡-HEX
void USL_Send_HEX(Usart_DataTypeDef *usart, uint8_t size, uint8_t *content){

	
	RingBuffer_WriteByteArray( usart->sendBuf,content,size);
	
	// ͨ�����ڽ����ݷ��ͳ�ȥ
    Usart_SendAll(usart);
}


//��������֡
JOHO_STATUS USL_RecvPackage(Usart_DataTypeDef *usart,PackageTypeDef *pkg){
	
		pkg->status = 0; // Package״̬��ʼ��	
    uint8_t bIdx = 0; // ���յĲ����ֽ�������
    uint16_t header = 0; // ֡ͷ
	
	
		// ����ʱ��ʼ,100
    Tick_CountdownBegin(JOHO_TIMEOUT_MS);
		// ���û�г�ʱ
    while (!Tick_CountdownIsTimeout()){
			if (RingBuffer_GetByteUsed(usart->recvBuf) == 0){
			// û���µ��ֽڶ�����, �����ȴ�
            continue;
        }

        if(pkg->status == JOHO_RECV_FLAG_CONTENT){
            // �������ݽ������
            // ����У����
            pkg->checksum = RingBuffer_ReadByte(usart->recvBuf);
            // ����״̬��-У�����Ѿ�����
            pkg->status = pkg->status | JOHO_RECV_FLAG_CHECKSUM;
            // ֱ�ӽ���У������
            if (JOHO_CalcChecksum(pkg) != pkg->checksum){
                // ��������ʱ
                Tick_CountdownCancel();
                // У�������
                return JOHO_STATUS_CHECKSUM_ERROR;
            }else{
                // ��������ʱ
                Tick_CountdownCancel();
                // ����֡���ճɹ�
                return JOHO_STATUS_SUCCESS;
            }
        }else if(pkg->status == JOHO_RECV_FLAG_SSTAT){
            // ״̬�Ѿ��������
            // ���ղ����ֽ�
				
            pkg->content[bIdx] = RingBuffer_ReadByte(usart->recvBuf);
            bIdx ++;
            // �ж��Ƿ�������
            if (bIdx == (pkg->size-2)){
                // ���Ϊ�����������
                pkg->status = JOHO_RECV_FLAG_CONTENT;
            }
        }
				else if(pkg->status == JOHO_RECV_FLAG_SIZE){
				// Size�Ѿ��������
					//����״̬
					pkg->sstat = RingBuffer_ReadByte(usart->recvBuf);
					pkg->status = JOHO_RECV_FLAG_SSTAT;

					if((pkg->size-2)==0)
					{// ���Ϊ�����������
                pkg->status = JOHO_RECV_FLAG_CONTENT;}
				}
				else if(pkg->status == JOHO_RECV_FLAG_US_ID){
            // ���ID�������
            // ���ճߴ���Ϣ
            pkg->size = RingBuffer_ReadByte(usart->recvBuf);
            // �жϳ���size�Ƿ�Ϸ�
            // ������size����JOHO_PACK_RESPONSE_MAX_SIZE���������
            if (pkg->size > (JOHO_PACK_RESPONSE_MAX_SIZE - 5)){
                // ��������ʱ
                Tick_CountdownCancel();
                return JOHO_STATUS_SIZE_TOO_BIG;
            }
            // ���óߴ������ɵı�־λ
            pkg->status = JOHO_RECV_FLAG_SIZE;
        }else if(pkg->status == JOHO_RECV_FLAG_HEADER){
            // ֡ͷ�ѽ��� 
            // ���ն��ID
            pkg->usId = RingBuffer_ReadByte(usart->recvBuf);
            // �ж�ָ���Ƿ�Ϸ�
            // �жϿ���ָ���Ƿ���Ч ָ�Χ����
            if (pkg->usId > JOHO_US_NUM){
                // ��������ʱ
                Tick_CountdownCancel();
                return JOHO_STATUS_UNKOWN_US_ID;
            }
            // ����usId�Ѿ����յ���־λ
            pkg->status = JOHO_RECV_FLAG_US_ID;
        }else{
            // ����֡ͷ
            if (header == 0){
                // ���յ�һ���ֽ�
                header = RingBuffer_ReadByte(usart->recvBuf);

                // �жϽ��յĵ�һ���ֽ��Ƿ���ȷ
                if (header != (JOHO_PACK_RESPONSE_HEADER&0xff)){
										// �����һ���ֽڴ��� header����Ϊ0
                    header = 0;
                }
            }else if(header == (JOHO_PACK_RESPONSE_HEADER&0xFF)){
                // ����֡ͷ�ڶ����ֽ�
                header =  header | (RingBuffer_ReadByte(usart->recvBuf) << 8);
								// ���ڶ����ֽ��Ƿ���ȷ
                if(header != JOHO_PACK_RESPONSE_HEADER){
                    header = 0;
                }else{
                    pkg->header = header;
                    // ֡ͷ���ճɹ�
                    pkg->status = JOHO_RECV_FLAG_HEADER; 
                }
            }else{
                header = 0;
            }
        }
			
		}
	// �ȴ���ʱ
    return JOHO_STATUS_TIMEOUT;

}


// ����֡ת��Ϊ�ֽ�����
void JOHO_Package2RingBuffer(PackageTypeDef *pkg,  RingBufferTypeDef *ringBuf){
    uint8_t checksum; // У���
//    // д��֡ͷ ������У�飬�ڼ���У��ʱ����
    RingBuffer_WriteUShort(ringBuf, pkg->header);
    // д����ID
    RingBuffer_WriteByte(ringBuf, pkg->usId);
    // д����ĳ���
    RingBuffer_WriteByte(ringBuf, pkg->size);
	//д��״̬��or ָ�����ͣ�
	RingBuffer_WriteByte(ringBuf, pkg->sstat);
    // д����������
	if(pkg->size != 2)
    RingBuffer_WriteByteArray(ringBuf, pkg->content, pkg->size-2);
    // ����У���
    checksum = RingBuffer_GetChecksum(ringBuf);
    // д��У���
    RingBuffer_WriteByte(ringBuf, checksum);
	
//	
//	printf("Auto get ServoID usId %d \r\n", pkg->usId);
//	printf("Auto get ServoID pkg->size %d \r\n", pkg->size);
//	printf("Auto get ServoID pkg->sstat %d \r\n", pkg->sstat);
//	printf("Auto get ServoID pkg->content %d \r\n", pkg->content[0]);
//	printf("Auto get ServoID checksum %d \r\n", checksum);

}



// ����Package��У���
uint8_t JOHO_CalcChecksum(PackageTypeDef *pkg){
    uint8_t checksum;
	// ��ʼ�����ζ���
	RingBufferTypeDef ringBuf;
	uint8_t pkgBuf[JOHO_PACK_RESPONSE_MAX_SIZE+1];
	RingBuffer_Init(&ringBuf, JOHO_PACK_RESPONSE_MAX_SIZE, pkgBuf);
    // ��Packageת��Ϊringbuffer
	// ��ת����ʱ��,���Զ��ļ���checksum
    JOHO_Package2RingBuffer(pkg, &ringBuf);
	// ��ȡ���ζ��ж�β��Ԫ��(��У��͵�λ��)
	checksum = RingBuffer_GetValueByIndex(&ringBuf, RingBuffer_GetByteUsed(&ringBuf)-1);
    return checksum;
}



/**
��������SDK
**/

//Ping ��� ״̬��ѯ
JOHO_STATUS US_Ping(Usart_DataTypeDef *usart, uint8_t servo_id){
	uint8_t statusCode; // ״̬��
	uint8_t ehcoServoId; // PING�õ��Ķ��ID
//	printf("[PING]Send Ping Package\r\n");
	// ���������
	JOHO_PackageBuild_Send(usart, servo_id, 2,CMDType_Ping, NULL);
	// ���շ��ص�Ping
	PackageTypeDef pkg;
	statusCode = USL_RecvPackage(usart, &pkg);
	if(statusCode == JOHO_STATUS_SUCCESS){
		// ��һ�����ID���Ƿ�ƥ��
		ehcoServoId = (uint8_t)pkg.usId;
		
		if (ehcoServoId != servo_id){
			// �����õ��Ķ��ID�Ų�ƥ��
			return JOHO_STATUS_ID_NOT_MATCH;
		}
//		printf("[succ]Auto get ServoID %d \r\n", ehcoServoId);
	}
	return statusCode;
}


//���ƶ���任λ��
void USL_SetServoAngle(Usart_DataTypeDef *usart, uint8_t servo_id, \
				float posi, uint16_t interval){
//����У��
if(posi > 4095)posi = 4095;
if(posi <0 )		posi  = 0;			

uint16_t posit = posi;
uint16_t time;			
uint8_t content[5];
					//���ӣ����1��-180�� FF FF 01 07 03 2A 00 00 03 E8 DF
content[0] =0x2A;
content[1] = posit	>> 8&0XFF;			
content[2] = posit	&0XFF;						
content[3] = interval	>> 8&0XFF;			
content[4] = interval	&0XFF;	
	/// ���������
	JOHO_PackageBuild_Send(usart, servo_id, 7,CMDType_Write,content);
	

}

//�ǶȲ�ѯ  ����ֵΪ0-4096 Ϊ����ֵ
uint16_t USL_GETPositionVal(Usart_DataTypeDef *usart, uint8_t servo_id){
uint16_t value = 0xffff; 
	uint8_t statusCode;// ״̬��
	uint8_t content[2];
					//���1��-180�� FF FF 01 07 03 2A 00 00 03 E8 DF
	content[0] =0x38;
	content[1] =0x02;
	/// ���������
	JOHO_PackageBuild_Send(usart, servo_id, 4,CMDType_Read,content);
	// ���շ��ص�pkg
	PackageTypeDef pkg;
	statusCode = USL_RecvPackage(usart, &pkg);
	if(statusCode==0){
//			printf("[succ]position %d \r\n", pkg.content[1]|(pkg.content[0]<<8));
			value = pkg.content[1]|(pkg.content[0]<<8);
	}
	else {value = value - statusCode;}
	return value;
}

//����Ť�ؿ��� 0�ر� 1��
void SET_Torque(Usart_DataTypeDef *usart, uint8_t servo_id,uint8_t isopen){
	uint8_t statusCode; // ״̬��
	uint8_t content[2];
	content[0] =0x28;content[1] =0x01;
	if(isopen == 0){content[1] =0x00;}
	// ���������-���ӣ�FF FF 01 04 03 28 01 CE
	JOHO_PackageBuild_Send(usart, servo_id, 4,CMDType_Write, content);

}

