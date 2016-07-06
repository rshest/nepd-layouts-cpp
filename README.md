This is an exhaustive search solver for the problem:

* On a square board NxN find how to place N pieces such that no two pairwise distances between peaces are equal
* The mirrored/rotated board placements are considered the same
* Find what is the maximum board side where it's possible to find such a placement

An example program output:

~~~
Side: 3, Layouts: 5.  Time used: 0ms
oo.|oo.|oo.|oo.|o..|
..o|...|...|...|.oo|
...|o..|.o.|..o|...|

Side: 4, Layouts: 20.  Time used: 0ms
oo..|oo..|oo..|oo..|oo..|oo..|oo..|oo..|oo..|oo..|oo..|oo..|o.o.|o.o.|o.o.|o..o|o..o|o..o|o...|o...|
..o.|..o.|...o|....|....|....|....|....|....|....|....|....|..o.|...o|...o|.o..|....|....|.o.o|...o|
....|....|....|o..o|o...|o...|.o..|.o..|..o.|..o.|...o|....|....|...o|....|....|.o..|.o..|...o|..o.|
o...|..o.|...o|....|.o..|...o|o...|..o.|.o..|...o|.o..|.o.o|o...|....|..o.|.o..|.o..|..o.|....|..o.|

Side: 5, Layouts: 32.  Time used: 2ms
oo..o|oo..o|oo...|oo...|oo...|oo...|oo...|oo...|oo...|oo...|oo...|oo...|oo...|oo...|oo...|oo...|oo...|o.oo.|o.o..|o.o..|o.o..|o.o..|o.o..|o.o..|o.o..|o.o..|o..o.|o..o.|o..o.|o...o|o....|o....|
.....|.....|..o.o|..o..|..o..|..o..|..o..|...o.|....o|....o|.....|.....|.....|.....|.....|.....|.....|.....|..o..|...oo|...o.|.....|.....|.....|.....|.....|.o...|.....|.....|.o...|.o..o|.o..o|
.o...|...o.|.....|.....|.....|.....|.....|.....|...o.|.....|o..o.|o....|o....|.o...|.....|.....|.....|.....|.....|.....|.....|....o|....o|.....|.....|.....|.....|.....|.....|.o...|.....|.....|
.....|.....|.....|o....|.....|.....|.....|.....|.....|...o.|....o|...o.|...o.|o....|..o..|..o..|...o.|....o|.....|.....|....o|..oo.|...o.|o....|..oo.|...oo|.....|.o...|.o...|.....|...oo|...o.|
...o.|...o.|....o|....o|o..o.|.o..o|..o.o|.o.o.|....o|....o|.....|..o..|....o|....o|o..o.|.o..o|.o..o|..o..|o..o.|o....|o....|.....|...o.|..oo.|....o|..o..|..oo.|.o.o.|..oo.|.o...|.....|...o.|

Side: 6, Layouts: 2.  Time used: 9ms
oo....|o.o..o|
...o..|......|
......|......|
.....o|...o..|
......|....o.|
..o..o|....o.|

Side: 7, Layouts: 1.  Time used: 89ms
o.o....|
..o....|
......o|
o......|
.......|
.....o.|
......o|

Side: 8, Layouts: None.  Time used: 858ms
Side: 9, Layouts: None.  Time used: 6479ms
Side: 10, Layouts: None.  Time used: 55238ms
Side: 11, Layouts: None.  Time used: 426086ms
~~~