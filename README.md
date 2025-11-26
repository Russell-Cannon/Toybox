# Toybox: Little Quilt for GLSL

## Why?

I find that a lot of rudimentary shaders never need iteration or even if statements. This project, with help from the Little Quilt language in class, was made to test how far I could get without either of them.

## The Basics

Each 'Toybox' script (`.tb`) compiles into GLSL code inside a HTML page that is spit out by the compiler. \
Every value (called a toy) in a Toybox script is a 3 dimensional Vector. \
There are literals for UV coordinates, time in seconds since startup, and any number. \
There are built-in operations for most of the existing operations in GLSL.

## Quick Start

1. Create a `first.tb` file.
2. Enter `UV`
3. Run `make run`
4. Enter `first.tb`
5. out.html will have been created. Open it in a browser.

## Documentation
### Layout
Toybox scripts allow any number of 'assignments' before the actual expression. \
#### Assignments
Assignments are always `<id> = <filepath>` or `<id> = <expression>`. \
If an expression is assigned to an ID. that ID can be used anywhere and it will be replaced with that expression. \
If a filepath is assigned to an ID, that ID is now a texture function which takes a UV and returns what the image found at that filepath looks like.
#### Expressions
After all assignments, the first stand-alone expression becomes the result of the script. Any expressions afterwards are ignored. \
Expressions are described in BNF notation as \
`<expression> ::= <function>(<expression>) | <literal>` \
See the following list of functions and literals for what those can be.


### Functions
#### Unary
1. Fract \
   Returns the fractional component of a number. Effectively mods the number by 1.0.
2. SawTooth \
   Describes a sawtooth pattern over x. When a value goes past 1.0, it returns the 1.0 minus the input. When used on the uv for a texture, it appears to mirror the texture past its border.
3. Sin
4. Cos
5. Tan
6. Normalize
7. Length
8. Texture
9. Ceiling
10. Floor
11. Round
12. Negate
13. Absolute
14. Screen \
    Takes a UV parameter and returns the distance from the uv to any edge of the screen.
15. Random \
    Takes a seed value and returns a pseudo-random number.
16. Noise \
    Takes a UV parameter and returns a portion of simplex noise that maps to that UV.

#### Binary
1. Modulus
2. Dot
3. Cross
4. Distance \
   Returns the distance between two points.
5. Direction \
   Returns the direction from the first point to the second.
6. Step \
   Takes edge and X and returns 1 if X > edge or 0 if x < edge
7. Polygon \
   Takes UV and N, where N is the number of sides
8. Circle \
   Takes UV and position

#### Trinary
1. Combine \
   Takes X, Y, and Z
2. Clamp \
   Takes X, minimum, and maximum
3. Mix \
   Takes X, Y, and T, where T is similarity from X (0.0) to Y (1.0)
4. SmoothStep \
   Takes an upper edge, a lower edge, and X. if X < lower edge, it returns 0. If X > upper edge, it returns 1. If lower edge < X < upper edge, it returns a value between 0 and 1, smoothly interpolated.
5. Line \
   Takes UV, start position, and an end position. It returns the distance from the point in UV to any point in the infinitely long line.
6. LineSegment \
   Takes UV, start position, and an end position. It returns the distance from the point in UV to any point in the line segment between the two points.

#### Other
1. Average \
   Takes any number of inputs and returns the average of them all.

### Literals
1. Time \
Returns the time in seconds since the page first loaded. 
2. UV \
Returns the position of the fragment on screen, from 0 to 1.
3. Number \
Returns a 3 dimensional vector whose axis are all the provided number.

### Swizzling
A given dimension of a toy can be accessed using the `.` operator. \
This returns the value of that dimension in the original toy. Deceptively, that value is itself another 3 dimensional vector where each dimension is the same. \
Only one dimension can be accessed at a time. I.e., `uv.x`. Unlike GLSL, `uv.xyx` is not allowed. \
XYZ, RGB, and UVW can be used interchangeably. 


## To Do

- [x] Basic Arithmetic symbols
  - [ ] Order of operations
  - [ ] |Abs| bars
- [x] Swizzling
- [x] Vector math
- [x] Distance fields
- [x] Map of AST
- [ ] Useful built-in functions
  - [ ] Sobel filter
  - [ ] Gaussian blur ?
  - [x] White noise
  - [x] Random noise
- [ ] Example projects
  - [x] Waving flag
  - [x] Cross stitch - needs rewrite
  - [ ] CRT Filter
  - [ ] Roald Dahl
  - [ ] Mandelbrot set ?
- [x] Function support
  - [ ] Allow references to other toybox files to be used as a sort of `val` toy. ?
- [x] Comment support
- [ ] Some way to communicate parameter names
  - Potentially in AST print-out?
- [x] External textures
  - [ ] Some way to record or screenshot the page in browser ?

## Credit

`checker-map.png` from https://www.oxpal.com/uv-checker-texture.html \
`cherub.png` from Raphael's _The Sistine Madonna_ \
`Toy.cpp:NGon::GenerateGLSL()` uses https://thebookofshaders.com/07/ for the code to make regular polygons using polar functions \
`Toy.cpp:Line::GenerateGLSL()` uses an equation in https://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line#Line_defined_by_an_equation \
`in.html` uses a Hash function from https://www.shadertoy.com/view/4djSRW \
`in.html` uses a Simplex noise function from https://github.com/stegu/webgl-noise 
