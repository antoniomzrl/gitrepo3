! gfortran integ.f90 util.f90 -o integ
! gfortran deriv.f90 util.f90 -o deriv

program deriv
  parameter(NN=5000)
  real t(NN), e(NN), f(NN)
  i=1
  do
     read(*,*,IOSTAT=ios) e(i), f(i)
     if(ios /= 0) exit
     i=i+1
  enddo

  N=i-1
  print*, '# Nd =', N
  call swap(e,N)
  call swap(f,N)

  !el original volteado
  open(1, file='1volt.txt')
  do i=1,N
     write(1,*) e(i), f(i)
  enddo
  close(1)

  fm=maxval(f)
  f=fm-f

  !reflejado
  open(1, file='2refl.txt')
  do i=1,N
     write(1,*) e(i), f(i)
  enddo
  close(1)

  mv=50
  call mm(e,N,mv)
  call mm(f,N,mv)
  N=N-mv-1

  !suavizado
  open(1, file='3suaviz.txt')
  do i=1,N
     write(1,*) e(i), f(i)
  enddo
  close(1)

  call dif(f,N)
  N=N-1

  !derivado
  open(1, file='4deriv.txt')
  do i=1,N
     write(1,*) e(i), f(i)
  enddo
  close(1)

  call mm(e,N,mv)
  call mm(f,N,mv)
  N=N-mv-1

  !resuavizado
  open(1, file='5dersuav.txt')
  do i=1,N
     write(1,*) e(i), f(i)
  enddo
  close(1)

  call rmneg(f,N)

  !sin negativos
  open(1, file='6dersuavpos.txt')
  do i=1,N
     write(1,*) e(i), f(i)
  enddo
  close(1)

  
  call half(e,N)
  call half(f,N)
  N=N/2
  call half(e,N)
  call half(f,N)
  N=N/2
  call half(e,N)
  call half(f,N)
  N=N/2

  !final
  open(1, file='7diezmado.txt')
  do i=1,N
     write(1,*) e(i), f(i)
  enddo
  close(1)
end program deriv
