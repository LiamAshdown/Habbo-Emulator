/*
* Liam Ashdown
* Copyright (C) 2019
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _HABBO_FUSE_RIGHTS_h
#define _HABBO_FUSE_RIGHTS_h
#include "Common/SharedDefines.h"
#endif /* _HABBO_FUSE_RIGHTS_h */

namespace SteerStone
{
    class Habbo;

    /// Holds Habbo Fuse rights
    class FuseRights
    {
    public:
        /// Constructor
        /// @p_Habbo : Fuse rights belonging to habbo
        FuseRights(Habbo* p_Habbo);

        /// Deconstructor
        ~FuseRights();

    public:
        /// LoadFuseRights
        /// Load User Fuse Rights from database
        void LoadFuseRights();

        /// SendFuseRights
        /// Send User fuse rights to client
        void SendFuseRights();

    private:
        std::vector<std::string> m_FuseRights;
        Habbo* m_Habbo;

    };
} ///< NAMESPACE STEERSTONE