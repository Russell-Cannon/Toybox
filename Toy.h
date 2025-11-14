#pragma once
#include <string>
#include <vector>

class Toy {
  public:
    Toy() {}
    virtual std::string Name();
    virtual std::string GenerateGLSL();
    void AddChild(Toy* toy);
    Toy* GetChild(int n) const;
    int NumChildren() const;
    bool Empty() const;

  protected:
    std::vector<Toy*> children;
};

// Literal Toys
class LitError : public Toy {
  public:
    std::string Name();
    std::string GenerateGLSL();
};
class LitUV : public Toy {
  public:
    std::string Name();
    std::string GenerateGLSL();
};
class LitTime : public Toy {
  public:
    std::string Name();
    std::string GenerateGLSL();
};
class LitNumber : public Toy {
  public:
    std::string Name();
    std::string GenerateGLSL();
    void SetNumber(const double _value);
    double GetNumber() const;

  protected:
    double value;
};
class Document : public Toy {
  public:
    std::string Name();
    std::string GenerateGLSL();
};

// Operations
////Unary
class Unary : public Toy {
  public:
    std::string Name();
    std::string GenerateGLSL();
};
class Fract : public Unary {
  public:
    std::string Name();
    std::string GenerateGLSL();
};
class SawTooth : public Unary {
  public:
    std::string Name();
    std::string GenerateGLSL();
};
class Sin : public Unary {
  public:
    std::string Name();
    std::string GenerateGLSL();
};
class Cos : public Unary {
  public:
    std::string Name();
    std::string GenerateGLSL();
};
class Tan : public Unary {
  public:
    std::string Name();
    std::string GenerateGLSL();
};
class Normalize : public Unary {
  public:
    std::string Name();
    std::string GenerateGLSL();
};
class Length : public Unary {
  public:
    std::string Name();
    std::string GenerateGLSL();
};
class Texture : public Unary {
  public:
    std::string Name();
    std::string GenerateGLSL();
};
class Ceiling : public Unary {
  public:
    std::string Name();
    std::string GenerateGLSL();
};
class Floor : public Unary {
  public:
    std::string Name();
    std::string GenerateGLSL();
};
class Round : public Unary {
  public:
    std::string Name();
    std::string GenerateGLSL();
};
class Negate : public Unary {
  public:
    std::string Name();
    std::string GenerateGLSL();
};
class Absolute : public Unary {
  public:
    std::string Name();
    std::string GenerateGLSL();
};
class X : public Unary {
  public:
    std::string Name();
    std::string GenerateGLSL();
};
class Y : public Unary {
  public:
    std::string Name();
    std::string GenerateGLSL();
};
class Z : public Unary {
  public:
    std::string Name();
    std::string GenerateGLSL();
};
class Screen : public Unary {
  public:
    std::string Name();
    std::string GenerateGLSL();
};
class Random : public Unary {
  public:
    std::string Name();
    std::string GenerateGLSL();
};
class Noise : public Unary {
  public:
    std::string Name();
    std::string GenerateGLSL();
};

////Binary
class Binary : public Toy {
  public:
    std::string Name();
    std::string GenerateGLSL();
};
class Multiply : public Binary {
  public:
    std::string Name();
    std::string GenerateGLSL();
};
class Divide : public Binary {
  public:
    std::string Name();
    std::string GenerateGLSL();
};
class Add : public Binary {
  public:
    std::string Name();
    std::string GenerateGLSL();
};
class Subtract : public Binary {
  public:
    std::string Name();
    std::string GenerateGLSL();
};
class Modulus : public Binary {
  public:
    std::string Name();
    std::string GenerateGLSL();
};
class Dot : public Binary {
  public:
    std::string Name();
    std::string GenerateGLSL();
};
class Cross : public Binary {
  public:
    std::string Name();
    std::string GenerateGLSL();
};
class Distance : public Binary {
  public:
    std::string Name();
    std::string GenerateGLSL();
};
class Direction : public Binary {
  public:
    std::string Name();
    std::string GenerateGLSL();
};

class Step : public Binary {
  public:
    std::string Name();
    std::string GenerateGLSL();
};
class NGon : public Binary {
  public:
    std::string Name();
    std::string GenerateGLSL();
};
class Circle : public Binary {
  public:
    std::string Name();
    std::string GenerateGLSL();
};

////Trinary
class Trinary : public Toy {
  public:
    std::string Name();
    std::string GenerateGLSL();
};
class Combine : public Trinary {
  public:
    std::string Name();
    std::string GenerateGLSL();
};
class Clamp : public Trinary {
  public:
    std::string Name();
    std::string GenerateGLSL();
};
class Mix : public Trinary {
  public:
    std::string Name();
    std::string GenerateGLSL();
};
class SmoothStep : public Trinary {
  public:
    std::string Name();
    std::string GenerateGLSL();
};
class Line : public Trinary {
  public:
    std::string Name();
    std::string GenerateGLSL();
};
class LineSegment : public Line {
  public:
    std::string Name();
    std::string GenerateGLSL();
};

//// n-ary
class Average : public Toy {
  public:
    std::string Name();
    std::string GenerateGLSL();
};
