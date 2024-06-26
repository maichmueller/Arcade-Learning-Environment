/* *****************************************************************************
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 * *****************************************************************************
 * A.L.E (Arcade Learning Environment)
 * Copyright (c) 2009-2013 by Yavar Naddaf, Joel Veness, Marc G. Bellemare and
 *   the Reinforcement Learning and Artificial Intelligence Laboratory
 * Released under the GNU General Public License; see License.txt for details.
 *
 * Based on: Stella  --  "An Atari 2600 VCS Emulator"
 * Copyright (c) 1995-2007 by Bradford W. Mott and the Stella team
 *
 * *****************************************************************************
 */

#include "ale/games/supported/Pitfall2.hpp"

#include "ale/games/RomUtils.hpp"

namespace ale {
using namespace stella;

Pitfall2Settings::Pitfall2Settings() { reset(); }

/* create a new instance of the rom */
RomSettings* Pitfall2Settings::clone() const {
  return new Pitfall2Settings(*this);
}

/* process the latest information from ALE */
void Pitfall2Settings::step(const System& system) {
  // update the reward
  int score = getDecimalScore(0xC9, 0xC8, 0xC7, &system);
  // We don't currently return a negative reward when the player gets
  // teleported back to the last checkpoint whilst the score is 0.
  int reward = score - m_score;
  m_reward = reward;
  // Pitfall 2 has no score wrapping. Maximum score is clamped to 199000.
  m_score = score;

  // Pitfall 2 has no lives so the player cannot die, even when the score
  // is zero.
  m_lives = 1;

  // There is an end state when the player completes the adventure but
  // not currently sure how to detect this without deeper investigation.
  // Instead test for maximum achievable score as a minimum.
  m_terminal = m_score == 199000;
}

/* is end of game */
bool Pitfall2Settings::isTerminal() const { return m_terminal; };

/* get the most recently observed reward */
reward_t Pitfall2Settings::getReward() const { return m_reward; }

/* is an action part of the minimal set? */
bool Pitfall2Settings::isMinimal(const Action& a) const {
  switch (a) {
    case PLAYER_A_NOOP:
    case PLAYER_A_FIRE:
    case PLAYER_A_UP:
    case PLAYER_A_RIGHT:
    case PLAYER_A_LEFT:
    case PLAYER_A_DOWN:
    case PLAYER_A_UPRIGHT:
    case PLAYER_A_UPLEFT:
    case PLAYER_A_DOWNRIGHT:
    case PLAYER_A_DOWNLEFT:
    case PLAYER_A_UPFIRE:
    case PLAYER_A_RIGHTFIRE:
    case PLAYER_A_LEFTFIRE:
    case PLAYER_A_DOWNFIRE:
    case PLAYER_A_UPRIGHTFIRE:
    case PLAYER_A_UPLEFTFIRE:
    case PLAYER_A_DOWNRIGHTFIRE:
    case PLAYER_A_DOWNLEFTFIRE:
      return true;
    default:
      return false;
  }
}

/* reset the state of the game */
void Pitfall2Settings::reset() {
  m_reward = 0;
  m_score = 4000;
  m_terminal = false;
  m_lives = 1;
}

/* saves the state of the rom settings */
void Pitfall2Settings::saveState(Serializer& ser) {
  ser.putInt(m_reward);
  ser.putInt(m_score);
  ser.putBool(m_terminal);
  ser.putInt(m_lives);
}

// loads the state of the rom settings
void Pitfall2Settings::loadState(Deserializer& ser) {
  m_reward = ser.getInt();
  m_score = ser.getInt();
  m_terminal = ser.getBool();
  m_lives = ser.getInt();
}

ActionVect Pitfall2Settings::getStartingActions() {
  return {PLAYER_A_UP};
}

}  // namespace ale
