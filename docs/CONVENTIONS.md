# Ocean Conventions

## Naming Scheme

### Open Methods

An "open method" would be considered a method that does not require an instance.
So either static helper functions or general helper functions that are used.
Open methods should follow the naming convention of a lowercase 'o' as the prefix to the name.
For example:

```cpp
oDoSomething() { ... }

oGetTime() { ... }

oConvertTo___() { ... }
```

### Classes, Structs, and Enums

Classes, structs, and enums should have no prefix, and should be Pascal case.
Names in pascal case start with a capital letter.
In case of the names with multiple words, all words will start with capital letters.
For example:

```cpp
enum Foo { ... };

struct Bar { ... };

class FooBar { ... };
```

A special case is when working with implementation specific versions of classes, or structs.
For example:

```cpp
/**
 * @brief General rendering class.
 */
class Renderer { ... };

/**
 * @brief OpenGL rendering class.
 */
class glRenderer { ... };

/**
 * @brief Vulkan rendering class.
 */
class vkRenderer { ... };
```

## Documentation

Documentation will be Doxygen compliant, using `/** */` (often refered to as JavaDoc or C-Style conventions).
For example:

```cpp
/**
 * @brief Function description
 *
 * @param p1 Parameter description
 * @param p2 Parameter description
 *
 * @return Return description
 */
void oFoo(i32 p1, i32 p2) { ... }
```

## Example Code

```cpp

/**
 * @brief A struct to track data and if they are active.
 */
struct Foo {
    u32 id; /** @brief The ID of the data Foo is tracking. */
    b8 isActive; /** @brief Record's if the data is active or not. */

};  // Foo

/**
 * @brief An enum of flags to control Bar.  
 */
enum BarFlags {
    /** @brief Flag to set verbose behavior. */
    BAR_VERBOSE = 0 << 1,
    /** @brief Flag to set ____. */
    BAR_____    = 0 << 2,

};  // BarFlags

/**
 * @brief A class that manages a set of data.
 */
class Bar {
public:
    /**
     * @brief Constructs a new Bar object.
     *
     * @param size The size of Bar data to create.
     * @param flags The BarFlags to use during construction. (OPTIONAL)
     */
    Bar(u32 size, const BarFlags flags = 0) { ... }
    ~Bar() = default;

    /**
     * @brief Description of DoSomething().
     */
    void DoSomething() { ... }

private:
    DynamicArray<Foo> m_ActiveData; /** @brief The list of Foo that is marked as active. */
    DynamicArray<Foo> m_InactiveData; /** @brief The list of Foo that is marked as indactive. */

};  // Bar

```
