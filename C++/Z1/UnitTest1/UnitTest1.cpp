#include "pch.h"
#include "CppUnitTest.h"
#include "../Z1/Z1.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:

		TEST_METHOD(TestStrToLow_LowStr)
		{
			string str = "abcdefg1234~|";
			string result = "abcdefg1234~|";
			strToLow(str);
			Assert::AreEqual(str, result);
		}
		TEST_METHOD(TestStrToLow_HighStr)
		{
			string str = "ABCDEFG1234~|";
			string result = "abcdefg1234~|";
			strToLow(str);
			Assert::AreEqual(str, result);
		}
		TEST_METHOD(TestStrToLow_MixedStr)
		{
			string str = "AbcDEfG1234~|";
			string result = "abcdefg1234~|";
			strToLow(str);
			Assert::AreEqual(str, result);
		}

		TEST_METHOD(TestCheckFileNotTxt_ShortPath)
		{
			string path = "D:/";
			Assert::AreEqual(checkFileNotTxt(path), true);
		}
		TEST_METHOD(TestCheckFileNotTxt_NoTxt)
		{
			string path = "D:/input";
			Assert::AreEqual(checkFileNotTxt(path), true);
		}
		TEST_METHOD(TestCheckFileNotTxt_Txt)
		{
			string path = "D:/input.txt";
			Assert::AreEqual(checkFileNotTxt(path), false);
		}

		TEST_METHOD(TestCheckStrToSym_NoSym)
		{
			string str = "akjsjkasfnasdadssddas";
			Assert::AreEqual(checkStringToSym(str), true);
		}
		TEST_METHOD(TestCheckFileNotTxt_WithSym)
		{
			string str = "asdknoisnaodasndoiasn";
			Assert::AreEqual(checkStringToSym(str), false);
		}

		TEST_METHOD(TestFindIndexOfIn_String1)
		{
			string mainString = "stringsstrings";
			string subString = "ing";
			Assert::AreEqual(findIndexOfIn(mainString, subString, 1), 4);
		}
		TEST_METHOD(TestFindIndexOfIn_String2)
		{
			string mainString = "stringsstrings";
			string subString = "sdf";
			Assert::AreEqual(findIndexOfIn(mainString, subString, 1), -1);
		}
	};
}
