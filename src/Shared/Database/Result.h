/*
* Liam Ashdown
* Copyright(C) 2019
*
* This program is free software : you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
*(at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
*but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.If not, see < http://www.gnu.org/licenses/>.
*/

#ifndef _DATABASE_RESULT_h
#define _DATABASE_RESULT_h
#include "../Common/SharedDefines.h"
#endif /* _DATABASE_RESULT_h */

namespace SteerStone
{
    /// Result class which retrieves ResultSet from Database if successfull query
    class Result
    {
    public:
        friend class QueryDatabase;

    public:
        /// Constructor
        Result() {}
        /// Deconstructor
        ~Result() {}

    public:
        /// Returns data type from database
        std::string GetString(uint8 p_I) const { return m_ResultSet->getString(p_I).c_str(); }
        uint16 GetUint8(uint8 p_I)       const { return (uint8)m_ResultSet->getUInt(p_I);    }
        uint16 GetUint16(uint8 p_I)      const { return (uint16)m_ResultSet->getUInt(p_I);   }
        int32 GetInt32(uint8 p_I)        const { return (int32)m_ResultSet->getInt(p_I);     }
        int64 GetInt64(uint8 p_I)        const { return (int64)m_ResultSet->getInt64(p_I);   }
        uint32 GetUint32(uint8 p_I)      const { return (uint32)m_ResultSet->getUInt(p_I);   }
        uint32 GetUint64(uint8 p_I)      const { return (uint64)m_ResultSet->getUInt64(p_I); }
        bool GetBool(uint8 p_I)          const { return (bool)m_ResultSet->getBoolean(p_I);  }
        double GetDouble(uint8 p_I)      const { return (double)m_ResultSet->getDouble(p_I); }
        float GetFloat(uint8 p_I)        const { return (float)m_ResultSet->getDouble(p_I);  }
        /// Get Info about database query
        bool GetNextResult()             const { return m_ResultSet->next();                 }
        std::size_t GetRowCount()        const { return m_ResultSet->rowsCount();            }

    public:
        std::unique_ptr<sql::ResultSet> m_ResultSet;                                                       ///< ResultSet from QueryResult class                    
    };
} ///< NAMESPACE STEERSTONE
