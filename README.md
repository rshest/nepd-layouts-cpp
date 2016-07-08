This is an exhaustive search solver for the problem:

* On a square board NxN find how to place N pieces such that no two pairwise distances between pieces are equal
* Mirrored/rotated board placements are considered to be the same
* Find what is the maximum board side, so that it's possible to find such a placement

![](img/good_bad.png)

An example program output:

~~~
Side: 3, Layouts: 5.  Time used: 0ms
|OO.|OO.|OO.|OO.|O..|
|..O|...|...|...|.OO|
|...|O..|.O.|..O|...|
Side: 4, Layouts: 23.  Time used: 0ms
|OO..|OO..|OO..|OO..|OO..|OO..|OO..|OO..|OO..|OO..|OO..|OO..|O.O.|O.O.|O.O.|O..O|O..O|O..O|O...|O...|.OO.|.OO.|.OO.|
|..O.|..O.|...O|....|....|....|....|....|....|....|....|....|..O.|...O|...O|.O..|....|....|.O.O|...O|O...|....|....|
|....|....|....|O..O|O...|O...|.O..|.O..|..O.|..O.|...O|....|....|...O|....|....|.O..|.O..|...O|..O.|....|O...|.O..|
|O...|..O.|...O|....|.O..|...O|O...|..O.|.O..|...O|.O..|.O.O|O...|....|..O.|.O..|.O..|..O.|....|..O.|..O.|.O..|..O.|
Side: 5, Layouts: 35.  Time used: 1ms
|OO..O|OO..O|OO...|OO...|OO...|OO...|OO...|OO...|OO...|OO...|OO...|OO...|OO...|OO...|OO...|OO...|OO...|O.OO.|O.O..|O.O..|O.O..|O.O..|O.O..|O.O..|O.O..|O.O..|O..O.|O..O.|O..O.|O...O|O....|O....|.OO..|.OO..|.O...|
|.....|.....|..O.O|..O..|..O..|..O..|..O..|...O.|....O|....O|.....|.....|.....|.....|.....|.....|.....|.....|..O..|...OO|...O.|.....|.....|.....|.....|.....|.O...|.....|.....|.O...|.O..O|.O..O|O....|.....|O....|
|.O...|...O.|.....|.....|.....|.....|.....|.....|...O.|.....|O..O.|O....|O....|.O...|.....|.....|.....|.....|.....|.....|.....|....O|....O|.....|.....|.....|.....|.....|.....|.O...|.....|.....|.....|.O...|...OO|
|.....|.....|.....|O....|.....|.....|.....|.....|.....|...O.|....O|...O.|...O.|O....|..O..|..O..|...O.|....O|.....|.....|....O|..OO.|...O.|O....|..OO.|...OO|.....|.O...|.O...|.....|...OO|...O.|..O.O|....O|.....|
|...O.|...O.|....O|....O|O..O.|.O..O|..O.O|.O.O.|....O|....O|.....|..O..|....O|....O|O..O.|.O..O|.O..O|..O..|O..O.|O....|O....|.....|...O.|..OO.|....O|..O..|..OO.|.O.O.|..OO.|.O...|.....|...O.|.....|...O.|...O.|
Side: 6, Layouts: 2.  Time used: 9ms
|OO....|O.O..O|
|...O..|......|
|......|......|
|.....O|...O..|
|......|....O.|
|..O..O|....O.|
Side: 7, Layouts: 1.  Time used: 63ms
|O.O....|
|..O....|
|......O|
|O......|
|.......|
|.....O.|
|......O|
Side: 8, Layouts: None.  Time used: 581ms
Side: 9, Layouts: None.  Time used: 4213ms
Side: 10, Layouts: None.  Time used: 35949ms
Side: 11, Layouts: None.  Time used: 426086ms
~~~

7x7 is the maximum board that has such a placement, and there is a single possible layout.

Starting from 16x16 it can be proven analytically that there is no such a board.