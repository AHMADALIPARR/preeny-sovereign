! Sovereign pure vector kernels — deterministic, no allocation, no external calls
! Intended for verification and bare-metal numeric substrates.

module sovereign_vector
  use, intrinsic :: iso_c_binding
  implicit none

contains

  ! Dot product of two length-n double arrays (caller owns memory)
  pure function sovereign_dot(n, a, b) result(s) bind(c, name="sovereign_dot")
    integer(c_int), value :: n
    real(c_double), intent(in) :: a(n), b(n)
    real(c_double) :: s
    integer :: i
    s = 0.0_c_double
    do i = 1, n
      s = s + a(i) * b(i)
    end do
  end function sovereign_dot

  ! Sum reduction
  pure function sovereign_sum(n, a) result(s) bind(c, name="sovereign_sum")
    integer(c_int), value :: n
    real(c_double), intent(in) :: a(n)
    real(c_double) :: s
    integer :: i
    s = 0.0_c_double
    do i = 1, n
      s = s + a(i)
    end do
  end function sovereign_sum

  ! Fill array with constant (in-place)
  pure subroutine sovereign_fill(n, a, val) bind(c, name="sovereign_fill")
    integer(c_int), value :: n
    real(c_double), intent(out) :: a(n)
    real(c_double), value :: val
    integer :: i
    do i = 1, n
      a(i) = val
    end do
  end subroutine sovereign_fill

  ! Scale array in-place
  pure subroutine sovereign_scale(n, a, s) bind(c, name="sovereign_scale")
    integer(c_int), value :: n
    real(c_double), intent(inout) :: a(n)
    real(c_double), value :: s
    integer :: i
    do i = 1, n
      a(i) = a(i) * s
    end do
  end subroutine sovereign_scale

end module sovereign_vector
