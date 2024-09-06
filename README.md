# date-utils-static static library

This library provides these functions:

## Checks

### bool checkDate(int day, int month, int year)
Checks if the date is valid, regarding to Gregorian calendar rules

## Conversions

### Class strDateConverter
This class provides methods to convert string formatted dates<br>
Once date format of both source and target strings are set, the rule of conversion is set and applied on all following requests of conversion<br>
Formatting chars are y, m and d<br>

#### unsigned int century
Sets the century used when year expansion is requested<br>
Current century by default

#### bool setFormats(const std::string& from_format, const std::string& to_format)
Sets the date formats of both source and target strings

#### bool setFromFmt(const std::string& format)
Sets the date format of source strings

#### bool setToFmt(const std::string& format)
Sets the date format of target strings

#### std::string FromFmt()
Returns the format of the source strings

#### std::string ToFmt()
Returns the format of the target strings

#### unsigned char isValid(const unsigned char mode)
Returns the validity status of the conversion engine:
* const unsigned char NONE: Both source and target date formats have not been set
* const unsigned char FROM: Only source date format has been set
* const unsigned char TO: Only target date format has been set
* const unsigned char BOTH: Both source and target format has been set

#### bool checkStrDate(const std::string& str)
Checks if the given string represents a date regarding to the source date format

#### std::string convStrDate(const std::string& date_str)
Returns a string formatted date using previously date formats set for source and target strings.
