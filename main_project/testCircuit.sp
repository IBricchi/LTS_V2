TEST CIRCUIT
*
* DIFFERENTIAL AMPLIFIER
*Q1 N003 N005 N001 0 PNP
*Q2 N002 N006 N001 0 PNP
*R1 N003 0 10k
*I1 N007 N001 2
*Q3 N003 N002 N004 0 NPN
*Q4 N002 N002 N004 0 NPN
*V1 N004 0 -20
*V2 N007 0 20
*V3 N006 0 SIN(0 1 2)
*V4 N005 0 SIN(0 1 13)
*
* PNP common emitter amplifier
*Q1 N002 N003 N004 0 PNP
*R1 N001 N002 10k
*R2 N004 0 3k
*V1 N003 0 SINE(-5 1 10)
*V2 N001 0 -20
*
* Darlington pair
*Q1 N001 N003 N002 0 NPN
*Q2 N001 N002 N004 0 NPN
*V1 N003 0 sin(0 2 10)
*R1 N004 0 10k
*
* Push Pull stage
*Q1 N003 N001 N002 0 NPN
*Q2 N004 N001 N002 0 PNP
*V1 N001 0 SIN(0 1 5)
*V2 N004 0 -20
*V3 N003 0 20
*R1 N002 0 10k
*
V1 N003 0 SINE(0 5 10)
R1 N004 0 2k
R2 N001 N002 2k
V2 N001 0 10
M1 N002 N003 N004 0 PNP
*
*Q1 N001 N003 N004 0 NPN
*R1 N002 N001 4.7k
*R2 N004 0 1k
*C1 N004 0 20µ
*R3 N003 0 1k
*R4 N002 N003 4.7k
*V2 N002 0 9
*V1 N003 0 Sin(0 2 10)
*
.model PNP PNP
.model NPN NPN
.model PMOS PMOS
.model NMOS NMOS
.tran 0.0001 0.5 0
.end
