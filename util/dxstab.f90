
! gfortran -o dxstab dxstab.f90

program dxstab
  real e(100), a(0:180)
  character*16 d1(0:180,100), d2(0:180,100)
  
  ie=0
  do
     ie=ie+1
     read(*,*,iostat=ios) e(ie)
     if(ios < 0) exit !eof
     print *, e(ie), 'eV'

     do j=0, 180
        read(*,*,iostat=ios) jj, d1(j,ie), d2(j,ie)
        if(jj /= j .or. ios /= 0) then
           print*, 'error:', jj, j, ios
           stop
        endif
     enddo
  enddo

  ie=ie-1
  print*, 'fin NE', ie

  open(1,file='dxselast.txt')
  open(2,file='dxsrotat.txt')

  write(1,*) '181', ie, 'KTC'
  write(2,*) '181', ie, 'KTC'
  write(1,'(99g16.5)') e(1:ie)
  write(2,'(99g16.5)') e(1:ie)

  do j=0,180
     write(1,*) j, d1(j,1:ie)
     write(2,*) j, d2(j,1:ie)
  enddo

  close(1)
  close(2)
  
end program dxstab

