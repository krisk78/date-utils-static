#pragma once

/*! \file date_utils.h
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

	/*! \brief Returns true if the given date is valid.
	*	\warning This function use the Gregorian calendar rules which adoption depends on nations.
	*/
	bool checkDate(int day, int month, int year);

	/*! \brief A string formatted date converter.
	*
	*	This class implements a conversion tool for string formatted dates. After initialization of from and to format strings,
	*	the member function convDate gives a way to convert dates through a fastener, without analyzing all the conditions of the conversion.
	*/
	class strDateConverter
	{
	public:
		/*! \brief Constants NONE, BOTH, FROM and TO are used with the function isValid to check each format validity or both. */
		static const unsigned char NONE = 0;
		static const unsigned char BOTH = 3;
		static const unsigned char FROM = 1;
		static const unsigned char TO = 2;

		/*! \brief Constructor. Initialize the default century to the current. */
		strDateConverter();

		/*! \brief Sets the from and to format strings and the valid status based on success or fail of the fastener construction. */
		bool setFormats(const std::string& from, const std::string& to) { setFromFmt(from); setToFmt(to); return m_valid = m_fromValid && m_toValid; }
		/*! \brief Sets the from format string. If the to format string is set, constructs the fastener and sets the valid status. */
		bool setFromFmt(const std::string& fmt) { return m_valid = setFmt(fmt, m_fromFmt, m_fromDelim, m_fromPos, m_fromLen, m_fromSep, m_fromValid) && m_toValid; }
		/*! \brief Returns the from format string. */
		std::string fromFmt() { return m_fromFmt; }
		/*! \brief Sets the to format string. If the from format string is set, constructs the fastener and sets the valid status. */
		bool setToFmt(const std::string& fmt);
		/*! \brief Returns the to format string. */
		std::string toFmt() { return m_toFmt; }
		/*! \brief Returns the validity status of the fastener (strDateConverter::BOTH) or of each format (strDateConverter::FROM or strDateConverter::TO).
		*
		*	In case of the return of the setFormats function is false, this function can be used to know which format is wrong.
		*	The result is a bitwise OR of validity status of given argument.
		*	In example, isValid(BOTH) returns BOTH if both formats are valid, NONE if they are both wrong, FROM if only the from format is valid and TO if only the to format is valid.
		*/
		const unsigned char isValid(const unsigned char mode);
		/*! \brief Return true if the given string is compliant with the from format string.
		*
		*	Always returns false if the from date string format is not valid.
		*/
		bool checkStrDate(const std::string& str);

		/*! \brief Returns a string formatted date converted from the given date using the formats previously set.
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