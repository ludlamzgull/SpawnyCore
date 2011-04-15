/* Copyright (C) 2010 Easy for TrinityCore <http://trinity-core.ru/>
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* SQL
 REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `Health_mod`, `Mana_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`) VALUES
 (100002, 0, 0, 0, 0, 0, 27153, 0, 0, 0, 'Change', 'Change Service', '', 0, 80, 80, 0, 35, 35, 1, 0.75, 2, 1755, 1755, 0, 1504, 1000, 1500, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 100, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 'npc_change');
 */

#include "ScriptPCH.h"
#include "Creature.h"
#include "Common.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Opcodes.h"
#include "Log.h"
#include "Bag.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "Item.h"
#include "UpdateData.h"
#include "ObjectAccessor.h"

#define MSG_GOSSIP_TEXT_CHANGE           "Exchange 1x Necrotic Rune for 70x Justice Points."
#define MSG_LOW_LEVEL                    "Your level is too low!"
#define MSG_NOT_ENOUGH                   "You don't have any Necrotic Runes left."
#define CHANGE_ITEM               		 22484
#define JUSTICE							 7000

class npc_change: public CreatureScript {
public:
	npc_change() :
		CreatureScript("npc_change") {
	}

	bool OnGossipHello(Player* pPlayer, Creature* pCreature) {
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, MSG_GOSSIP_TEXT_CHANGE, GOSSIP_SENDER_MAIN, 1);
		pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pCreature->GetGUID());
		return true;
	}

	bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 uiSender,
			uint32 uiAction) {
		if (uiSender != GOSSIP_SENDER_MAIN)
			return false;

		pPlayer->PlayerTalkClass->ClearMenus();

		switch (uiAction) {
		case 1:
			if (pPlayer->getLevel() < 70) {
				pCreature->MonsterWhisper(MSG_LOW_LEVEL, pPlayer->GetGUID());
				pPlayer->CLOSE_GOSSIP_MENU();
				break;
			}
		case 2:
			if (pPlayer->GetItemByEntry(CHANGE_ITEM)) {
				//				for (uint8 bag = INVENTORY_SLOT_BAG_START; bag< INVENTORY_SLOT_BAG_END; ++bag) {
				//					const Bag* const pBag = (Bag*) pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, bag);
				//					if (pBag) {
				//						for (uint8 slot = 0; slot < pBag->GetBagSize(); ++slot) {
				//							Item* const pItem = pPlayer->GetItemByPos(bag, slot);
				//							if (pItem==CHANGE_ITEM){
				//								uint32 count = 1;
				//							pPlayer->DestroyItemCount(pItem, count, true); // remove item from inventory
				//						}
				//					}
				//				}
				//			}
				uint32 count = 1;
				pPlayer->DestroyItemCount(CHANGE_ITEM, count, true, true);
				pPlayer->ModifyCurrency(395, (JUSTICE));
				pCreature->MonsterWhisper(
						"Congratz, you've been rewarded with 70 Justice Points.",
						pPlayer->GetGUID());
				pPlayer->CLOSE_GOSSIP_MENU();
				break;
			} else {
				pCreature->MonsterWhisper(MSG_NOT_ENOUGH, pPlayer->GetGUID());
			}
		default:
			pPlayer->CLOSE_GOSSIP_MENU();
		}
		return true;
	}
};

void AddSC_npc_change() {
	new npc_change;
}
