Module_World_IR.initv2(Pins.D4)
Module_World_IR.IR_callbackUserV2(function(message: number) {  
    if (Module_World_IR.IR_readV2(RemoteButton.Power)) {
        basic.showNumber(1)
    } 
    if (Module_World_IR.IR_readV2(RemoteButton.Up)) {
        basic.showNumber(2)
    }
})