#!/bin/bash

CTRL_INPUT=0x8
CTRL_ENABLE=0x4
CTRL_CAPTURE=0x1

./ttctool.exe -dapollo.c2c.vu7p -v -x0;

# load the ipbus input buffers
./mgttool.exe -dapollo.c2c.vu7p -q1 -c0  -brx -p -fmemfiles/memfile_VMStubTEInner_0.dat ; sleep 1;
./mgttool.exe -dapollo.c2c.vu7p -q1 -c1  -brx -p -fmemfiles/memfile_VMStubTEInner_1.dat ; sleep 1;
./mgttool.exe -dapollo.c2c.vu7p -q1 -c2  -brx -p -fmemfiles/memfile_VMStubTEOuter_0.dat ; sleep 1;
./mgttool.exe -dapollo.c2c.vu7p -q1 -c3  -brx -p -fmemfiles/memfile_VMStubTEOuter_1.dat ; sleep 1;
./mgttool.exe -dapollo.c2c.vu7p -q2 -c0  -brx -p -fmemfiles/memfile_AllStub_tetc.dat ; sleep 1;

# load data into memories
./regtool.exe -dapollo.c2c.vu7p -rpayload.csr.ctrl0.algo_ctrl ${CTRL_INPUT}
sleep 1;

# run the TE-TC chain
./regtool.exe -dapollo.c2c.vu7p -rpayload.csr.ctrl0.algo_ctrl ${CTRL_ENABLE}
sleep 1;

# play results out of the payload 
./regtool.exe -dapollo.c2c.vu7p -rpayload.csr.ctrl0.algo_ctrl ${CTRL_CAPTURE}
sleep 1;

# capture the data using :
#./mgttool.exe -dapollo.c2c.vu7p -q1 -c0  -btx --capture > capture_TrackletParameters.log ;
./mgttool.exe -dapollo.c2c.vu7p -q1 -c1  -btx --capture > capture_TPROJ_BarrelPS.log ;
#./mgttool.exe -dapollo.c2c.vu7p -q1 -c2  -btx --capture > capture_TPROJ_Barrel2S_0.log ;
#./mgttool.exe -dapollo.c2c.vu7p -q1 -c3  -btx --capture > capture_TPROJ_Barrel2S_1.log ;
#./mgttool.exe -dapollo.c2c.vu7p -q2 -c0  -btx --capture > capture_TPROJ_Disk_0.log ;
#./mgttool.exe -dapollo.c2c.vu7p -q2 -c1  -btx --capture > capture_TPROJ_Disk_1.log ;

#./print_capture.exe capture_TPROJ_BarrelPS.log ;
#./print_capture_long.exe capture_TrackletParameters.log ;
