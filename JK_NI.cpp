#include "JK_NI.h"

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
	m_isUsingSync = false;

	m_Handle_AnalogInput 	 = 0x00;
	m_Handle_Addr		 	 = 0x00;
	m_Handle_Control 		 = 0x00;
	m_Handle_UnitSycn 		 = 0x00;
	m_Handle_DataInput 		 = 0x00;
	m_Handle_DataOutput 	 = 0x00;
	m_Handle_AnalogOutput    = 0x00;

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

bool JK_ITLab::NICard::init(bool in_isUsingSync)
{
#ifdef JK_NI_TESTING
	return true;
#else
	m_isUsingSync = in_isUsingSync;

	if(DAQmxFailed(DAQmxCreateTask("",&m_Handle_AnalogInput)))return false;
	if(DAQmxFailed(DAQmxCreateAIVoltageChan(m_Handle_AnalogInput,//핸들
	"Dev1/ai0:15",//물리적 채널
	"",//채널의 이름
	DAQmx_Val_RSE,//입력모드 설정
	-10.0,//입력 기대 최소값
	10.0,//입력 기대 최대값
	DAQmx_Val_Volts,//전압을 받을 것 인지?
	NULL)))return false;//예약영역
	if(DAQmxFailed(DAQmxStartTask(m_Handle_AnalogInput)))return false;

	//아날로그 출력 핸들 생성
	if(DAQmxFailed(DAQmxCreateTask("",&m_Handle_AnalogOutput)))return false;
	if(DAQmxFailed(DAQmxCreateAOVoltageChan(m_Handle_AnalogOutput, "Dev1/ao0:1","", 0.0, 10.0, DAQmx_Val_Volts, NULL)))return false;
	if(DAQmxFailed(DAQmxStartTask(m_Handle_AnalogOutput)))return false;

	//디지털 아웃풋 핸들 생성 - 명령에 따라 다르게 사용하기 때문에 시작하지 않는다.
	if(DAQmxFailed(DAQmxCreateTask("",&m_Handle_DataOutput)))return false;
	if(DAQmxFailed(DAQmxCreateDOChan(m_Handle_DataOutput, "Dev1/port0/line0:7", "", DAQmx_Val_ChanForAllLines)))return false;

	//디지털 인풋 핸들 생성 - 명령에 따라 다르게 사용하기 때문에 시작하지 않는다.
	if(DAQmxFailed(DAQmxCreateTask("",&m_Handle_DataInput)))return false;
	if(DAQmxFailed(DAQmxCreateDIChan(m_Handle_DataInput, "Dev1/port0/line0:7", "", DAQmx_Val_ChanForAllLines)))return false;

	//주소 핸들 생성 - Digital Output
	if(DAQmxFailed(DAQmxCreateTask("",&m_Handle_Addr)))return false;
	if(DAQmxFailed(DAQmxCreateDOChan(m_Handle_Addr, "Dev1/port1/line0:7", "", DAQmx_Val_ChanForAllLines)))return false;
	if(DAQmxFailed(DAQmxStartTask(m_Handle_Addr)))return false;

	//컨트롤 핸들 생성 - Digital Output
	if(DAQmxFailed(DAQmxCreateTask("",&m_Handle_Control)))return false;
	if(DAQmxFailed(DAQmxCreateDOChan(m_Handle_Control, "Dev1/port2/line0:3", "", DAQmx_Val_ChanPerLine)))return false;
	if(DAQmxFailed(DAQmxStartTask(m_Handle_Control)))return false;

	//유닛 동기화 핸들 생성 - Digital Input
	if(DAQmxFailed(DAQmxCreateTask("",&m_Handle_UnitSycn)))return false;
	if(DAQmxFailed(DAQmxCreateDIChan(m_Handle_UnitSycn, "Dev1/port2/line6:7", "", DAQmx_Val_ChanPerLine)))return false;
	if(DAQmxFailed(DAQmxStartTask(m_Handle_UnitSycn)))return false;
	return true;
#endif
}

void JK_ITLab::NICard::release()
{
#ifdef JK_NI_TESTING
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
#ifdef JK_NI_TESTING
	return ;
#else
	float64 voltage[2];
	voltage[0] = m_ni_ad_output[0];//VOLTAGE_SET(in_voltage);
	voltage[1] = m_ni_ad_output[1];

	//아날로그 핸들을 이용해서 데이터를 내보낸다.
	if(DAQmxFailed(DAQmxWriteAnalogF64(m_Handle_AnalogOutput,1,1,10.0,DAQmx_Val_GroupByChannel, voltage, NULL, NULL)))return;
	return;
#endif
}

bool JK_ITLab::NICard::get_input(unsigned char in_cardAddress)
{
 	unsigned char Addr_Byte[8] = {0,};

	//주소생성
	for(int i = 0 ; i < 8 ; i++)
	{
		if((0x01 << i) & in_cardAddress)
			Addr_Byte[i] = 0x01;
	}

	int channel = -1;
	switch(in_cardAddress)
	{
		case CARD_ADDRESS_INPUT_32CH_1:
			channel = 0;
			break;

		case CARD_ADDRESS_INPUT_32CH_2:
			channel = 1;
			break;

		case CARD_ADDRESS_INPUT_32CH_3:
			channel = 2;
			break;

		case CARD_ADDRESS_INPUT_32CH_4:
			channel = 3;
			break;

		default:
			return false;
	}

	//1~8번핀
	Addr_Byte[0] = 0x00;
	Addr_Byte[1] = 0x00;
	if(!recv(m_input[channel], Addr_Byte))
		return false;

	//9~16번핀
	Addr_Byte[0] = 0x01;
	Addr_Byte[1] = 0x00;
	if(!recv(&m_input[channel][8], Addr_Byte))
		return false;

	//17~24번핀
	Addr_Byte[0] = 0x00;
	Addr_Byte[1] = 0x01;
	if(!recv(&m_input[channel][16], Addr_Byte))
		return false;

	//25~32번핀
	Addr_Byte[0] = 0x01;
	Addr_Byte[1] = 0x01;
	if(!recv(&m_input[channel][24], Addr_Byte))
		return false;

	return true;
}

//==============================================================================

bool JK_ITLab::NICard::set_output(unsigned char in_cardAddress)
{
	char Data_Byte[8] = {0,};
	char Addr_Byte[8] = {0,};

	//주소만들기
	for(int i = 0 ; i < 8 ; i++)
	{
		if((0x01 << i) & in_cardAddress)
			Addr_Byte[i] = 0x01;
	}

	int channel = -1;
	switch(in_cardAddress)
	{
		case CARD_ADDRESS_OUTPUT_32CH_1:
			channel = 0;
			break;

		case CARD_ADDRESS_OUTPUT_32CH_2:
			channel = 1;
			break;

		case CARD_ADDRESS_OUTPUT_32CH_3:
			channel = 2;
			break;

		case CARD_ADDRESS_OUTPUT_32CH_4:
			channel = 3;
			break;

		default:
			return false;
	}

	//00 1~8
	Addr_Byte[0] = 0x00;
	Addr_Byte[1] = 0x00;
	memset(Data_Byte, 0x00, sizeof(Data_Byte));
	memcpy(Data_Byte, m_output[channel], sizeof(Data_Byte));

	if(!send(Addr_Byte, Data_Byte, true))
		return false;

	//10 9~16
	Addr_Byte[0] = 0x01;
	Addr_Byte[1] = 0x00;
	memset(Data_Byte, 0x00, sizeof(Data_Byte));
	memcpy(Data_Byte, &m_output[channel][8], sizeof(Data_Byte));

	if(!send(Addr_Byte, Data_Byte, true))
		return false;

	//01 17~24
	Addr_Byte[0] = 0x00;
	Addr_Byte[1] = 0x01;
	memset(Data_Byte, 0x00, sizeof(Data_Byte));
	memcpy(Data_Byte, &m_output[channel][16], sizeof(Data_Byte));

	if(!send(Addr_Byte, Data_Byte, true))
		return false;

	//11 25~32
	Addr_Byte[0] = 0x01;
	Addr_Byte[1] = 0x01;
	memset(Data_Byte, 0x00, sizeof(Data_Byte));
	memcpy(Data_Byte, &m_output[channel][24], sizeof(Data_Byte));

	if(!send(Addr_Byte, Data_Byte, true))
		return false;

	return true;
}

//==============================================================================

bool JK_ITLab::NICard::test_short_24ch(int in_numOfPin, int in_shortDelay, unsigned char in_cardAddress)
{
	if(in_numOfPin > 24)
		return false;

	char Addr_Byte[8] = {0,};
	char Data_Byte[8] = {0,};

	//주소만들기
	for(int i = 0 ; i < 8 ; i++)
	{
		if((0x01 << i) & in_cardAddress)
			Addr_Byte[i] = 0x01;
	}

	int channel = -1;
	switch(in_cardAddress)
	{
		case CARD_ADDRESS_SHORT_24CH_1:
			channel = 0;
			break;

		case CARD_ADDRESS_SHORT_24CH_2:
			channel = 1;
			break;

		case CARD_ADDRESS_SHORT_24CH_3:
			channel = 2;
			break;

		case CARD_ADDRESS_SHORT_24CH_4:
			channel = 3;
			break;

		default:
			return false;
	}

	//데이터 초기화
	for(int x = 0; x < 50; x++)
		memset(m_short_data[channel][x], 0x00, sizeof(m_short_data[channel][x]));

	//카드 초기화
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

	//Control - 0~5-4개씩 6-enabled
	Addr_Byte[0] = 0x01;
	Addr_Byte[1] = 0x01;
	if(!send(Addr_Byte, Data_Byte, true))
		return false;


	float mulLoop = in_numOfPin/4;
	if(in_numOfPin%4 > 0.0)
		mulLoop++;

	//mulCh-멀티채널(4개씩 켜지는 것) sigCh-싱글채널(1갸씩 켜지는 것)
	for(int sigCh = 0; sigCh < in_numOfPin; sigCh++)
	{
		int pin = 0;
		//주소설정
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

		//핀 적용
		memset(Data_Byte, 0x00, sizeof(Data_Byte));
		Data_Byte[pin] = 0x01;
		if(!send(Addr_Byte, Data_Byte, true))
			return false;

		//컨트롤 바이트 설정
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

			memcpy(&m_short_data[channel][sigCh][mulCh*4], &AnalogData, sizeof(double)*4);
		}

		//초기화
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

	//Control - 초기화
	Addr_Byte[0] = 0x01;
	Addr_Byte[1] = 0x01;
	memset(Data_Byte, 0x00, sizeof(Data_Byte));
	if(!send(Addr_Byte, Data_Byte, true))
		return false;

	return true;
}

//==============================================================================

bool JK_ITLab::NICard::test_short_40ch(int in_numOfPin, int in_shortDelay, unsigned char in_cardAddress)
{
#ifdef JK_NI_TESTING
	return true;
#else
	if(in_numOfPin > 40)
		return false;

	char Addr_Byte[8] = {0,};
	char Data_Byte[8] = {0,};

	//주소만들기
	for(int i = 0 ; i < 8 ; i++)
	{
		if((0x01 << i) & in_cardAddress)
			Addr_Byte[i] = 0x01;
	}

	int channel = -1;
	switch(in_cardAddress)
	{
		case CARD_ADDRESS_IP_SHORT_MUX_40CH_1:
			channel = 0;
			break;

		case CARD_ADDRESS_IP_SHORT_MUX_40CH_2:
			channel = 1;
			break;

		case CARD_ADDRESS_IP_SHORT_MUX_40CH_3:
			channel = 2;
			break;

		case CARD_ADDRESS_IP_SHORT_MUX_40CH_4:
			channel = 3;
			break;

		default:
			return false;
	}
	//데이터 초기화
	memset(m_short_data, 0x00, sizeof(m_short_data));

	//카드 초기화
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

	//01 true Control - 0~5-8개씩 6-enabled
	Addr_Byte[0] = 0x01;
	Addr_Byte[1] = 0x01;
	if(!send(Addr_Byte, Data_Byte, true))
		return false;

	//control 핀 설정.
	float mulLoop = in_numOfPin/8;
	if(in_numOfPin%8 > 0.0)
		mulLoop++;

	//mulCh-멀티채널(8개씩 켜지는 것) sigCh-싱글채널(1개씩 켜지는 것)
	for(int sigCh = 0; sigCh < in_numOfPin; sigCh++)
	{
		bool isReadByteHi;
		int pin = 0;
		//주소설정
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

		//핀 적용
		memset(Data_Byte, 0x00, sizeof(Data_Byte));
		Data_Byte[pin] = 0x01;
		if(!send(Addr_Byte, Data_Byte, isReadByteHi))
			return false;

		//컨트롤 바이트 설정
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

			memcpy(&m_short_data[channel][sigCh][mulCh*8], &AnalogData, sizeof(double)*8);
		}

		//초기화
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

	//Control - 초기화
	Addr_Byte[0] = 0x01;
	Addr_Byte[1] = 0x00;
	memset(Data_Byte, 0x00, sizeof(Data_Byte));
	if(!send(Addr_Byte, Data_Byte, true))
		return false;

	return true;
#endif
}
//==============================================================================

bool JK_ITLab::NICard::set_ad(unsigned char in_cardAddress)
{
	char Data_Byte[8] = {0,};
	char Addr_Byte[8] = {0,};

	//주소만들기
	for(int i = 0 ; i < 8 ; i++)
	{
		if((0x01 << i) & in_cardAddress)
			Addr_Byte[i] = 0x01;
	}

	int channel = -1;
	switch(in_cardAddress)
	{
		case CARD_ADDRESS_AD_24CH_1:
			channel = 0;
			break;

		case CARD_ADDRESS_AD_24CH_2:
			channel = 1;
			break;

		case CARD_ADDRESS_AD_24CH_3:
			channel = 2;
			break;

		case CARD_ADDRESS_AD_24CH_4:
			channel = 3;
			break;

		default:
			return false;
	}

	//00 1~8
	Addr_Byte[0] = 0x00;
	Addr_Byte[1] = 0x00;
	memcpy(Data_Byte, &m_ad_set[channel][0], sizeof(Data_Byte));
	if(!send(Addr_Byte, Data_Byte, false))
		return false;

	//10 9~16
	Addr_Byte[0] = 0x01;
	Addr_Byte[1] = 0x00;
	memcpy(Data_Byte, &m_ad_set[channel][8], sizeof(Data_Byte));
	if(!send(Addr_Byte, Data_Byte, false))
		return false;

	//01 17~24
	Addr_Byte[0] = 0x00;
	Addr_Byte[1] = 0x01;
	memcpy(Data_Byte, &m_ad_set[channel][16], sizeof(Data_Byte));
	if(!send(Addr_Byte, Data_Byte, false))
		return false;

	return true;
}

//==============================================================================

bool JK_ITLab::NICard::set_onoff_output(unsigned char in_cardAddress)
{
 	char Data_Byte[8] = {0,};
	char Addr_Byte[8] = {0,};

	//주소만들기
	for(int i = 0 ; i < 8 ; i++)
	{
		if((0x01 << i) & in_cardAddress)
			Addr_Byte[i] = 0x01;
	}

	int channel = -1;
	switch(in_cardAddress)
	{
		case CARD_ADDRESS_ON_OFF_OUTPUT_24CH_1:
			channel = 0;
			break;

		case CARD_ADDRESS_ON_OFF_OUTPUT_24CH_2:
			channel = 1;
			break;

		case CARD_ADDRESS_ON_OFF_OUTPUT_24CH_3:
			channel = 2;
			break;

		case CARD_ADDRESS_ON_OFF_OUTPUT_24CH_4:
			channel = 3;
			break;

		default:
			return false;
	}

	//00 1~8
	Addr_Byte[0] = 0x00;
	Addr_Byte[1] = 0x00;
	memset(Data_Byte, 0x00, sizeof(Data_Byte));
	memcpy(Data_Byte, m_on_off_output[channel], sizeof(Data_Byte));

	if(!send(Addr_Byte, Data_Byte, true)) return false;

	//10 9~16
	Addr_Byte[0] = 0x01;
	Addr_Byte[1] = 0x00;
	memset(Data_Byte, 0x00, sizeof(Data_Byte));
	memcpy(Data_Byte, &m_on_off_output[channel][8], sizeof(Data_Byte));

	if(!send(Addr_Byte, Data_Byte, true)) return false;

	//01 17~24
	Addr_Byte[0] = 0x00;
	Addr_Byte[1] = 0x01;
	memset(Data_Byte, 0x00, sizeof(Data_Byte));
	memcpy(Data_Byte, &m_on_off_output[channel][16], sizeof(Data_Byte));

	if(!send(Addr_Byte, Data_Byte, true)) return false;

	return true;
}

//==============================================================================

bool JK_ITLab::NICard::set_current(unsigned char in_cardAddress)
{
 	char Addr_Byte[8] = {0,};
	char Data_Byte[8] = {0,};

	//주소만들기
	for(int i = 0 ; i < 8 ; i++)
	{
		if((0x01 << i) & in_cardAddress)
			Addr_Byte[i] = 0x01;
	}

	int channel = -1;
	switch(in_cardAddress)
	{
		case CARD_ADDRESS_CURRENT_4CH_1:
			channel = 0;
			break;

		case CARD_ADDRESS_CURRENT_4CH_2:
			channel = 1;
			break;

		case CARD_ADDRESS_CURRENT_4CH_3:
			channel = 2;
			break;

		case CARD_ADDRESS_CURRENT_4CH_4:
			channel = 3;
			break;

		default:
			return false;
	}
	Data_Byte[0] = m_current_shunt[channel][0];
	Data_Byte[1] = m_current_shunt[channel][1];
	Data_Byte[2] = m_current_shunt[channel][2];
	Data_Byte[3] = m_current_shunt[channel][3];
	Data_Byte[4] = m_current_ad[channel][0];
	Data_Byte[5] = m_current_ad[channel][1];
	Data_Byte[6] = m_current_ad[channel][2];
	Data_Byte[7] = m_current_ad[channel][3];
	if(!send(Addr_Byte, Data_Byte, true))
		return false;//채널 명령 전송

	return true;
}

//==============================================================================

void JK_ITLab::NICard::get_ni_ad()
{
#ifdef JK_NI_TESTING
	return ;
#else
	int32 read_analog;
	double Temp_AnalogData[16] = {0,};//AD_MAX_COUNT;
	double Temp_Input[16] = {0,};

    for(int i = 0 ; i < ANALOG_SAMPLING_COUNT; i++)
	{
		DAQmxReadAnalogF64(m_Handle_AnalogInput, -1, 10.0, DAQmx_Val_GroupByChannel, Temp_AnalogData, 16, &read_analog, NULL);
		/*
		DAQmxReadAnalogF64(taskHandle, -핸들
		1000, - 한번에 읽어올 채널당 샘플 갯수
		10.0,  - 입력에 대한 타임아웃 시간(초)
		DAQmx_Val_GroupByChannel, - 여러채널을 입력시 입력을 받는 데이터의 구성방식
		data, - 입력받을 배열
		1000, - 샘플이 입력될 배열의 사이즈
		&read, - 채널당 입력받은 실질적인 데이터 갯수
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
#ifdef JK_NI_TESTING
	return true;
#else
	int32 read, Sample;
	char Control_Byte[4] = {0,};

	DAQmxStopTask(m_Handle_DataInput);//input stop
	DAQmxStartTask(m_Handle_DataOutput);//output start

	/*
	DAQmxWriteDigitalLines(
	핸들 값,
	채널당 샘플 수,
	자동 시작 여부 true or false,
	타임아웃,
	데이터 레이아웃,
	데이터가 들어갈 곳,
	예약영역 NULL,
	예약영역 NULL);
	*/
	//Gate 닫기
	Control_Byte[0] = 0x01;
	Control_Byte[1] = 0x01;
	Control_Byte[2] = 0x01;
	Control_Byte[3] = 0x01;
	DAQmxWriteDigitalLines(m_Handle_Control, 1, 1, 10.0, DAQmx_Val_GroupByChannel, Control_Byte, NULL, NULL);


	Control_Byte[0] = 0x01; //GATE - LOW(OPEN) HIGH(CLOSE)
	Control_Byte[1] = 0x01; //DIR - 방향 HIGH(SEND) LOW(RECV) - Send이므로 High
	if(isReadByteHigh)
		Control_Byte[2] = 0x01; //READ - LOW일때부터 읽는다. - 레벨링방식 여기서는 write만 하므로 옵션값으로 사용한다.
	else
		Control_Byte[2] = 0x00;
	Control_Byte[3] = 0x01; //WRITE - LOW에서HIGH로갈때 읽는다. - 엣지방식

	DAQmxWriteDigitalLines(m_Handle_Addr, 1, 1, 10.0, DAQmx_Val_GroupByChannel, in_addr, NULL, NULL);
	DAQmxWriteDigitalLines(m_Handle_DataOutput, 1, 1, 10.0, DAQmx_Val_GroupByChannel, in_data, NULL, NULL);
	DAQmxWriteDigitalLines(m_Handle_Control, 1, 1, 10.0, DAQmx_Val_GroupByChannel, Control_Byte, NULL, NULL);

	//게이트 오픈
	Control_Byte[0] = 0x00;
	DAQmxWriteDigitalLines(m_Handle_Control, 1, 1, 10.0, DAQmx_Val_GroupByChannel, Control_Byte, NULL, NULL);

	//Write Low
	Control_Byte[3] = 0x00;
	DAQmxWriteDigitalLines(m_Handle_Control, 1, 1, 10.0, DAQmx_Val_GroupByChannel, Control_Byte, NULL, NULL);

	bool return_value = UnitSyncCheck();

	//Weite High - 엣지 방식이므로
	Control_Byte[3] = 0x01;
	DAQmxWriteDigitalLines(m_Handle_Control, 1, 1, 10.0, DAQmx_Val_GroupByChannel, Control_Byte, NULL, NULL);

	//Gate 닫기
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
#ifdef JK_NI_TESTING
	return true;
#else
	int32 read, Sample;
	char Control_Byte[4] = {0,};
	char Read_Byte[8] = {0,};

	DAQmxStopTask(m_Handle_DataOutput);//input stop
	DAQmxStartTask(m_Handle_DataInput);//output start

	Control_Byte[0] = 0x01;
	Control_Byte[1] = 0x01;
	Control_Byte[2] = 0x01;//read만 내린다.
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
	Control_Byte[2] = 0x00;//read만 내린다.
	Control_Byte[3] = 0x01;
	DAQmxWriteDigitalLines(m_Handle_Control, 1, 1, 10.0, DAQmx_Val_GroupByChannel, Control_Byte, NULL, NULL);

	bool return_value = UnitSyncCheck();

	DAQmxReadDigitalLines(m_Handle_DataInput, 1, 10.0, DAQmx_Val_GroupByChannel, Read_Byte, 8, &read, &Sample, NULL);

	Control_Byte[0] = 0x01;
	Control_Byte[1] = 0x01;
	Control_Byte[2] = 0x01;//read만 내린다.
	Control_Byte[3] = 0x01;
	DAQmxWriteDigitalLines(m_Handle_Control, 1, 1, 10.0, DAQmx_Val_GroupByChannel, Control_Byte, NULL, NULL);

	DAQmxStopTask(m_Handle_DataInput);

	//데이터 복사
	memcpy(out_data, Read_Byte, sizeof(Read_Byte));

	return return_value;
#endif
}

bool JK_ITLab::NICard::UnitSyncCheck()
{
	int32 read, Sample;
	char UnitSync = -1;

	if(!m_isUsingSync)
	{
		Sleep_Process(READ_WRITE_TIME);
		return true;//사용하지 않을 경우에 슬립후 true리턴한다.
	}
	unsigned long Tick = GetTickCount();
	while(true)
	{
		DAQmxReadDigitalLines(m_Handle_UnitSycn, 1, 10.0, DAQmx_Val_GroupByChannel, &UnitSync, 1, &read, &Sample, NULL);

		if(UnitSync == 0)//0이면 동기화 완료.
			break;

		if(GetTickCount() - Tick > UNIT_SYNC_TIMEOUT_MS)
		{
			return false;
		}
	}
	return true;
}

bool JK_ITLab::NICard::set_onoff_input(unsigned char in_cardAddress, bool is_40pin)
{
	char Data_Byte[8] = {0,};
	char Addr_Byte[8] = {0,};

	//주소만들기
	for(int i = 0 ; i < 8 ; i++)
	{
		if((0x01 << i) & in_cardAddress)
			Addr_Byte[i] = 0x01;
	}

	int channel = -1;
	switch(in_cardAddress)
	{
		case CAED_ADDRESS_ON_OFF_INPUT_SET_24CH_1:
			channel = 0;
			break;

		case CAED_ADDRESS_ON_OFF_INPUT_SET_24CH_2:
			channel = 1;
			break;

		case CAED_ADDRESS_ON_OFF_INPUT_SET_24CH_3:
			channel = 2;
			break;

		case CAED_ADDRESS_ON_OFF_INPUT_SET_24CH_4:
			channel = 3;
			break;

		default:
		return false;
	}

	if(is_40pin && in_cardAddress == CARD_ADDRESS_ON_OFF_INPUT_SET_40CH_2)
    	channel = 1;

	//00 1~8
	Addr_Byte[0] = 0x00;
	Addr_Byte[1] = 0x00;
	if(is_40pin)Addr_Byte[2] = 0x00;
	memset(Data_Byte, 0x00, sizeof(Data_Byte));
	memcpy(Data_Byte, m_on_off_input_pull_up[channel], sizeof(Data_Byte));
	if(!send(Addr_Byte, Data_Byte, true)) return false;
	memcpy(Data_Byte, m_on_off_input_pull_down[channel], sizeof(Data_Byte));
	if(!send(Addr_Byte, Data_Byte, false)) return false;

	//10 9~16
	Addr_Byte[0] = 0x01;
	Addr_Byte[1] = 0x00;
	if(is_40pin)Addr_Byte[2] = 0x00;
	memset(Data_Byte, 0x00, sizeof(Data_Byte));
	memcpy(Data_Byte, &m_on_off_input_pull_up[channel][8], sizeof(Data_Byte));
	if(!send(Addr_Byte, Data_Byte, true)) return false;
	memcpy(Data_Byte, &m_on_off_input_pull_down[channel][8], sizeof(Data_Byte));
	if(!send(Addr_Byte, Data_Byte, false)) return false;

	//01 17~24
	Addr_Byte[0] = 0x00;
	Addr_Byte[1] = 0x01;
	if(is_40pin)Addr_Byte[2] = 0x00;
	memset(Data_Byte, 0x00, sizeof(Data_Byte));
	memcpy(Data_Byte, &m_on_off_input_pull_up[channel][16], sizeof(Data_Byte));
	if(!send(Addr_Byte, Data_Byte, true)) return false;
	memcpy(Data_Byte, &m_on_off_input_pull_down[channel][16], sizeof(Data_Byte));
	if(!send(Addr_Byte, Data_Byte, false)) return false;

	if(is_40pin)
	{
		//01 25~32
		Addr_Byte[0] = 0x01;
		Addr_Byte[1] = 0x01;
		Addr_Byte[2] = 0x00;
		memset(Data_Byte, 0x00, sizeof(Data_Byte));
		memcpy(Data_Byte, &m_on_off_input_pull_up[channel][24], sizeof(Data_Byte));
		if(!send(Addr_Byte, Data_Byte, true)) return false;
		memcpy(Data_Byte, &m_on_off_input_pull_down[channel][24], sizeof(Data_Byte));
		if(!send(Addr_Byte, Data_Byte, false)) return false;

		//01 33~40
		Addr_Byte[0] = 0x00;
		Addr_Byte[1] = 0x00;
		Addr_Byte[2] = 0x01;
		memset(Data_Byte, 0x00, sizeof(Data_Byte));
		memcpy(Data_Byte, &m_on_off_input_pull_up[channel][32], sizeof(Data_Byte));
		if(!send(Addr_Byte, Data_Byte, true)) return false;
		memcpy(Data_Byte, &m_on_off_input_pull_down[channel][32], sizeof(Data_Byte));
		if(!send(Addr_Byte, Data_Byte, false)) return false;
    }

	return true;
}

bool JK_ITLab::NICard::get_onoff_input(unsigned char in_cardAddress, bool is_40pin)
{
	char Read_Byte[8] = {0,};
	char Addr_Byte[8] = {0,};

	//주소만들기
	for(int i = 0 ; i < 8 ; i++)
	{
		if((0x01 << i) & in_cardAddress)
			Addr_Byte[i] = 0x01;
	}

	int channel = -1;
	switch(in_cardAddress)
	{
		case CARD_ADDRESS_ON_OFF_INPUT_READ_24CH_1:
			channel = 0;
			break;

		case CARD_ADDRESS_ON_OFF_INPUT_READ_24CH_2:
			channel = 1;
			break;

		case CARD_ADDRESS_ON_OFF_INPUT_READ_24CH_3:
			channel = 2;
			break;

		case CARD_ADDRESS_ON_OFF_INPUT_READ_24CH_4:
			channel = 3;
			break;

		default:
		return false;
	}

	if(is_40pin && in_cardAddress == CARD_ADDRESS_ON_OFF_INPUT_READ_40CH_2)
		channel = 1;

	Addr_Byte[0] = 0x00;
	Addr_Byte[1] = 0x00;
	Addr_Byte[2] = 0x00;
	if(!recv(Read_Byte, Addr_Byte))
		return false;
	memcpy(&m_on_off_input[channel][0], Read_Byte, sizeof(Read_Byte));

	Addr_Byte[0] = 0x01;
	Addr_Byte[1] = 0x00;
	Addr_Byte[2] = 0x00;
	if(!recv(Read_Byte, Addr_Byte))
		return false;
	memcpy(&m_on_off_input[channel][8], Read_Byte, sizeof(Read_Byte));

	Addr_Byte[0] = 0x00;
	Addr_Byte[1] = 0x01;
	Addr_Byte[2] = 0x00;
	if(!recv(Read_Byte, Addr_Byte))
		return false;
	memcpy(&m_on_off_input[channel][16], Read_Byte, sizeof(Read_Byte));

	Addr_Byte[0] = 0x01;
	Addr_Byte[1] = 0x01;
	Addr_Byte[2] = 0x00;
	if(!recv(Read_Byte, Addr_Byte))
		return false;
	memcpy(&m_on_off_input[channel][24], Read_Byte, sizeof(Read_Byte));

	Addr_Byte[0] = 0x00;
	Addr_Byte[1] = 0x00;
	Addr_Byte[2] = 0x01;
	if(!recv(Read_Byte, Addr_Byte))
		return false;
	memcpy(&m_on_off_input[channel][32], Read_Byte, sizeof(Read_Byte));

	return true;
}

bool JK_ITLab::NICard::set_mux(unsigned char in_cardAddress)
{
 	char Data_Byte[8] = {0,};
	char Addr_Byte[8] = {0,};

	//주소만들기
	for(int i = 0 ; i < 8 ; i++)
	{
		if((0x01 << i) & in_cardAddress)
			Addr_Byte[i] = 0x01;
	}

	int channel = -1;
	switch(in_cardAddress)
	{
		case CARD_ADDRESS_IP_MUX_40CH_1:
			channel = 0;
			break;

		case CARD_ADDRESS_IP_MUX_40CH_2:
			channel = 1;
			break;

		case CARD_ADDRESS_IP_MUX_40CH_3:
			channel = 2;
			break;

		case CARD_ADDRESS_IP_MUX_40CH_4:
			channel = 3;
			break;

		case CARD_ADDRESS_IP_MUX_40CH_5:
			channel = 4;
			break;

		case CARD_ADDRESS_IP_MUX_40CH_6:
			channel = 5;
			break;

		case CARD_ADDRESS_IP_MUX_40CH_7:
			channel = 6;
			break;

		case CARD_ADDRESS_IP_MUX_40CH_8:
			channel = 7;
			break;

		default:
			return false;
	}

	//00 1~8
	Addr_Byte[0] = 0x00;
	Addr_Byte[1] = 0x00;
	memset(Data_Byte, 0x00, sizeof(Data_Byte));
	memcpy(Data_Byte, m_mux[channel], sizeof(Data_Byte));
	if(!send(Addr_Byte, Data_Byte, false)) return false;

	//10 9~16
	Addr_Byte[0] = 0x01;
	Addr_Byte[1] = 0x00;
	memset(Data_Byte, 0x00, sizeof(Data_Byte));
	memcpy(Data_Byte, &m_mux[channel][8], sizeof(Data_Byte));
	if(!send(Addr_Byte, Data_Byte, false)) return false;

	//01 17~24
	Addr_Byte[0] = 0x00;
	Addr_Byte[1] = 0x01;
	memset(Data_Byte, 0x00, sizeof(Data_Byte));
	memcpy(Data_Byte, &m_mux[channel][16], sizeof(Data_Byte));
	if(!send(Addr_Byte, Data_Byte, false)) return false;

	//01 25~32
	Addr_Byte[0] = 0x01;
	Addr_Byte[1] = 0x01;
	memset(Data_Byte, 0x00, sizeof(Data_Byte));
	memcpy(Data_Byte, &m_mux[channel][24], sizeof(Data_Byte));
	if(!send(Addr_Byte, Data_Byte, false)) return false;

	//01 33~40
	Addr_Byte[0] = 0x00;
	Addr_Byte[1] = 0x00;
	memset(Data_Byte, 0x00, sizeof(Data_Byte));
	memcpy(Data_Byte, &m_mux[channel][32], sizeof(Data_Byte));
	if(!send(Addr_Byte, Data_Byte, true)) return false;

	return true;
}


