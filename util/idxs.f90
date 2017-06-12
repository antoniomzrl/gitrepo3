
program idxs
  real, allocatable :: a(:), e(:), x(:,:), y(:)
  character*1500 li
  
  read(*,*) Na, Ne
  allocate( e(Ne), a(Na), x(Na,Ne), y(Ne) )

  read(*,*) tmp, e(1:Ne)

  do i=1, Na
     read(*,*) a(i), x(i,1:Ne)
  enddo

  open(1, file='idxs.tmp')
  
  !write(*,*) Na, Ne, 'idxs'
  write(1,'(a1,*(g0.5:","))') ',', e(1:Ne)

  nNa = 0
  do i=1,Na
     write(1,'(*(g0.5:","))') a(i), x(i,1:Ne)
     nNa=nNa+1
     if(a(i) < 11) then
        da = (a(i+1)-a(i))/10.0
        do j=1, 9
           aa=a(i)+j*da
           y(1:Ne)= x(i,1:Ne) + (x(i+1,1:Ne)-x(i,1:Ne)) &
                / (a(i+1)-a(i))*(aa-a(i))
           write(1,'(*(g0.5:","))') aa, y(1:Ne)
           nNa=nNa+1
        enddo
     endif
  enddo
  close(1)
  
  open(1, file='idxs.tmp')
  write(*,*) nNa, ',', Ne, ', idxs'
  do
     read(1, '(a)', IOSTAT=ios) li
     if(ios /= 0) exit
     write(*,*) trim(li)
  enddo
end program idxs
