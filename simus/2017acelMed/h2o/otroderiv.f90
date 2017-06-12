

program deriv
  parameter(NN=5000)
  real e(NN), f(NN), der(NN)
  i=1
  do
     read(*,*,IOSTAT=ios) e(i), f(i)
     if(ios /= 0) exit
     i=i+1
  enddo

  N=i-1
  print*, '# Nd =', N
 
  do i=1, N-1
     der(i) = (f(i+1)-f(i)) / (e(i+1)-e(i))
  enddo
  der(N) = 0
  
  !derivado
  open(1, file='deriv.txt')
  do i=1,N
     write(1,*) e(i), der(i)
  enddo
  close(1)

  !derivado interp
  open(1, file='derivitp.txt')
  do i=1,N-1
     dx = e(i+1)-e(i)
     dy = der(i+1)-der(i)
     do j=0,99
        ee = e(i) + j*dx/100.0
        dd = der(i) + dy/dx * (ee-e(i))
        write(1,*) ee, dd
     enddo
  enddo
  close(1)

end program deriv
