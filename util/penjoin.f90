program penjoin
  parameter(Nmax=10000)
  character(len=800) fn, line
  real x(Nmax), y(Nmax)
  
  Narg=IArgC()
  if( Narg == 0) then       ! con namelist
     print*, 'Error, faltan ficheros'
     stop
  endif

  y = 0

  do j=1, Narg
     call GetArg(j, fn)
     print *, j, trim(fn)
     open(1, file=trim(fn), status='OLD')
     N = 0
     do
        read(1, '(a)', IOSTAT=ios) line
        if(ios /= 0) exit   ! Final de fichero

        read(line, *, IOSTAT=ios) a, b, c
        if(ios /= 0) cycle  ! Datos no vlidos
        N = N+1
        x(N) = a
        y(N) = y(N) + b
     enddo
     print*, 'Nolines:', N
     close(1)
  enddo

  open(2, file='penjoin.csv')
  do i=1,N
     write(2,*) x(i), ',', y(i)
  enddo
  close(2)

end program penjoin
