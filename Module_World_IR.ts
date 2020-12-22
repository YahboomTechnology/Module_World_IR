/*
Copyright (C): 2020, Shenzhen Yahboom Tech
modified by chengengyue
*/

/**
 * IR remote
 */
//% icon="\uf1eb" weight=19 color="#ad7e2e"
namespace Module_World_IR {

    let irstate:number;
    let state:number;
		
    /**
    * initialization
    */
    //% blockId=Mbit_IR_init
    //% blockGap=20 weight=90
    //% block="connect ir receiver to %pin"
	//% group="micro:bit(V1)"
    //% shim=Module_World_IR::init
    export function init(pin: Pins): void {
      return
    }
  
    /**
    * button pushed.
    */
    //% blockId=Mbit_IR_received_event
    //% blockGap=20 weight=89
	//% group="micro:bit(V1)"
    //% block="on |%btn| button pressed"
    //% shim=Module_World_IR::onPressEvent
    export function onPressEvent(btn: RemoteButton, body:Action): void {
      return
    }
			
    //% blockId=Mbit_IR_initv2
    //% blockGap=20 weight=8
	//% group="micro:bit(V2)"
    //% color="#867455"
    //% block="connect ir receiver to %pin"
    //% shim=Module_World_IR::initv2
    export function initv2(pin: Pins): void {
      return
    }

    //% advanced=true shim=Module_World_IR::irCode
    function irCode(): number {
        return 0;
    }

    //% weight=5
    //% group="micro:bit(V2)"
    //% color="#867455"
    //% blockId=IR_readV2 block="read IR |%btn| pressed"
    export function IR_readV2(btn: RemoteButton): boolean {
		let btnr = valuotokeyConversion();
		return btn == btnr;
    }

    //% weight=2
    //% group="micro:bit(V2)"
    //% color="#867455"
    //% blockId=IR_callbackUserV2 block="on IR received"
    //% draggableParameters
    export function IR_callbackUserV2(cb: (message: number) => void) {
        state = 1;
        control.onEvent(11, 22, function() {
            cb(irstate)
        }) 
    }

    function valuotokeyConversion():number{
        let irdata:number;
        switch(irCode()){
            case 0xff00:irdata = 0;break;
            case 0xfe01:irdata = 1;break;
            case 0xfd02:irdata = 2;break;
            case 0xfb04:irdata = 4;break;
            case 0xfa05:irdata = 5;break;
            case 0xf906:irdata = 6;break;
            case 0xf708:irdata = 8;break;
            case 0xf609:irdata = 9;break;
            case 0xf50a:irdata = 10;break;
            case 0xf30c:irdata = 12;break;
            case 0xf20d:irdata = 13;break;
            case 0xf10e:irdata = 14;break;
            case 0xef10:irdata = 16;break;
            case 0xee11:irdata = 17;break;
            case 0xed12:irdata = 18;break;
            case 0xeb14:irdata = 20;break;
            case 0xea15:irdata = 21;break;
            case 0xe916:irdata = 22;break;
            case 0xe718:irdata = 24;break;
            case 0xe619:irdata = 25;break;
            case 0xe51a:irdata = 26;break;
            default:
             irdata = -1;
        }
        return irdata;
    }

    basic.forever(() => {
        if(state == 1){
            irstate = valuotokeyConversion();
            if(irstate != -1){
                control.raiseEvent(11, 22)
            }
        }
    })

  }
  