
!./a.out < tpsp.txt

program dist
  real tp(8,3), sp(65,3)
  do i=1,8
     read(*,*) tp(i,1:3)
  enddo
  read(*,*)
  do i=1,65
     read(*,*) sp(i,1:3)
  enddo

!!$  do i=1,8
!!$     print*, tp(i,1:3)
!!$  enddo
!!$  print*
!!$  do i=1,65
!!$     print*, sp(i,1:3)
!!$  enddo

  do i=1,8
     dm = 1e8
     jm = 0
     do j=1,65
        dj = (tp(i,1)-sp(j,1))**2 + (tp(i,2)-sp(j,2))**2 + (tp(i,3)-sp(j,3))**2
        if(dj < dm) then
           dm = dj
           jm = j
        endif
     enddo
     print*, i, '(', tp(i,1:3), ')', sqrt(dm), jm, '(', sp(jm,1:3), ')'
  enddo

end program dist
