/*
 * Ultimate Unprecacher
 * Copyright (c) 2018 Alik Aslanyan <cplusplus256@gmail.com>
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

#include "logger.h"

Logger::Logger(const Logger::OutputType &outputType) :
	outputType_(outputType),
	consoleFunction_([](const std::string&) {}),
	fileFunction_([](const std::string&) { }),
	displayBits_()
{

}

Logger::OutputType Logger::outputType() const noexcept
{
	return outputType_;
}

void Logger::setOutputType(const Logger::OutputType &outputType) noexcept
{
	outputType_ = outputType;
}

std::function<void (const std::string &)> Logger::consoleFunction() const noexcept
{
	return consoleFunction_;
}

void Logger::setConsoleFunction(const std::function<void (const std::string &)> &consoleFunction) noexcept
{
	consoleFunction_ = consoleFunction;
}

std::function<void (const std::string &)> Logger::fileFunction() const noexcept
{
	return fileFunction_;
}

void Logger::setFileFunction(const std::function<void (const std::string &)> &fileFunction) noexcept
{
	fileFunction_ = fileFunction;
}

bool Logger::getCategoryDisplayed(const std::size_t category) const noexcept
{
	return displayBits_[category];
}

bool Logger::getCategoryDisplayed(const Logger::CategoryType &category) const noexcept
{
	return getCategoryDisplayed(enum_index(category));
}

void Logger::setCategoryDisplayed(const Logger::CategoryType &category, bool flag) noexcept
{
	setCategoryDisplayed(enum_index(category), flag);
}

void Logger::setCategoryDisplayed(const std::size_t category, bool flag) noexcept
{
	displayBits_[category] = flag;
}

bool Logger::log(const Logger::CategoryType &category, const std::string &str) const noexcept
{
	if(!displayBits_[enum_index(category)] || outputType_ == Logger::OutputType::NoOutput)
		return false;

	std::string endMessage;

	switch (category)
	{
		case Logger::CategoryType::Debug:
			endMessage="Debug: ";
			break;
		case Logger::CategoryType::Info:
			endMessage="Info: ";
			break;
		case Logger::CategoryType::Warning:
			endMessage="Warning: ";
			break;
		case Logger::CategoryType::Error:
			endMessage="Error: ";
			break;
		case Logger::CategoryType::CriticalError:
			endMessage="Critical error: ";
			break;
	}
	endMessage+=str;

	switch(outputType_)
	{
		case Logger::OutputType::NoOutput: // Just to silence warning
			break;
		case Logger::OutputType::Console:
			consoleFunction_(endMessage);
			break;
		case Logger::OutputType::File:
			fileFunction_(endMessage);
			break;
		case Logger::OutputType::Both:
			consoleFunction_(endMessage);
			fileFunction_(endMessage);
			break;
	}

	return true;
}

bool Logger::debug(const std::string &str) const noexcept
{
	return log(Logger::CategoryType::Debug, str);
}

bool Logger::info(const std::string &str) const noexcept
{
	return log(Logger::CategoryType::Info, str);
}

bool Logger::warning(const std::string &str) const noexcept
{
	return log(Logger::CategoryType::Warning, str);
}

bool Logger::error(const std::string &str) const noexcept
{
	return log(Logger::CategoryType::Error, str);
}

bool Logger::criticalError(const std::string &str) const noexcept
{
	return log(Logger::CategoryType::CriticalError, str);
}
