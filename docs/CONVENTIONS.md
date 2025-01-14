# Ocean Conventions

| Table of Contents |
| ----------------- |
| [General Formatting](#general-formatting) |

## [Namespace Formatting]()

## [Class Formatting]()

## [Struct Formatting]()

## [Enum Formatting]()

## [Operator Syntax Reference]()

## General Formatting

Files are expected to use 4-space indentations, not tab indentations. This is partially due to tab indentation is known to have viewing errors from source control (Github / Git).

Note that the following is subject to change before version 1.0 release:  
Documentation is expected to follow conventional doxygen styles, i.e.:

```c++
/**
 * @brief Description of Foo
 */
class Foo {
private:
    enum A {
        // TODO: Fill enum A.
    };  // A

    struct B {
        // TODO: Fill struct B data.
    };  // B

public:
    Foo() : p_Data(nullptr), i(0) { }
    /**
     * @brief Description of Foo's custom constructor.
     *
     * @param i Description of parameter 'i'.
     */
    Foo(int i);
    ~Foo() = default;

    /**
     * @brief Description of DoSomething.
     *
     * @param i Description of parameter 'i'.
     *
     * @return Description of return behavior.
     */
    bool DoSomething(int i);

private:
    char* p_Data; /** @brief Pointer to X data. */

    int m_Length; /** @brief The length of @ref(p_Data). */

};  // Foo
```
