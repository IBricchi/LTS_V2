Dependent source test
*
R1 N001 N002 10k
R2 N004 0 3k
V1 N003 0 SINE(5 1 10)
V2 N001 0 20
Q1 N002 N003 N004 NPN
*
.model NPN NPN
.tran 0.0001 0.5 0
.end