#include "gtest/gtest.h"
#include "../StringLibrary/String.h"


TEST(String, DefaultConstructor)
{
  const String str{37};
  EXPECT_EQ(37, str.maxSize);
  EXPECT_EQ(0, str.length);
  EXPECT_NE(nullptr, str.buffer);
  EXPECT_STREQ("", str.buffer);
}

TEST(String, Constructor)
{   
  const String str{"Hello", 37};
  EXPECT_EQ(37, str.maxSize);
  EXPECT_EQ(5, str.length);
  EXPECT_NE(nullptr, str.buffer);
  EXPECT_STREQ("Hello", str.buffer);
}


TEST(String, Append)
{
  String str{"Hello", 37};
  EXPECT_EQ(37, str.maxSize);
  EXPECT_EQ(5, str.length);
  EXPECT_NE(nullptr, str.buffer);
  EXPECT_STREQ("Hello", str.buffer);
  str.append(" World!");
  EXPECT_EQ(37, str.maxSize);
  EXPECT_EQ(12, str.length);
  EXPECT_NE(nullptr, str.buffer);
  EXPECT_STREQ("Hello World!", str.buffer);
}

TEST(String, AppendLine)
{
  String str{37};
  str.appendLine("Hello");
  EXPECT_EQ(37, str.maxSize);
  EXPECT_EQ(6, str.length);
  EXPECT_NE(nullptr, str.buffer);
  EXPECT_STREQ("Hello\n", str.buffer);
  str.appendLine(" World!");
  EXPECT_EQ(37, str.maxSize);
  EXPECT_EQ(14, str.length);
  EXPECT_NE(nullptr, str.buffer);
  EXPECT_STREQ("Hello\n World!\n", str.buffer);
}



TEST(String, Copy_Constructor)
{
  String str{"Hello", 37};
  String cpy = str;
  EXPECT_EQ(37, cpy.maxSize);
  EXPECT_EQ(5, cpy.length);
  EXPECT_NE(nullptr, cpy.buffer);
  EXPECT_NE(str.buffer, cpy.buffer);
  EXPECT_STREQ("Hello", cpy.buffer);
  cpy.append(" World!");
  EXPECT_EQ(37, str.maxSize);
  EXPECT_EQ(5, str.length);
  EXPECT_NE(nullptr, str.buffer);
  EXPECT_STREQ("Hello", str.buffer);
}

TEST(String, Copy_Assignment)
{
  String str{"Hello", 37};
  String cpy{3};
  cpy.append("AB");
  cpy = str;
  EXPECT_EQ(37, cpy.maxSize);
  EXPECT_EQ(5, cpy.length);
  EXPECT_NE(nullptr, cpy.buffer);
  EXPECT_NE(str.buffer, cpy.buffer);
  EXPECT_STREQ("Hello", cpy.buffer);
  cpy.append(" World!");
  EXPECT_EQ(37, str.maxSize);
  EXPECT_EQ(5, str.length);
  EXPECT_NE(nullptr, str.buffer);
  EXPECT_STREQ("Hello", str.buffer);
}


TEST(String, Move)
{
  String str{"Hello", 37};
  char* oldBufferAddr = str.buffer;
  
  String cpy = std::move(str);

  EXPECT_EQ(37, cpy.maxSize);
  EXPECT_EQ(5, cpy.length);
  EXPECT_EQ(oldBufferAddr, cpy.buffer);
  EXPECT_STREQ(cpy.buffer, "Hello");
  
  EXPECT_EQ(0, str.maxSize);
  EXPECT_EQ(0, str.length);
  EXPECT_EQ(nullptr, str.buffer);
}

TEST(String, Move_Assignment)
{
  String str{"Hello", 37};
  char* oldBufferAddr = str.buffer;
  
  String cpy{3};
  cpy.append("AB");
  cpy = std::move(str);
  
  EXPECT_EQ(37, cpy.maxSize);
  EXPECT_EQ(5, cpy.length);
  EXPECT_NE(nullptr, cpy.buffer);
  EXPECT_EQ(oldBufferAddr, cpy.buffer);
  EXPECT_STREQ(cpy.buffer, "Hello");
  
  EXPECT_EQ(0, str.maxSize);
  EXPECT_EQ(0, str.length);
  EXPECT_EQ(nullptr, str.buffer);
}


TEST(String, EqualityOperator)
{ 
  EXPECT_TRUE(String("Hello", 15) == String("Hello", 17));
  EXPECT_FALSE(String("Hello", 15) == String("Helloa", 17));
  EXPECT_FALSE(String("Hello", 15) == String("Hallo", 17));
  EXPECT_FALSE(String("Hello", 15) == String("", 17));
  EXPECT_FALSE(String("", 15) == String("", 17));
}


TEST(String, InequalityOperator)
{
  EXPECT_FALSE(String("Hello", 15) != String("Hello", 17));
  EXPECT_TRUE(String("Hello", 15) != String("Helloa", 17));
  EXPECT_TRUE(String("Hello", 15) != String("Hallo", 17));
  EXPECT_TRUE(String("Hello", 15) != String("", 17));
  EXPECT_TRUE(String("", 15) != String("", 17));
}


TEST(String, AddOperator)
{
  String hello("Hello", 15);
  String world("World", 15);
  String helloWorld = hello + world;
  EXPECT_STREQ("Hello", hello.buffer);
  EXPECT_EQ(5, hello.length);
  EXPECT_STREQ("World", world.buffer);
  EXPECT_EQ(5, world.length);
  EXPECT_STREQ("HelloWorld", helloWorld.buffer);
  EXPECT_EQ(10, helloWorld.length);
}

TEST(String, CompoundAddOperator)
{
  String hello("Hello", 15);
  String world("World", 15);
  String& helloWorld = hello += world;
  EXPECT_STREQ("HelloWorld", hello.buffer);
  EXPECT_EQ(10, hello.length);
  EXPECT_STREQ("World", world.buffer);
  EXPECT_EQ(5, world.length);
  EXPECT_EQ(helloWorld.buffer, hello.buffer);
}

TEST(String, IndexOperator)
{
  String hello("Hello", 15);
  EXPECT_EQ('o', hello[4]);
}

TEST(String, LessThanOperator)
{
  EXPECT_TRUE(String("Hello", 15) < String("Hellob", 15));
  EXPECT_TRUE(String("Hella", 15) < String("Hello", 15));
  EXPECT_TRUE(String("a", 15) < String("bbbbbb", 15));
  EXPECT_TRUE(String("", 15) < String("bbbbbb", 15));
  EXPECT_FALSE(String("Hello", 15) < String("Hello", 15));
  
  
  EXPECT_FALSE(String("Hellob", 15) < String("Hello", 15));
  EXPECT_FALSE(String("Hello", 15)  < String("Hella", 15));
  EXPECT_FALSE(String("bbbbbb", 15) < String("a", 15));
  EXPECT_FALSE(String("bbbbbb", 15) < String("", 15));
}

TEST(String, GreaterThanOperator)
{
  EXPECT_FALSE(String("Hello", 15) > String("Hellob", 15));
  EXPECT_FALSE(String("Hella", 15) > String("Hello", 15));
  EXPECT_FALSE(String("a", 15) > String("bbbbbb", 15));
  EXPECT_FALSE(String("", 15) > String("bbbbbb", 15));
  EXPECT_FALSE(String("Hello", 15) > String("Hello", 15));
  
  EXPECT_TRUE(String("Hellob", 15) > String("Hello", 15));
  EXPECT_TRUE(String("Hello", 15) > String("Hella", 15));
  EXPECT_TRUE(String("bbbbbb", 15) > String("a", 15));
  EXPECT_TRUE(String("bbbbbb", 15) > String("", 15));
}

TEST(String, Replace)
{
  String str{"Hello", 32};
  str.Replace('l', 'p');
  EXPECT_STREQ("Heppo", str.buffer);
}

TEST(String, GetString)
{
  String str{"Hello", 32};
  EXPECT_EQ(str.buffer, str.getString());
}

TEST(String, IndexOf)
{
EXPECT_EQ(2, String("Hello", 15).IndexOf(String{"ll",12}));
EXPECT_EQ(2, String("Hello", 15).IndexOf(String{"llo",12}));
EXPECT_EQ(-1, String("Hello", 15).IndexOf(String{"lla",12}));
EXPECT_EQ(-1, String("Hello", 15).IndexOf(String{"p",12}));
EXPECT_EQ(0, String("Hello", 15).IndexOf(String{"H",12}));
EXPECT_EQ(-1, String("Hello", 15).IndexOf(String{"",12}));
}

