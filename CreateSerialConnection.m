function [ com ] = CreateSerialConnection( portName,baudrate)
    com = serial(portName,'Baudrate', baudrate,'Terminator','','StopBits',1,'Parity','none','FlowControl','none','Timeout',0.1);
    fopen(com);
end
