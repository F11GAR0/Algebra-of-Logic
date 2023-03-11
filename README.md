>Example:

######Expression from MatBuro.ru
```c
P&Q>(Q&!P>R&Q)
```

should return this:
```c
P Q R  RES
0 0 0 1
0 0 1 1
0 1 0 1
0 1 1 1
1 0 0 1
1 0 1 1
1 1 0 1
1 1 1 1
```

######XOR
```c
A+B
```

should return this:
```c
A B  RES
0 0 0
0 1 1
1 0 1
1 1 0
```

######NOT
```c
!A
```

should return this:
```c
A  RES
0 1
1 0
```

######Equivalent 
```c
!(A+B)=(A=B)
```

should return this:
```c
A B  RES
0 0 1
0 1 1
1 0 1
1 1 1
```
