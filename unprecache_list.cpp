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

#include "unprecache_list.h"
#include "cstring_utils.h"

#include "sdk_util_custom.h"

unprecache_list::unprecache_list(char* szListFile)
{
    m_Lists[0] = new std::set<unprecache_list_string>();
    m_Lists[1] = new std::set<unprecache_list_string>();
    m_Lists[2] = new std::set<unprecache_list_string>();
    m_szLastModel = nullptr;
    loadFromFile(szListFile);

}

unprecache_list::~unprecache_list()
{
    delete m_Lists[0];
    delete m_Lists[1];
    delete m_Lists[2];
}
bool unprecache_list::soundExists(const char *szSound)
{
    return stringExists(szSound, 1);
}

bool unprecache_list::modelExists(const char *szModel)
{
    return stringExists(szModel, 0);
}

bool unprecache_list::spriteExists(const char *szSprite)
{
    return stringExists(szSprite, 2);
}


bool inline unprecache_list::stringExists(const char *szString, short iIndex)
{
    if(m_szLastModel!=nullptr && strcmp(m_szLastModel, szString) == 0)
    {
        return m_iLastResult;
    }
    m_iLastResult = m_Lists[iIndex]->find(szString)!=m_EndIterators[iIndex];
    m_szLastModel = (char*) szString;
    return m_iLastResult;
}

void unprecache_list::loadFromFile(char* szListFile)
{
    FILE* hFile = fopen(szListFile, "r");

    if(hFile == NULL)
    {
        UTIL_LogError("[Error] Cannot open file %s", szListFile);
        return;
    }
    m_Lists[0]->clear();
    m_Lists[1]->clear();
    m_Lists[2]->clear();
    char szBuffer[256];

    while(!feof(hFile))
    {
        fgets(szBuffer,sizeof(szBuffer),hFile);
        UTIL_RemoveComments(szBuffer);
        trim(szBuffer);
        if(szBuffer[0]!=0)
        {
            short iIndex = 0;
            int iReadLen = strlen(szBuffer);
            if(str_nends_with(".mdl", szBuffer, iReadLen))
            {
                    if(starts_with(szBuffer,"models/"))
                    {
                        str_remove_first_chars(szBuffer, 7);
                    }
                    iIndex = 0;
            }
            else if(str_nends_with(".wav", szBuffer, iReadLen))
            {
                if(starts_with(szBuffer,"sound/"))
                {
                    str_remove_first_chars(szBuffer, 6);
                }
                iIndex = 1;
            }
            else if(str_nends_with(".spr", szBuffer, iReadLen))
            {
                if(starts_with(szBuffer,"sprites/"))
                {
                    str_remove_first_chars(szBuffer, 8);
                }
                iIndex = 2;
            }
            else
            {
                UTIL_LogError("[Error] Unrecognized line postfix");
            }
            m_Lists[iIndex]->insert(str_copy(szBuffer));
        }
    }
    fclose(hFile);
    if(m_Lists[0]->empty() && m_Lists[1]->empty() && m_Lists[2]->empty())
    {
        UTIL_LogError("[Error] Not loaded anything!");
    }
    m_EndIterators[0] = m_Lists[0]->end();
    m_EndIterators[1] = m_Lists[1]->end();
    m_EndIterators[2] = m_Lists[2]->end();
}


