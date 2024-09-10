#pragma once

/*! \file date-utils-static.hpp
*	\brief Few date functions. See functions documentation for details.
*	\author Christophe COUAILLET
*/

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#endif

#include <array>
#include <string>

/*! \brief Namespace of the library functions and classes */

namespace date_utils
{

	/*! \brief Checks if the given date is valid or not
	*	\param day, month, year	components of the date to check
	*	\return true if the date is valid else false
	*	\warning This function use the Gregorian calendar rules which adoption depends on nations.
	*/
	bool checkDate(int day, int month, int year);

	/*! \brief A string formatted date converter
	*
	*	This class implements a conversion tool for string formatted dates. Once from and to format strings are set, the rule of conversion is build and used for all following conversion requests
	*/
	class strDateConverter
	{
	public:
		/*! \brief Constant returned to indicate that both source and target format strings are not valid. */
		static const unsigned char NONE = 0;
		/*!	\brief Constant used to check if both source and target format strings are valid */
		static const unsigned char BOTH = 3;
		/*! \brief Constant used to check if source format string is valid */
		static const unsigned char FROM = 1;
		/*! \brief Constant used to check if target format string is valid */
		static const unsigned char TO = 2;

		/*! \brief Constructor. Initialize the default century to the current. */
		strDateConverter();

		/*! \brief Sets the from and to format strings and the valid status based on success or fail of the fastener construction.
		*	\param from Date format of the source
		*	\param to	Date format of the target
		*	\return	true if both format strings have been successfully analyzed, else false
		*/
		bool setFormats(const std::string& from, const std::string& to) { setFromFmt(from); setToFmt(to); return m_valid = m_fromValid && m_toValid; }

		/*! \brief Sets the from format string. If the to format string is set, constructs the fastener and sets the valid status.
		*	\param fmt Date format of the source
		*	\return true if format string has been successfully analyzed, else false
		*/
		bool setFromFmt(const std::string& fmt) { return m_valid = setFmt(fmt, m_fromFmt, m_fromDelim, m_fromPos, m_fromLen, m_fromSep, m_fromValid) && m_toValid; }
		
		/*! \brief Returns the from format string.
		*	\return the format string used for source
		*/
		std::string fromFmt() { return m_fromFmt; }

		/*! \brief Sets the to format string. If the from format string is set, constructs the fastener and sets the valid status.
		*	\param fmt the format string to use for target
		*	\return true if format string has been successfully analyzed and the fastener has been generated, else false
		*/
		bool setToFmt(const std::string& fmt);

		/*! \brief Returns the to format string.
		*	\return the format string used for target
		*/
		std::string toFmt() { return m_toFmt; }

		/*! \brief Returns the validity status of the fastener (strDateConverter::BOTH) or of each format (strDateConverter::FROM or strDateConverter::TO).
		*	\param mode any constant TO, FROM or BOTH
		*	\return any constant NONE, TO, FROM or BOTH regarding the argument passed and the validity status of the involved format strings
		*
		*	In case of the return of the setFormats function is false, this function can be used to know which format is wrong.
		*	The result is a bitwise OR of validity status of given argument.
		*	In example, isValid(BOTH) returns BOTH if both formats are valid, NONE if they are both wrong, FROM if only the from format is valid and TO if only the to format is valid.
		*/
		const unsigned char isValid(const unsigned char mode);

		/*! \brief Return true if the given string is compliant with the from format string.
		*	\param str the string to check with the source format string
		*	\return true if the passed string is compliant with the source format string, else false
		*
		*	Always returns false if the from date string format is not valid.
		*/
		bool checkStrDate(const std::string& str);

		/*! \brief Returns a string formatted date converted from the given date using the formats previously set.
		*	\param date a string representing a date, using the source format string
		*	\return the converted string date using the target format string
		*
		*	Returns an empty string if the valid status of the fastener is not true. This function doesn't check the validity of the given string date.
		*	Use the appropriate function checkStrDate to do this.
		*/
		std::string convStrDate(const std::string& date);

		/*! \brief Sets or returns the century used to fill converted string date if needed.

			The value of century is initialized at the current century by the constructor.
		*/
		unsigned int century{ 0 };

	private:

		std::string m_fromFmt;
		std::string m_toFmt;
		bool m_fromDelim{ false };
		bool m_toDelim{ false };
		std::array<unsigned char, 3> m_fromPos;
		std::array<unsigned char, 3> m_toPos;
		std::array<unsigned char, 3> m_fromLen;
		std::array<unsigned char, 3> m_toLen;
		char m_fromSep{ ' ' };
		char m_toSep{ ' ' };
		std::array<unsigned char, 3> m_toOrder;
		bool m_fromValid{ false };
		bool m_toValid{ false };
		bool m_valid{ false };

		const unsigned char UNDEFINED_COMP = -1;
		const unsigned char YEAR_COMP = 0;
		const unsigned char MONTH_COMP = 1;
		const unsigned char DAY_COMP = 2;

		bool setFmt(const std::string& fmt, std::string& m_fmt, bool& m_delim, std::array<unsigned char, 3>& m_pos, std::array<unsigned char, 3>& m_len, char& m_sep, bool& m_val);
	};

}