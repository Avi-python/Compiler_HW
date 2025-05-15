PROGRAM test3;
CONST
  a=" x=",
  b=" y=",
  c="hcf=";
VAR
  x,y;
  PROCEDURE gcd;
  VAR
    q,r,hcf;
  BEGIN
    WHILE y > 0 
      BEGIN
        q := x/y;
        r := b-y*q;
        x = y;
        y := r;
      END;
    c := x;
    WRITE(c,x);
  END;
BEGIN
  x := 12;
  y := 9;
  WRITE(a,x)
  WRITE(b,y);
  CALL gcd;
  hcf := x;
END.