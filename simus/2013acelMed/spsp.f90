program spsp
  character*800 linea
  real e(0:1000), f(0:1000), x(0:1000), y(0:1000)

  ! Read
  N = 0
  do
     read(*, '(a)', IOSTAT=ios) linea
     if(ios /= 0) exit   ! Fuera del bulcle, fin de fichero

     read(linea, *, IOSTAT=ios) a, b
     if(ios /= 0) then
        !print *, 'error: ', trim(linea)
        cycle  ! Nueva iteraci�n
     else
        N = N+1
        e(N) = a
        f(N) = b
     endif
  enddo

  ! Sort
  do j=1,N
     do i=1,N
        if(e(i) < e(i-1) ) then
           tmp=e(i)
           e(i)=e(i-1)
           e(i-1)=tmp
           tmp=f(i)
           f(i)=f(i-1)
           f(i-1)=tmp
           !print *, j, i
        endif
     enddo
  enddo

  sy=0
  do i=1, 1000
     x(i) = e(1)+ (i-1)* 0.02
     if(x(i)> e(N) ) exit

     ip = Near(x(i), e, N)
     ia = max(ip-5, 1)
     ib = min(ip+5, N)

     w = 0
     y(i) = 0
     do j = ia, ib
        di=abs(x(i)-e(j))
        if(di<0.00001) then
           wi=1e10
        else
           wi = 1.0/di
        endif
        w = w + wi
        y(i) = y(i) + f(j)*wi
     enddo
     y(i) = y(i)/w

     !y(i) = f(ip)

     sy = sy + y(i)
     !print *, 'sy', i, x(i), y(i), sy
  enddo


  do i=1, 1000
     if(x(i)> e(N) ) exit
     y(i) = y(i)/sy
     print*, x(i), y(i)
  enddo

  !print *, 'sy = ', sy

end program spsp



integer function Near(x, e, N)
  real e(*)

  i = 1
  j = N
  do
     k = (i+j)/2
     if( e(k)<x ) then
        i = k
     else
        j = k
     endif
     !print*, 'ikj', i, k, j

     if( (j-i)<2 ) exit
  enddo

  Near = k
end function Near
