q_test for Windows DOS box 2002/6/1 
=== check user_eg === 
"user_eg 'QR1 p n Qt' 'v p' Fp" 
"user_eg2 'QR1 p' 'v p' Fp" 
=== check front ends ========================================================== 
"qconvex -" 
"qconvex ." 
"qconvex" 
"rbox c D3 | qconvex s n Qt" 
"rbox c D2 | qconvex i Qt " 
"rbox c D2 | qconvex o" 
"rbox 1000 s | qconvex s Tv FA" 
"rbox c d D2 | qconvex s Qc Fx" 
"rbox y 1000 W0 | qconvex s n " 
"rbox y 1000 W0 | qconvex s QJ" 
"rbox d G1 D12 | qconvex QR0 FA" 
"rbox c D6 | qconvex FA TF500" 
"rbox c P0 d D2 | qconvex p Fa Fc FP FI Fn FN FS Fv Fx" 
"rbox c d D2 | qconvex s i QV0" 
"rbox c | qconvex Q0" 
"qvoronoi -" 
"qvoronoi ." 
"qvoronoi" 
"rbox c P0 D2 | qvoronoi s o" 
"rbox c P0 D2 | qvoronoi Fi" 
"rbox c P0 D2 | qvoronoi Fo" 
"rbox c P0 D2 | qvoronoi Fv" 
"rbox c P0 D2 | qvoronoi s Qu Qt Fv" 
"rbox c P0 D2 | qvoronoi Qu Fo" 
"rbox c G1 d D2 | qvoronoi s p " 
"rbox c G1 d D2 | qvoronoi QJ p " 
"rbox c P-0.1 P+0.1 P+0.1 D2 | qvoronoi s Fc FP FQ Fn FN " 
"rbox P0 c D2 | qvoronoi s Fv QV0" 
"qdelaunay -" 
"qdelaunay ." 
"qdelaunay" 
"rbox c P0 D2 | qdelaunay s o" 
"rbox c P0 D2 | qdelaunay i" 
"rbox c P0 D2 | qdelaunay Fv" 
"rbox c P0 D2 | qdelaunay s Qu Qt Fv" 
"rbox c G1 d D2 | qdelaunay s i" 
"rbox c G1 d D2 | qdelaunay Ft" 
"rbox c G1 d D2 | qdelaunay QJ s Ft" 
"rbox M3,4 z 100 D2 | qdelaunay s" 
"rbox c P-0.1 P+0.1 P+0.1 D2 | qdelaunay s Fx Fa Fc FP FQ Fn FN" 
"rbox P0 P0 c D2 | qdelaunay s FP QV0" 
"qhalf -" 
"qhalf ." 
"qhalf" 
"rbox d | qhull FQ n | qhalf s Qt H0,0,0 Fp" 
"rbox c | qhull FQ FV n | qhalf s i" 
"rbox c | qhull FQ FV n | qhalf o" 
"rbox d D2 | qhull FQ n | qhalf s H0 Fc FP Fn FN FQ Fv Fx" 
=== check quality of merges for ${d:-`hostname`} ${d:-`date`} 
"rbox 1000 W0 t | qhull QR2 QJ s Fs Tv" 
"rbox 1000 W0 t | qhull QR2 s Fs Tv" 
"rbox 1000 s t | qhull C0.02 Qc Tv" 
"rbox 500 s D4 t | qhull C0.01 Qc Tv" 
"rbox 1000 s t | qhull C-0.02 Qc Tv" 
"rbox 1000 s t | qhull C-0.02 Qc Tv" 
"rbox 1000 s D4 t | qhull C-0.01 Qc Tv" 
"rbox 200 s D5 t | qhull C-0.01 Qx Qc Tv" 
"rbox 100 s D6 t | qhull C-0.001 Qx Qc Tv" 
"rbox 1000 W1e-4 t | qhull C-1e-6 Qc Tv" 
"rbox 1000 W5e-4 D4 t | qhull C-1e-5 Qc Tv" 
"rbox 400 W1e-3 D5 t | qhull C-1e-5 Qx Qc Tv" 
=== check input format etc. ${d:-`date`} 
"rbox d h | qhull Fd FV n FD  Tcv | qhull Fd H Fp Tcv" 
"rbox 10 h | qhull Fd FD p Tcv | qhull Fd d Tcv " 
=== check rbox ${d:-`date`} 
"rbox 3 n D2" 
"rbox 3 D2" 
"rbox 3 h D2" 
"rbox 3 z D2" 
"rbox 3 z h D2" 
"rbox 3 B10 D2" 
"rbox 3 z B10 D2" 
"rbox 4 L2 r D2" 
"rbox 8 L2 D2" 
"rbox 4 L4 r D3" 
"rbox y" 
"rbox 10 M3,4" 
"rbox 4 L4 s D5 W1e-3 " 
"rbox 10 L2 s D3 | qhull Tcv" 
"rbox 10 L4 s W1e-3 D3 | qhull Tcv" 
"rbox 10 L6 D3 | qhull Tcv" 
"rbox 10 L1.1 s D4 | qhull Tcv" 
"rbox y r 100 W0 O0.5 | qhull s p Tcv" 
"rbox x r 100 W0 O0.5 | qhull s p Tcv" 
"rbox 12 D8 | qhull Tcv" 
"rbox 12 D9 | qhull Tcv" 
"rbox 1000 D4 | qhull s i A-0.97 C0.2 A0.7 Tcv" 
"rbox 3 D2 | qhull Qb0B1:-2 p " 
"rbox 100 r D2 | qhull Pd0:0.7 PD0:0.8 n Tcv" 
"rbox 1000 s | qhull  C0.05 Tcv" 
"rbox 1000 s t | qhull Qm C0.05 Tcv" 
"rbox 500 D2 | qhull n A-0.95 C0.1 Tcv" 
"rbox 500 s P1,1,1 | qhull QgG0 Pp Tcv" 
"rbox d | qhull m" 
"rbox c D2 | qhull Q0 Tcv" 
"rbox d D2 | qhull Tcv" 
"rbox c D3 | qhull Q0 Tcv" 
"rbox d D3 | qhull Tcv" 
"rbox c D4 | qhull Q0 Tcv" 
"rbox d D4 | qhull Tcv" 
"rbox c D5 | qhull Q0 Tcv" 
"rbox d D5 | qhull Tcv" 
"rbox c D6 | qhull Q0 Tcv" 
"rbox d D6 | qhull Tcv" 
"rbox d D7 | qhull Tcv" 
"rbox c D2 | qhull Tcv C-0" 
"rbox c D3 | qhull Tcv C-0" 
"rbox c D4 | qhull Tcv C-0" 
"rbox c D5 | qhull Tcv C-0" 
"rbox c D6 | qhull Tcv C-0" 
"rbox c D7 | qhull Tv C-0" 
"rbox 20 l D3 | qhull Tcv" 
"rbox 100 s D2 | qhull Tcv" 
"rbox 100 s D3 | qhull Tcv" 
"rbox 100 s D4 | qhull Tcv" 
"rbox 100 s c D4 | qhull Tcv" 
"rbox 100 s d G1.5 D4 | qhull Tcv" 
"rbox 100 s W1e-2 | qhull Tcv" 
"rbox 100 | qhull Tcv" 
"rbox 100 W1e-3 | qhull Tcv" 
"rbox 100 r D2 | qhull Tcv" 
"rbox 100 r s Z1 | qhull Tcv" 
"rbox 100 r s Z1 G0.1 | qhull Tcv C-0" 
"rbox 100 s Z1 G0.1 | qhull Tcv " 
"rbox 100 s Z1e-5 G0.1 | qhull Tc Pp" 
=== check qhull output formats ${d:-`date`} 
"rbox 5 r s D2 | qhull Tcv" 
"rbox 5 r s D2 | qhull s " 
"rbox 5 r s D2 | qhull s o " 
"rbox 5 r s D2 | qhull f" 
"rbox 5 r s D2 | qhull i " 
"rbox 5 r s D2 | qhull m " 
"rbox 5 r s D2 | qhull n " 
"rbox 5 r s D2 | qhull p " 
"rbox 5 r s D2 | qhull o " 
"rbox 5 r s D2 | qhull Fx" 
"rbox 5 r s D2 | qhull p n i p p" 
"rbox 10 D3 | qhull f Tcv" 
"rbox 10 D3 | qhull i" 
"rbox 10 D3 | qhull p " 
"rbox 10 D3 | qhull o " 
"rbox 10 D3 | qhull Fx " 
"rbox 27 M1,0,1 | qhull Qc" 
"rbox 50 D3 s | qhull C0.1 Qc Pd0d1d2 s p Tcv" 
"rbox 10 D2 P0 P1e-15 | qhull d Qc FP s Tcv" 
"rbox 100 s | qhull C-0.003 Qc FP s" 
"rbox 100 s D2 | qhull C0.1 i Fx Tcv" 
"rbox 4 s D3 | qhull Qc Ghipv Tcv " 
"rbox 6 D4 | qhull f Tcv" 
"rbox 6 D4 | qhull i" 
"rbox 6 D4 | qhull p " 
"rbox 6 D4 | qhull o" 
"rbox 1000 s D2 | qhull FA Tcv" 
"rbox 1000 s | qhull FA Tcv" 
"rbox c D4 | qhull FA Tcv" 
"rbox c D5 | qhull FA Tcv" 
"rbox 10 D2 | qhull d FA Tcv" 
"rbox 10 D2 | qhull d Qu FA Tcv" 
"rbox 10 D2 | qhull FA Tcv" 
"rbox 10 c D2 | qhull Fx Tcv" 
"rbox 1000 s | qhull FS Tcv" 
"rbox 10 W0 D2 | qhull p Qc FcC Tcv" 
"rbox 4 z h s D2 | qhull Fd s n FD Tcv" 
"rbox 6 s D3 | qhull C-0.1 Qc FF s FQ Fi n Fo FQ FI Fm Fn FN FO FO FQ Fs FS FV Fv Tcv" 
"rbox P0.5,0.5 P0.5,0.5 W0 5 D2 | qhull d FN Qc" 
"rbox 10 D3 | qhull Fa PA5" 
"rbox 10 D3 | qhull Fa PF0.4" 
=== test Qt 
"rbox c | qhull Qt s o Tcv" 
"rbox c | qhull Qt f i" 
"rbox c | qhull Qt m n" 
"rbox c | qhull Qt p o" 
"rbox c | qhull Qt Fx" 
"rbox c | qhull Qt FA s Fa" 
"rbox 6 r s c G0.1 D2 | qhull Qt d FA Tcv" 
"rbox 6 r s c G0.1 D2 | qhull d FA Tcv" 
"rbox 6 r s c G0.1 D2 | qhull Qt v p Tcv" 
"rbox c | qhull Qt C-0.1 Qc FF s FQ Fi n Fo FQ FI Fm Fn FN FO FO FQ Fs FS FV Fv Tcv" 
"rbox 6 r s c G0.1 D2 P0.1,0.1 | qhull s FP d FO Qt" 
"RBOX 100 W0 | QHULL Tv Q11 FO" 
=== test unbounded intersection 
"rbox c | qhull PD0:0.5 n | qhull H0 Fp Tcv" 
"rbox 1000 W1e-3 D3 | qhull PA8 Fa FS s n Tcv " 
"rbox 1000 W1e-3 D3 | qhull C-0.01 PM10 Fm n Tcv Qc" 
"rbox 1000 W1e-3 D3 | qhull C-0.01 PA8 PG n Tcv Qc" 
"type q_test.log.1" 
=== check Delaunay/Voronoi ${d:-`date`} 
"rbox 10 D2 | qhull d Tcv" 
"rbox 10 D2 | qhull d Qz Tcv" 
"rbox 10 D3 | qhull d Tcv" 
"rbox c | qhull d Qz Ft Tcv" 
"rbox 10 s D2 c | qhull d Tcv" 
"rbox 10 s D2 | qhull d Tcv Qz Q8" 
"rbox 10 D2 | qhull d Tcv p" 
"rbox 10 D2 | qhull d Tcv i" 
"rbox 10 D2 | qhull d Tcv o" 
"rbox 10 D2 | qhull v Tcv o" 
"rbox 10 D2 | qhull v Tcv p" 
"rbox 10 D2 | qhull v Tcv G" 
"rbox 10 D2 | qhull v Tcv Fv" 
"rbox 10 D2 | qhull v Tcv Fi" 
"rbox 10 D2 | qhull v Tcv Fo" 
"rbox 10 D2 | qhull v Qu Tcv o Fv Fi Fo" 
"rbox 10 D3 | qhull v Tcv Fv" 
"rbox 10 D3 | qhull v Tcv Fi" 
"rbox 10 D3 | qhull v Tcv Fo" 
"rbox 10 D3 | qhull v Qu Tcv o Fv Fi Fo" 
"rbox 5 D2 | qhull v f FnN o" 
=== check Halfspace ${d:-`date`} 
"rbox 100 s D4 | qhull FA FV n s Tcv | qhull H Fp Tcv | qhull FA  Tcv" 
"rbox d D3 | qhull FQ n s FD Tcv | qhull Fd H0.1,0.1 Fp FQ Tcv" 
"rbox 5 r D2 | qhull s n Tcv | qhull H0 Fp Tcv" 
=== check qhull ${d:-`date`} 
"rbox 10 s D3 | qhull Tcv" 
"rbox 10 s D3 | qhull f Pd0:0.5 Pd2 Tcv" 
"rbox 10 s D3 | qhull f Tcv PD2:-0.5" 
"rbox 10 s D3 | qhull QR-1" 
"rbox 10 s D3 | qhull QR-40" 
"rbox 1000 D3 | qhull Tcvs" 
"rbox 100 s D3 | qhull TcvV-2" 
"rbox 100 s D3 | qhull TcvC2" 
"rbox 100 s D3 | qhull TcvV2" 
"rbox 100 s D3 | qhull T1cvV2P2" 
"rbox 100 s D3 | qhull TcvF100" 
"rbox 100 s D3 | qhull Qf Tcv" 
"rbox 100 D3 | qhull Tcv" 
"rbox 100 D3 | qhull Qs Tcv" 
"rbox 100 D5 | qhull Qs Tcv" 
"rbox 100 D3 | qhull Qr Tcv" 
"rbox 100 D3 | qhull Qxv Tcv" 
"rbox 100 D3 | qhull Qi f Pd0 Pd1 Pd2 Tcv" 
"rbox c d | qhull Qc f Tcv" 
"rbox c d | qhull Qc p Tcv" 
"rbox 100 D3 | qhull QbB FO Tcv" 
"rbox 1000 D2 B1e6 | qhull d Qbb FO Tcv" 
"rbox 10 D3 | qhull QbB p Tcv" 
"rbox 10 D3 | qhull Qbb p Tcv" 
"rbox 10 D3 | qhull Qb0:-10B2:20 p Tcv" 
"rbox 10 D3 | qhull Qb0:-10B2:20 p Tcv | qhull QbB p Tcv" 
"rbox 10 D3 | qhull Qb1:0B1:0 d Tcv Q8" 
"rbox 10 D3 | qhull Qb1:0B1:0B2:0 d Tcv Q8" 
"rbox 10 D3 | qhull Qb1:0 d Tcv " 
"rbox 10 D3 | qhull Qb1:0B1:0  Tcv" 
"== next command will error ==" 
"rbox 10 D2 | qhull Qb1:1B1:1 Tcv" 
"rbox 200 L20 D2 t | qhull FO Tcv C-0" 
"rbox 1000 L20 t | qhull FO Tcv C-0" 
"rbox 200 L20 D4 t | qhull FO Tcv C-0" 
"rbox 200 L20 D5 t | qhull FO Tcv Qx" 
"rbox 1000 W1e-3 s D2 t | qhull d FO Tcv Qu Q0" 
"rbox 1000 W1e-3 s D2 t | qhull d FO Tcv Qu C-0" 
=== check joggle and TRn ${d:-`date`} 
"rbox 100 W0 | qhull QJ1e-14 Qc TR100 Tv" 
"rbox 100 W0 | qhull QJ1e-13 Qc TR100 Tv" 
"rbox 100 W0 | qhull QJ1e-12 Qc TR100 Tv" 
"rbox 100 W0 | qhull QJ1e-11 Qc TR100 Tv" 
"rbox 100 W0 | qhull QJ1e-10 Qc TR100 Tv" 
"rbox 100 | qhull d QJ Qb0:1e4 QB0:1e5 Qb1:1e4 QB1:1e6 Qb2:1e5 QB2:1e7 FO Tv" 
=== check precision options ${d:-`date`} 
"rbox 100 D3 s | qhull E0.01 Qx Tcv FO" 
"rbox 100 D3 W1e-1 | qhull W1e-3 Tcv" 
"rbox 100 D3 W1e-1 | qhull W1e-2 Tcv Q0" 
"rbox 100 D3 W1e-1 | qhull W1e-2 Tcv" 
"rbox 100 D3 W1e-1 | qhull W1e-1 Tcv" 
"rbox 15 D2 P0 P1e-14,1e-14 | qhull d Quc Tcv" 
"rbox 15 D3 P0 P1e-12,1e-14,1e-14 | qhull d Qcu Tcv" 
"rbox 1000 s D3 | qhull C-0.01 Tcv Qc" 
"rbox 1000 s D3 | qhull C-0.01 V0 Qc Tcv" 
"rbox 1000 s D3 | qhull C-0.01 U0 Qc Tcv" 
"rbox 1000 s D3 | qhull C-0.01 V0 Qcm Tcv" 
"rbox 1000 s D3 | qhull C-0.01 Qcm Tcv" 
"rbox 1000 s D3 | qhull C-0.01 Q1 FO Tcv Qc" 
"rbox 1000 s D3 | qhull C-0.01 Q2 FO Tcv Qc" 
"rbox 1000 s D3 | qhull C-0.01 Q3 FO Tcv Qc" 
"rbox 1000 s D3 | qhull C-0.01 Q4 FO Tcv Qc" 
=== this may generate an error ${d:-`date`} 
"rbox 1000 s D3 | qhull C-0.01 Q5 FO Tcv " 
=== this should generate an error ${d:-`date`} 
"rbox 1000 s D3 | qhull C-0.01 Q6 FO Po Tcv Qc" 
"rbox 1000 s D3 | qhull C-0.01 Q7 FO Tcv Qc" 
"rbox 1000 s D3 | qhull C-0.01 Qx Tcv Qc" 
"rbox 100 s D3 t | qhull R1e-3 Tcv Qc" 
"rbox 100 s D3 t | qhull R1e-2 Tcv Qc" 
"rbox 500 s D3 t | qhull R0.05 A-1 Tcv Qc" 
"rbox 100 W0 D3 t | qhull R1e-3 Tcv Qc" 
"rbox 100 W0 D3 t | qhull R1e-3 Qx Tcv Qc" 
"rbox 100 W0 D3 t | qhull R1e-2 Tcv Qc" 
"rbox 100 W0 D3 t | qhull R1e-2 Qx Tcv Qc" 
"rbox 500 W0 D3 t | qhull R0.05 A-1 Tcv Qc" 
"rbox 500 W0 D3 t | qhull R0.05 Qx Tcv Qc" 
"rbox 1000 W1e-20 t | qhull Tcv Qc" 
"rbox 1000 W1e-20 D4 t | qhull Tcv Qc" 
"rbox 500 W1e-20 D5 t | qhull Tv Qc" 
"rbox 100 W1e-20 D6 t | qhull Tv Qc" 
"rbox 50 W1e-20 D6 t | qhull Qv Tv Qc" 
"rbox 10000 D4 t | qhull QR0 Qc C-0.01 A0.3 Tv" 
"rbox 1000 D2 t | qhull d QR0 Qc C-1e-8 Qu Tv" 
"rbox 300 D5 t |qhull A-0.999 Qc Tcv" 
"rbox 100 D6 t |qhull A-0.9999 Qc Tcv" 
"rbox 50 D7 t |qhull A-0.99999 Qc Tcv W0.1" 
=== check bad cases for Qhull.  May cause errors ${d:-`date`} 
"RBOX 1000 L100000 s G1e-6 t | QHULL Tv" 
"RBOX 1000 L100000 s G1e-6 t | QHULL Tv Q10" 
"rbox 1000 s Z1 G1e-13 t | qhull Tv" 
"RBOX 1000 s W1e-13 P0 t | QHULL d Qbb Qc Tv" 
"RBOX 1000 s W1e-13 t | QHULL d Tv" 
"RBOX 1000 s W1e-13 t D2 | QHULL d Tv" 
======================================================= 
======================================================= 
=== The following commands may cause errors =========== 
======================================================= 
======================================================= 
"rbox c D7 | qhull Q0 Tcv" 
"rbox 100 s D3 | qhull Q0 E1e-3 Tc Po" 
"rbox 100 s D3 | qhull Q0 E1e-2 Tc Po" 
"rbox 100 s D3 | qhull Q0 E1e-1 Tc Po" 
"rbox 100 s D3 | qhull Q0 R1e-3 Tc Po" 
"rbox 100 s D3 | qhull Q0 R1e-2 Tc Po" 
"rbox 100 s D3 | qhull Q0 R0.05 Tc" 
"rbox 100 s D3 | qhull Q0 R0.05 Tc Po" 
"rbox 1000 W1e-7 | qhull Q0 Tc Po" 
"rbox 50 s | qhull Q0 V0.05 W0.01 Tc Po" 
"rbox 100 s D5 | qhull Q0 R1e-2 Tc Po" 
"qhull" 
"qhull ." 
"qhull -" 
"rbox" 
"# end of q_test" 
