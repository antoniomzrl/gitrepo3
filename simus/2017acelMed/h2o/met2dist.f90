!fichero MET mean energy tranfer -> distrib Eloss

program met2dist
  parameter(Nm=500,M=6000000)
  real x(Nm), y(Nm), xx(M), yy(M), ff(M)

  i=1
  do
     read(*,*,IOSTAT=ios) x(i), y(i)
     if(ios /= 0) exit
     i=i+1
  enddo

  Nd=i-1
  print*, '# N =', Nd


  dx=2
  i=1

  do
     xx(i) = x(1) + (i-1)*dx
     if(xx(i) > x(Nd)) exit
     yy(i)=0
     do j=1,Nd
        if(xx(i) >= x(j) .and. xx(i) <= x(j+1)) then
           a  = x(j)
           b  = x(j+1)
           fa = y(j)
           fb = y(j+1)
           yy(i) = (fa*(b-xx(i)) + fb*(xx(i)-a))/(b-a)
        endif
     enddo
     i=i+1
  enddo
  Nd=i
  
  !interpolado
!!$  open(1, file='1metitp.txt')
!!$  do i=1,Nd
!!$     write(1,*) xx(i), yy(i)
!!$  enddo
!!$  close(1)

  
  ff(1) = yy(1)
  Fi = 0
  Gi = 0
  
  do i=2, Nd
     Fi = Fi + ff(i-1)
     Gi = Gi + ff(i-1)*xx(i-1)
     ff(i) = (yy(i)*Fi - Gi) / (xx(i)-yy(i))
     print*, i, ff(i-1), xx(i-1), Fi, Gi, xx(i), yy(i), ff(i)
  enddo
  
  
end program met2dist
