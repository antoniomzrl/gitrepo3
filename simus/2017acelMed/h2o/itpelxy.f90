program itpel
  
  character*80 li
  real, allocatable :: x(:), y(:)

  allocate( x(1000), y(1000) )

  N = 1
  do
     read(*, '(a)', IOSTAT=ios) li
     if(ios /= 0) exit   ! Fuera del bulcle, fin de fichero
     if(li(1:1) == '#') cycle !com
     
     read(li, *, IOSTAT=ios) x(N), y(N)
     if(ios /= 0) cycle  ! Nueva iterac
     N = N+1
  enddo

  N = N-1


  print*, x(1), y(1), y(1)
  do i=2, N
     xm = (x(i) + x(i-1))/2.0
     a  = x(i-1)
     b  = x(i)
     fa = y(i-1)
     fb = y(i)
     ym = (fa*(b-xm) + fb*(xm-a))/(b-a); !linear
     zm = exp( (log(fa)*log(b/xm)+log(fb)*log(xm/a))/log(b/a) ) !log
     print*, xm, ym, zm
     print*, x(i), y(i), y(i)
  enddo

end program itpel
