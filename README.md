# 209 C++ OOP

In this course, will dive into Object-Oriented Programming, which allows you to apply all learnt principles of C# to C++ as well.

For all these functions, DO NOT USE:
- the standard C String-Functions like `str_cmp()`, `str_cpy()` etc.
- the C++ String Class `string`

## Passing Criteria
Fully implement the String Class described in 1-classes including:
- copy and move constructors
- operators
- unit tests

## Excellent Criteria
- Fulfil all Passing Criteria
- Implement the following String Functions as well:

### Automatic resizing of the buffer

```c++
String a{"Hello"}, // buffer size should be as big as it needs to be
a.appendLine(", World!"); // now, resize the buffer
a.appendLine("This is Marc.");
```

### Compare Strings

```c++
String a{"Hello"};
String b{"Hello"};
String c{"World"};

printf("%d\n", a == b); // 1
printf("%d\n", c == b); // 0
```

### IndexOf

```c++
String a{"Hello, World!"};

printf("%d\n", a.indexOf("ello")); // 1
printf("%d\n", a.indexOf("World")); // 7
printf("%d\n", a.indexOf("Marc")); // -1
```

### Replace

```c++
String a{"Hello World"};
a.replace("l", "T");
a.print(); // HeTTo WorTd
a.replace("HeTTo", "Bye");
a.print(); // Bye WorTd
```

## Bonus Criteria
- Implement the Dynamic Array described in 3-classes-and-files
  - including compile-time polymorphism
