#BNF

```bnf
<statement> ::=   "if" "(" <expression> ")" <statement>
		| "if" "(" <expression> ")" <statement> "else" <statement>
		| "while" "(" <expression> ")" <statement>
		| "{" <statement list> "}"
		| <type> <identifier> ";"
		| <type> <identifier> "=" <expression> ";"
		| <identifier> "=" <expression> ";"

<statement list> ::=
		| <statement>
		| <statement> <statement list>

<type> ::= "int"

<identifier> ::=  "A" | "B" | "C" | ... | "Z"
		| "a" | "b" | "c" | ... | "z"

<expression> ::= //the example from class.
```