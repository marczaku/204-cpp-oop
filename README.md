# 209 C++ OOP

In this course, will dive into Object-Oriented Programming, which allows you to apply all learnt principles of C# to C++ as well.

## Passing Criteria
Fully implement the String Class described in 3-classes-and-files including:
- copy and move constructors
- operators

And the Dynamic Array described in 3-classes-and-files including:
- compile-time polymorphism

## Excellent Criteria
Implement the following functions as well:

### Automatic resizing of the buffer

```cpp
String a{"Hello"}, // buffer size should be as big as it needs to be
a.appendLine(", World!"); // now, resize the buffer
a.appendLine("This is Marc.");
```

### Compare Strings

```cpp
String a{"Hello"};
String b{"Hello"};
String c{"World"};

printf("%d\n", a == b); // 1
printf("%d\n", c == b); // 0
```

### IndexOf

```cpp
String a{"Hello, World!"};

printf("%d\n", a.indexOf("ello")); // 1
printf("%d\n", a.indexOf("World")); // 7
printf("%d\n", a.indexOf("Marc")); // -1
```

### Replace

```cpp
String a{"Hello World"};
a.replace("l", "T");
a.print(); // HeTTo WorTd
a.replace("HeTTo", "Bye");
a.print(); // Bye WorTd
```