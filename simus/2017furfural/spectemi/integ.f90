! gfortran integ.f90 util.f90 -o integ
! gfortran deriv.f90 util.f90 -o deriv
program integral
  parameter(NN=100000)
  real t(NN), e(NN), f(NN), g(NN)
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
  g = f

  call integ(g,N)

  open(1, file='integ.csv')
  write(1,*) '# E, f, F'
  do i=1,N
     write(1,*) e(i)*1e6, ',', f(i)*1e-9, ',', g(i)*1e-9
  enddo
  close(1)
end program integral
