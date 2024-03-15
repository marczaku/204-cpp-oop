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
Ensures that we don't need to provide buffer sizes anymore
- instead, the string buffer automatically resizes as more space is needed
- create a new buffer with the correct size
- copy all characters from the old buffer and the new data to the new buffer
```c++
String a{"Hello"}, // buffer size should be as big as it needs to be
a.appendLine(", World!"); // now, resize the buffer
a.appendLine("This is Marc.");
```

### Compare Strings
Returns `true` if two strings are truly identical
- compare the strings one char at a time
```c++
String a{"Hello"};
String b{"Hello"};
String c{"World"};

printf("%d\n", a == b); // 1
printf("%d\n", c == b); // 0
```

### IndexOf
Returns the index of the first occurrence of one string in another
- or `-1` if not found
```c++
String a{"Hello, World!"};

printf("%d\n", a.indexOf("ello")); // 1
printf("%d\n", a.indexOf("World")); // 7
printf("%d\n", a.indexOf("Marc")); // -1
```

### Replace
Replaces all occurrences of one string in another string with a third string
```c++
String a{"Hello World"};
a.replace("l", "T");
a.print(); // HeTTo WorTd
a.replace("HeTTo", "Bye");
a.print(); // Bye WorTd
```

## Additional Resources for the Assignment
It is recommended that you write the unit tests yourself, but you need help or want to validate your class in the end, check out the provided [Test File](XX-string-resources/1-test-file/).

It is recommended that you create the String Header File yourself, but if you need help or want to validate your class in the end, check out the provided [Header File](XX-string-resources/2-string-header-file/).

It is recommended that you set up a solution with a String Library Project and a String Unit Test Project yourself. But if you're unable to set it up yourself, check out the provided [Solution](XX-string-resources/3-solution/).

The Cherno has made a [Great Video](https://www.youtube.com/watch?v=ehMg6zvXuMY) about a String class just to explain Copy and Move. I recommend it as a resource for this assignment.

To understand, what we're even trying to build, it can help to check out a video about C++'s existing `string`-Class [Right Here](https://www.youtube.com/watch?v=ijIxcB9qjaU).

## Bonus Criteria
- Implement the Dynamic Array described in 3-classes-and-files
  - including compile-time polymorphism
