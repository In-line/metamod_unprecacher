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

#include "unprecache_list_string.h"

unprecache_list_string::unprecache_list_string(char *szStr)
{
	m_szString = szStr;
}

unprecache_list_string::unprecache_list_string(const char *szStr)
{
	m_szString = (char*) szStr;
}

unprecache_list_string::unprecache_list_string()
{
	m_szString = nullptr;
}



/*unprecache_list_string::operator char *() const
{
	return m_szString;
}
*/

bool unprecache_list_string::operator<(const unprecache_list_string szS2) const
{
	register char* szStr = m_szString;
	register char* szStr2 = szS2.c_str();

	for(register int i = 0;;++i)
	{
		if(szStr[i] < szStr2[i])
		{
			return true;
		}
		else if(szStr[i] > szStr2[i])
		{
			return false;
		}
		else if(szStr[i]=='\0' || szStr2[i]=='\0')
		{
			return false;
		}
	}
	return false;
}

char* unprecache_list_string::c_str() const
{
	return m_szString;
}
