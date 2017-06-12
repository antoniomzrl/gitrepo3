
! gfortran -o mfactor mfactor.f90

program mfactor
  parameter(N=10) ! N# columns
  
  real x(N)
  character(LEN=800) line
  
  read(*, '(a)') line
  write(*, '(a)') trim(line)
  
  do 
     read(*,*) x(1:N)
     !print *, x(1:N)
     
     !x(2) = x(2) * x(N) !elast
     !x(3) = x(3) * x(N) !ion
     !x(4) = x(4) * x(N) !electr excit
     !x(5) = x(5) * x(N) !disNeu
     x(2:N-1) = x(2:N-1)* x(N)
     print *, x(1:N)
  enddo
end program mfactor
