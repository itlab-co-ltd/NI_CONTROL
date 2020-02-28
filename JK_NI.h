/*============================================================================//
Ver 6.2 - 2020.02.28

Made By Jaden Kim

@UpDate
	- 2019.01.29
		�����ϰ� �ϱ� ���� ���κ����� �̿��ؼ� �����ϵ��� ����
	- 2019.06.17
		AD Output 2ch�� ����.
	- 2019.09.10
		������ ī�� �ּ�->ä�ι�ȣ�� ����(����ϱ� �� ���ϵ���.)
		��ũ�� �����ϵ��� ����->��ũ ���ϴ°Ŵ� �������� �Ⱦ��� �׽�Ʈ�� �� �Ǿ� ����.
		ON OFF INPUT �ڵ����� 40������ üũ - 24�� ���Ŀ� ������ ������� SET & READ��
	- 2019.11.27
		ai ���� ���������ϵ��� ����
		#ifdef JK_NI_TESTING�� ���� ��� �ȵǴ��� #if JK_NI_TESTING�κ���
		ad float -> double�� ����
	- 2020.02.28
		ON OFF INPUT �ڵ� 40��üũ ���� - 32������ OFF�� �ȉ�...
		40ä�� �������� �����Ѵ�.
@Error
//============================================================================*/

#pragma once

#include <vcl.h>
#include "NIDAQmx.h"
#pragma link "NIDAQmx.lib"

#define JK_NI_TESTING							1

//Card Address
#define CARD_ADDRESS_INPUT_32CH_1				0x00
#define CARD_ADDRESS_INPUT_32CH_2			   	0x04
#define CARD_ADDRESS_INPUT_32CH_3 			  	0x08
#define CARD_ADDRESS_INPUT_32CH_4 			 	0x0c

#define CARD_ADDRESS_OUTPUT_32CH_1	 		   	0x00
#define CARD_ADDRESS_OUTPUT_32CH_2	 		 	0x04
#define CARD_ADDRESS_OUTPUT_32CH_3	 		 	0x08
#define CARD_ADDRESS_OUTPUT_32CH_4	 			0x0c

#define CARD_ADDRESS_SHORT_24CH_1 				0x10
#define CARD_ADDRESS_SHORT_24CH_2 			   	0x14
#define CARD_ADDRESS_SHORT_24CH_3 			  	0x18
#define CARD_ADDRESS_SHORT_24CH_4 			 	0x1c

#define CARD_ADDRESS_IP_SHORT_MUX_40CH_1		0x10
#define CARD_ADDRESS_IP_SHORT_MUX_40CH_2		0x14
#define CARD_ADDRESS_IP_SHORT_MUX_40CH_3		0x18
#define CARD_ADDRESS_IP_SHORT_MUX_40CH_4		0x1c

#define CARD_ADDRESS_AD_24CH_1					0x20
#define CARD_ADDRESS_AD_24CH_2					0x24
#define CARD_ADDRESS_AD_24CH_3					0x28
#define CARD_ADDRESS_AD_24CH_4					0x2c

#define CARD_ADDRESS_ON_OFF_OUTPUT_24CH_1	   	0x30
#define CARD_ADDRESS_ON_OFF_OUTPUT_24CH_2	   	0x34
#define CARD_ADDRESS_ON_OFF_OUTPUT_24CH_3	 	0x38
#define CARD_ADDRESS_ON_OFF_OUTPUT_24CH_4	   	0x3c

#define CARD_ADDRESS_CURRENT_4CH_1	 			0x40
#define CARD_ADDRESS_CURRENT_4CH_2	 			0x44
#define CARD_ADDRESS_CURRENT_4CH_3	 			0x48
#define CARD_ADDRESS_CURRENT_4CH_4	 			0x4c

#define CAED_ADDRESS_ON_OFF_INPUT_SET_24CH_1	0X50
#define CAED_ADDRESS_ON_OFF_INPUT_SET_24CH_2	0X54
#define CAED_ADDRESS_ON_OFF_INPUT_SET_24CH_3	0X58
#define CAED_ADDRESS_ON_OFF_INPUT_SET_24CH_4	0X5c

#define CARD_ADDRESS_ON_OFF_INPUT_SET_40CH_1	0x50
#define CARD_ADDRESS_ON_OFF_INPUT_SET_40CH_2	0x58

#define CARD_ADDRESS_ON_OFF_INPUT_READ_24CH_1   0x60
#define CARD_ADDRESS_ON_OFF_INPUT_READ_24CH_2   0x64
#define CARD_ADDRESS_ON_OFF_INPUT_READ_24CH_3   0x68
#define CARD_ADDRESS_ON_OFF_INPUT_READ_24CH_4   0x6c

#define CARD_ADDRESS_ON_OFF_INPUT_READ_40CH_1   0x60
#define CARD_ADDRESS_ON_OFF_INPUT_READ_40CH_2	0x68

#define CARD_ADDRESS_IP_MUX_40CH_1 				0x80
#define CARD_ADDRESS_IP_MUX_40CH_2 				0x84
#define CARD_ADDRESS_IP_MUX_40CH_3 				0x88
#define CARD_ADDRESS_IP_MUX_40CH_4 				0x8c
#define CARD_ADDRESS_IP_MUX_40CH_5 				0x90
#define CARD_ADDRESS_IP_MUX_40CH_6 				0x94
#define CARD_ADDRESS_IP_MUX_40CH_7 				0x98
#define CARD_ADDRESS_IP_MUX_40CH_8 				0x9c

#define UNIT_SYNC_TIMEOUT_MS					100
#define ANALOG_SAMPLING_COUNT					100
#define READ_WRITE_TIME                         10

#define VOLTAGE_SET(org_voltage)				((1.0/4.0)*org_voltage)//1�� ������ 4��Ʈ�� ���´�.

namespace JK_ITLab
{
	class NICard
	{
		public:
						NICard();//������

		bool 			init(int ai_end = 15);
		void 			release();//������

		//ni card
		void			set_ni_ad();                                //���� ���
		void			get_ni_ad();                       			//���� �Է�

		//for 0x00~0x0f input output
		bool			get_input(unsigned char in_ch);
		bool			set_output(unsigned char in_ch);

		//for 0x10~0x1f	short
		bool  			test_short_24ch(int in_numOfPin, int in_shortDelay_ms, unsigned char in_ch);//�ɼ�, ��Ʈ����
		bool  			test_short_40ch(int in_numOfPin, int in_shortDelay_ms, unsigned char in_ch);//CARD_ADDRESS_IP_SHORT_MUX_40CH ��

        //for 0x20~0x2f	ad
		bool			set_ad(unsigned char in_ch);

		//for 0x30~0x3f onoff output
		bool			set_onoff_output(unsigned char in_ch);

		//for 0x40~0x4f current
		bool			set_current(unsigned char in_ch);

		//for 0x50~0x5f & 0x60~0x6f - 40ä���� 0,2�� ����(1���� �� ��.)
		bool			set_onoff_input(unsigned char in_ch);
		bool			get_onoff_input(unsigned char in_ch);

		//for 0x80~0x9f
		bool			set_mux(unsigned char in_ch);

		double			m_ni_ad_output[2];
		double			m_ni_ad_input[16];					//AD ����
		char 			m_input[4][32];						//�Է����� ���� �迭 - ä��, IO
		char 			m_output[4][32];					//������� ���� �迭 - ä��, IO
		char			m_ad_set[4][24];					//AD�Է�����
		double 			m_short_data[4][40][40];			//��Ʈ���� ���� �迭 - ä��, IO��Ʈ����(40��*40��)
		char 			m_on_off_output[4][24];				//onoffoutput����
		char			m_on_off_input_pull_up[4][40];
		char			m_on_off_input_pull_down[4][40];
		char			m_on_off_input[4][40];
		char 			m_mux[8][40];
		char			m_current_shunt[4][4];
		char			m_current_ad[4][4];

		private:
		TaskHandle 		m_Handle_Addr;//�ּ� �ڵ� ��.
		TaskHandle 		m_Handle_Control;//��Ʈ�� �ڵ� ��.
		TaskHandle 		m_Handle_UnitSycn;//Unit�� ����ȭ�� �ϱ�����.
		TaskHandle 		m_Handle_DataInput;//digital input
		TaskHandle 		m_Handle_DataOutput;//digital ouput
		TaskHandle 		m_Handle_AnalogOutput;//analog output
		TaskHandle 		m_Handle_AnalogInput;
		int				m_ai_end;

		bool            UnitSyncCheck();
		bool 			send(char *in_addr, char *in_data, bool isReadByteHigh);
		bool 			recv(char *out_data, char *in_addr);
	};
}


DWORD GetMicroSecond()
{
	LARGE_INTEGER frequency;
	LARGE_INTEGER now;

	if ( !QueryPerformanceFrequency(&frequency) )
		return GetTickCount();

	if ( !QueryPerformanceCounter(&now) )
		return GetTickCount();

	return ((now.QuadPart) / (frequency.QuadPart/1000000));
}

void Sleep_Process(DWORD Sleep_Ms)
{
	DWORD Start_Time = GetMicroSecond();

	while(GetMicroSecond() - Start_Time < (Sleep_Ms) * 1000)
	{
	}
}

JK_ITLab::NICard::NICard()
{
	m_Handle_AnalogInput 	 = 0x00;
	m_Handle_Addr		 	 = 0x00;
	m_Handle_Control 		 = 0x00;
	m_Handle_UnitSycn 		 = 0x00;
	m_Handle_DataInput 		 = 0x00;
	m_Handle_DataOutput 	 = 0x00;
	m_Handle_AnalogOutput    = 0x00;

	m_ai_end = 0;
	memset(m_input  , 0x00, sizeof(m_input));
	memset(m_output , 0x00, sizeof(m_output));
	memset(m_ad_set, 0x00, sizeof(m_ad_set));
	memset(m_ni_ad_output, 0x00, sizeof(m_ni_ad_output));
	memset(m_ni_ad_input, 0x00, sizeof(m_ni_ad_input));
	memset(m_on_off_output , 0x00, sizeof(m_on_off_output));
	memset(m_on_off_input_pull_up , 0x00, sizeof(m_on_off_input_pull_up));
	memset(m_on_off_input_pull_down , 0x00, sizeof(m_on_off_input_pull_down));
	memset(m_mux, 0x00, sizeof(m_mux));
	memset(m_short_data, 0x00, sizeof(m_short_data));
	memset(m_current_shunt, 0x00, sizeof(m_current_shunt));
	memset(m_current_ad, 0x00, sizeof(m_current_ad));
}

bool JK_ITLab::NICard::init(int ai_end)
{
#if JK_NI_TESTING
	return true;
#else
	m_ai_end = ai_end;
	AnsiString Str = "Dev1/ai0:";
	Str += ai_end;
	if(DAQmxFailed(DAQmxCreateTask("",&m_Handle_AnalogInput)))return false;
	if(DAQmxFailed(DAQmxCreateAIVoltageChan(m_Handle_AnalogInput,//�ڵ�
	Str.c_str(),//������ ä��
	"",//ä���� �̸�
	DAQmx_Val_RSE,//�Է¸�� ����
	-10.0,//�Է� ��� �ּҰ�
	10.0,//�Է� ��� �ִ밪
	DAQmx_Val_Volts,//������ ���� �� ����?
	NULL)))return false;//���࿵��
	if(DAQmxFailed(DAQmxStartTask(m_Handle_AnalogInput)))return false;

	//�Ƴ��α� ��� �ڵ� ����
	if(DAQmxFailed(DAQmxCreateTask("",&m_Handle_AnalogOutput)))return false;
	if(DAQmxFailed(DAQmxCreateAOVoltageChan(m_Handle_AnalogOutput, "Dev1/ao0:1","", 0.0, 10.0, DAQmx_Val_Volts, NULL)))return false;
	if(DAQmxFailed(DAQmxStartTask(m_Handle_AnalogOutput)))return false;

	//������ �ƿ�ǲ �ڵ� ���� - ��ɿ� ���� �ٸ��� ����ϱ� ������ �������� �ʴ´�.
	if(DAQmxFailed(DAQmxCreateTask("",&m_Handle_DataOutput)))return false;
	if(DAQmxFailed(DAQmxCreateDOChan(m_Handle_DataOutput, "Dev1/port0/line0:7", "", DAQmx_Val_ChanForAllLines)))return false;

	//������ ��ǲ �ڵ� ���� - ��ɿ� ���� �ٸ��� ����ϱ� ������ �������� �ʴ´�.
	if(DAQmxFailed(DAQmxCreateTask("",&m_Handle_DataInput)))return false;
	if(DAQmxFailed(DAQmxCreateDIChan(m_Handle_DataInput, "Dev1/port0/line0:7", "", DAQmx_Val_ChanForAllLines)))return false;

	//�ּ� �ڵ� ���� - Digital Output
	if(DAQmxFailed(DAQmxCreateTask("",&m_Handle_Addr)))return false;
	if(DAQmxFailed(DAQmxCreateDOChan(m_Handle_Addr, "Dev1/port1/line0:7", "", DAQmx_Val_ChanForAllLines)))return false;
	if(DAQmxFailed(DAQmxStartTask(m_Handle_Addr)))return false;

	//��Ʈ�� �ڵ� ���� - Digital Output
	if(DAQmxFailed(DAQmxCreateTask("",&m_Handle_Control)))return false;
	if(DAQmxFailed(DAQmxCreateDOChan(m_Handle_Control, "Dev1/port2/line0:3", "", DAQmx_Val_ChanPerLine)))return false;
	if(DAQmxFailed(DAQmxStartTask(m_Handle_Control)))return false;

	//���� ����ȭ �ڵ� ���� - Digital Input
	if(DAQmxFailed(DAQmxCreateTask("",&m_Handle_UnitSycn)))return false;
	if(DAQmxFailed(DAQmxCreateDIChan(m_Handle_UnitSycn, "Dev1/port2/line6:7", "", DAQmx_Val_ChanPerLine)))return false;
	if(DAQmxFailed(DAQmxStartTask(m_Handle_UnitSycn)))return false;
	return true;
#endif
}

void JK_ITLab::NICard::release()
{
#if JK_NI_TESTING
	return ;
#else
	if(m_Handle_AnalogInput != 0)
	{
		DAQmxStopTask(m_Handle_AnalogInput);
		DAQmxClearTask(m_Handle_AnalogInput);
	}

	if(m_Handle_Addr != 0)
	{
		DAQmxStopTask(m_Handle_Addr);
		DAQmxClearTask(m_Handle_Addr);
	}

	if(m_Handle_Control != 0)
	{
		DAQmxStopTask(m_Handle_Control);
		DAQmxClearTask(m_Handle_Control);
	}

	if(m_Handle_UnitSycn != 0)
	{
		DAQmxStopTask(m_Handle_UnitSycn);
		DAQmxClearTask(m_Handle_UnitSycn);
	}

	if(m_Handle_DataInput != 0)
	{
		DAQmxStopTask(m_Handle_DataInput);
		DAQmxClearTask(m_Handle_DataInput);
	}

	if(m_Handle_DataOutput != 0)
	{
		DAQmxStopTask(m_Handle_DataOutput);
		DAQmxClearTask(m_Handle_DataOutput);
	}

	if(m_Handle_AnalogOutput != 0)
	{
		DAQmxStopTask(m_Handle_AnalogOutput);
		DAQmxClearTask(m_Handle_AnalogOutput);
	}
#endif
}

void JK_ITLab::NICard::set_ni_ad()
{
#if JK_NI_TESTING
	return ;
#else
	float64 voltage[2];
	voltage[0] = m_ni_ad_output[0];//VOLTAGE_SET(in_voltage);
	voltage[1] = m_ni_ad_output[1];

	//�Ƴ��α� �ڵ��� �̿��ؼ� �����͸� ��������.
	if(DAQmxFailed(DAQmxWriteAnalogF64(m_Handle_AnalogOutput,1,1,10.0,DAQmx_Val_GroupByChannel, voltage, NULL, NULL)))return;
	return;
#endif
}

bool JK_ITLab::NICard::get_input(unsigned char in_ch)
{
	if(in_ch >= 4)
		return false;

 	unsigned char Addr_Byte[8] = {0,};

	unsigned char Card_Addr = 0;
	switch(in_ch)
	{
		case 0:
			Card_Addr = CARD_ADDRESS_INPUT_32CH_1;
			break;

		case 1:
			Card_Addr = CARD_ADDRESS_INPUT_32CH_2;
			break;

		case 2:
			Card_Addr = CARD_ADDRESS_INPUT_32CH_3;
			break;

		case 3:
			Card_Addr = CARD_ADDRESS_INPUT_32CH_4;
			break;

		default:
			return false;
	}

	//�ּһ���
	for(int i = 0 ; i < 8 ; i++)
	{
		if((0x01 << i) & Card_Addr)
			Addr_Byte[i] = 0x01;
	}

	//1~8����
	Addr_Byte[0] = 0x00;
	Addr_Byte[1] = 0x00;
	if(!recv(m_input[in_ch], Addr_Byte))
		return false;

	//9~16����
	Addr_Byte[0] = 0x01;
	Addr_Byte[1] = 0x00;
	if(!recv(&m_input[in_ch][8], Addr_Byte))
		return false;

	//17~24����
	Addr_Byte[0] = 0x00;
	Addr_Byte[1] = 0x01;
	if(!recv(&m_input[in_ch][16], Addr_Byte))
		return false;

	//25~32����
	Addr_Byte[0] = 0x01;
	Addr_Byte[1] = 0x01;
	if(!recv(&m_input[in_ch][24], Addr_Byte))
		return false;

	return true;
}

//==============================================================================

bool JK_ITLab::NICard::set_output(unsigned char in_ch)
{
	if(in_ch >= 4)
		return false;

	char Data_Byte[8] = {0,};
	char Addr_Byte[8] = {0,};

	unsigned char Card_Addr = 0;
	switch(in_ch)
	{
		case 0:
			Card_Addr = CARD_ADDRESS_OUTPUT_32CH_1;
			break;

		case 1:
			Card_Addr = CARD_ADDRESS_OUTPUT_32CH_2;
			break;

		case 2:
			Card_Addr = CARD_ADDRESS_OUTPUT_32CH_3;
			break;

		case 3:
			Card_Addr = CARD_ADDRESS_OUTPUT_32CH_4;
			break;

		default:
			return false;
	}

	//�ּҸ����
	for(int i = 0 ; i < 8 ; i++)
	{
		if((0x01 << i) & Card_Addr)
			Addr_Byte[i] = 0x01;
	}

	//00 1~8
	Addr_Byte[0] = 0x00;
	Addr_Byte[1] = 0x00;
	memset(Data_Byte, 0x00, sizeof(Data_Byte));
	memcpy(Data_Byte, m_output[in_ch], sizeof(Data_Byte));

	if(!send(Addr_Byte, Data_Byte, true))
		return false;

	//10 9~16
	Addr_Byte[0] = 0x01;
	Addr_Byte[1] = 0x00;
	memset(Data_Byte, 0x00, sizeof(Data_Byte));
	memcpy(Data_Byte, &m_output[in_ch][8], sizeof(Data_Byte));

	if(!send(Addr_Byte, Data_Byte, true))
		return false;

	//01 17~24
	Addr_Byte[0] = 0x00;
	Addr_Byte[1] = 0x01;
	memset(Data_Byte, 0x00, sizeof(Data_Byte));
	memcpy(Data_Byte, &m_output[in_ch][16], sizeof(Data_Byte));

	if(!send(Addr_Byte, Data_Byte, true))
		return false;

	//11 25~32
	Addr_Byte[0] = 0x01;
	Addr_Byte[1] = 0x01;
	memset(Data_Byte, 0x00, sizeof(Data_Byte));
	memcpy(Data_Byte, &m_output[in_ch][24], sizeof(Data_Byte));

	if(!send(Addr_Byte, Data_Byte, true))
		return false;

	return true;
}

//==============================================================================

bool JK_ITLab::NICard::test_short_24ch(int in_numOfPin, int in_shortDelay, unsigned char in_ch)
{
	if(in_ch >= 4)
		return false;

	if(in_numOfPin > 24)
		return false;

	char Addr_Byte[8] = {0,};
	char Data_Byte[8] = {0,};

	unsigned char Card_Addr = 0;
	switch(in_ch)
	{
		case 0:
			Card_Addr = CARD_ADDRESS_SHORT_24CH_1;
			break;

		case 1:
			Card_Addr = CARD_ADDRESS_SHORT_24CH_2;
			break;

		case 2:
			Card_Addr = CARD_ADDRESS_SHORT_24CH_3;
			break;

		case 3:
			Card_Addr = CARD_ADDRESS_SHORT_24CH_4;
			break;

		default:
			return false;
	}

	//�ּҸ����
	for(int i = 0 ; i < 8 ; i++)
	{
		if((0x01 << i) & Card_Addr)
			Addr_Byte[i] = 0x01;
	}


	//������ �ʱ�ȭ
	memset(m_short_data[in_ch], 0x00, sizeof(m_short_data[in_ch]));

	//ī�� �ʱ�ȭ
	//00 1~8
	Addr_Byte[0] = 0x00;
	Addr_Byte[1] = 0x00;
	if(!send(Addr_Byte, Data_Byte, true))
		return false;

	//10 9~16
	Addr_Byte[0] = 0x01;
	Addr_Byte[1] = 0x00;
	if(!send(Addr_Byte, Data_Byte, true))
		return false;

	//01 17~24
	Addr_Byte[0] = 0x00;
	Addr_Byte[1] = 0x01;
	if(!send(Addr_Byte, Data_Byte, true))
		return false;

	//Control - 0~5-4���� 6-enabled
	Addr_Byte[0] = 0x01;
	Addr_Byte[1] = 0x01;
	if(!send(Addr_Byte, Data_Byte, true))
		return false;


	float mulLoop = in_numOfPin/4;
	if(in_numOfPin%4 > 0.0)
		mulLoop++;

	//mulCh-��Ƽä��(4���� ������ ��) sigCh-�̱�ä��(1���� ������ ��)
	for(int sigCh = 0; sigCh < in_numOfPin; sigCh++)
	{
		int pin = 0;
		//�ּҼ���
		if(sigCh < 8)
		{
			Addr_Byte[0] = 0x00;
			Addr_Byte[1] = 0x00;
			pin = sigCh;
		}else if(sigCh < 16){
			Addr_Byte[0] = 0x01;
			Addr_Byte[1] = 0x00;
			pin = sigCh - 8;
		}else{
			Addr_Byte[0] = 0x00;
			Addr_Byte[1] = 0x01;
			pin = sigCh - 16;
		}

		//�� ����
		memset(Data_Byte, 0x00, sizeof(Data_Byte));
		Data_Byte[pin] = 0x01;
		if(!send(Addr_Byte, Data_Byte, true))
			return false;

		//��Ʈ�� ����Ʈ ����
		Addr_Byte[0] = 0x01;
		Addr_Byte[1] = 0x01;
		for(int mulCh = 0; mulCh < int(mulLoop); mulCh++)
		{
			memset(Data_Byte, 0x00, sizeof(Data_Byte));
			Data_Byte[mulCh] = 0x01;
			Data_Byte[6] = 0x01;

			if(!send(Addr_Byte, Data_Byte, true))
				return false;

			int32 read_analog;
			double AnalogData[16] = {0,};

			Sleep_Process(in_shortDelay);
			DAQmxReadAnalogF64(m_Handle_AnalogInput, -1, 10.0, DAQmx_Val_GroupByChannel, AnalogData, 16, &read_analog, NULL);

			memcpy(&m_short_data[in_ch][sigCh][mulCh*4], &AnalogData, sizeof(double)*4);
		}

		//�ʱ�ȭ
		if(sigCh < 8)
		{
			Addr_Byte[0] = 0x00;
			Addr_Byte[1] = 0x00;
		}else if(sigCh < 16){
			Addr_Byte[0] = 0x01;
			Addr_Byte[1] = 0x00;
		}else{
			Addr_Byte[0] = 0x00;
			Addr_Byte[1] = 0x01;
		}
		memset(Data_Byte, 0x00, sizeof(Data_Byte));

		if(!send(Addr_Byte, Data_Byte, true))
			return false;
	}

	//Control - �ʱ�ȭ
	Addr_Byte[0] = 0x01;
	Addr_Byte[1] = 0x01;
	memset(Data_Byte, 0x00, sizeof(Data_Byte));
	if(!send(Addr_Byte, Data_Byte, true))
		return false;

	return true;
}

//==============================================================================

bool JK_ITLab::NICard::test_short_40ch(int in_numOfPin, int in_shortDelay, unsigned char in_ch)
{
	if(in_ch >= 4)
		return false;

#if JK_NI_TESTING
	return true;
#else
	if(in_numOfPin > 40)
		return false;

	char Addr_Byte[8] = {0,};
	char Data_Byte[8] = {0,};

	unsigned char Card_Addr = 0;
	switch(in_ch)
	{
		case 0:
			Card_Addr = CARD_ADDRESS_IP_SHORT_MUX_40CH_1;
			break;

		case 1:
			Card_Addr = CARD_ADDRESS_IP_SHORT_MUX_40CH_2;
			break;

		case 2:
			Card_Addr = CARD_ADDRESS_IP_SHORT_MUX_40CH_3;
			break;

		case 3:
			Card_Addr = CARD_ADDRESS_IP_SHORT_MUX_40CH_4;
			break;

		default:
			return false;
	}

	//�ּҸ����
	for(int i = 0 ; i < 8 ; i++)
	{
		if((0x01 << i) & Card_Addr)
			Addr_Byte[i] = 0x01;
	}
	//������ �ʱ�ȭ
	memset(m_short_data, 0x00, sizeof(m_short_data));

	//ī�� �ʱ�ȭ
	//00 1~8
	Addr_Byte[0] = 0x00;
	Addr_Byte[1] = 0x00;
	if(!send(Addr_Byte, Data_Byte, false))
		return false;

	//10 9~16
	Addr_Byte[0] = 0x01;
	Addr_Byte[1] = 0x00;
	if(!send(Addr_Byte, Data_Byte, false))
		return false;

	//01 17~24
	Addr_Byte[0] = 0x00;
	Addr_Byte[1] = 0x01;
	if(!send(Addr_Byte, Data_Byte, false))
		return false;

	//01 17~24
	Addr_Byte[0] = 0x00;
	Addr_Byte[1] = 0x01;
	if(!send(Addr_Byte, Data_Byte, false))
		return false;

	//11 25~32
	Addr_Byte[0] = 0x01;
	Addr_Byte[1] = 0x01;
	if(!send(Addr_Byte, Data_Byte, false))
		return false;

	//00 true 33~40
	Addr_Byte[0] = 0x00;
	Addr_Byte[1] = 0x00;
	if(!send(Addr_Byte, Data_Byte, true))
		return false;

	//01 true Control - 0~5-8���� 6-enabled
	Addr_Byte[0] = 0x01;
	Addr_Byte[1] = 0x01;
	if(!send(Addr_Byte, Data_Byte, true))
		return false;

	//control �� ����.
	float mulLoop = in_numOfPin/8;
	if(in_numOfPin%8 > 0.0)
		mulLoop++;

	//mulCh-��Ƽä��(8���� ������ ��) sigCh-�̱�ä��(1���� ������ ��)
	for(int sigCh = 0; sigCh < in_numOfPin; sigCh++)
	{
		bool isReadByteHi;
		int pin = 0;
		//�ּҼ���
		if(sigCh < 8)
		{
			Addr_Byte[0] = 0x00;
			Addr_Byte[1] = 0x00;
			isReadByteHi = false;
			pin = sigCh;
		}else if(sigCh < 16){
			Addr_Byte[0] = 0x01;
			Addr_Byte[1] = 0x00;
			isReadByteHi = false;
			pin = sigCh - 8;
		}else if(sigCh < 24){
			Addr_Byte[0] = 0x00;
			Addr_Byte[1] = 0x01;
			isReadByteHi = false;
			pin = sigCh - 16;
		}else if(sigCh < 32){
			Addr_Byte[0] = 0x01;
			Addr_Byte[1] = 0x01;
			isReadByteHi = false;
			pin = sigCh - 24;
		}else{
			Addr_Byte[0] = 0x00;
			Addr_Byte[1] = 0x00;
			isReadByteHi = true;
			pin = sigCh - 32;
		}

		//�� ����
		memset(Data_Byte, 0x00, sizeof(Data_Byte));
		Data_Byte[pin] = 0x01;
		if(!send(Addr_Byte, Data_Byte, isReadByteHi))
			return false;

		//��Ʈ�� ����Ʈ ����
		Addr_Byte[0] = 0x01;
		Addr_Byte[1] = 0x00;
		isReadByteHi = true;
		for(int mulCh = 0; mulCh < int(mulLoop); mulCh++)
		{
			memset(Data_Byte, 0x00, sizeof(Data_Byte));
			Data_Byte[mulCh] = 0x01;
			Data_Byte[7] = 0x01;
			if(!send(Addr_Byte, Data_Byte, isReadByteHi))
				return false;

			int32 read_analog;
			double AnalogData[16] = {0,};

			Sleep_Process(in_shortDelay);
			DAQmxReadAnalogF64(m_Handle_AnalogInput, -1, 10.0, DAQmx_Val_GroupByChannel, AnalogData, 16, &read_analog, NULL);

			memcpy(&m_short_data[in_ch][sigCh][mulCh*8], &AnalogData, sizeof(double)*8);
		}

		//�ʱ�ȭ
		if(sigCh < 8)
		{
			Addr_Byte[0] = 0x00;
			Addr_Byte[1] = 0x00;
			isReadByteHi = false;
			pin = sigCh;
		}else if(sigCh < 16){
			Addr_Byte[0] = 0x01;
			Addr_Byte[1] = 0x00;
			isReadByteHi = false;
			pin = sigCh - 8;
		}else if(sigCh < 24){
			Addr_Byte[0] = 0x00;
			Addr_Byte[1] = 0x01;
			isReadByteHi = false;
			pin = sigCh - 16;
		}else if(sigCh < 32){
			Addr_Byte[0] = 0x01;
			Addr_Byte[1] = 0x01;
			isReadByteHi = false;
			pin = sigCh - 24;
		}else{
			Addr_Byte[0] = 0x00;
			Addr_Byte[1] = 0x00;
			isReadByteHi = true;
			pin = sigCh - 32;
		}
		memset(Data_Byte, 0x00, sizeof(Data_Byte));
		if(!send(Addr_Byte, Data_Byte, isReadByteHi))
			return false;
		if(!send(Addr_Byte, Data_Byte, true))
			return false;
	}

	//Control - �ʱ�ȭ
	Addr_Byte[0] = 0x01;
	Addr_Byte[1] = 0x00;
	memset(Data_Byte, 0x00, sizeof(Data_Byte));
	if(!send(Addr_Byte, Data_Byte, true))
		return false;

	return true;
#endif
}
//==============================================================================

bool JK_ITLab::NICard::set_ad(unsigned char in_ch)
{
	if(in_ch >= 4)
		return false;

	char Data_Byte[8] = {0,};
	char Addr_Byte[8] = {0,};

	unsigned char Card_Addr = 0;
	switch(in_ch)
	{
		case 0:
			Card_Addr = CARD_ADDRESS_AD_24CH_1;
			break;

		case 1:
			Card_Addr = CARD_ADDRESS_AD_24CH_2;
			break;

		case 2:
			Card_Addr = CARD_ADDRESS_AD_24CH_3;
			break;

		case 3:
			Card_Addr = CARD_ADDRESS_AD_24CH_4;
			break;

		default:
			return false;
	}

	//�ּҸ����
	for(int i = 0 ; i < 8 ; i++)
	{
		if((0x01 << i) & Card_Addr)
			Addr_Byte[i] = 0x01;
	}

	//00 1~8
	Addr_Byte[0] = 0x00;
	Addr_Byte[1] = 0x00;
	memcpy(Data_Byte, &m_ad_set[in_ch][0], sizeof(Data_Byte));
	if(!send(Addr_Byte, Data_Byte, true))
		return false;

	//10 9~16
	Addr_Byte[0] = 0x01;
	Addr_Byte[1] = 0x00;
	memcpy(Data_Byte, &m_ad_set[in_ch][8], sizeof(Data_Byte));
	if(!send(Addr_Byte, Data_Byte, true))
		return false;

	//01 17~24
	Addr_Byte[0] = 0x00;
	Addr_Byte[1] = 0x01;
	memcpy(Data_Byte, &m_ad_set[in_ch][16], sizeof(Data_Byte));
	if(!send(Addr_Byte, Data_Byte, true))
		return false;

	return true;
}

//==============================================================================

bool JK_ITLab::NICard::set_onoff_output(unsigned char in_ch)
{
	if(in_ch >= 4)
		return false;

 	char Data_Byte[8] = {0,};
	char Addr_Byte[8] = {0,};

	unsigned char Card_Addr = 0;
	switch(in_ch)
	{
		case 0:
			Card_Addr = CARD_ADDRESS_ON_OFF_OUTPUT_24CH_1;
			break;

		case 1:
			Card_Addr = CARD_ADDRESS_ON_OFF_OUTPUT_24CH_2;
			break;

		case 2:
			Card_Addr = CARD_ADDRESS_ON_OFF_OUTPUT_24CH_3;
			break;

		case 3:
			Card_Addr = CARD_ADDRESS_ON_OFF_OUTPUT_24CH_4;
			break;

		default:
			return false;
	}

	//�ּҸ����
	for(int i = 0 ; i < 8 ; i++)
	{
		if((0x01 << i) & Card_Addr)
			Addr_Byte[i] = 0x01;
	}

	//00 1~8
	Addr_Byte[0] = 0x00;
	Addr_Byte[1] = 0x00;
	memset(Data_Byte, 0x00, sizeof(Data_Byte));
	memcpy(Data_Byte, m_on_off_output[in_ch], sizeof(Data_Byte));

	if(!send(Addr_Byte, Data_Byte, true)) return false;

	//10 9~16
	Addr_Byte[0] = 0x01;
	Addr_Byte[1] = 0x00;
	memset(Data_Byte, 0x00, sizeof(Data_Byte));
	memcpy(Data_Byte, &m_on_off_output[in_ch][8], sizeof(Data_Byte));

	if(!send(Addr_Byte, Data_Byte, true)) return false;

	//01 17~24
	Addr_Byte[0] = 0x00;
	Addr_Byte[1] = 0x01;
	memset(Data_Byte, 0x00, sizeof(Data_Byte));
	memcpy(Data_Byte, &m_on_off_output[in_ch][16], sizeof(Data_Byte));

	if(!send(Addr_Byte, Data_Byte, true)) return false;

	return true;
}

//==============================================================================

bool JK_ITLab::NICard::set_current(unsigned char in_ch)
{
	if(in_ch >= 4)
		return false;

 	char Addr_Byte[8] = {0,};
	char Data_Byte[8] = {0,};

	unsigned char Card_Addr = 0;
	switch(in_ch)
	{
		case 0:
			Card_Addr = CARD_ADDRESS_CURRENT_4CH_1;
			break;

		case 1:
			Card_Addr = CARD_ADDRESS_CURRENT_4CH_2;
			break;

		case 2:
			Card_Addr = CARD_ADDRESS_CURRENT_4CH_3;
			break;

		case 3:
			Card_Addr = CARD_ADDRESS_CURRENT_4CH_4;
			break;

		default:
			return false;
	}

	//�ּҸ����
	for(int i = 0 ; i < 8 ; i++)
	{
		if((0x01 << i) & Card_Addr)
			Addr_Byte[i] = 0x01;
	}


	Data_Byte[0] = m_current_shunt[in_ch][0];
	Data_Byte[1] = m_current_shunt[in_ch][1];
	Data_Byte[2] = m_current_shunt[in_ch][2];
	Data_Byte[3] = m_current_shunt[in_ch][3];
	Data_Byte[4] = m_current_ad[in_ch][0];
	Data_Byte[5] = m_current_ad[in_ch][1];
	Data_Byte[6] = m_current_ad[in_ch][2];
	Data_Byte[7] = m_current_ad[in_ch][3];
	if(!send(Addr_Byte, Data_Byte, true))
		return false;//ä�� ��� ����

	return true;
}

//==============================================================================

void JK_ITLab::NICard::get_ni_ad()
{
#if JK_NI_TESTING
	return ;
#else
	int32 read_analog;
	double Temp_AnalogData[16] = {0,};//AD_MAX_COUNT;
	double Temp_Input[16] = {0,};

    for(int i = 0 ; i < ANALOG_SAMPLING_COUNT; i++)
	{
		DAQmxReadAnalogF64(m_Handle_AnalogInput, -1, 10.0, DAQmx_Val_GroupByChannel, Temp_AnalogData, 16, &read_analog, NULL);
		/*
		DAQmxReadAnalogF64(taskHandle, -�ڵ�
		1000, - �ѹ��� �о�� ä�δ� ���� ����
		10.0,  - �Է¿� ���� Ÿ�Ӿƿ� �ð�(��)
		DAQmx_Val_GroupByChannel, - ����ä���� �Է½� �Է��� �޴� �������� �������
		data, - �Է¹��� �迭
		1000, - ������ �Էµ� �迭�� ������
		&read, - ä�δ� �Է¹��� �������� ������ ����
		NULL);
		http://guslabview.tistory.com/35
		*/
		for(int j = 0; j < 16; j++)
			Temp_Input[j] += (float)Temp_AnalogData[j];
	}

	for(int i = 0; i < 16; i++)
		m_ni_ad_input[i] = Temp_Input[i] / (float)ANALOG_SAMPLING_COUNT;
#endif
}

//==============================================================================

bool JK_ITLab::NICard::send(char *in_addr, char *in_data, bool isReadByteHigh)
{
#if JK_NI_TESTING
	return true;
#else
	int32 read, Sample;
	char Control_Byte[4] = {0,};

	DAQmxStopTask(m_Handle_DataInput);//input stop
	DAQmxStartTask(m_Handle_DataOutput);//output start

	/*
	DAQmxWriteDigitalLines(
	�ڵ� ��,
	ä�δ� ���� ��,
	�ڵ� ���� ���� true or false,
	Ÿ�Ӿƿ�,
	������ ���̾ƿ�,
	�����Ͱ� �� ��,
	���࿵�� NULL,
	���࿵�� NULL);
	*/
	//Gate �ݱ�
	Control_Byte[0] = 0x01;
	Control_Byte[1] = 0x01;
	Control_Byte[2] = 0x01;
	Control_Byte[3] = 0x01;
	DAQmxWriteDigitalLines(m_Handle_Control, 1, 1, 10.0, DAQmx_Val_GroupByChannel, Control_Byte, NULL, NULL);


	Control_Byte[0] = 0x01; //GATE - LOW(OPEN) HIGH(CLOSE)
	Control_Byte[1] = 0x01; //DIR - ���� HIGH(SEND) LOW(RECV) - Send�̹Ƿ� High
	if(isReadByteHigh)
		Control_Byte[2] = 0x01; //READ - LOW�϶����� �д´�. - ��������� ���⼭�� write�� �ϹǷ� �ɼǰ����� ����Ѵ�.
	else
		Control_Byte[2] = 0x00;
	Control_Byte[3] = 0x01; //WRITE - LOW����HIGH�ΰ��� �д´�. - �������

	DAQmxWriteDigitalLines(m_Handle_Addr, 1, 1, 10.0, DAQmx_Val_GroupByChannel, in_addr, NULL, NULL);
	DAQmxWriteDigitalLines(m_Handle_DataOutput, 1, 1, 10.0, DAQmx_Val_GroupByChannel, in_data, NULL, NULL);
	DAQmxWriteDigitalLines(m_Handle_Control, 1, 1, 10.0, DAQmx_Val_GroupByChannel, Control_Byte, NULL, NULL);

	//����Ʈ ����
	Control_Byte[0] = 0x00;
	DAQmxWriteDigitalLines(m_Handle_Control, 1, 1, 10.0, DAQmx_Val_GroupByChannel, Control_Byte, NULL, NULL);

	//Write Low
	Control_Byte[3] = 0x00;
	DAQmxWriteDigitalLines(m_Handle_Control, 1, 1, 10.0, DAQmx_Val_GroupByChannel, Control_Byte, NULL, NULL);

	bool return_value = UnitSyncCheck();

	//Weite High - ���� ����̹Ƿ�
	Control_Byte[3] = 0x01;
	DAQmxWriteDigitalLines(m_Handle_Control, 1, 1, 10.0, DAQmx_Val_GroupByChannel, Control_Byte, NULL, NULL);

	//Gate �ݱ�
	Control_Byte[0] = 0x01;
	Control_Byte[1] = 0x01;
	Control_Byte[2] = 0x01;
	Control_Byte[3] = 0x01;
	DAQmxWriteDigitalLines(m_Handle_Control, 1, 1, 10.0, DAQmx_Val_GroupByChannel, Control_Byte, NULL, NULL);

	DAQmxStopTask(m_Handle_DataOutput);
	return return_value;
#endif
}

bool JK_ITLab::NICard::recv(char *out_data, char *in_addr)
{
#if JK_NI_TESTING
	return true;
#else
	int32 read, Sample;
	char Control_Byte[4] = {0,};
	char Read_Byte[8] = {0,};

	DAQmxStopTask(m_Handle_DataOutput);//input stop
	DAQmxStartTask(m_Handle_DataInput);//output start

	Control_Byte[0] = 0x01;
	Control_Byte[1] = 0x01;
	Control_Byte[2] = 0x01;//read�� ������.
	Control_Byte[3] = 0x01;
	DAQmxWriteDigitalLines(m_Handle_Control, 1, 1, 10.0, DAQmx_Val_GroupByChannel, Control_Byte, NULL, NULL);

	DAQmxWriteDigitalLines(m_Handle_Addr, 1, 1, 10.0, DAQmx_Val_GroupByChannel, in_addr, NULL, NULL);

	Control_Byte[0] = 0x00;
	Control_Byte[1] = 0x00;
	Control_Byte[2] = 0x01;
	Control_Byte[3] = 0x01;
	DAQmxWriteDigitalLines(m_Handle_Control, 1, 1, 10.0, DAQmx_Val_GroupByChannel, Control_Byte, NULL, NULL);

	Control_Byte[0] = 0x00;
	Control_Byte[1] = 0x00;
	Control_Byte[2] = 0x00;//read�� ������.
	Control_Byte[3] = 0x01;
	DAQmxWriteDigitalLines(m_Handle_Control, 1, 1, 10.0, DAQmx_Val_GroupByChannel, Control_Byte, NULL, NULL);

	bool return_value = UnitSyncCheck();

	DAQmxReadDigitalLines(m_Handle_DataInput, 1, 10.0, DAQmx_Val_GroupByChannel, Read_Byte, 8, &read, &Sample, NULL);

	Control_Byte[0] = 0x01;
	Control_Byte[1] = 0x01;
	Control_Byte[2] = 0x01;//read�� ������.
	Control_Byte[3] = 0x01;
	DAQmxWriteDigitalLines(m_Handle_Control, 1, 1, 10.0, DAQmx_Val_GroupByChannel, Control_Byte, NULL, NULL);

	DAQmxStopTask(m_Handle_DataInput);

	//������ ����
	memcpy(out_data, Read_Byte, sizeof(Read_Byte));

	return return_value;
#endif
}

bool JK_ITLab::NICard::UnitSyncCheck()
{
	int32 read, Sample;
	char UnitSync = -1;

	unsigned long Tick = GetTickCount();
	while(true)
	{
		DAQmxReadDigitalLines(m_Handle_UnitSycn, 1, 10.0, DAQmx_Val_GroupByChannel, &UnitSync, 1, &read, &Sample, NULL);

		if(UnitSync == 0)//0�̸� ����ȭ �Ϸ�.
			break;

		if(GetTickCount() - Tick > UNIT_SYNC_TIMEOUT_MS)
		{
			return false;
		}
	}
	return true;
}

bool JK_ITLab::NICard::set_onoff_input(unsigned char in_ch)
{
	if(in_ch >= 4)
		return false;

	char Data_Byte[8] = {0,};
	char Addr_Byte[8] = {0,};

	unsigned char Card_Addr = 0;
	switch(in_ch)
	{
		case 0:
			Card_Addr = CAED_ADDRESS_ON_OFF_INPUT_SET_24CH_1;
			break;

		case 1:
			Card_Addr = CAED_ADDRESS_ON_OFF_INPUT_SET_24CH_2;
			break;

		case 2:
			Card_Addr = CAED_ADDRESS_ON_OFF_INPUT_SET_24CH_3;
			break;

		case 3:
			Card_Addr = CAED_ADDRESS_ON_OFF_INPUT_SET_24CH_4;
			break;

		default:
			return false;
	}

	//�ּҸ����
	for(int i = 0 ; i < 8 ; i++)
	{
		if((0x01 << i) & Card_Addr)
			Addr_Byte[i] = 0x01;
	}

	//00 1~8
	Addr_Byte[0] = 0x00;
	Addr_Byte[1] = 0x00;
	memset(Data_Byte, 0x00, sizeof(Data_Byte));
	memcpy(Data_Byte, m_on_off_input_pull_up[in_ch], sizeof(Data_Byte));
	if(!send(Addr_Byte, Data_Byte, true)) return false;
	memcpy(Data_Byte, m_on_off_input_pull_down[in_ch], sizeof(Data_Byte));
	if(!send(Addr_Byte, Data_Byte, false)) return false;

	//10 9~16
	Addr_Byte[0] = 0x01;
	Addr_Byte[1] = 0x00;
	memset(Data_Byte, 0x00, sizeof(Data_Byte));
	memcpy(Data_Byte, &m_on_off_input_pull_up[in_ch][8], sizeof(Data_Byte));
	if(!send(Addr_Byte, Data_Byte, true)) return false;
	memcpy(Data_Byte, &m_on_off_input_pull_down[in_ch][8], sizeof(Data_Byte));
	if(!send(Addr_Byte, Data_Byte, false)) return false;

	//01 17~24
	Addr_Byte[0] = 0x00;
	Addr_Byte[1] = 0x01;
	memset(Data_Byte, 0x00, sizeof(Data_Byte));
	memcpy(Data_Byte, &m_on_off_input_pull_up[in_ch][16], sizeof(Data_Byte));
	if(!send(Addr_Byte, Data_Byte, true)) return false;
	memcpy(Data_Byte, &m_on_off_input_pull_down[in_ch][16], sizeof(Data_Byte));
	if(!send(Addr_Byte, Data_Byte, false)) return false;

	//01 25~32
	Addr_Byte[0] = 0x01;
	Addr_Byte[1] = 0x01;
	Addr_Byte[2] = 0x00;
	memset(Data_Byte, 0x00, sizeof(Data_Byte));
	memcpy(Data_Byte, &m_on_off_input_pull_up[in_ch][24], sizeof(Data_Byte));
	if(!send(Addr_Byte, Data_Byte, true)) return false;
	memcpy(Data_Byte, &m_on_off_input_pull_down[in_ch][24], sizeof(Data_Byte));
	if(!send(Addr_Byte, Data_Byte, false)) return false;

	//01 33~40
	Addr_Byte[0] = 0x00;
	Addr_Byte[1] = 0x00;
	Addr_Byte[2] = 0x01;
	memset(Data_Byte, 0x00, sizeof(Data_Byte));
	memcpy(Data_Byte, &m_on_off_input_pull_up[in_ch][32], sizeof(Data_Byte));
	if(!send(Addr_Byte, Data_Byte, true)) return false;
	memcpy(Data_Byte, &m_on_off_input_pull_down[in_ch][32], sizeof(Data_Byte));
	if(!send(Addr_Byte, Data_Byte, false)) return false;

	return true;
}

bool JK_ITLab::NICard::get_onoff_input(unsigned char in_ch)
{
	if(in_ch >= 4)
		return false;

	char Read_Byte[8] = {0,};
	char Addr_Byte[8] = {0,};

	unsigned char Card_Addr = 0;
	switch(in_ch)
	{
		case 0:
			Card_Addr = CARD_ADDRESS_ON_OFF_INPUT_READ_24CH_1;
			break;

		case 1:
			Card_Addr = CARD_ADDRESS_ON_OFF_INPUT_READ_24CH_2;
			break;

		case 2:
			Card_Addr = CARD_ADDRESS_ON_OFF_INPUT_READ_24CH_3;
			break;

		case 3:
			Card_Addr = CARD_ADDRESS_ON_OFF_INPUT_READ_24CH_4;
			break;

		default:
			return false;
	}

	//�ּҸ����
	for(int i = 0 ; i < 8 ; i++)
	{
		if((0x01 << i) & Card_Addr)
			Addr_Byte[i] = 0x01;
	}

	Addr_Byte[0] = 0x00;
	Addr_Byte[1] = 0x00;
	Addr_Byte[2] = 0x00;
	if(!recv(Read_Byte, Addr_Byte))
		return false;
	memcpy(&m_on_off_input[in_ch][0], Read_Byte, sizeof(Read_Byte));

	Addr_Byte[0] = 0x01;
	Addr_Byte[1] = 0x00;
	Addr_Byte[2] = 0x00;
	if(!recv(Read_Byte, Addr_Byte))
		return false;
	memcpy(&m_on_off_input[in_ch][8], Read_Byte, sizeof(Read_Byte));

	Addr_Byte[0] = 0x00;
	Addr_Byte[1] = 0x01;
	Addr_Byte[2] = 0x00;
	if(!recv(Read_Byte, Addr_Byte))
		return false;
	memcpy(&m_on_off_input[in_ch][16], Read_Byte, sizeof(Read_Byte));

	Addr_Byte[0] = 0x01;
	Addr_Byte[1] = 0x01;
	Addr_Byte[2] = 0x00;
	if(!recv(Read_Byte, Addr_Byte))
		return false;
	memcpy(&m_on_off_input[in_ch][24], Read_Byte, sizeof(Read_Byte));

	Addr_Byte[0] = 0x00;
	Addr_Byte[1] = 0x00;
	Addr_Byte[2] = 0x01;
	if(!recv(Read_Byte, Addr_Byte))
		return false;
	memcpy(&m_on_off_input[in_ch][32], Read_Byte, sizeof(Read_Byte));

	return true;
}

bool JK_ITLab::NICard::set_mux(unsigned char in_ch)
{
	if(in_ch >= 8)
		return false;

	char Data_Byte[8] = {0,};
	char Addr_Byte[8] = {0,};

	unsigned char Card_Addr = 0;
	switch(in_ch)
	{
		case 0:
			Card_Addr = CARD_ADDRESS_IP_MUX_40CH_1;
			break;

		case 1:
			Card_Addr = CARD_ADDRESS_IP_MUX_40CH_2;
			break;

		case 2:
			Card_Addr = CARD_ADDRESS_IP_MUX_40CH_3;
			break;

		case 3:
			Card_Addr = CARD_ADDRESS_IP_MUX_40CH_4;
			break;

		case 4:
			Card_Addr = CARD_ADDRESS_IP_MUX_40CH_5;
			break;

		case 5:
			Card_Addr = CARD_ADDRESS_IP_MUX_40CH_6;
			break;

		case 6:
			Card_Addr = CARD_ADDRESS_IP_MUX_40CH_7;
			break;

		case 7:
			Card_Addr = CARD_ADDRESS_IP_MUX_40CH_8;
			break;


		default:
			return false;
	}

	//�ּҸ����
	for(int i = 0 ; i < 8 ; i++)
	{
		if((0x01 << i) & Card_Addr)
			Addr_Byte[i] = 0x01;
	}

	//00 1~8
	Addr_Byte[0] = 0x00;
	Addr_Byte[1] = 0x00;
	memset(Data_Byte, 0x00, sizeof(Data_Byte));
	memcpy(Data_Byte, m_mux[in_ch], sizeof(Data_Byte));
	if(!send(Addr_Byte, Data_Byte, false)) return false;

	//10 9~16
	Addr_Byte[0] = 0x01;
	Addr_Byte[1] = 0x00;
	memset(Data_Byte, 0x00, sizeof(Data_Byte));
	memcpy(Data_Byte, &m_mux[in_ch][8], sizeof(Data_Byte));
	if(!send(Addr_Byte, Data_Byte, false)) return false;

	//01 17~24
	Addr_Byte[0] = 0x00;
	Addr_Byte[1] = 0x01;
	memset(Data_Byte, 0x00, sizeof(Data_Byte));
	memcpy(Data_Byte, &m_mux[in_ch][16], sizeof(Data_Byte));
	if(!send(Addr_Byte, Data_Byte, false)) return false;

	//01 25~32
	Addr_Byte[0] = 0x01;
	Addr_Byte[1] = 0x01;
	memset(Data_Byte, 0x00, sizeof(Data_Byte));
	memcpy(Data_Byte, &m_mux[in_ch][24], sizeof(Data_Byte));
	if(!send(Addr_Byte, Data_Byte, false)) return false;

	//01 33~40
	Addr_Byte[0] = 0x00;
	Addr_Byte[1] = 0x00;
	memset(Data_Byte, 0x00, sizeof(Data_Byte));
	memcpy(Data_Byte, &m_mux[in_ch][32], sizeof(Data_Byte));
	if(!send(Addr_Byte, Data_Byte, true)) return false;

	return true;
}
