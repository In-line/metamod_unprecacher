/*
 * Ultimate Unprecacher
 * Copyright (c) 2016 Alik Aslanyan <cplusplus256@gmail.com>
 *
 *
 *
 *    This program is free software; you can redistribute it and/or modify it
 *    under the terms of the GNU General Public License as published by the
 *    Free Software Foundation; either version 2 of the License, or (at
 *    your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful, but
 *    WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software Foundation,
 *    Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *    In addition, as a special exception, the author gives permission to
 *    link the code of this program with the Half-Life Game Engine ("HL
 *    Engine") and Modified Game Libraries ("MODs") developed by Valve,
 *    L.L.C ("Valve").  You must obey the GNU General Public License in all
 *    respects for all of the code used other than the HL Engine and MODs
 *    from Valve.  If you modify this file, you may extend this exception
 *    to your version of the file, but you are not obligated to do so.  If
 *    you do not wish to do so, delete this exception statement from your
 *    version.
 *
 */

#include "string_utils.h"

#include <algorithm>
#include <iomanip>
#include <sstream>
std::vector<std::string> parseString(const std::string &inputString, bool removeQuotes/*=true*/, bool anySpaceCharacters/*=true*/) noexcept
{
	std::vector<std::string> returnVector;


	auto start = inputString.begin();
	auto end = inputString.end();

	enum class PARSING_STATUS
	{
		PARSING_PAUSE = 0,
		PARSING_STARTED = 1,
		PARSING_QUOTE = 2,
	};
	PARSING_STATUS status = PARSING_STATUS::PARSING_PAUSE;
	std::string insertString;

	for(auto index = start; index!=end;++index)
	{
		switch (*index)
		{
			case '"': case '\'':
			{
				if(!removeQuotes)
					insertString+=*index;

				if(status == PARSING_STATUS::PARSING_QUOTE)
				{
					returnVector.push_back(insertString);
					insertString.clear();
					status = PARSING_STATUS::PARSING_PAUSE;
				}
				else
					status = PARSING_STATUS::PARSING_QUOTE;

				break;
			}
			default:
			{
				if( (anySpaceCharacters && std::isspace(*index)) ||
						(!anySpaceCharacters && *index == ' '))
				{
					if(status == PARSING_STATUS::PARSING_QUOTE)
						insertString+=*index;
					else if(status == PARSING_STATUS::PARSING_STARTED)
					{
						returnVector.push_back(insertString);
						insertString.clear();
						status = PARSING_STATUS::PARSING_PAUSE;
					}
				}
				else
				{
					insertString+=*index;
					if(status != PARSING_STATUS::PARSING_QUOTE)
						status = PARSING_STATUS::PARSING_STARTED;
				}

				break;
			}
		}
	}
	if(!insertString.empty())
		returnVector.push_back(insertString);

	if(returnVector.empty()) // If result is empty, as result send empty string -> {""}
		returnVector.push_back("");

	return returnVector;
}

bool is_string_whitespaces(const std::string &input) noexcept
{
	if(input.empty())
		return false;

	for(auto &c : input)
	{
		if(!isspace(c))
			return false;
	}
	return true;
}

bool starts_with(const std::string &input,const std::string &prefix) noexcept
{
	if(input.empty() || prefix.empty())
		return false;

	auto inputEnd = input.cend();
	auto prefixEnd = prefix.cend();

	for(auto inputIter = input.cbegin(), prefixIter = prefix.cbegin()
			;; ++inputIter, ++prefixIter)
	{
		if(prefixIter == prefixEnd) // If prefix ended and all checks pased it is Okey
		{
			return true;
		}

		if(
			 (inputIter == inputEnd) ||// Check for end
			 (*inputIter != *prefixIter) // Safe compare after check
			 )
		{
			return false;
		}

	}
	//return true;
}

std::string removeComments(const std::string &str) noexcept
{
	return str.substr(0, str.find("//"));
}

std::string trim(const std::string &str) noexcept
{
	std::string::size_type firstNonWhiteSpace = 0;
	for(auto &c : str)
	{
		if(std::isspace(c))
			++firstNonWhiteSpace;
		else
			break;
	}

	std::string::size_type lastNonWhiteSpace = str.size();

	{
		auto crend = str.crend();
		for(auto it = str.crbegin(); it != crend && std::isspace(*it); ++it)
		{
			--lastNonWhiteSpace;
		}
	}

	return str.substr(firstNonWhiteSpace, lastNonWhiteSpace - firstNonWhiteSpace);
}

std::string str_put_time(const std::tm* tmb, const char* fmt ) noexcept
{
	std::stringstream ss;
	ss << std::put_time(tmb, fmt);
	return ss.str();
}
