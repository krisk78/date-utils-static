#include <gtest/gtest.h>
#include <date-utils-static.hpp>

TEST(checkDate_Test, Valid_Date)
{
	EXPECT_TRUE(checkDate(29, 2, 2020));
}

TEST(checkDate_Test, Not_Valid_Date)
{
	EXPECT_FALSE(checkDate(29, 2, 2100));
}

class strDateConverterTest : public ::testing::Test
{
protected:
	void SetUp() override
	{
		dc0.setFormats("d.m.y", "yyyymmdd");
		dc1.setFormats("ddmmyyyy", "d/m/y");
	}

	// void TearDown override {}

	strDateConverter dc0;
	strDateConverter dc1;
	strDateConverter dc2;
};

TEST_F(strDateConverterTest, Set_Invalid_Format)
{
	EXPECT_EQ(dc2.isValid(strDateConverter::BOTH), strDateConverter::NONE);
	EXPECT_FALSE(dc2.setFromFmt("dmy"));
	EXPECT_FALSE(dc2.setFromFmt("dddmmmyyyyy"));
	EXPECT_FALSE(dc2.setFromFmt("dd/mm.yy"));
	EXPECT_FALSE(dc2.setFromFmt("dmdy"));
	EXPECT_FALSE(dc2.setFromFmt("dd/dd/yyyy"));
}

TEST_F(strDateConverterTest, Set_Valid_Format)
{
	EXPECT_EQ(dc0.isValid(strDateConverter::BOTH), strDateConverter::BOTH);
	EXPECT_EQ(dc1.isValid(strDateConverter::BOTH), strDateConverter::BOTH);
}

TEST_F(strDateConverterTest, checkStrDate_Valid)
{
	EXPECT_TRUE(dc0.checkStrDate("29.2.2020"));
	EXPECT_TRUE(dc1.checkStrDate("29022020"));
}

TEST_F(strDateConverterTest, checkStrDate_Not_Valid)
{
	EXPECT_FALSE(dc0.checkStrDate("29.2.2100"));
	EXPECT_FALSE(dc0.checkStrDate("31.4.2020"));
	EXPECT_FALSE(dc1.checkStrDate("0112020"));
}

TEST_F(strDateConverterTest, convStrDate)
{
	EXPECT_STREQ(dc0.convStrDate("29.2.2020").c_str(), "20200229");
	EXPECT_STREQ(dc0.convStrDate("1.1.2020").c_str(), "20200101");
	EXPECT_STREQ(dc1.convStrDate("29022020").c_str(), "29/02/2020");
	EXPECT_STREQ(dc1.convStrDate("31042020").c_str(), "31/04/2020");	// convStrDate does not check if date is valid
}

//int main(int argc, char** argv)
//{
//	::testing::InitGoogleTest(&argc, argv);
//	return RUN_ALL_TESTS();
//}
