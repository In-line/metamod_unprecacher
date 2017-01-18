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

#ifndef LOGGER_H
#define LOGGER_H

#include <memory>
#include <string>
#include <functional>
#include <bitset>

class Logger
{
public:
	enum class OutputType
	{
		NoOutput,
		Console,
		File,
		Both,
		LastElement = Both,
	};

	enum class CategoryType
	{
		Debug,
		Info,
		Warning,
		Error,
		CriticalError,
		LastElement = CriticalError,
	};

private:
	OutputType outputType_;
	std::function<void (const std::string &)> consoleFunction_;
	std::function<void (const std::string &)> fileFunction_;
	std::bitset<5> displayBits_;

	template<class T>
	constexpr std::size_t enum_index(const T& value) const noexcept
	{
		static_assert(std::is_enum<T>::value, "Enum is required");
		return static_cast<std::size_t>(value);
	}
public:
	Logger(const OutputType &outputType = OutputType::NoOutput);

	bool log(const CategoryType &category, const std::string &str) const noexcept;

	bool debug(const std::string &str) const noexcept;
	bool info(const std::string &str) const noexcept;
	bool warning(const std::string &str) const noexcept;
	bool error(const std::string &str) const noexcept;
	bool criticalError(const std::string &str) const noexcept;

	OutputType outputType() const noexcept;
	void setOutputType(const OutputType &outputType) noexcept;

	std::function<void (const std::string &)> consoleFunction() const noexcept;
	void setConsoleFunction(const std::function<void (const std::string &)> &consoleFunction) noexcept;

	std::function<void (const std::string &)> fileFunction() const noexcept;
	void setFileFunction(const std::function<void (const std::string &)> &fileFunction) noexcept;

	bool getCategoryDisplayed(const std::size_t category) const noexcept;
	bool getCategoryDisplayed(const CategoryType &category) const noexcept;

	void setCategoryDisplayed(const std::size_t category, bool flag) noexcept;
	void setCategoryDisplayed(const CategoryType &category, bool flag) noexcept;
};

#endif // LOGGER_H
