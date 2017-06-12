program itpel
  
  character*80 li
  real, allocatable :: x(:), y(:)

  dx = 0.1 ! step eV

  read(*,*) Ntabs
  write(*,*) Ntabs

  do k=1, Ntabs
     read(*,*)  li
     write(*,*) trim(li)
     read(*,*)  li
     write(*,*) trim(li)
     read(*,*)  li
     write(*,*) trim(li)
     read(*,*) N

     if(k == 1) allocate( x(2*N), y(2*N) )

     do i=1, N
        read(*,*) x(i), y(i)
     enddo

     xmin = x(1)
     xmax = x(N)
     NN = (xmax -xmin)/dx +1

     write(*,*) NN

     do i=1,NN
        xx = xmin + dx*(i-1)

        do j=1,N-1
           if(xx >= x(j) .and. xx <= x(j+1)) then
              a  = x(j)
              b  = x(j+1)
              fa = y(j)
              fb = y(j+1)
           !!print*, 'xxxxxxxxxx', xx, j, a, b
           endif
        enddo

        if( xx < 100) then  ! linear
           yy = (fa*(b-xx) + fb*(xx-a))/(b-a);
        else                ! log-log
           yy = exp( (log(fa)*log(b/xx)+log(fb)*log(xx/a))/log(b/a) )
        endif

        write(*,*) xx, yy
        write(k+10,*) xx, yy
     enddo

  enddo

end program itpel
