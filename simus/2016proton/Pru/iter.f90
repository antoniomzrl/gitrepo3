program iter
  ! f95 -fdefault-real-8  iter.f90
  ! ./a.out < ../../xsd/water_liquid_DXS_proton.txt 
  ! 2*pi*INTEGRAL(sen(ang)*DCS*d(ang))
  
  real angu(0:180), dxs(8,0:180)
  real txs(8), su(8), rem(8), xsc(8), aL(8)
  txs(1:8) = (/ 74.5080349762 , 44.6332340527 , 26.78848 , 16.07851 , &
       8.579828 , 3.484484 , 0.9112259 , 0.06608287 /)


  read(*,*)
  read(*,*)
  read(*,*)

  pi = 4.0*atan(1.0)
  deg=pi/180
  su=0

  do i=1, 180
     read(*,*) angu(i), dxs(1:8,i)
     ang = angu(i)*deg
     dang = 1.0*deg
     su(1:8) = su(1:8) + dxs(1:8,i)*sin(ang)*dang
  enddo

  su = su * 2*pi
  rem = txs - su
  print*, 'txs', txs(1:8)
  print*, 'su ', su(1:8)
  print*, 'rem', rem(1:8)

  do i=1,8
     print*, txs(i), su(i), rem(i), rem(i)*180/pi/pi
  enddo

  x=1
  dx=0.1
  ar=0
  do
     if(x < dx*1.1) dx = dx*0.1
     if(x < 1e-4) exit
     x = x-dx

     do i=1,8
        xsc(i) = XSLog(deg, dxs(i,1), 2*deg, dxs(i,2), x*deg)
     enddo
     print '(f10.4, 8g15.7)', x , xsc(1:8)
  enddo
  
  do i=8,1,-1
     dx=0.00001
     print*,'column', i, 'xs=', txs(i), 'remain=', rem(i), 'dx=',dx
     x=1
     ar=0
     do
        if(x < dx*1.1) dx = dx*0.1
        xant = x
        xsant = xs
        x = x -dx
        xs = XSLog(deg, dxs(i,1), 2*deg, dxs(i,2), x*deg)
        ar = ar + 2*pi*xs*sin(x*deg)*dx*deg
        dif = rem(i) -ar
        !print '(f12.6, 8g20.10)', x, xs, ar, rem(i), txs(i), dif
        if(dif < 0) then
           print*, 'found column', i
           print '(a10,f12.6,g25.15)', 'angLxs',xant, xsant
           exit
        endif
        if(x < 1e-5) exit
     enddo
  enddo

  aL(1:8) = (/ 0.5 , 0.5 , 0.47966 , 0.28587 , 0.11695 , 0.04666 , 0.01394 , 0.0056 /)

  do j=0,99
     x = j/100.0
     do i=1,8
        if(x < aL(i)) then
           xx = aL(i)
        else
           xx = x
        endif
        xsc(i) = XSLog(deg, dxs(i,1), 2*deg, dxs(i,2), xx*deg)
     enddo
     print'(f12.6,9g20.10)', x, xsc(1:8)
  enddo
    
end program


function XSLog(a, fa, b, fb, x)
  v = (log(fa)-log(fb))*(log(a)-log(x))/(log(b)-log(a)) + log(fa)
  XSLog = exp(v)
end function XSLog
