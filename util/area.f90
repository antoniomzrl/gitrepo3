program area
  
  character*80 li
  real, allocatable :: x(:), y(:)
  NN = 100000
  allocate( x(NN), y(NN) )

  i=0
  a1=0
  a2=0
  do
     read(*, '(a)', IOSTAT=ios) li
     if(ios /= 0) exit   ! Fuera del bulcle, fin de fichero
     if(li(1:1) == '#') cycle !com
     i = i+1
     read(li, *, IOSTAT=ios) x(i), y(i)

     if(i>1) then
        a1 = a1 + (x(i)-x(i-1))*y(i-1)
        a2 = a2 + (x(i)-x(i-1))*y(i)
        !print*, i, x(i), y(i), a1, a2, abs(a1-a2)/a1
     endif
  enddo

  print*, 'Ndat:', i-1
  print*, 'area:', a1, a2, abs(a1-a2)/a1
end program area
