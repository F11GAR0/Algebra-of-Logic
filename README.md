>Example:

###### Expression from MatBuro.ru
```c
P&Q>(Q&!P>R&Q)
```

should return this:
```c
P Q R RES
0 0 0 1
0 0 1 1
0 1 0 1
0 1 1 1
1 0 0 1
1 0 1 1
1 1 0 1
1 1 1 1
```
____

###### XOR
```c
A+B
```

should return this:
```c
A B RES
0 0 0
0 1 1
1 0 1
1 1 0
```
____

###### NOT
```c
!A
```

should return this:
```c
A RES
0 1
1 0
```
____

###### Equivalent 
```c
!(A+B)=(A=B)
```

should return this:
```c
A B RES
0 0 1
0 1 1
1 0 1
1 1 1
```
____

>Screenshot:
![Image alt](https://github.com/F11GAR0/Algebra-of-Logic/blob/main/data/screen.png)
