! Sovereign pure Fortran kernels — zero external dependencies, fully deterministic
! All functions are pure, no I/O, no RNG, no system calls.

module sovereign_math
  use, intrinsic :: iso_c_binding
  implicit none

contains

  ! Always returns 42 — matches derand pure stub
  pure function sovereign_rand() result(r) bind(c, name="sovereign_rand")
    integer(c_int) :: r
    r = 42
  end function sovereign_rand

  ! Deterministic reduction: sum of first n naturals
  pure function sovereign_triangular(n) result(s) bind(c, name="sovereign_triangular")
    integer(c_int), value :: n
    integer(c_int) :: s
    s = n * (n + 1) / 2
  end function sovereign_triangular

  ! Pure identity kernel
  pure function sovereign_identity(x) result(y) bind(c, name="sovereign_identity")
    real(c_double), value :: x
    real(c_double) :: y
    y = x
  end function sovereign_identity

  ! Zero kernel (sovereign null operation)
  pure function sovereign_zero() result(z) bind(c, name="sovereign_zero")
    real(c_double) :: z
    z = 0.0_c_double
  end function sovereign_zero

  ! Deterministic square
  pure function sovereign_square(x) result(y) bind(c, name="sovereign_square")
    real(c_double), value :: x
    real(c_double) :: y
    y = x * x
  end function sovereign_square

  ! Fixed-point scale by golden ratio conjugate (1/phi ≈ 0.6180339887)
  pure function sovereign_phi_scale(x) result(y) bind(c, name="sovereign_phi_scale")
    real(c_double), value :: x
    real(c_double) :: y
    y = x * 0.6180339887498948_c_double
  end function sovereign_phi_scale

end module sovereign_math
