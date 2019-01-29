#include <gtest/gtest.h>
#include <string>

/*
 * The program should answer "Fizz" if the input number is multiple of 3, Buzz - if you specify a number which is multiple of 5,
 * and FizzBuzz - if the number is a multiple of 3 and 5. In other cases the program should not answer. So, the conditions are:
 * a multiple of 3 = Fizz
 * a multiple of 5 = Buzz
 * a multiple of 15 = FizzBuzz
 * any other number = <nothing>
*/


std::string FizzBuzz(int value)
{
    if(!(value % 3))
    {
        return "Fizz";
    }
    return "";
}

TEST(FizzBuzzTest, CheckAnyOtehrNubmer_ReturnNothing)
{
    EXPECT_EQ("", FizzBuzz(4));
}

TEST(FizzBuzzTest, CheckMultiplyOf_3_ReturnFizz)
{
    EXPECT_EQ("Fizz", FizzBuzz(6));
}
