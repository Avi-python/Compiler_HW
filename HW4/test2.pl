PROGRAM test2;
CONST
  a=" x=",
  b=" y=";
VAR
  x, y;
BEGIN
  x := 3;
  WHILE x!=0 DO
    BEGIN
      y := x^3+6;
      WRITE(a,x);
      WRITE(b,y);
      a:= x-1;
    END;
END.