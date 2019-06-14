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

#ifndef _CATALOGUE_ITEM_DEFINITIONS_H
#define _CATALOGUE_ITEM_DEFINITIONS_H
#include "Common/SharedDefines.h"
#endif /* _CATALOGUE_ITEM_DEFINITIONS_H */

namespace SteerStone
{
    namespace ItemBehaviour
    {
        static const std::string WallItem = "wall_item";
    }; ///< NAMESPACE ITEMBEHAVIOUR

    class ItemDefinition
    {
    public:
        friend class ItemManager;;

    public:
        /// Constructor
        ItemDefinition();

        /// Deconstructor
        ~ItemDefinition();

    public:
        /// HasBehaviour
        /// Returns whether this item has a specific behaviour
        /// @p_Behaviour : Behaviour we are searching for
        bool HasBehaviour(std::string const p_Behaviour);

        /// GetIcon
        /// @p_SpriteId : Id we are searching for
        std::string GetIcon(uint32 const p_SpriteId);

    public:
        uint32 GetId()                  const { return m_Id;            }
        std::string GetSprite()         const { return m_Sprite;        }
        uint32 GetSpriteId()            const { return m_SpriteId;      }
        std::string GetName()           const { return m_Name;          }
        std::string GetDescription()    const { return m_Description;   }
        std::string GetColour()         const { return m_Colour;        }
        uint32 GetLength()              const { return m_Length;        }
        uint32 GetWidth()               const { return m_Width;         }
        double GetTopHeight()           const { return m_TopHeight;     }
        std::string GetMaxStatus()      const { return m_MaxStatus;     }
        std::string GetBehaviour()      const { return m_Behaviour;     }
        std::string GetInteractor()     const { return m_Interactor;    }
        bool IsTradable()               const { return m_IsTradable;    }
        bool IsRecyclable()             const { return m_IsRecyclable;  }
        std::string GetDrinkIds()       const { return m_DrinkIds;      }

    private:
        uint32 m_Id;
        std::string m_Sprite;
        uint32 m_SpriteId;
        std::string m_Name;
        std::string m_Description;
        std::string m_Colour;
        uint32 m_Length;
        uint32 m_Width;
        double m_TopHeight;
        std::string m_MaxStatus;
        std::string m_Behaviour;
        std::string m_Interactor;
        bool m_IsTradable;
        bool m_IsRecyclable;
        std::string m_DrinkIds;
    };
} ///< NAMESPACE STEERSTONE