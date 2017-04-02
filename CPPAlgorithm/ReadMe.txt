
Rubik's cube unfolded - cube representation in the application:

Internal cube faces coding and their mapping to my real cube colors:

U - upper face  -  (W) white
F - front face  -  (R) red
D - lower face  -  (Y) yellow
L - left face   -  (G) green
R - right face  -  (B) blue
B - back face   -  (P) purple

A single cubie's (face) is ID-ied with a letter and a number, where letter
is a face designation (F-front, U-upper, D-down/bottom, B-back, L-left and
R-right) and a number indicates exact position of the cubie's face on the 
face of the cube. 
All cubies with number 4 are stationary (middle of the cube).

Below is the state representing a solved cube:

                        +---+---+---+
                        |U0 |U1 |U2 |
                        +---+---+---+
                        |U3 |U4 |U5 |
                        +---+---+---+
                        |U6 |U7 |U8 |
            +---+---+---+---+---+---+---+---+---+---+---+---+
            |L0 |L1 |L2 |F0 |F1 |F2 |R0 |R1 |R2 |B0 |B1 |B2 |
            +---+---+---+---+---+---+---+---+---+---+---+---+
            |L3 |L4 |L5 |F3 |F4 |F5 |R3 |R4 |R5 |B3 |B4 |B5 |
            +---+---+---+---+---+---+---+---+---+---+---+---+
            |L6 |L7 |L8 |F6 |F7 |F8 |R6 |R7 |R8 |B6 |B7 |B8 |
            +---+---+---+---+---+---+---+---+---+---+---+---+
                        |D0 |D1 |D2 |
                        +---+---+---+
                        |D3 |D4 |D5 |
                        +---+---+---+
                        |D6 |D7 |D8 |
                        +---+---+---+

This is a solved cube coded with color letters instead of cubie ID-s and with
extra connection lines showing how the 3-d cube was unfolded:
                        
                               -----------------------
                              |                       |
                        +---+---+---+                 |
               ---------| W | W | W |---------        |
              |         +---+---+---+         |       |
              |    -----| W | W | W |-----    |       |
              |   |     +---+---+---+     |   |---    |
              |   |     | W | W | W |     |   |   |   |
            +---+---+---+---+---+---+---+---+---+---+---+---+
        *-->| G | G | G | R | R | R | B | B | B | P | P | P |-->*
            +---+---+---+---+---+---+---+---+---+---+---+---+
       **-->| G | G | G | R | R | R | B | B | B | P | P | P |-->**
            +---+---+---+---+---+---+---+---+---+---+---+---+
      ***-->| G | G | G | R | R | R | B | B | B | P | P | P |-->***
            +---+---+---+---+---+---+---+---+---+---+---+---+
              |   |     | Y | Y | Y |     |   |   |   |
              |   |     +---+---+---+     |   |---    |
              |    -----| Y | Y | Y |-----    |       |
              |         +---+---+---+         |       |
               ---------| Y | Y | Y |---------        |
                        +---+---+---+                 |
                              |                       |
                               -----------------------          

Regardles of the current state of the cube, the center (middle) cubies remain
stationary and these faces are still referenced to by their middle cubies even
though the edge and other cubies may at the moment come from different planes.

Example of a scrambled cube:

          R6 L1 R8
          F1 U4 B3
          U8 L7 L8

D2 U7 F2  R0 D3 D0  F6 R5 D8  B6 U3 F8
D5 L4 D1  F7 F4 D7  B7 R4 U5  R1 B4 R7
D6 F3 L2  U6 R3 U0  L0 B1 R2  U2 B5 L6

          F0 F5 B2
          L5 D4 U1
          B8 L3 B0

A input notation of above cube state (the notation accepted by program in
command line arguments) is presented below:

R6 L1 R8 F1 U4 B3 U8 L7 L8 D2 U7 F2 D5 L4 D1 D6 F3 L2 R0 D3 D0 F7 F4 D7 U6 R3 
U0 F6 R5 D8 B7 R4 U5 L0 B1 R2 B6 U3 F8 R1 B4 R7 U2 B5 L6 F0 F5 B2 L5 D4 U1 B8
L3 B0

Note that a sequence of scrambled cube faces separated with spaces are in the 
following order:

   U, L, F, R, B, D

So, the upper face is entered line by line, then left face, line by line,
then front face etc.

Solutions produced by program assume that the cube is being held with the
front face (F) toward user, upper (U) face being on the top, bottom (D) face
on the bottom and back (B) face being in the back (opposite to F).

The next level of abstraction is a plane, which contains all cubies from a
single face and adjacent neighboring faces, forming a plane or a layer of
cubies that move together when the plane is rotated.

E.g.:
   
Front plane consists of faces:
   F0..F8, U6..U8, R0, R3, R6, D0..D2, L2, L5, L8.
   Note that faces L2, F0 and U6 represent a single corner cubie, just as
   faces U8, F2 and R0 etc., while U7-F1, U3-L1, U5-R1 and U1-B1 pairs 
   represent edge cubies of the upper face and adjacent left, right, bottom
   and front faces.
Back plane (one on the opposite side) consists of faces:
   B0..B8, R2, R5, R8, L0, L3, L6, U0..U2, D6..D8.
Upper plane consists of faces:
   L0..L2, F0..F2, U0..U8, R0..R2, B0..B2.
Bottom plane consists of faces:
   L6..L8, F6..F8, D0..D8, R6..R8, B6..B8.
Left plane consists of faces:
   F0, F3, F6, U0, U3, U6, D0, D3, D6, L0..L8, B2, B5, B8.
Right plane consists of faces:
   F2,5,8, U2,5,8, D2,5,8, R0..R8, B0,3,6.

                        
Solved upper layer cross (## - not important):


                        +---+---+---+
                        |## |U1 |## |
                        +---+---+---+
                        |U3 |U4 |U5 |
                        +---+---+---+
                        |## |U7 |## |
            +---+---+---+---+---+---+---+---+---+---+---+---+
            |## |L1 |## |## |F1 |## |## |R1 |## |## |B1 |## |
            +---+---+---+---+---+---+---+---+---+---+---+---+
            |## |L4 |## |## |F4 |## |## |R4 |## |## |B4 |## |
            +---+---+---+---+---+---+---+---+---+---+---+---+
            |## |## |## |## |## |## |## |## |## |## |## |## |
            +---+---+---+---+---+---+---+---+---+---+---+---+
                        |## |## |## |
                        +---+---+---+
                        |## |D4 |## |
                        +---+---+---+
                        |## |## |## |
                        +---+---+---+ 
 
Cube move notation:

The solutions generated by program as well as command line input and
input to some internal methods follows the nomenclature described below.

Letters L, R, U, D, F, B represent corresponding planes being rotated
90 degrees clockwise.
If above letters are followed by 'i', the planes are rotated 90 degrees
counter-clockwise.

Example sequence of moves:

LLDFBiLLFiBDLL

translates to following cube moves:

- Rotate left plane 180 degrees.
- Rotate bottom plane 90 degrees clockwise.
- Rotate front plane 90 degrees clockwise.
- Rotate back plane 90 degrees counter-clockwise.
- Rotate left plane 180 degrees.
- Rotate front plane 90 degrees counter-clockwise.
- Rotate back plane 90 degrees clockwise.
- Rotate bottom plane 90 degrees clockwise.
- Rotate left plane 180 degrees.

Examples of equivalent moves:

LL = LiLi
BB = BiBi
BBB = Bi

Examples of self-cancelling moves (leading to the same cube state as 
before they were performed):

DDDD
LLi

Cubie model:

For cube solving, it is more officient to represent cube internally
as cubie model. This representation consists of total of 40 states.
20 states for corners and edges permutation and 20 states for corresponding
orientation of the cubies.

Edges are numbered 1-12 and corners 1-8.
When referring to the facelet presentation described earlier, the corners
to facelets conversions are:
Corner #  - Facelet symbol (3-corner faces)
1 - U0 (L0 U0 B2)
2 - F6 (L8 F6 D0)
3 - R8 (R8 B6 D8)
4 - U8 (U8 F2 R0)
5 - U6 (L2 U6 F0)
6 - L6 (L6 B8 D6)
7 - F8 (F8 R6 D2)
8 - U2 (U2 R2 B0)

and the edges to facelets conversions are:
Edge # - Facelet symbol (2-egde faces)
1 - U3 (U3 L1)
2 - L7 (L7 D3)
3 - U5 (U5 R1)
4 - R7 (R7 D5)
5 - L3 (L3 B5)
6 - L5 (L5 F3)
7 - F5 (F5 R3)
8 - R5 (R5 B3)
9 - F1 (F1 U7)
10 - F7 (F7 D1)
11 - B7 (B7 D7)
12 - U1 (U1 B1)

Which can be represented in memory as (solved cube):
ep = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
eo = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
cp = {1, 2, 3, 4, 5, 6, 7, 8};
co = {0, 0, 0, 0, 0, 0, 0, 0};

Edges can have good (1) or bad (0) orientation.
Corners can have good (0), clockwise twist (1) or counter clock wise twist (2) orientation.

------------------------------------------------------------------------------
Release notes:

5/4/2015

Version 3.0.0
* Thistlethwaite 45 algorithm.
* Cubie model.

3/6/2015

Version 2.6
* Internal test case now can be invoked from command line arguments, not only
  from interactive mode.
* Bug fixes (minor bug in test case).

Version 2.5
* Bug fixes, memory leaks eradicated.
* Internal test case introduced in interactive mode.
* Initial cube state can now be color coded (option -e) also instead of 
  providing unique face codes in command line arguments (option -i).

Version 2.4
* External test case script.
------------------------------------------------------------------------------
