// utils.cpp : Defines the functions for the static library.
//

#include <string>
#include <time.h>

//#ifdef _WIN32
//#include <Windows.h>
//#else
//#endif // _WIN32

#include <date-utils-static.hpp>
#include <str-utils-static.hpp>

namespace date = date_utils;
namespace str = str_utils;

bool date_utils::checkDate(int day, int month, int year)
{
    if (day < 1 || day > 31)
        return false;
    if (month < 1 || month > 12)
        return false;
    if ((month == 4 || month == 6 || month == 9 || month == 11) && day == 31)
        return false;
    if (month == 2)
    {
        if (day == 30)
            return false;
        bool leap = (year % 4 == 0) && (year % 100 != 0) || (year % 400 == 0);
        if (!leap && day == 29)
            return false;
    }
    return true;
}

date::strDateConverter::strDateConverter()
{
    auto t = time(0);
#ifdef _WIN32
    std::tm now;
    localtime_s(&now, &t);
    century = now.tm_year / 100 + 19;
#else
    century = localtime(&t)->tm_year / 100 + 19;
#endif
    m_fromLen = {};
    m_fromPos = {};
    m_toLen = {};
    m_toPos = {};
    m_toOrder = {};
}

bool date::strDateConverter::setToFmt(const std::string& fmt)
{
    bool m_valid = m_fromValid && setFmt(fmt, m_toFmt, m_toDelim, m_toPos, m_toLen, m_toSep, m_toValid);
    if (m_valid)
    {
        // defines the order of converted components based on their position in the format
        m_toOrder = { YEAR_COMP, MONTH_COMP, DAY_COMP };
        for (unsigned char i = 0; i < 2; i++)
            for (unsigned char j = i + 1; j < 3; j++)
                if (m_toPos[m_toOrder[j]] < m_toPos[m_toOrder[i]])
                    std::swap(m_toOrder[i], m_toOrder[j]);
    }
    return m_valid;
}

const unsigned char date::strDateConverter::isValid(const unsigned char mode)
{
    unsigned char result = NONE;
    if (m_fromValid && (mode & FROM) == FROM)
        result = result | FROM;
    if (m_toValid && (mode & TO) == TO)
        result = result | TO;
    return result;
}

bool date::strDateConverter::checkStrDate(const std::string& str)
{
    if (!m_fromValid)
        return false;
    auto date = str::trimc(str);
    if (date.length() > m_fromFmt.length() + 5)
        return false;
    std::array<unsigned long, 3> compvals;
    compvals.fill(0);
    if (m_fromDelim)
    {
        std::vector<std::string> comps = str::split(date, m_fromSep);
        if (comps.size() != 3)
            return false;
        for (size_t i = 0; i < 3; i++)
        {
            auto comp = m_fromPos[i];
            size_t proc;
            try
            {
                compvals[i] = stoul(comps[comp], &proc);
            }
            catch (...)
            {
                return false;
            }
            if (compvals[i] < 0)
                return false;
            if ((m_fromLen[i] == 2 || m_fromLen[i] == 4) && proc != m_fromLen[i])
                return false;
        }
    }
    else
    {
        for (size_t i = 0; i < 3; i++)
        {
            auto comp = date.substr(m_fromPos[i], m_fromLen[i]);
            size_t proc;
            try
            {
                compvals[i] = stoul(comp, &proc);
            }
            catch (...)
            {
                return false;
            }
            if (compvals[i] < 0)
                return false;
            if ((m_fromLen[i] == 2 || m_fromLen[i] == 4) && proc != m_fromLen[i])
                return false;
        }
    }
    if (m_fromLen[YEAR_COMP] == 2)
        compvals[YEAR_COMP] += century;
    return checkDate(compvals[DAY_COMP], compvals[MONTH_COMP], compvals[YEAR_COMP]);
}

std::string date::strDateConverter::convStrDate(const std::string& date)
{
    std::string result{};
    std::vector<std::string> comps{};
    if (m_fromDelim)
    {
        auto comps2 = str::split(date, m_fromSep);
        if (comps2.size() != 3)
            return result;
        for (unsigned char i = 0; i < 3; i++)
        {
            auto comp = comps2[m_fromPos[i]];
            comps.push_back(comp);
        }
    }
    else
    {
        for (unsigned char i = 0; i < 3; i++)
        {
            if (date.length() > m_fromPos[i])
            {
                auto comp = date.substr(m_fromPos[i], m_fromLen[i]);
                comps.push_back(comp);
            }
        }
        if (comps.size() != 3)
            return result;
    }
    if (comps[YEAR_COMP].length() == 2)
        comps[YEAR_COMP].insert(0, std::to_string(century));
    for (unsigned char i = 0; i < 3; i++)
        if (comps[i].length() < m_toLen[i])
            comps[i].insert(0, 1, '0');         // no need to do it more than once
    for (unsigned char i = 0; i < 3; i++)
    {
        result += comps[m_toOrder[i]];
        if (i < 2 && m_toDelim)
            result += m_toSep;
    }
    return result;
}

bool date::strDateConverter::setFmt(const std::string& fmt, std::string& m_fmt, bool& m_delim, std::array<unsigned char, 3>& m_pos, std::array<unsigned char, 3>& m_len, char& m_sep, bool& m_val)
{
    m_val = true;
    m_delim = false;
    m_pos.fill(0);
    m_len.fill(0);
    auto fmtc = str::to_lower(str::trimc(fmt));
    m_fmt = fmtc;
    auto itr = fmtc.find_first_not_of("dmy");
    if (itr != std::string::npos)
    {
        m_delim = true;
        m_sep = fmtc[itr];
    }
    if (m_delim)
    {
        std::vector<std::string> comps = str::split(fmtc, m_sep);
        if (comps.size() != 3)
            return m_val = false;
        for (unsigned char i = 0; i < comps.size(); i++)
        {
            if (comps[i].length() == 0)
                return m_val = false;
            switch (comps[i][0])
            {
                case 'd':
                    itr = comps[i].find_first_not_of('d');
                    if (itr != std::string::npos || comps[i].length() > 2)
                        return m_val = false;
                    m_pos[DAY_COMP] = i;
                    m_len[DAY_COMP] = (unsigned char)comps[i].length();
                    break;
                case 'm':
                    itr = comps[i].find_first_not_of('m');
                    if (itr != std::string::npos || comps[i].length() > 2)
                        return m_val = false;
                    m_pos[MONTH_COMP] = i;
                    m_len[MONTH_COMP] = (unsigned char)comps[i].length();
                    break;
                case 'y':
                    itr = comps[i].find_first_not_of('y');
                    if (itr != std::string::npos || comps[i].length() == 3 || comps[i].length() > 4)
                        return m_val = false;
                    m_pos[YEAR_COMP] = i;
                    m_len[YEAR_COMP] = (unsigned char)comps[i].length();
            }
        }
    }
    else
    {
        auto curcomp = UNDEFINED_COMP;
        for (unsigned char i = 0; i < fmtc.length(); i++)
        {
            switch (fmtc[i])
            {
                case 'd':
                    if (curcomp != DAY_COMP)
                    {
                        m_pos[DAY_COMP] = i;
                        curcomp = DAY_COMP;
                    }
                    m_len[DAY_COMP]++;
                    break;
                case 'm':
                    if (curcomp != MONTH_COMP)
                    {
                        m_pos[MONTH_COMP] = i;
                        curcomp = MONTH_COMP;
                    }
                    m_len[MONTH_COMP]++;
                    break;
                case 'y':
                    if (curcomp != YEAR_COMP)
                    {
                        m_pos[YEAR_COMP] = i;
                        curcomp = YEAR_COMP;
                    }
                    m_len[YEAR_COMP]++;
            }
        }
        if (m_len[DAY_COMP] != 2 || m_len[MONTH_COMP] != 2 || m_len[YEAR_COMP] != 2 && m_len[YEAR_COMP] != 4)
            return m_val = false;
    }
    return m_val;
}
