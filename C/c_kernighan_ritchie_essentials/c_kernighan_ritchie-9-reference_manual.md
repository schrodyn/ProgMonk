#Keywords

```c
auto          double      int          struct
break         else        long         switch
case          enum        register     typedef
char          extern      return       union
const         float       short        unsigned
continue      for         signed       void
default       goto        sizeof       volatile
do            if          static       while
```

Some implementations also reserve the words `fortran` and `asm`.

#Arithmetic Conversions

This pattern is called the *usual arithmetic conversions*.

  * First, if either operand is `long double`, the other is converted 
  to `long double`.
  * Otherwise, if either operand is `double`, the other is converted to  double.
  * Otherwise, if either operand is `float`, the other is converted to 
  `float`.
  * Otherwise, the integral promotions are performed on both operands; 
  then, if either operand is `unsigned long int`, the other is 
  converted to `unsigned long int`.
  * Otherwise, if one operand is `long int` and the other is 
  `unsigned int`, the effect depends on whether a `long int` can 
  represent all values of an `unsigned int`; if so, the 
  `unsigned int` operand is converted to `long int`; if not, both are 
  converted to `unsigned long int`.
  * Otherwise, if one operand is `long int`, the other is converted to 
  `long int`.
  * Otherwise, if either operand is `unsigned int`, the other is 
  converted to `unsigned int`.
  * Otherwise, both operands have type `int`.
