

subroutine swap(x,N)
  real x(N)

  do i=1,N/2
     t=x(i)
     x(i)=x(N-i+1)
     x(N-i+1)=t
  enddo
end subroutine swap


subroutine mm(x,N,m)
  real x(N)
  real, allocatable :: t(:)
  allocate( t(N) )

  do i=1,N-m+1
     t(i)=sum(x(i:i+m-1))/m
  enddo
  x=t
end subroutine mm


subroutine dif(x,N)
  real x(N)

  do i=1,N-1
     x(i)=x(i+1)-x(i)
  enddo
end subroutine dif


subroutine integ(x,N)
  real x(N)

  do i=2,N
     x(i)=x(i)+x(i-1)
  enddo
  !x = x/x(N)
end subroutine integ


subroutine rmneg(x,N)
  real x(N)

  do i=1,N
     if(x(i) < 0 ) x(i)=1e-10
  enddo
end subroutine rmneg


subroutine half(x,N)
  real x(N)

  do i=1,N/2
     x(i)=(x(2*i)+x(2*i-1))*0.5
  enddo
end subroutine half
