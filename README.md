Toybox: Little Quilt for GLSL
===
## Why?
I find that a lot of redimentary shaders never need iteration or even if statements. This project, with help from the Little Quilt language in class, was made to test how far I could get without either of them.

## The Basics
Each 'Toybox' script (`.tb`) compiles into GLSL code inside a HTML page that is spit out by the compiler. \
Every value in a Toybox script is a Vector3. \
There are literals for UV coordinates, time in seconds since startup, and any number. \
There are built-in operations for most of the existing operations in GLSL.

## Quick Start
1. Create a `first.tb` file.
2. Enter `UV`
3. Run `make run`
4. Enter `first.tb`
5. out.html will have been created. Open it in a browser.

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
- [ ] Function support
  - [ ] Allow references to other toybox files to be used as a sort of `val` toy. ?
- [x] Comment support
- [ ] Some way to communicate parameter names
  - Potentially in AST print-out?
- [x] External textures
- [ ] Some way to record or screenshot the page in browser ?
