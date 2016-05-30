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

#ifndef UNPRECACHE_LIST_STRING_H
#define UNPRECACHE_LIST_STRING_H
#include <cstring>

class unprecache_list_string
{
private:
	char* m_szString;
	size_t m_iStringLen;
	inline char* CopyString(char *szStr, int iLen)
	{
		char* szReturn = new char[iLen + 1];
		strcpy(szReturn, szStr);
		return szReturn;
	}

public:
	inline unprecache_list_string(char *szStr)
	{
		m_iStringLen = strlen(szStr);
		m_szString = CopyString(szStr, m_iStringLen);
	}

	inline unprecache_list_string(const char *szStr)
	{
		m_iStringLen = strlen(szStr);
		m_szString = CopyString((char*)szStr, m_iStringLen);
	}

	inline unprecache_list_string()
	{
		m_iStringLen = 0;
		m_szString = nullptr;
	}
	unprecache_list_string& operator=(unprecache_list_string&&) = delete;	// Disallow moving

	inline unprecache_list_string( const unprecache_list_string &obj)
	{
		m_szString = CopyString(obj.m_szString, obj.m_iStringLen);
		m_iStringLen = obj.m_iStringLen;
	}

	inline ~unprecache_list_string()
	{
		if(m_szString!=nullptr) delete[] m_szString;
	}

	inline bool operator<(const unprecache_list_string szS2) const
	{
		char* szStr2 = szS2.c_str();
		if(m_szString == nullptr)
			return szStr2==m_szString;

		register char c;
		register char c2;
		for(unsigned int i = 0;;++i)
		{
			c = m_szString[i];
			c2 = szStr2[i];
			if(c < c2)
			{
				return true;
			}
			else if(c > c2 || c=='\0' || c2=='\0')
			{
				return false;
			}
		}
		return false;
	}

	inline char* c_str() const
	{
		return m_szString;
	}
};

#endif // UNPRECACHE_LIST_STRING_H
