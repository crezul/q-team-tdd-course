/*### Bank OCR

Your manager has recently purchased a machine that assists in reading letters and faxes sent in by branch offices. The machine scans the paper documents, and produces a file with a number of entries. You will write a program to parse this file.

#### Specification
#### User Story 1

The following format is created by the machine:
```
    _  _     _  _  _  _  _
  | _| _||_||_ |_   ||_||_|
  ||_  _|  | _||_|  ||_| _|
```
Each entry is 3 lines long, and each line has 27 characters. 3 lines of each entry contain an account number written using pipes and underscores.

Each account number should have 9 digits, all of which should be in the range 0-9. A normal file contains around 500 entries.

Write a program that can take this file and parse it into actual account numbers.

Example input and output
```
 _  _  _  _  _  _  _  _  _
| || || || || || || || || |
|_||_||_||_||_||_||_||_||_|

=> 000000000

  |  |  |  |  |  |  |  |  |
  |  |  |  |  |  |  |  |  |

=> 111111111

 _  _  _  _  _  _  _  _  _
 _| _| _| _| _| _| _| _| _|
|_ |_ |_ |_ |_ |_ |_ |_ |_

=> 222222222

 _  _  _  _  _  _  _  _  _
 _| _| _| _| _| _| _| _| _|
 _| _| _| _| _| _| _| _| _|

=> 333333333

|_||_||_||_||_||_||_||_||_|
  |  |  |  |  |  |  |  |  |

=> 444444444

 _  _  _  _  _  _  _  _  _
|_ |_ |_ |_ |_ |_ |_ |_ |_
 _| _| _| _| _| _| _| _| _|

=> 555555555

 _  _  _  _  _  _  _  _  _
|_ |_ |_ |_ |_ |_ |_ |_ |_
|_||_||_||_||_||_||_||_||_|

=> 666666666

 _  _  _  _  _  _  _  _  _
  |  |  |  |  |  |  |  |  |
  |  |  |  |  |  |  |  |  |

=> 777777777

 _  _  _  _  _  _  _  _  _
|_||_||_||_||_||_||_||_||_|
|_||_||_||_||_||_||_||_||_|

=> 888888888

 _  _  _  _  _  _  _  _  _
|_||_||_||_||_||_||_||_||_|
 _| _| _| _| _| _| _| _| _|

=> 999999999

    _  _     _  _  _  _  _
  | _| _||_||_ |_   ||_||_|
  ||_  _|  | _||_|  ||_| _|

=> 123456789
```
*/
#include <gtest/gtest.h>
#include <string>
#include <map>

const unsigned short g_digitLen = 3;
const unsigned short g_linesInDigit = 3;
struct Digit
{
    std::string lines[g_linesInDigit];
};

const unsigned short g_digitsOnDisplay = 9;
struct Display
{
    std::string lines[g_linesInDigit];
};

const Digit s_digit0 = { " _ ",
                         "| |",
                         "|_|"
                       };
const Digit s_digit1 = { "   ",
                         "  |",
                         "  |"
                       };
const Digit s_digit2 = { " _ ",
                         " _|",
                         "|_ "
                       };
const Digit s_digit3 = { " _ ",
                         " _|",
                         " _|"
                       };
const Digit s_digit4 = { "   ",
                         "|_|",
                         "  |"
                       };
const Digit s_digit5 = { " _ ",
                         "|_ ",
                         " _|"
                       };
const Digit s_digit6 = { " _ ",
                         "|_ ",
                         "|_|"
                       };
const Digit s_digit7 = { " _ ",
                         "  |",
                         "  |"
                       };
const Digit s_digit8 = { " _ ",
                         "|_|",
                         "|_|"
                       };
const Digit s_digit9 = { " _ ",
                         "|_|",
                         " _|"
                       };

const Display s_displayAll0 = { " _  _  _  _  _  _  _  _  _ ",
                                "| || || || || || || || || |",
                                "|_||_||_||_||_||_||_||_||_|"
};

const Display s_displayAll1 = { "                           ",
                                "  |  |  |  |  |  |  |  |  |",
                                "  |  |  |  |  |  |  |  |  |"
};

const Display s_displayAll2 = {  " _  _  _  _  _  _  _  _  _ ",
                                 " _| _| _| _| _| _| _| _| _|",
                                 "|_ |_ |_ |_ |_ |_ |_ |_ |_ "
};

const Display s_displayAll3 = { " _  _  _  _  _  _  _  _  _ ",
                                " _| _| _| _| _| _| _| _| _|",
                                " _| _| _| _| _| _| _| _| _|"
};

const Display s_displayAll4 = { "                           ",
                                "|_||_||_||_||_||_||_||_||_|",
                                "  |  |  |  |  |  |  |  |  |"
};

const Display s_displayAll5 = { " _  _  _  _  _  _  _  _  _ ",
                                "|_ |_ |_ |_ |_ |_ |_ |_ |_ ",
                                " _| _| _| _| _| _| _| _| _|"
};

const Display s_displayAll6 = { " _  _  _  _  _  _  _  _  _ ",
                                "|_ |_ |_ |_ |_ |_ |_ |_ |_ ",
                                "|_||_||_||_||_||_||_||_||_|"
};

const Display s_displayAll7 = { " _  _  _  _  _  _  _  _  _ ",
                                "  |  |  |  |  |  |  |  |  |",
                                "  |  |  |  |  |  |  |  |  |"
};

const Display s_displayAll8 = { " _  _  _  _  _  _  _  _  _ ",
                                "|_||_||_||_||_||_||_||_||_|",
                                "|_||_||_||_||_||_||_||_||_|"
};

const Display s_displayAll9 = { " _  _  _  _  _  _  _  _  _ ",
                                "|_||_||_||_||_||_||_||_||_|",
                                " _| _| _| _| _| _| _| _| _|"
};

const Digit s_display123456789 = {   "    _  _     _  _  _  _  _ ",
                                     "  | _| _||_||_ |_   ||_||_|",
                                     "  ||_  _|  | _||_|  ||_| _|"
};

const Digit s_displayAll01 =   { " _    ",
                                 "| |  |",
                                 "|_|  |"
};

std::map<int, int> g_DigitsTable;


bool operator == (const Digit& left, const Digit& right)
{
    for(int i = 0; i< g_linesInDigit; ++i)
    {
        if(left.lines[i] != right.lines[i])
        {
            return false;
        }
    }
    return true;
}

std::map<int, int> g_ImagesTable;

int getCheckSum(const std::string digit[], int startPos = 0, int length = g_linesInDigit)
{
    int res = 0;

    for(int i = 0; i<length; ++i)
    {
        for(int j = startPos; j< startPos + length; ++j)
        {
            int val = digit[i][j] == ' ' ? 0 : 1;
            res += std::pow(i+1, j+1-startPos)*val;
        }
    }

    return res;
}

void BuildDigitsTable()
{
    g_ImagesTable[0] = getCheckSum(s_digit0.lines);
    g_ImagesTable[1] = getCheckSum(s_digit1.lines);
    g_ImagesTable[2] = getCheckSum(s_digit2.lines);
    g_ImagesTable[3] = getCheckSum(s_digit3.lines);
    g_ImagesTable[4] = getCheckSum(s_digit4.lines);
    g_ImagesTable[5] = getCheckSum(s_digit5.lines);
    g_ImagesTable[6] = getCheckSum(s_digit6.lines);
    g_ImagesTable[7] = getCheckSum(s_digit7.lines);
    g_ImagesTable[8] = getCheckSum(s_digit8.lines);
    g_ImagesTable[9] = getCheckSum(s_digit9.lines);
}

std::string Image2Number(const Digit& display)
{

    std::string result = "";
    for(size_t i = 0; i < display.lines->size(); i+=g_linesInDigit)
    {
        for(auto pair : g_ImagesTable)
        {
            if(pair.second == getCheckSum(display.lines, i, g_linesInDigit))
            {
                result +=  std::to_string(pair.first);
                break;
            }
        }
    }


    return result;
}

TEST(BankOCRTest, zero_Digit_is_zero_number)
{
    BuildDigitsTable();
    EXPECT_EQ("0", Image2Number(s_digit0));
}


TEST(BankOCRTest, one_Digit_is_one_number)
{
    BuildDigitsTable();
    EXPECT_EQ("1", Image2Number(s_digit1));
}

TEST(BankOCRTest, zero_one_Digit_is_zero_one_number)
{
    BuildDigitsTable();
    EXPECT_EQ("01", Image2Number(s_displayAll01));
}

TEST(BankOCRTest, full_test_Digit_is_123456789_number)
{
    BuildDigitsTable();
    EXPECT_EQ("123456789", Image2Number(s_display123456789));
}
