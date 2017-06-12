
program medvar
  real x(22), s(22), s2(22), v(22)
  character*800 linea
  x = 0
  N = 0
  s = 0
  s2 = 0
  
  do
     read(*, '(a)', IOSTAT=ios) linea
     if(ios /= 0) exit   ! Fuera del bulcle, fin de fichero
     if(linea(1:1) == '#') cycle !com
     
     read(linea, *, IOSTAT=ios) x(1:15)
     !!if(ios /= 0) cycle  ! Nueva iterac
     
     N = N+1
     s = s+x
     s2 = s2+x*x
  enddo

  s = s/N
  s2 = s2/N
  v = sqrt(s2-s*s)
!!$  print*, '  N:', N
!!$  print*, 'Med:   ', s
!!$  print*, 'Des:   ', v
!!$  print*, 'RelErr:', v/s
!!$  print*, s*3600*100

  print*, 'column   Med   Desv   RelErr'
  do i=1,15
     print*, i, s(i), v(i), v(i)/s(i)
  enddo
  
end program medvar
