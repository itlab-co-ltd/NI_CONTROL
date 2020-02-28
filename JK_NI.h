/*============================================================================//
Ver 6.1 - 2019.11.27

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
