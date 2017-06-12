C  ****  Input file.
C
C  Each line in the input data file consists of a 6-character keyword
C  (columns 1-6) followed either by numerical data (in free format) or
C  by a character string, which start at the 8th column. The keywords
C  serve only as reminders for the user and are not used by the program.
C  The program ignores the comments in square brackets, as well as any
C  other text following the first blank after the last numerical datum,
C  or after the character string, in each line.
C
C  Example: Parallel electron beam impinging normally, from below, on an
C           Al foil.
C
C  ....+....1....+....2....+....3....+....4....+....5....+....6....+....
C  TITLE  Aluminium foil
C  KPAR   1        [Primary particles: 1=electron, 2=photon, 3=positron]
C  E0     511.0E3                                [Initial energy, in eV]
C  Sfile spectrumfile.txt
C  Z0     -10.0                      [Z-coordinate of the source, in cm]
C  POLAR  0.0 0.0                  [Polar angle interval limits, in deg]
C  SIMPAR 1.0E4 1.0E4 1.0E4 0.10 0.10 1.0E3 -1.0E3 [EABSs,C1,C2,Wcc,Wcr]
C  PFNAME al.mat                     [Material file name, 18 characters]
C  THICKN 0.02                                   [Slab thickness, in cm]
C  DSMAX  0.004                     [Maximum allowed step length, in cm]
C  NSIMSH 2147483647         [Desired number of showers, max=2147483647]
C  TIME   600                         [Allotted simulation time, in sec]
C  RSEED  12345 54321                    [Random number generator seeds]
C  ....+....1....+....2....+....3....+....4....+....5....+....6....+....
C
C  The input data are the following:
C
C  TITLE  : title of the job (up to 75 characters).
C  KPAR   : kind of primary particle (1=electrons, 2=photons or 3=posi-
C           trons).
C  E0     : initial energy of the primary particles (in eV).
C  Z0     : z-coordinate of the radiation source (Z0, in cm). Primary
C           particles start from the point (0,0,Z0).
C  POLAR  : limits of the interval in which the initial polar angle is
C           sampled, THETA1 and THETA2 (in deg). Initial directions are
C           distributed uniformly over the spherical sector defined by
C           these limits.
C  SIMPAR : absorption energies, EABS(1:3) in eV, elastic scattering
C           simulation parameters, C1 and C2, and cutoff energy losses
C           for inelastic collisions and bremsstrahlung emission, Wcc
C           and Wcr in eV. One line for each material.
C  PFNAME : name of PENELOPE's input material data file (18 characters).
C  THICKN : slab thickness, in cm.
C  DSMAX  : maximum allowed step length, in cm.
C  NSIMSH : number of showers to be simulated.
C  TIME   : alloted computation time (in sec).
C  RSEED  : seeds of the random number generator.
C

C     SPECTR Ei,Pi                 [E bin: lower-end and total probability]
C  SPECTR : For a source with continuous (stepwise constant) spectrum,
C           each 'SPECTR' line gives the lower end-point of an energy
C           bin of the source spectrum (Ei) and the associated relative
C           probability (Pi), integrated over the bin. Up to NSEM=1000
C           lines, in arbitrary order. The upper end of the spectrum is
C           defined by entering a line with Ei equal to the upper energy
C           end point and with a negative Pi value.
C     DEFAULT: none



      
C     >>>>  Notice that the number and order of lines cannot be altered.
C
C  The program is aborted when an incorrect input datum is found. The
C  incorrect quantity appears in the last line of the output file.
C
C  The clock subroutine (TIMER) must be adapted to each computer/com-
C  piler configuration.
C
C  *********************************************************************
C                       MAIN PROGRAM
C  *********************************************************************
      IMPLICIT DOUBLE PRECISION (A-H,O-Z), INTEGER*4 (I-N)
      CHARACTER*6 KWORD
      CHARACTER*132 BUFFER, buf
      CHARACTER*200 PFILE, PMFILE
      character*132 sfile, word(10)

C
      PARAMETER (PI=3.1415926535897932D0, TWOPI=2.0D0*PI,
     1  RA2DE=180.0D0/PI, DE2RA=PI/180.0D0)
C
C  ****  MAIN-PENELOPE commons.
C
      PARAMETER (MAXMAT=10)
      DIMENSION PMFILE(MAXMAT)

      COMMON/CSIMPA/EABS(3,MAXMAT),C1(MAXMAT),C2(MAXMAT),WCC(MAXMAT),
     1  WCR(MAXMAT)
      COMMON/TRACK/E,X,Y,Z,U,V,W,WGHT,KPAR,IBODY,MAT,ILB(5)
      COMMON/RSEED/ISEED1,ISEED2
C
C  ************  Counters.
C
C  ****  Discrete counters
      DIMENSION PRIM(3),SEC(3,3),  ! Numbers of particles.
     1          COL(8)             ! Numbers of interactions.
      DIMENSION
     1  AVTL(3),AVTL2(3),     ! Total path length.
     1  AVW(2),AVW2(2),       ! Final polar director cosine.
     1  AVA(2),AVA2(2),       ! Final polar angle.
     1  AVE(2),AVE2(2),       ! Final energy.
     1  AVNI(8,3),AVNI2(8,3), ! Number of interaction events.
     1  AVLD(2),AVLD2(2)      ! Final lateral displacement.
      DATA PRIM,SEC,AVTL,AVTL2,AVW,AVW2,AVA,AVA2,AVE,AVE2,
     1     AVNI,AVNI2,AVLD,AVLD2/82*0.0D0/
C  ****  Continuous distributions.
      PARAMETER (NCH=2000,NCH3=3*NCH,NCH9=9*NCH,NCH6=6*NCH,
     1           NCH18=18*NCH)
C  ----  Depth-dose distribution.
      DIMENSION DOSZ(NCH),DOSZ2(NCH),DOSZP(NCH),LDOSZ(NCH)
      DATA DOSZ,DOSZ2,DOSZP,LDOSZ/NCH3*0.0D0,NCH*0/
C  ----  Deposited charge distribution.
      DIMENSION CHRZ(NCH),CHRZ2(NCH),CHRZP(NCH),LCHRZ(NCH)
      DATA CHRZ,CHRZ2,CHRZP,LCHRZ/NCH3*0.0D0,NCH*0/
C  ----  Deposited energy distribution.
      DIMENSION EDEP(NCH),EDEP2(NCH),BSE(3)
      DATA EDEP,EDEP2/NCH*0.0D0,NCH*0.0D0/
C  ----  Path length distributions.
      DIMENSION PDTL(3,NCH),PDTL2(3,NCH)
      DATA PDTL,PDTL2/NCH3*0.0D0,NCH3*0.0D0/
C  ----  Angular distributions of emerging particles.
      DIMENSION PDA(3,NCH),PDA2(3,NCH),PDAP(3,NCH),LPDA(3,NCH)
      DATA PDA,PDA2,PDAP,LPDA/NCH9*0.0D0,NCH3*0/
C  ----  Energy distributions of emerging particles.
      DIMENSION PDE(3,2,NCH),PDE2(3,2,NCH),PDEP(3,2,NCH),LPDE(3,2,NCH)
      DATA PDE,PDE2,PDEP,LPDE/NCH18*0.0D0,NCH6*0/
C
      parameter(NsMax=2000)
      real ei(0:NsMax), fi(0:NsMax)
      EXTERNAL RAND
      logical lspec

      lspec = .false.
      Dmin=0.0                  !1.0D-35

      npt=0
      nptE=0
      nptI=0
      nptN=0
      nptE2=0
      nptI2=0
      nptN2=0


      TotDep=0
      Ndep=0
C
C  ****  Time counter initialization.
C
      CALL TIME0
C
C  ------------------------  Read input file.
C
      !OPEN(6,FILE='stdout.txt')
      WRITE(6,1000)
 1000 FORMAT(//3X,43('*')/3X,'**   Program PENSLAB. ',
     1  'Input data file.   **'/3X,43('*'))
C
C  ****  Title.
C
      READ(5,'(A6,1X,A65)') KWORD,BUFFER
      WRITE(6,1010) BUFFER
 1010 FORMAT(/3X,A65)
C
C  ****  Primary particles.
C
      READ(5,'(A6,1X,A65)') KWORD,BUFFER
      READ(BUFFER,*) KPARP
      IF(KPARP.LT.1.OR.KPARP.GT.3) KPARP=1
      IF(KPARP.EQ.1) WRITE(6,1021)
 1021 FORMAT(/3X,'Primary particles: electrons')
      IF(KPARP.EQ.2) WRITE(6,1022)
 1022 FORMAT(/3X,'Primary particles: photons')
      IF(KPARP.EQ.3) WRITE(6,1023)
 1023 FORMAT(/3X,'Primary particles: positrons')
C
C  ****  Initial energy of primary particles.
C
      READ(5,'(A6,1X,A65)') KWORD,BUFFER
      if(KWORD .eq. 'E0') then
         READ(BUFFER,*) E0
      else if(KWORD .eq. 'SFILE') then
         lspec = .true.

         do i=1, len_trim(buffer)
            if( buffer(i:i) == ' ') then
               ib=i
               exit
            endif
         enddo

         sfile=buffer(:ib)
         buf=buffer(ib:)
         read(buf,*), Efactor
         
         open(1, file=trim(sfile), status='OLD')
         Ns = 0
         do
            read(1, *, IOSTAT=ios) ei(Ns+1), fi(Ns+1)
            if(ios /= 0) exit   ! Final de fichero
            Ns = Ns +1
            if(Ns > NsMax) then
               print*, 'Error, cambiar parameter NsMax'
               stop
            endif
         enddo
         close(1)
         print*, 'Spectrum file = ', trim(sfile), Ns
         print*, 'Efactor:        ', Efactor
         
         do i=2, Ns !acumul
            fi(i) = fi(i-1) + fi(i)
         enddo
         do i=1, Ns             !normali
            ei(i) = ei(i)*Efactor
            fi(i) = fi(i)/fi(Ns)
            !print*, 'ieifi', i, ei(i), fi(i)
         enddo
         ei(0)=max(0.0, ei(1)-( ei(2)-ei(1)))
         fi(0)=0
         E0 = ei(Ns)
      endif

      print*, '(Max) Initial energy = ', E0, ' eV'

      WRITE(6,1030) E0
 1030 FORMAT(3X,'Initial energy = ',g20.5,' eV')
      EPMAX=E0
C  ----  Positrons eventually give annihilation gamma-rays. The maximum
C  energy of annihilation photons is .lt. 1.21*(E0+Me*c2).
       IF(KPARP.EQ.3) EPMAX=1.21D0*(E0+5.12D5)
       print *, 'EPMAX', epmax
C
C  ****  Source position.
C
      X0=0.0D0
      Y0=0.0D0
      READ(5,'(A6,1X,A65)') KWORD,BUFFER
      READ(BUFFER,*) Z0, x0, y0
      WRITE(6,1040) Z0, x0, y0
 1040 FORMAT(3X,'Particle source at Z0 = ',g20.5,' cm')
C
C  ****  Initial direction.
C
      READ(5,'(A6,1X,A65)') KWORD,BUFFER
      READ(BUFFER,*) ANG1,ANG2
      WRITE(6,1050) ANG1,ANG2
 1050 FORMAT(3X,'Initial polar angle in the interval = (',F8.3,',',
     1  F8.3,') deg')
      W0L=COS(ANG1*DE2RA)
      W0U=COS(ANG2*DE2RA)
      IF(ABS(W0L).GT.1.0D0.OR.ABS(W0U).GT.1.0D0) then
         print*, 'stop. w0L w0U = ', w0L w0U
         STOP
      endif
      W0UL=W0U-W0L
C
C  ****  Simulation parameters.
C
      NMAT=1
      DO M=1,NMAT
        READ(5,'(A6,1X,A65)') KWORD,BUFFER
        READ(BUFFER,*) EABS(1,M),EABS(2,M),EABS(3,M),C1(M),C2(M),
     1    WCC(M),WCR(M)
        WRITE(6,1060)
 1060   FORMAT(/3X,'**** Simulation mode:')
        IF(EABS(1,M).LT.1.0D2) EABS(1,M)=1.0D2
        IF(EABS(2,M).LT.1.0D2) EABS(2,M)=1.0D2
        IF(EABS(3,M).LT.1.0D2) EABS(3,M)=1.0D2
        WRITE(6,1061) EABS(1,M)
 1061 FORMAT(3X,'Electron absorption energy = ',g20.5,' eV')
        WRITE(6,1062) EABS(2,M)
 1062 FORMAT(3X,'  Photon absorption energy = ',g20.5,' eV')
        WRITE(6,1063) EABS(3,M)
 1063 FORMAT(3X,'Positron absorption energy = ',g20.5,' eV')
        WRITE(6,1064) C1(M),C2(M),WCC(M),WCR(M)
 1064 FORMAT(3X,'Electron-positron transport parameters:',
     1 /4X,'C1 =',g13.6,',      C2 =',g13.6,/3X,'Wcc =',g13.6,
     2  ' eV,  Wcr =',g13.6,' eV')
      ENDDO
C
C  ****  Initialization of PENELOPE.
C
      READ(5,'(A6,1X,A132)') KWORD,PFILE
      WRITE(6,1070) PFILE
 1070 FORMAT(/3X,'PENELOPE''s material definition file: ',A200)
C
! Lectura densidad del michero .mat
      open(81, file=trim(PFILE))
      read(81,*)
      read(81,*)
      read(81,*) word(1:3), density
      print*, 'Densidad (g/cm3):', density
      close(81)

      !OPEN(15,FILE=PFILE)
      OPEN(16,FILE='material.dat')
        INFO=4
        !CALL PEINIT(EPMAX,NMAT,15,16,INFO)
        !CALL PEINIT(EPMAX,NMAT,16,INFO,trim(PFILE) )
        pmfile(1) = trim(pfile)
        print *, 'PEINIT EPMAX ', EPMAX
        CALL PEINIT(EPMAX,NMAT,16,INFO, pmfile)
      !CLOSE(UNIT=15)
      !CLOSE(UNIT=16)
C
C  ****  Slab thickness.
C
      READ(5,'(A6,1X,A65)') KWORD,BUFFER
      READ(BUFFER,*) THICK
      WRITE(6,1080) THICK, thick*1e4
 1080 FORMAT(/3X,'Slab thickness = ',g20.5,' cm', g20.5, ' um')
      IF(THICK.LT.0.0D0) STOP
      IF(Z0.LT.0.0D0) Z0=0.0D0
      IF(Z0.GT.THICK) Z0=THICK
C
      READ(5,'(A6,1X,A65)') KWORD,BUFFER
      READ(BUFFER,*) DSMAX
      IF(DSMAX.LT.1.0D-8) DSMAX=1.0D25
      WRITE(6,1090) DSMAX
 1090 FORMAT(3X,'Maximum step length (DSMAX) =',g20.5,' cm')
C
C  ****  Desired number of showers and computation time available.
C
      READ(5,'(A6,1X,A65)') KWORD,BUFFER
      READ(BUFFER,*) NTOT
      WRITE(6,1100) NTOT
 1100 FORMAT(/3X,'Number of showers to be simulated = ',I11)
      IF(NTOT.LT.1) THEN
        WRITE(6,'(A5,A75)') KWORD,BUFFER
        WRITE(6,*) 'A positive integer value was expected.'
        STOP 'A positive integer value was expected.'
      ENDIF
C
      READ(5,'(A6,1X,A65)') KWORD,BUFFER
      READ(BUFFER,*) ITIME
      WRITE(6,1110) ITIME
 1110 FORMAT(3X,'Computation time available = ',I6,' sec')
      IF(ITIME.LT.1) STOP
C
      CALL TIMER(TSEC)
      TSECIN=TSEC
      TSECA=ITIME+TSEC
C
C  ****  Random initialization.
C
      READ(5,'(A6,1X,A65)') KWORD,BUFFER
      READ(BUFFER,*) ISEED1,ISEED2
      print*, 'random seeds:', ISEED1,ISEED2

      READ(5,'(A6,1X,A65)') KWORD,BUFFER
      READ(BUFFER,*) Iverbose
      print*, 'Verbose =', Iverbose

      if(Iverbose > 0) then
         open(77, file='xyze.txt')
         open(78, file='Eloss.d')
         open(79, file='iangle.d')
         open(80, file='eangle.d')
      endif

C
C  ************  Bin sizes.
C
      IF(KPARP.EQ.1) THEN
        BSE(1)=E0/DFLOAT(NCH)
        BSE(2)=E0/DFLOAT(NCH)
        BSE(3)=E0/DFLOAT(NCH)
        BSDE=E0/DBLE(NCH)
        TLMAX=2.5D0*PRANGE(E0,KPARP,1)
      ELSE IF(KPAR.EQ.2) THEN
        BSE(1)=E0/DFLOAT(NCH)
        BSE(2)=E0/DFLOAT(NCH)
        BSE(3)=E0/DFLOAT(NCH)
        BSDE=E0/DBLE(NCH)
        TLMAX=3.5D0*PRANGE(E0,KPARP,1)
      ELSE
        BSE(1)=EPMAX/DFLOAT(NCH)
        BSE(2)=EPMAX/DFLOAT(NCH)
        BSE(3)=E0/DFLOAT(NCH)
        BSDE=E0/DBLE(NCH)
        TLMAX=2.5D0*PRANGE(E0,KPARP,1)
      ENDIF
C
      BSTL=TLMAX/DFLOAT(NCH)
      BSA=180.0D0/DFLOAT(NCH)
      BSZ=THICK/DFLOAT(NCH)
C  ----  The factor 1.0000001 serves to ensure that the upper limit of
C  the tallied interval is within the last channel (otherwise, the array
C  dimensions could be exceeded).
      BSE(1)=1.0000001D0*BSE(1)
      BSE(2)=1.0000001D0*BSE(2)
      BSE(3)=1.0000001D0*BSE(3)
      BSDE=1.0000001D0*BSDE
      BSA=1.0000001D0*BSA
      BSZ=1.0000001D0*BSZ

      print*, 'E0 NCH BSE', E0, NCH, BSE(1:3)
C
C  ****  Initialize other variables.
C
      IBODY=1
      MAT=1
      WGHT0=1.0D0
      TDEP=0.0D0
      TDEP2=0.0D0
C
      N=0  ! Shower counter.
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C  ------------------------  Shower simulation starts here.
C
  101 N=N+1     ! New shower.
      ILB(1)=1  ! Identifies primary particles.
      ILB(2)=0
      ILB(3)=0
      ILB(4)=0
      ILB(5)=0
C
C  ************  Initial state of the primary particle.
C
      KPAR=KPARP
      WGHT=WGHT0
C  ****  Initial direction.
      W=W0L+W0UL*RAND(1.0D0)
      PHI=TWOPI*RAND(2.0D0)
      R=SQRT(1.0D0-W*W)
      U=R*COS(PHI)
      V=R*SIN(PHI)
C  ****  Initial position.
      X=X0
      Y=Y0
      ! AMR
      !print*, 'nnnn', N
c$$$      if(N .lt.51) then
c$$$         x=rand()/10 ! 1mm beam
c$$$         x = x/10          ! 0.1 mm
c$$$         !y=(rand()-0.5)/10
c$$$         !print*, 'xy ', x,' ',y
c$$$      endif

      IF(Z0.LT.0.0D0) THEN
        IF(W.GT.0.0D0) THEN
          Z=0.0D0
        ELSE
          GO TO 106
        ENDIF
      ELSE IF(Z0.LT.THICK) THEN
        Z=Z0
      ELSE
        IF(W.LT.0.0D0) THEN
          Z=THICK
        ELSE
          GO TO 106
        ENDIF
      ENDIF
C     ****  Initial energy.
      if(lspec) then
         call random_number(rn)
         ka=0 ; kb=Ns
         do
            kk = (ka+kb)/2;
            if(fi(kk) < rn) then
               ka=kk
            else
               kb=kk
            endif
            if( (kb-ka)<2) exit
         enddo
         dx = ei(kb)-ei(ka)
         dF = fi(kb)-fi(ka)
         s = (rn -fi(ka)) / dF
         E = ei(ka) + s*dx
         
         if(E < 100.0) E=100.0 ! call start no admite < 100 eV

         !print*, ka, kb, ei(ka), ei(kb), fi(ka), fi(kb), dx, dF, s
         !print*, 'rnE', rn, E
      else
         E = E0
      endif

      Eant = E
C
C  ************  Initialization of primary particle counters.
C
      DO I=1,8
        COL(I)=0.0D0  ! Numbers of collisions.
      ENDDO
      DEPP=E0*WGHT    ! Deposited energy within the slab.
      TL=0.0D0        ! Track length.
C  ---------------------------------------------------------------------
C  ------------------------  Track simulation begins here.
C
      CALL CLEANS          ! Cleans secondary stack.
C
  102 CONTINUE
      CALL START           ! Starts simulation in current medium.
C
  103 CONTINUE
      CALL JUMP(DSMAX,DS)  ! Determines segment length.
C  ****  New position.
      X=X+U*DS
      Y=Y+V*DS
      Z=Z+W*DS
C
      TL=TL+DS  ! Accumulated path length.
C  ****  Transmitted particle.
      IF(Z.GE.THICK) THEN
        IEXIT=1  ! Labels transmitted particles.
        DSCOR=(Z-THICK)/(DABS(W)+1.0D-30)
        Z=THICK
        X=X-U*DSCOR
        Y=Y-V*DSCOR
        TL=TL-DSCOR
        DEPP=DEPP-E*WGHT
        GO TO 104
      ENDIF
C  ****  Backscattered particle.
      IF(Z.LE.0.0D0) THEN
        IEXIT=2  ! Labels backscattered particles.
        DSCOR=DABS(Z)/(DABS(W)+1.0D-30)
        Z=0.0D0
        X=X-U*DSCOR
        Y=Y-V*DSCOR
        TL=TL-DSCOR
        DEPP=DEPP-E*WGHT
        GO TO 104
      ENDIF
C
      Eant=E

      CALL KNOCK(DE,ICOL)        ! Simulates the interaction event.
      COL(ICOL)=COL(ICOL)+1.0D0  ! The interaction is counted.
C     write(6,'(3i3,1p,5e13.5,i3)') n,kpar,icol,de,ds,z,w,e,ilb(1)

      Elost=Eant-E
      ang=dacos(w)*180/pi
      if( Elost > 1e-5) then
         TotDep=TotDep +Elost
         Ndep=Ndep+1
      endif

      !print*, z,y,x,E,Elost
      if(Iverbose > 0) then
         ity = 0;
         if(Elost > 0 ) ity = 1

         if(kpar == 1) then
            ipar = 2
         else if(kpar == 2) then
            ipar = 1
         else if(kpar == 3) then
            ipar = 3
         endif

         !write(77, *) ity, Elost
         write(77,*) z*10,y*10,x*10, E,Elost,Elost,0, ipar,ity,0,"pen"

         !if( Elost > 1e-5) then
            !write(77,'(5g16.5)') x*10, y*10, z*10, E, Elost
            !write(77,'(2g15.4,3(g15.4,i3))')z*10,y*10,x*10,0,E,0,Elost,0
            !write(78,*) Elost
            !write(79,*) ang, Eant-E
         !endif
      endif

C
C  ****  Depth-dose histogram. A good example of multiple counter.
      KZ=1.0D0+Z/BSZ           ! Channel number.
      IF(N.NE.LDOSZ(KZ)) THEN  ! First contribution of the shower.
        DOSZ(KZ)=DOSZ(KZ)+DOSZP(KZ)  ! Cleaning partial counters.
        DOSZ2(KZ)=DOSZ2(KZ)+DOSZP(KZ)**2
        DOSZP(KZ)=DE*WGHT
        LDOSZ(KZ)=N  ! Shower label.
      ELSE  ! The shower contributed previously.
        DOSZP(KZ)=DOSZP(KZ)+DE*WGHT
      ENDIF
C  ****  Test for absorption.
      IF(E.LT.EABS(KPAR,1)) THEN
        IEXIT=3  ! Labels absorbed particles.
        ICHR=KPAR-2
        IF(ICHR.NE.0) THEN
          IF(N.NE.LCHRZ(KZ)) THEN
            CHRZ(KZ)=CHRZ(KZ)+CHRZP(KZ)
            CHRZ2(KZ)=CHRZ2(KZ)+CHRZP(KZ)**2
            CHRZP(KZ)=ICHR*WGHT
            LCHRZ(KZ)=N
          ELSE
            CHRZP(KZ)=CHRZP(KZ)+ICHR*WGHT
          ENDIF
        ENDIF
        GO TO 104
      ENDIF
C
      GO TO 103
C  ------------------------  The simulation of the track ends here.
C  ---------------------------------------------------------------------
  104 CONTINUE
C
C  ************  Particle counters.
C
      ANGLE=DACOS(W)
      IF(ILB(1).EQ.1) THEN
        PRIM(IEXIT)=PRIM(IEXIT)+WGHT0
        AVTL(IEXIT)=AVTL(IEXIT)+TL*WGHT0
        AVTL2(IEXIT)=AVTL2(IEXIT)+(TL*WGHT0)**2
        IF(IEXIT.LT.3) THEN
          AVE(IEXIT)=AVE(IEXIT)+E*WGHT0
          AVE2(IEXIT)=AVE2(IEXIT)+(E*WGHT0)**2
          AVW(IEXIT)=AVW(IEXIT)+W*WGHT0
          AVW2(IEXIT)=AVW2(IEXIT)+(W*WGHT0)**2
          AVA(IEXIT)=AVA(IEXIT)+ANGLE*WGHT0
          AVA2(IEXIT)=AVA2(IEXIT)+(ANGLE*WGHT0)**2
          DLAT=DSQRT(X*X+Y*Y)
          AVLD(IEXIT)=AVLD(IEXIT)+DLAT*WGHT0
          AVLD2(IEXIT)=AVLD2(IEXIT)+(DLAT*WGHT0)**2

          !print*, '>>> ', e, w, angle
          if(iexit .eq. 1) then
             npt=npt+1          ! transmitido

             if( abs(E0-E) .gt. 1.0) then
                nptI=nptI+1     ! inelastic
             else
                if( angle .gt. 0.01) then
                   nptE=nptE+1  ! elastic
                else
                   nptN=nptN+1  ! Intacto
                endif
             endif

             if( abs(E0-E) .gt. 0.001) then
                nptI2=nptI2+1     ! inelastic
             else
                if( angle .gt. 0.000001) then
                   nptE2=nptE2+1  ! elastic
                else
                   nptN2=nptN2+1  ! Intacto
                endif
             endif

          endif
       ENDIF
        DO KC=1,8
          AVNI(KC,IEXIT)=AVNI(KC,IEXIT)+(COL(KC)*WGHT0)
          AVNI2(KC,IEXIT)=AVNI2(KC,IEXIT)+(COL(KC)*WGHT0)**2
        ENDDO
C  ****  Path length distribution.
        K=1.0D0+TL/BSTL
        IF(K.GT.0.AND.K.LE.NCH) THEN
          PDTL(IEXIT,K)=PDTL(IEXIT,K)+WGHT0
          PDTL2(IEXIT,K)=PDTL(IEXIT,K)+WGHT0**2
        ENDIF
      ELSE
        SEC(KPAR,IEXIT)=SEC(KPAR,IEXIT)+WGHT
      ENDIF
C
C  ************  Other counters.
C
      IF(IEXIT.LT.3) THEN
C  ****  Angular distribution of emerging particles.
        K=1.0D0+ANGLE*RA2DE/BSA
        IF(N.NE.LPDA(KPAR,K)) THEN
          PDA(KPAR,K)=PDA(KPAR,K)+PDAP(KPAR,K)
          PDA2(KPAR,K)=PDA2(KPAR,K)+PDAP(KPAR,K)**2
          PDAP(KPAR,K)=WGHT
          LPDA(KPAR,K)=N
        ELSE
          PDAP(KPAR,K)=PDAP(KPAR,K)+WGHT
        ENDIF
C  ****  Energy distribution of emerging particles.
        K=1.0D0+E/BSE(KPAR)
        IF(K.GT.0.AND.K.LE.NCH) THEN
          IF(N.NE.LPDE(KPAR,IEXIT,K)) THEN
            PDE(KPAR,IEXIT,K)=PDE(KPAR,IEXIT,K)+PDEP(KPAR,IEXIT,K)
            PDE2(KPAR,IEXIT,K)=PDE2(KPAR,IEXIT,K)+PDEP(KPAR,IEXIT,K)**2
            PDEP(KPAR,IEXIT,K)=WGHT
            LPDE(KPAR,IEXIT,K)=N
          ELSE
            PDEP(KPAR,IEXIT,K)=PDEP(KPAR,IEXIT,K)+WGHT
          ENDIF
        ENDIF
      ENDIF
C
C  ************  Any secondary left?
C
      CALL SECPAR(LEFT)
      IF(LEFT.GT.0) THEN
C  ****  Correct the dose counters to avoid double-counting.
        KZ=1.0D0+Z/BSZ
        IF(N.NE.LDOSZ(KZ)) THEN
          DOSZ(KZ)=DOSZ(KZ)+DOSZP(KZ)
          DOSZ2(KZ)=DOSZ2(KZ)+DOSZP(KZ)**2
          DOSZP(KZ)=-E*WGHT
          LDOSZ(KZ)=N
        ELSE
          DOSZP(KZ)=DOSZP(KZ)-E*WGHT
        ENDIF
        IF(N.NE.LCHRZ(KZ)) THEN
          CHRZ(KZ)=CHRZ(KZ)+CHRZP(KZ)
          CHRZ2(KZ)=CHRZ2(KZ)+CHRZP(KZ)**2
          CHRZP(KZ)=(2-KPAR)*WGHT
          LCHRZ(KZ)=N
        ELSE
          CHRZP(KZ)=CHRZP(KZ)+(2-KPAR)*WGHT
        ENDIF
        GO TO 102
      ENDIF
C
C  ------------------------  The simulation of the shower ends here.
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
  105 CONTINUE
      TDEP=TDEP+DEPP
      TDEP2=TDEP2+DEPP**2
      IF(DEPP.GT.1.0D0) THEN
        KE=1.0D0+DEPP/BSDE
        IF(KE.GT.0.AND.KE.LE.NCH) THEN
          EDEP(KE)=EDEP(KE)+WGHT0
          EDEP2(KE)=EDEP2(KE)+WGHT0**2
        ENDIF
      ENDIF
C
C  ----  End the simulation after the alloted time or after completing
C        NTOT showers.
C
  106 CONTINUE
      CALL TIMER(TSEC)
      IF(TSEC.LT.TSECA.AND.N.LT.NTOT) GO TO 101
C
C  ****  Transfer contents of partial counters to global counters.
C
      DO K=1,NCH
        DOSZ(K)=DOSZ(K)+DOSZP(K)
        DOSZ2(K)=DOSZ2(K)+DOSZP(K)**2
        CHRZ(K)=CHRZ(K)+CHRZP(K)
        CHRZ2(K)=CHRZ2(K)+CHRZP(K)**2
        DO KPAR=1,3
          PDA(KPAR,K)=PDA(KPAR,K)+PDAP(KPAR,K)
          PDA2(KPAR,K)=PDA2(KPAR,K)+PDAP(KPAR,K)**2
          PDE(KPAR,1,K)=PDE(KPAR,1,K)+PDEP(KPAR,1,K)
          PDE2(KPAR,1,K)=PDE2(KPAR,1,K)+PDEP(KPAR,1,K)**2
          PDE(KPAR,2,K)=PDE(KPAR,2,K)+PDEP(KPAR,2,K)
          PDE2(KPAR,2,K)=PDE2(KPAR,2,K)+PDEP(KPAR,2,K)**2
        ENDDO
      ENDDO
C
C  ------------------------  Print simulation results.
C
C     IEXIT: 1=transmitted, 2=backscattered, 3=absorbed.
C
      TOTN=N*WGHT0
      WRITE(6,3000)
 3000 FORMAT(///3X,35('*')/3X,'**   Program PENSLAB. Resu',
     1  'lts.   **'/3X,35('*'))
C
      TSIM=MAX(1.0D0,TSEC-TSECIN)
      WRITE(6,3001) TSIM
 3001 FORMAT(/3X,'Simulation time ..................... ',g20.5,' sec')
      TAVS=TOTN/TSIM
      WRITE(6,3002) TAVS
 3002 FORMAT(3X,'Simulation speed ............. ',g20.5,' showers/sec')
      WRITE(6,3003) TOTN
 3003 FORMAT(//3X,'Simulated primary particles ...........  ',g20.5,' ')
      WRITE(6,3004) PRIM(1)
 3004 FORMAT(/3X,'Transmitted primary particles ........... ',g20.5,' ')
      WRITE(6,3005) PRIM(2)
 3005 FORMAT(3X,'Backscattered primary particles ......... ',g20.5,' ')
      WRITE(6,3006) PRIM(3)
 3006 FORMAT(3X,'Absorbed primary particles .............. ' g20.5,' ')
      WRITE(6,3007) SEC(1,1),SEC(2,1),SEC(3,1),
     1              SEC(1,2),SEC(2,2),SEC(3,2),
     1              SEC(1,3),SEC(2,3),SEC(3,3)
 3007 FORMAT(/3X,'Simulated secondary particles:'/19X,46('-')
     1 /19X,'|  electrons   |   photons    |  positrons   |',1P,
     2 /3X,62('-')/3X,'|  transmitted  |',3(g15.5,1X,'|')
     3 /3X,62('-')/3X,'| backscattered |',3(g15.5,1X,'|')
     4 /3X,62('-')/3X,'|   absorbed    |',3(g15.5,1X,'|')
     5 /3X,62('-'))
C
      FT=(PRIM(1)+SEC(KPARP,1))/TOTN
      ERR=3.0D0*SQRT(ABS(FT*(1.0D0-FT)/TOTN))  ! assumes WGHT0=1.
      WRITE(6,3008) FT,ERR
 3008 FORMAT(/3X,'Fractional transmission ............ ',
     1  g15.5,' +-',g8.1)
      FB=(PRIM(2)+SEC(KPARP,2))/TOTN
      ERR=3.0D0*SQRT(ABS(FB*(1.0D0-FB)/TOTN))
      WRITE(6,3009) FB,ERR
 3009 FORMAT(3X,'Fractional backscattering .......... ',
     1  g15.5,' +-',g8.1)
      FA=PRIM(3)/TOTN
      ERR=3.0D0*SQRT(ABS(FA*(1.0D0-FA)/TOTN))
      WRITE(6,3010) FA,ERR
 3010 FORMAT(3X,'Fractional absorption .............. ',
     1  g15.5,' +-',g8.1)
C
C  ************  Average values for primary particles.
C
      DO I=1,3
        DF=1.0D0/MAX(PRIM(I),1.0D0)
        AVTL2(I)=3.0D0*DF*SQRT(ABS(AVTL2(I)-AVTL(I)**2*DF))
        AVTL(I)=AVTL(I)*DF
        DO KC=1,8
          IF(AVNI(KC,I).GT.0.5D0) THEN
            AVNI2(KC,I)=3.0D0*DF*SQRT(ABS(AVNI2(KC,I)-AVNI(KC,I)**2*DF))
            AVNI(KC,I)=AVNI(KC,I)*DF
          ELSE
            AVNI2(KC,I)=0.0D0
            AVNI(KC,I)=0.0D0
          ENDIF
        ENDDO
        IF(I.LT.3) THEN
          AVE2(I)=3.0D0*DF*SQRT(ABS(AVE2(I)-AVE(I)**2*DF))
          AVE(I)=AVE(I)*DF
          AVW2(I)=3.0D0*DF*SQRT(ABS(AVW2(I)-AVW(I)**2*DF))
          AVW(I)=AVW(I)*DF
          AVA2(I)=3.0D0*DF*RA2DE*SQRT(ABS(AVA2(I)-AVA(I)**2*DF))
          AVA(I)=AVA(I)*RA2DE*DF
          AVLD2(I)=3.0D0*DF*SQRT(ABS(AVLD2(I)-AVLD(I)**2*DF))
          AVLD(I)=AVLD(I)*DF
        ENDIF
      ENDDO
C
      WRITE(6,3011)
 3011 FORMAT(//3X,'Mean number of events per primary track:')
      WRITE(6,3012)
 3012 FORMAT(/6X,'** Transmitted particles:')
      IF(KPARP.EQ.2) THEN
      WRITE(6,3013) AVNI(1,1),AVNI2(1,1)
 3013 FORMAT(6X,'Coherent (Rayleigh) ............. ',g20.5,
     1  ' +-',g8.1)
      WRITE(6,3014) AVNI(2,1),AVNI2(2,1)
 3014 FORMAT(6X,'Incoherent (Compton) ............ ',g20.5,
     1  ' +-',g8.1)
      WRITE(6,3015) AVNI(3,1),AVNI2(3,1)
 3015 FORMAT(6X,'Photoelectric absorption ........ ',g20.5,
     1  ' +-',g8.1)
      WRITE(6,3016) AVNI(4,1),AVNI2(4,1)
 3016 FORMAT(6X,'Pair production ................. ',g20.5,
     1  ' +-',g8.1)
      ELSE
      WRITE(6,3017) AVNI(1,1),AVNI2(1,1)
 3017 FORMAT(6X,'Hinges (soft events) ............ ',g20.5,
     1  ' +-',g8.1)
      WRITE(6,3018) AVNI(2,1),AVNI2(2,1)
 3018 FORMAT(6X,'Hard elastic collisions ......... ',g20.5,
     1  ' +-',g8.1)
      WRITE(6,3019) AVNI(3,1),AVNI2(3,1)
 3019 FORMAT(6X,'Hard inelastic collisions ....... ',g20.5,
     1  ' +-',g8.1)
      WRITE(6,3020) AVNI(4,1),AVNI2(4,1)
 3020 FORMAT(6X,'Hard bremsstrahlung emissions ... ',g20.5,
     1  ' +-',g8.1)
      WRITE(6,3021) AVNI(5,1),AVNI2(5,1)
 3021 FORMAT(6X,'Inner-shell ionizations ......... ',g20.5,
     1  ' +-',g8.1)
      WRITE(6,3022) AVNI(7,1),AVNI2(7,1)
 3022 FORMAT(6X,'Delta interactions .............. ',g20.5,
     1  ' +-',g8.1)
      ENDIF
      WRITE(6,3023)
 3023 FORMAT(/6X,'** Backscattered particles:')
      IF(KPARP.EQ.2) THEN
        WRITE(6,3013) AVNI(1,2),AVNI2(1,2)
        WRITE(6,3014) AVNI(2,2),AVNI2(2,2)
        WRITE(6,3015) AVNI(3,2),AVNI2(3,2)
        WRITE(6,3016) AVNI(4,2),AVNI2(4,2)
      ELSE
        WRITE(6,3017) AVNI(1,2),AVNI2(1,2)
        WRITE(6,3018) AVNI(2,2),AVNI2(2,2)
        WRITE(6,3019) AVNI(3,2),AVNI2(3,2)
        WRITE(6,3020) AVNI(4,2),AVNI2(4,2)
        WRITE(6,3021) AVNI(5,2),AVNI2(5,2)
        WRITE(6,3022) AVNI(7,2),AVNI2(7,2)
      ENDIF
      WRITE(6,3024)
 3024 FORMAT(/6X,'** Absorbed particles:')
      IF(KPARP.EQ.2) THEN
        WRITE(6,3013) AVNI(1,3),AVNI2(1,3)
        WRITE(6,3014) AVNI(2,3),AVNI2(2,3)
        WRITE(6,3015) AVNI(3,3),AVNI2(3,3)
        WRITE(6,3016) AVNI(4,3),AVNI2(4,3)
      ELSE
        WRITE(6,3017) AVNI(1,3),AVNI2(1,3)
        WRITE(6,3018) AVNI(2,3),AVNI2(2,3)
        WRITE(6,3019) AVNI(3,3),AVNI2(3,3)
        WRITE(6,3020) AVNI(4,3),AVNI2(4,3)
        WRITE(6,3021) AVNI(5,3),AVNI2(5,3)
        WRITE(6,3022) AVNI(7,3),AVNI2(7,3)
      ENDIF
C
      WRITE(6,3025)
 3025 FORMAT(//3X,'Average final energy:')
      WRITE(6,3026) AVE(1),AVE2(1)
 3026 FORMAT(6X,'Transmitted particles ........... ',g20.5,
     1  ' +-',g8.1,' eV')
      WRITE(6,3027) AVE(2),AVE2(2)
 3027 FORMAT(6X,'Backscattered particles ......... ',g20.5,
     1  ' +-',g8.1,' eV')
C
C  ****  Deposited energy.
C
      DF=1.0D0/TOTN
      QER=3.0D0*DF*SQRT(ABS(TDEP2-TDEP**2*DF))
      QAV=TDEP*DF
      WRITE(6,3028) QAV,QER
 3028 FORMAT(/3X,'Average deposited energy ........... ',g20.5,
     1  ' +-',g8.1,' eV')
C
      WRITE(6,3029)
 3029 FORMAT(/3X,'Average track length:')
      WRITE(6,3030) AVTL(1),AVTL2(1)
 3030 FORMAT(6X,'Transmitted particles ........... ',g20.5,
     1  ' +-',g8.1,' cm')
      WRITE(6,3031) AVTL(2),AVTL2(2)
 3031 FORMAT(6X,'Backscattered particles ......... ',g20.5,
     1  ' +-',g8.1,' cm')
      WRITE(6,3032) AVTL(3),AVTL2(3)
 3032 FORMAT(6X,'Absorbed particles .............. ',g20.5,
     1  ' +-',g8.1,' cm')
C
      WRITE(6,3033)
 3033 FORMAT(/3X,'Average lateral displacement:')
      WRITE(6,3030) AVLD(1),AVLD2(1)
      WRITE(6,3031) AVLD(2),AVLD2(2)
C
      WRITE(6,3034)
 3034 FORMAT(/3X,'Mean value of the polar cosine of the exit',
     1  ' direction:')
      WRITE(6,3035) AVW(1),AVW2(1)
 3035 FORMAT(6X,'Transmitted particles ........... ',g20.5,
     1  ' +-',g8.1)
      WRITE(6,3036) AVW(2),AVW2(2)
 3036 FORMAT(6X,'Backscattered particles ......... ',g20.5,
     1  ' +-',g8.1)
C
      WRITE(6,3037)
 3037 FORMAT(/3X,'Mean value of the polar angle of the exit dir',
     1  'ection:')
      WRITE(6,3038) AVA(1),AVA2(1)
 3038 FORMAT(6X,'Transmitted particles ........... ',g20.5,
     1  ' +-',g8.1,' deg')
      WRITE(6,3039) AVA(2),AVA2(2)
 3039 FORMAT(6X,'Backscattered particles ......... ',g20.5,
     1  ' +-',g8.1,' deg')
C
C  ------------------------  Print tallied distributions.
C
C  ************  Depth-dose.
C
      OPEN(9,FILE='psddose.dat')
      WRITE(9,9010)
 9010 FORMAT(
     1  1X,'#  Results from PENSLAB.',
     1 /1X,'#  Depth dose distribution.',
     1 /1X,'#  1st column: Z (cm).',
     1 /1X,'#  2nd and 3rd columns: dose and STU (eV/(cm*particle)).',/)
      DF=1.0D0/TOTN
      DO K=1,NCH
        XX=(K-0.5D0)*BSZ
        YERR=3.0D0*SQRT(ABS(DOSZ2(K)-DOSZ(K)**2*DF))
        YAV=DOSZ(K)*DF/BSZ
        YERR=YERR*DF/BSZ
        WRITE(9,'(1X,1P,3E14.6)')
     1     XX,MAX(YAV,Dmin),MAX(YERR,Dmin)
      ENDDO
      CLOSE(9)
C
C  ************  Depth distribution of deposited charge.
C
      OPEN(9,FILE='psdchar.dat')
      WRITE(9,9020)
 9020 FORMAT(
     1  1X,'#  Results from PENSLAB.',
     1 /1X,'#  Depth distribution of deposited charge.',
     1 /1X,'#  1st column: Z (cm).',
     1 /1X,'#  2nd and 3rd columns:',
     1      ' charge density and STU (e/(cm*particle)).',/)
      DF=1.0D0/TOTN
      DO K=1,NCH
        XX=(K-0.5D0)*BSZ
        YERR=3.0D0*SQRT(ABS(CHRZ2(K)-CHRZ(K)**2*DF))
        YAV=CHRZ(K)*DF/BSZ
        YERR=YERR*DF/BSZ
        WRITE(9,'(1X,1P,3E14.6)') XX,YAV,YERR
      ENDDO
      CLOSE(9)
C
C  ************  Path length distributions of primary particles.
C
C  ****  Transmitted particles.
      OPEN(9,FILE='pstltr.dat')
      WRITE(9,9030)
 9030 FORMAT(
     1  1X,'#  Results from PENSLAB.',
     1 /1X,'#  Path length distribution of transmitted particles.',
     1 /1X,'#  1st column: Z (cm).',
     1 /1X,'#  2nd and 3rd columns: probability density and STU',
     1         ' (1/(cm*particle)).',/)
      DF=1.0D0/TOTN
      DO K=1,NCH
        XX=(K-0.5D0)*BSTL
        YERR=3.0D0*SQRT(ABS(PDTL2(1,K)-PDTL(1,K)**2*DF))
        YAV=PDTL(1,K)*DF/BSTL
        YERR=YERR*DF/BSTL
        WRITE(9,'(1X,1P,3E14.6)')
     1     XX,MAX(YAV,Dmin),MAX(YERR,Dmin)
      ENDDO
      CLOSE(9)
C  ****  Backscattered particles.
      OPEN(9,FILE='pstlbk.dat')
      WRITE(9,9040)
 9040 FORMAT(
     1  1X,'#  Results from PENSLAB.',
     1 /1X,'#  Path length distribution of backscattered particles.',
     1 /1X,'#  1st column: Z (cm).',
     1 /1X,'#  2nd and 3rd columns: probability density and STU',
     1         ' (1/(cm*particle)).',/)
      DF=1.0D0/TOTN
      DO K=1,NCH
        XX=(K-0.5D0)*BSTL
        YERR=3.0D0*SQRT(ABS(PDTL2(2,K)-PDTL(2,K)**2*DF))
        YAV=PDTL(2,K)*DF/BSTL
        YERR=YERR*DF/BSTL
        WRITE(9,'(1X,1P,3E14.6)')
     1     XX,MAX(YAV,Dmin),MAX(YERR,Dmin)
      ENDDO
      CLOSE(9)
C  ****  Absorbed particles.
      OPEN(9,FILE='pstlab.dat')
      WRITE(9,9050)
 9050 FORMAT(
     1  1X,'#  Results from PENSLAB.',
     1 /1X,'#  Path length distribution of absorbed particles.',
     1 /1X,'#  1st column: Z (cm).',
     1 /1X,'#  2nd and 3rd columns: probability density and STU',
     1         ' (1/(cm*particle)).',/)
      DF=1.0D0/TOTN
      DO K=1,NCH
        XX=(K-0.5D0)*BSTL
        YERR=3.0D0*SQRT(ABS(PDTL2(3,K)-PDTL(3,K)**2*DF))
        YAV=PDTL(3,K)*DF/BSTL
        YERR=YERR*DF/BSTL
        WRITE(9,'(1X,1P,3E14.6)')
     1     XX,MAX(YAV,Dmin),MAX(YERR,Dmin)
      ENDDO
      CLOSE(9)
C
C  ************  Angular distribution of emerging particles.
C
      OPEN(9,FILE='psanel.dat')
      WRITE(9,9060)
 9060 FORMAT(
     1  1X,'#  Results from PENSLAB.',
     1 /1X,'#  Angular distribution of emerging electrons.',
     1 /1X,'#  1st column: angle (deg).',
     1 /1X,'#  2nd and 3rd columns: probability density and STU',
     1         ' (1/deg)',
     1 /1X,'#  4th and 5th columns: probability density and STU',
     1         ' (1/sr)',/)
      DF=1.0D0/TOTN
      DO K=1,NCH
        XX=(K-0.5D0)*BSA
        YERR=3.0D0*SQRT(ABS(PDA2(1,K)-PDA(1,K)**2*DF))
C  ****  Probability per unit angle (1/deg).
        YAV=PDA(1,K)*DF/BSA
        YERR=YERR*DF/BSA
C  ****  Probability per unit solid angle (1/sr).
        XXR=(K-1.0D0)*BSA/RA2DE
        FACT=BSA/((COS(XXR)-COS(XXR+BSA/RA2DE))*TWOPI)
        YAV2=YAV*FACT
        YERR2=YERR*FACT
        WRITE(9,'(1X,1P,5E14.6)')
     1     XX,MAX(YAV,Dmin),MAX(YERR,Dmin),
     2     MAX(YAV2,Dmin),MAX(YERR2,Dmin)
      ENDDO
      CLOSE(9)
C
      OPEN(9,FILE='psanga.dat')
      WRITE(9,9070)
 9070 FORMAT(
     1  1X,'#  Results from PENSLAB.',
     1 /1X,'#  Angular distribution of emerging photons.',
     1 /1X,'#  1st column: angle (deg).',
     1 /1X,'#  2nd and 3rd columns: probability density and STU',
     1         ' (1/deg)',
     1 /1X,'#  4th and 5th columns: probability density and STU',
     1         ' (1/sr)',/)
      DF=1.0D0/TOTN
      DO K=1,NCH
        XX=(K-0.5D0)*BSA
        YERR=3.0D0*SQRT(ABS(PDA2(2,K)-PDA(2,K)**2*DF))
C  ****  Probability per unit angle (1/deg).
        YAV=PDA(2,K)*DF/BSA
        YERR=YERR*DF/BSA
C  ****  Probability per unit solid angle (1/sr).
        XXR=(K-1.0D0)*BSA/RA2DE
        FACT=BSA/((COS(XXR)-COS(XXR+BSA/RA2DE))*TWOPI)
        YAV2=YAV*FACT
        YERR2=YERR*FACT
        WRITE(9,'(1X,1P,5E14.6)')
     1     XX,MAX(YAV,Dmin),MAX(YERR,Dmin),
     2     MAX(YAV2,Dmin),MAX(YERR2,Dmin)
      ENDDO
      CLOSE(9)
C
      OPEN(9,FILE='psanpo.dat')
      WRITE(9,9080)
 9080 FORMAT(
     1  1X,'#  Results from PENSLAB.',
     1 /1X,'#  Angular distribution of emerging positrons.',
     1 /1X,'#  1st column: angle (deg).',
     1 /1X,'#  2nd and 3rd columns: probability density and STU',
     1         ' (1/deg)',
     1 /1X,'#  4th and 5th columns: probability density and STU',
     1         ' (1/sr)',/)
      DF=1.0D0/TOTN
      DO K=1,NCH
        XX=(K-0.5D0)*BSA
        YERR=3.0D0*SQRT(ABS(PDA2(3,K)-PDA(3,K)**2*DF))
C  ****  Probability per unit angle (1/deg).
        YAV=PDA(3,K)*DF/BSA
        YERR=YERR*DF/BSA
C  ****  Probability per unit solid angle (1/sr).
        XXR=(K-1.0D0)*BSA/RA2DE
        FACT=BSA/((COS(XXR)-COS(XXR+BSA/RA2DE))*TWOPI)
        YAV2=YAV*FACT
        YERR2=YERR*FACT
        WRITE(9,'(1X,1P,5E14.6)')
     1     XX,MAX(YAV,Dmin),MAX(YERR,Dmin),
     2     MAX(YAV2,Dmin),MAX(YERR2,Dmin)
      ENDDO
      CLOSE(9)
C
C  ************  Energy distributions of emerging particles.
C
C  ****  Transmitted electrons.
      OPEN(9,FILE='pseneltr.dat')
      WRITE(9,9090)
 9090 FORMAT(
     1  1X,'#  Results from PENSLAB.',
     1 /1X,'#  Energy distribution of transmitted electrons.',
     1 /1X,'#  1st column: E (eV).',
     1 /1X,'#  2nd and 3rd columns: probability density and STU',
     1         ' (1/(eV*particle)).',/)
      DF=1.0D0/TOTN
      DO K=1,NCH
        XX=(K-0.5D0)*BSE(1)
        YERR=3.0D0*SQRT(ABS(PDE2(1,1,K)-PDE(1,1,K)**2*DF))
        YAV=PDE(1,1,K)*DF/BSE(1)
        YERR=YERR*DF/BSE(1)
        WRITE(9,'(1X,1P,3E14.6)')
     1     XX,MAX(YAV,Dmin),MAX(YERR,Dmin)
      ENDDO
      CLOSE(9)
C  ****  Backscattered electrons.
      OPEN(9,FILE='psenelbk.dat')
      WRITE(9,9100)
 9100 FORMAT(
     1  1X,'#  Results from PENSLAB.',
     1 /1X,'#  Energy distribution of backscattered electrons.',
     1 /1X,'#  1st column: E (eV).',
     1 /1X,'#  2nd and 3rd columns: probability density and STU',
     1         ' (1/(eV*particle)).',/)
      DF=1.0D0/TOTN
      DO K=1,NCH
        XX=(K-0.5D0)*BSE(1)
        YERR=3.0D0*SQRT(ABS(PDE2(1,2,K)-PDE(1,2,K)**2*DF))
        YAV=PDE(1,2,K)*DF/BSE(1)
        YERR=YERR*DF/BSE(1)
        WRITE(9,'(1X,1P,3E14.6)')
     1     XX,MAX(YAV,Dmin),MAX(YERR,Dmin)
      ENDDO
      CLOSE(9)
C  ****  Transmitted photons.
      OPEN(9,FILE='psengatr.dat')
      WRITE(9,9110)
 9110 FORMAT(
     1  1X,'#  Results from PENSLAB.',
     1 /1X,'#  Energy distribution of transmitted photons.',
     1 /1X,'#  1st column: E (eV).',
     1 /1X,'#  2nd and 3rd columns: probability density and STU',
     1         ' (1/(eV*particle)).',/)
      DF=1.0D0/TOTN
      DO K=1,NCH
        XX=(K-0.5D0)*BSE(2)
        YERR=3.0D0*SQRT(ABS(PDE2(2,1,K)-PDE(2,1,K)**2*DF))
        YAV=PDE(2,1,K)*DF/BSE(2)
        YERR=YERR*DF/BSE(2)
        WRITE(9,'(1X,1P,3E14.6)')
     1     XX,MAX(YAV,Dmin),MAX(YERR,Dmin)
      ENDDO
      CLOSE(9)
C  ****  Backscattered photons.
      OPEN(9,FILE='psengabk.dat')
      WRITE(9,9120)
 9120 FORMAT(
     1  1X,'#  Results from PENSLAB.',
     1 /1X,'#  Energy distribution of backscattered photons.',
     1 /1X,'#  1st column: E (eV).',
     1 /1X,'#  2nd and 3rd columns: probability density and STU',
     1         ' (1/(eV*particle)).',/)
      DF=1.0D0/TOTN
      DO K=1,NCH
        XX=(K-0.5D0)*BSE(2)
        YERR=3.0D0*SQRT(ABS(PDE2(2,2,K)-PDE(2,2,K)**2*DF))
        YAV=PDE(2,2,K)*DF/BSE(2)
        YERR=YERR*DF/BSE(2)
        WRITE(9,'(1X,1P,3E14.6)')
     1     XX,MAX(YAV,Dmin),MAX(YERR,Dmin)
      ENDDO
      CLOSE(9)
C  ****  Transmitted positrons.
      OPEN(9,FILE='psenpotr.dat')
      WRITE(9,9130)
 9130 FORMAT(
     1  1X,'#  Results from PENSLAB.',
     1 /1X,'#  Energy distribution of transmitted positrons.',
     1 /1X,'#  1st column: E (eV).',
     1 /1X,'#  2nd and 3rd columns: probability density and STU',
     1         ' (1/(eV*particle)).',/)
      DF=1.0D0/TOTN
      DO K=1,NCH
        XX=(K-0.5D0)*BSE(3)
        YERR=3.0D0*SQRT(ABS(PDE2(3,1,K)-PDE(3,1,K)**2*DF))
        YAV=PDE(3,1,K)*DF/BSE(3)
        YERR=YERR*DF/BSE(3)
        WRITE(9,'(1X,1P,3E14.6)')
     1     XX,MAX(YAV,Dmin),MAX(YERR,Dmin)
      ENDDO
      CLOSE(9)
C  ****  Backscattered positrons.
      OPEN(9,FILE='psenpobk.dat')
      WRITE(9,9140)
 9140 FORMAT(
     1  1X,'#  Results from PENSLAB.',
     1 /1X,'#  Energy distribution of backscattered positrons.',
     1 /1X,'#  1st column: E (eV).',
     1 /1X,'#  2nd and 3rd columns: probability density and STU',
     1         ' (1/(eV*particle)).',/)
      DF=1.0D0/TOTN
      DO K=1,NCH
        XX=(K-0.5D0)*BSE(3)
        YERR=3.0D0*SQRT(ABS(PDE2(3,2,K)-PDE(3,2,K)**2*DF))
        YAV=PDE(3,2,K)*DF/BSE(3)
        YERR=YERR*DF/BSE(3)
        WRITE(9,'(1X,1P,3E14.6)')
     1     XX,MAX(YAV,Dmin),MAX(YERR,Dmin)
      ENDDO
      CLOSE(9)
C
C  ************  Distribution of deposited energy.
C
      OPEN(9,FILE='psedepm.dat')
      WRITE(9,9150)
 9150 FORMAT(
     1  1X,'#  Results from PENSLAB.',
     1 /1X,'#  Distribution of deposited energy.',
     1 /1X,'#  1st column: E (eV).',
     1 /1X,'#  2nd and 3rd columns: probability density and STU',
     1         ' (1/(eV*particle)).',/)
      IF(KPARP.EQ.3) WRITE(9,9151)
 9151 FORMAT(1X,'#  WARNING: For positrons, tallied only for E<E0.',/)
      DF=1.0D0/TOTN
      DO J=1,NCH
        XX=(J-0.5D0)*BSDE
        YERR=3.0D0*SQRT(ABS(EDEP2(J)-EDEP(J)**2*DF))
        YAV=EDEP(J)*DF/BSDE
        YERR=YERR*DF/BSDE
        WRITE(9,'(1X,1P,3E14.6)')
     1    XX,MAX(YAV,Dmin),MAX(YERR,Dmin)
      ENDDO
      CLOSE(9)
C
      !print*, '(2) random seeds:', ISEED1,ISEED2
      WRITE(6,3099)
 3099 FORMAT(//3X,'***  END  ***')
      write(*,'(a16,8i8)') 'Tot npt N E I = ',
     &     Ntot,npt,nptN,nptE,nptI, nptN-nptN2,nptE-nptE2,nptI-nptI2




      Edepo=TotDep/NTOT
      AvEdepo=TotDep/Ndep
      StoppingPower=(Edepo*1e-6)/THICK/density !MeV*cm2/g
      write(*,'(a12,g15.8,g12.5,g20.10)') 
     & 'EoEdDxSPAvEl', E0, StoppingPower, AvEdepo


      CLOSE(6)
      STOP
      END
