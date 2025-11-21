#pragma once
#include <memory>
#include <string>
#include <vector>

class Toy {
  public:
    Toy();
    virtual std::string Name();
    virtual std::string GenerateGLSL();
    void AddChild(std::shared_ptr<Toy> toy);
    void RemoveChild(std::shared_ptr<Toy> toy);
    std::shared_ptr<Toy> GetChild(int n) const;
    virtual std::shared_ptr<Toy> Clone();
    int NumChildren() const;
    bool Empty() const;
    void Print(bool last = true, std::string pre = "");

  protected:
    std::vector<std::shared_ptr<Toy>> children;
};

#define BASE                    \
    std::string Name();         \
    std::string GenerateGLSL(); \
    std::shared_ptr<Toy> Clone()

// Literal Toys
class LitError : public Toy {
  public:
    BASE;
};
class LitUV : public Toy {
  public:
    BASE;
};
class LitTime : public Toy {
  public:
    BASE;
};
class LitNumber : public Toy {
  public:
    BASE;
    void SetNumber(const double _value);
    double GetNumber() const;

  protected:
    double value;
};

// Other
class Document : public Toy {
  public:
    BASE;
};
class Parameter : public Toy {
  public:
    Parameter(std::string _ID);
    BASE;
    std::string ID;
};

// Operations
////Unary
class Unary : public Toy {
  public:
    BASE;
};
class Fract : public Unary {
  public:
    BASE;
};
class SawTooth : public Unary {
  public:
    BASE;
};
class Sin : public Unary {
  public:
    BASE;
};
class Cos : public Unary {
  public:
    BASE;
};
class Tan : public Unary {
  public:
    BASE;
};
class Normalize : public Unary {
  public:
    BASE;
};
class Length : public Unary {
  public:
    BASE;
};
class Texture : public Unary {
  public:
    BASE;
    void SetTextureID(int i);
    int GetTextureID() const;

  protected:
    int textureID = 0;
};
class Ceiling : public Unary {
  public:
    BASE;
};
class Floor : public Unary {
  public:
    BASE;
};
class Round : public Unary {
  public:
    BASE;
};
class Negate : public Unary {
  public:
    BASE;
};
class Absolute : public Unary {
  public:
    BASE;
};
class X : public Unary {
  public:
    BASE;
};
class Y : public Unary {
  public:
    BASE;
};
class Z : public Unary {
  public:
    BASE;
};
class Screen : public Unary {
  public:
    BASE;
};
class Random : public Unary {
  public:
    BASE;
};
class Noise : public Unary {
  public:
    BASE;
};

////Binary
class Binary : public Toy {
  public:
    BASE;
};
class Multiply : public Binary {
  public:
    BASE;
};
class Divide : public Binary {
  public:
    BASE;
};
class Add : public Binary {
  public:
    BASE;
};
class Subtract : public Binary {
  public:
    BASE;
};
class Modulus : public Binary {
  public:
    BASE;
};
class Dot : public Binary {
  public:
    BASE;
};
class Cross : public Binary {
  public:
    BASE;
};
class Distance : public Binary {
  public:
    BASE;
};
class Direction : public Binary {
  public:
    BASE;
};

class Step : public Binary {
  public:
    BASE;
};
class NGon : public Binary {
  public:
    BASE;
};
class Circle : public Binary {
  public:
    BASE;
};

////Trinary
class Trinary : public Toy {
  public:
    BASE;
};
class Combine : public Trinary {
  public:
    BASE;
};
class Clamp : public Trinary {
  public:
    BASE;
};
class Mix : public Trinary {
  public:
    BASE;
};
class SmoothStep : public Trinary {
  public:
    BASE;
};
class Line : public Trinary {
  public:
    BASE;
};
class LineSegment : public Line {
  public:
    BASE;
};

//// n-ary
class Average : public Toy {
  public:
    BASE;
};
