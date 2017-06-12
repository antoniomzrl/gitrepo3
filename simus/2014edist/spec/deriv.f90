program deriv
  parameter(NN=5000)
  real t(NN), e(NN), f(NN), g(NN)
  i=1
  do
     read(*,*,IOSTAT=ios) e(i), f(i), g(i)
     if(ios /= 0) exit
     i=i+1
  enddo

  N=i-1
  print*, '# Nd =', N
  call swap(e,N)
  call swap(f,N)
  call swap(g,N)

  !el original volteado
  open(1, file='1origvolt.txt')
  do i=1,N
     write(1,*) e(i), f(i), g(i)
  enddo
  close(1)

  fm=maxval(f)
  gm=maxval(g)
  f=fm-f
  g=gm-g

  !reflejado
  open(1, file='2refl.txt')
  do i=1,N
     write(1,*) e(i), f(i), g(i)
  enddo
  close(1)

  mv=50

  call mm(e,N,mv)
  call mm(f,N,mv)
  call mm(g,N,mv)
  N=N-mv-1

  !suavizado
  open(1, file='3suaviz.txt')
  do i=1,N
     write(1,*) e(i), f(i), g(i)
  enddo
  close(1)

  call dif(f,N)
  call dif(g,N)
  N=N-1

  !derivado
  open(1, file='4deriv.txt')
  do i=1,N
     write(1,*) e(i), f(i), g(i)
  enddo
  close(1)

  call mm(e,N,mv)
  call mm(f,N,mv)
  call mm(g,N,mv)
  N=N-mv-1

  !resuavizado
  open(1, file='5dersuav.txt')
  do i=1,N
     write(1,*) e(i), f(i), g(i)
  enddo
  close(1)

  call rmneg(f,N)
  call rmneg(g,N)

  !sin negativos
  open(1, file='6dersuavpos.txt')
  do i=1,N
     write(1,*) e(i), f(i), g(i)
  enddo
  close(1)

  
  call half(e,N)
  call half(f,N)
  call half(g,N)
  N=N/2
  call half(e,N)
  call half(f,N)
  call half(g,N)
  N=N/2
  call half(e,N)
  call half(f,N)
  call half(g,N)
  N=N/2

  !final
  open(1, file='spectremis.txt')
  open(2, file='spectr1p5mTorr.txt')
  do i=1,N
     write(1,*) e(i), f(i)
     write(2,*) e(i), g(i)
  enddo
  close(1)
  close(2)
end program deriv


subroutine swap(x,N)
  real x(N)

  do i=1,N/2
     t=x(i)
     x(i)=x(N-i+1)
     x(N-i+1)=t
  enddo
end subroutine swap


subroutine mm(x,N,m)
  real x(N)
  real, allocatable :: t(:)
  allocate( t(N) )

  do i=1,N-m+1
     t(i)=sum(x(i:i+m-1))/m
  enddo
  x=t
end subroutine mm


subroutine dif(x,N)
  real x(N)

  do i=1,N-1
     x(i)=x(i+1)-x(i)
  enddo
end subroutine dif


subroutine rmneg(x,N)
  real x(N)

  do i=1,N
     if(x(i) < 0 ) x(i)=1e-10
  enddo
end subroutine rmneg


subroutine half(x,N)
  real x(N)

  do i=1,N/2
     x(i)=(x(2*i)+x(2*i-1))*0.5
  enddo
end subroutine half
