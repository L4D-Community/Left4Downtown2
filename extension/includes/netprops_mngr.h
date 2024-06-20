/**
 * vim: set ts=4 :
 * =============================================================================
 * Left 4 Downtown SourceMod Extension
 * Copyright (C) 2021-2024 A1mDev (A1m`)
 * =============================================================================
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 3.0, as published by the
 * Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * As a special exception, AlliedModders LLC gives you permission to link the
 * code of this program (as well as its derivative works) to "Half-Life 2," the
 * "Source Engine," the "SourcePawn JIT," and any Game MODs that run on software
 * by the Valve Corporation.  You must obey the GNU General Public License in
 * all respects for all other code used.  Additionally, AlliedModders LLC grants
 * this exception to all derivative works.  AlliedModders LLC defines further
 * exceptions, found in LICENSE.txt (as of this writing, version JULY-31-2007),
 * or <http://www.sourcemod.net/license.php>.
 *
 * Version: $Id$
 */

#ifndef _INCLUDE_L4D2_NETPROPS_MNGR_H_
#define _INCLUDE_L4D2_NETPROPS_MNGR_H_

#include <map>
#include <string>
#include <memory>

class CNetPropMngr
{
public:
    CNetPropMngr(const char* pzsClassName, const char* pzsNetPropName)
    {
        m_iOffset = 0;
        
        m_sClassName = std::string(pzsClassName);
        m_sNetpropName = std::string(pzsNetPropName);
        std::string sKeyName = std::string() + pzsClassName + "::" + pzsNetPropName;

        CNetPropMngr::m_iInitOffsets[sKeyName] = std::make_unique<CNetPropMngr*>(this);
    }

    static bool Init(char* error, size_t maxlength)
    {
        for (auto& element : CNetPropMngr::m_iInitOffsets) {
            CNetPropMngr* netpropRef = *element.second.get();
            
            std::string sKeyName = std::string() + netpropRef->GetClassName() + "::" + netpropRef->GetNetPropName();
            if (netpropRef == NULL) {
                snprintf(error, maxlength, "Unable to find netprop '%s'", sKeyName.c_str());
                return false;
            }

            sm_sendprop_info_t info;
            if (!gamehelpers->FindSendPropInfo(netpropRef->GetClassName(), netpropRef->GetNetPropName(), &info)) {
                snprintf(error, maxlength, "Unable to find netprop '%s'", sKeyName.c_str());
                return false;
            }

            netpropRef->m_iOffset = info.actual_offset;
            if (netpropRef->GetOffset() <= 0) {
                snprintf(error, maxlength, "Unable to find netprop '%s'", sKeyName.c_str());

                return false;
            }

            Msg("Received offset '%d' for netprop '%s'""\n", netpropRef->GetOffset(), sKeyName.c_str());
        }

        CNetPropMngr::m_iInitOffsets.clear();
        return true;
    }

    inline uint16_t GetOffset()
    {
        return m_iOffset;
    }

    inline const char* GetClassName()
    {
        return m_sClassName.c_str();
    }

    inline const char* GetNetPropName()
    {
        return m_sNetpropName.c_str();
    }

private:
    static inline std::map<std::string, std::unique_ptr<CNetPropMngr*>> m_iInitOffsets;

private:
    uint16_t m_iOffset;

    std::string m_sClassName;
    std::string m_sNetpropName;
};

#endif // _INCLUDE_L4D2_NETPROPS_MNGR_H_