/*
Copyright (C): 2020, Shenzhen Yahboom Tech
modified by chengengyue
*/

#include "pxt.h"
#include <map>
#include <vector>
#include "ReceiverIR.h"
using namespace pxt;
typedef vector<Action> vA;

enum class RemoteButton {
    Power = 0x0,
	  Up = 0x1,
	  Left = 0x4,
	  Right = 0x6,
	  Down = 0x9,
	  Light = 0x2,
	  BEEP = 0x5,
	  Plus = 0xc,
	  Minus = 0xe,
	  TLeft = 0x8,
	  TRight = 0xA,
	  NUM0 = 0xD,
	  NUM1 = 0x10,
	  NUM2 = 0x11,
	  NUM3 = 0x12,
	  NUM4 = 0x14,
	  NUM5 = 0x15,
	  NUM6 = 0x16,
	  NUM7 = 0x18,
	  NUM8 = 0x19,
	  NUM9 = 0x1A
};

enum class Pins{
    D1 = 20,
    D2 = 18,
    D3 = 1,
    D4 = 2
};

namespace Module_World_IR {
  map<RemoteButton, vA> actions;
  map<RemoteButton, uint32_t> lastact;
  Timer tsb;
  uint8_t buf[32];
  uint32_t now;
  ReceiverIR *rx;
  RemoteIR::Format fmt = RemoteIR::UNKNOWN;
  
    int ir_code = 0x00;
    int ir_addr = 0x00;
    int data;
    int pinv2 = 0;

  void cA(vA runner){for(int i=0;i<runner.size();i++){runAction0(runner[i]);} }

  void onReceivable(){
    int x = rx->getData(&fmt, buf, 32 * 8);
    if(actions.find((RemoteButton)buf[2]) == actions.end()) return;
    now = tsb.read_ms();
    if(now - lastact[(RemoteButton)buf[2]] < 100) return;
    lastact[(RemoteButton)buf[2]] = now;
    cA(actions[(RemoteButton)buf[2]]);
  }


  void monitorIR(){
    while(1){
      while(rx->getState() != ReceiverIR::Received){
        uBit.sleep(50);
      }
      onReceivable();
    }
  }

  //%
  void init(Pins pin){
    rx = new ReceiverIR((PinName)pin);
    tsb.start(); //interrupt timer for debounce
    create_fiber(monitorIR);
  }
  
  //%
  void initv2(Pins pin){
	pinv2 = (PinName)pin;
  }

  //%
  void onPressEvent(RemoteButton btn, Action body) {
    actions[btn].push_back(body);
  }

    int logic_value(){
        uint32_t lasttime = system_timer_current_time_us();
        uint32_t nowtime;
		if(pinv2 == 20)
			while(!uBit.io.P12.getDigitalValue());
		else if(pinv2 == 18)
			while(!uBit.io.P8.getDigitalValue());
		else if(pinv2 == 1)
			while(!uBit.io.P2.getDigitalValue());
		else 
			while(!uBit.io.P1.getDigitalValue());
        nowtime = system_timer_current_time_us();
        if((nowtime - lasttime) > 400 && (nowtime - lasttime) < 700){
			if(pinv2 == 20)
				while(uBit.io.P12.getDigitalValue());
			else if(pinv2 == 18)
				while(uBit.io.P8.getDigitalValue());
			else if(pinv2 == 1)
				while(uBit.io.P2.getDigitalValue());
			else
				while(uBit.io.P1.getDigitalValue());
            lasttime = system_timer_current_time_us();
            if((lasttime - nowtime)>400 && (lasttime - nowtime) < 700){
                return 0;
            }else if((lasttime - nowtime)>1500 && (lasttime - nowtime) < 1800){
                return 1;
			}
        }
        return -1;
    }

    void pulse_deal(){
        int i;
        ir_addr=0x00;
        for(i=0; i<16;i++ )
        {
			if(logic_value() == 1)
			{
				ir_addr |=(1<<i);
			}
        }
        ir_code=0x00;
        for(i=0; i<16;i++ )
        {
			if(logic_value() == 1)
			{
				ir_code |=(1<<i);
			}
        }

    }

    void remote_decode(void){
        data = 0x00;
        uint32_t lasttime = system_timer_current_time_us();
        uint32_t nowtime;
		if(pinv2 == 20)
			while(uBit.io.P12.getDigitalValue()){
				nowtime = system_timer_current_time_us();
				if((nowtime - lasttime) > 100000){
					ir_code = 0xff00;
					return;
				}
			}
		else if(pinv2 == 18)
			while(uBit.io.P8.getDigitalValue()){
				nowtime = system_timer_current_time_us();
				if((nowtime - lasttime) > 100000){
					ir_code = 0xff00;
					return;
				}
			}
		else if(pinv2 == 1)
			while(uBit.io.P2.getDigitalValue()){
				nowtime = system_timer_current_time_us();
				if((nowtime - lasttime) > 100000){
					ir_code = 0xff00;
					return;
				}
			}
		else 
			while(uBit.io.P1.getDigitalValue()){
				nowtime = system_timer_current_time_us();
				if((nowtime - lasttime) > 100000){
					ir_code = 0xff00;
					return;
				}
			}
        lasttime = system_timer_current_time_us();
		if(pinv2 == 20)
			while(!uBit.io.P12.getDigitalValue());
		else if(pinv2 == 18)
			while(!uBit.io.P8.getDigitalValue());
		else if(pinv2 == 1)
			while(!uBit.io.P2.getDigitalValue());
		else
			while(!uBit.io.P1.getDigitalValue());
        nowtime = system_timer_current_time_us();
        if((nowtime - lasttime) < 10000 && (nowtime - lasttime) > 8000){
			if(pinv2 == 20)
				while(uBit.io.P12.getDigitalValue());
			else if(pinv2 == 18)
				while(uBit.io.P8.getDigitalValue());
			else if(pinv2 == 1)
				while(uBit.io.P2.getDigitalValue());
			else
				while(uBit.io.P1.getDigitalValue());
            lasttime = system_timer_current_time_us();
            if((lasttime - nowtime) > 4000 && (lasttime - nowtime) < 5000){
                pulse_deal();
                data = ir_code;
                return;
            }else if((lasttime - nowtime) > 2000 && (lasttime - nowtime) < 2500){
				if(pinv2 == 20)
					while(!uBit.io.P12.getDigitalValue());
				else if(pinv2 == 18)
					while(!uBit.io.P8.getDigitalValue());
				else if(pinv2 == 1)
					while(!uBit.io.P2.getDigitalValue());
				else
					while(!uBit.io.P1.getDigitalValue());
                nowtime = system_timer_current_time_us();
                if((nowtime - lasttime) > 500 && (nowtime - lasttime) < 700){
                    data = ir_code;
                    return;
                }
            }
        }
    }

    //% 
    int irCode(){
        remote_decode();
        return data;
    }

}
