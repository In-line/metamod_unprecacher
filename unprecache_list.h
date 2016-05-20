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

#ifndef UNPRECACHE_LIST_H
#define UNPRECACHE_LIST_H
using namespace std;
#include <set>
#include "unprecache_list_string.h"
class unprecache_list
{
public:
	unprecache_list();
	virtual ~unprecache_list();
	bool soundExists(const char *szSound);
	bool modelExists(const char *szModel);
	bool spriteExists(const char *szSprite);
	void loadFromFile(char* szListFile);
private:
	std::set<unprecache_list_string> *m_Lists[3];
	std::set<unprecache_list_string>::iterator m_aEndIterators[3];
	char* m_szLastModel;
	bool m_iLastResult;

	bool stringExists(const char *szString,const short iIndex = 0);


};

#endif // UNPRECACHE_LIST_H
