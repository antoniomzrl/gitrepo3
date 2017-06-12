program integ
  real a(0:180), dxs(0:180)

  open(1, file='dxs.txt')
  read(1,*) ! skip 1st line comment

  read(1,*, iostat=ios) angMin, dxsmin
  do i=0, int(angMin)
     a(i)=i
     dxs(i)=dxsmin
  enddo
  
  do i=int(angmin+1), 180
     read(1,*, iostat=ios) a(i), dxs(int(a(i)))
     if(ios /= 0) exit
     print*, int(a(i)), dxs(i), ios
     if(a(i)> iangMin) iangMin = a(i)
  enddo

  print*, 'angMin: ', int(angMin)
  

  pi = 4.0*atan(1.0)
  dang = 1.0*pi/180.0
  su = 0
  do i=0, 180
     ang = a(i)*pi/180.0
     s = 2.0 * pi * dxs(i) *sin(ang) *dang
     su = su + s
     print*, i, dxs(i), s, su
  enddo

  !2*pi*INTEGRAL(sen(ang)*DCS*d(ang))
  print*, 'txs:', su
  
end program integ
