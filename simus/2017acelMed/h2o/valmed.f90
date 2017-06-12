

program valmed
  parameter(NN=5000)
  real e(NN), f(NN)
  i=1
  do
     read(*,*,IOSTAT=ios) e(i), f(i)
     if(ios /= 0) exit
     i=i+1
  enddo

  N=i-1
  print*, '# Nd =', N
  
  s = 0
  sx = 0
  do i=1, N-1
     h = e(i+1) -e(i)
     s = s + f(i)*h
     sx = sx + f(i)*e(i)*h
     vm = sx/s
     write(*,*) e(i), f(i), vm
  enddo

end program valmed
