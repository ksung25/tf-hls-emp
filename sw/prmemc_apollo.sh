#!/bin/bash

CTRL_INPUT=0x8
CTRL_ENABLE=0x4
CTRL_CAPTURE=0x1

./ttctool.exe -dapollo.c2c.vu7p -v -x0;

# load the ipbus input buffers
./mgttool.exe -dapollo.c2c.vu7p -q1 -c0  -brx -p -fmemfiles/memfile_TProj.dat ; sleep 1;
./mgttool.exe -dapollo.c2c.vu7p -q1 -c1  -brx -p -fmemfiles/memfile_VMStubME.dat ; sleep 1;
./mgttool.exe -dapollo.c2c.vu7p -q1 -c2  -brx -p -fmemfiles/memfile_AllStub.dat ; sleep 1;

# load data into memories
./regtool.exe -dapollo.c2c.vu7p -rpayload.csr.ctrl0.algo_ctrl ${CTRL_INPUT}
sleep 1;
# start the processing with BX=0
./regtool.exe -dapollo.c2c.vu7p -rpayload.csr.ctrl0.bx_ctrl 0x0
./regtool.exe -dapollo.c2c.vu7p -rpayload.csr.ctrl0.algo_ctrl ${CTRL_ENABLE}
sleep 1;

# load data into memories
./regtool.exe -dapollo.c2c.vu7p -rpayload.csr.ctrl0.algo_ctrl ${CTRL_INPUT}
sleep 1;
# start the processing with BX=1
./regtool.exe -dapollo.c2c.vu7p -rpayload.csr.ctrl0.bx_ctrl 0x1
./regtool.exe -dapollo.c2c.vu7p -rpayload.csr.ctrl0.algo_ctrl ${CTRL_ENABLE}
sleep 1;

# play results out of the payload 
./regtool.exe -dapollo.c2c.vu7p -rpayload.csr.ctrl0.algo_ctrl ${CTRL_CAPTURE}
sleep 1;

# capture the data using :
./mgttool.exe -dapollo.c2c.vu7p -q1 -c3 -btx --capture > capture_prmemc.log ;
#./print_capture.exe capture_prmemc.log ;
