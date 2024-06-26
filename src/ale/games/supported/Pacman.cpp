/* *****************************************************************************
 *
 * This wrapper was authored by Stig Petersen, March 2014
 *
 * Xitari
 *
 * Copyright 2014 Google Inc.
 *
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
#include "ale/games/supported/Pacman.hpp"

#include "ale/games/RomUtils.hpp"

namespace ale {
using namespace stella;

PacmanSettings::PacmanSettings() { reset(); }

/* create a new instance of the rom */
RomSettings* PacmanSettings::clone() const {
  return new PacmanSettings(*this);
}

/* process the latest information from ALE */
void PacmanSettings::step(const System& system) {
  reward_t score = getDecimalScore(0xCC, 0xCE, 0xD0, &system);

  // update the reward
  m_reward = score - m_score;
  m_score = score;

  // update terminal status
  m_lives = readRam(&system, 0x98) + 1;
  int animationCounter = readRam(&system, 0xE4);
  m_terminal = m_lives == 1 && animationCounter == 0x3F;
}


/* is end of game */
bool PacmanSettings::isTerminal() const { return m_terminal; }

/* get the most recently observed reward */
reward_t PacmanSettings::getReward() const { return m_reward; }

/* is an action part of the minimal set? */
bool PacmanSettings::isMinimal(const Action& a) const {
  switch (a) {
    case PLAYER_A_NOOP:
    case PLAYER_A_UP:
    case PLAYER_A_RIGHT:
    case PLAYER_A_LEFT:
    case PLAYER_A_DOWN:
      return true;
    default:
      return false;
  }
}


/* reset the state of the game */
void PacmanSettings::reset() {
  m_reward   = 0;
  m_score    = 0;
  m_terminal = false;
  m_lives    = 4;
}

/* saves the state of the rom settings */
void PacmanSettings::saveState(Serializer& ser) {
  ser.putInt(m_reward);
  ser.putInt(m_score);
  ser.putBool(m_terminal);
  ser.putInt(m_lives);
}

// loads the state of the rom settings
void PacmanSettings::loadState(Deserializer& ser) {
  m_reward = ser.getInt();
  m_score = ser.getInt();
  m_terminal = ser.getBool();
  m_lives = ser.getInt();
}

// According to https://atariage.com/manual_html_page.php?SoftwareLabelID=342
// the left difficulty switch sets how long the power pill is active for.
DifficultyVect PacmanSettings::getAvailableDifficulties() {
  return {0, 1};
}

// According to https://atariage.com/manual_html_page.php?SoftwareLabelID=342
// there are 8 variations of the game with both one and two-players for each.
// Game modes alter the speed of both Pac-man and the ghosts.
ModeVect PacmanSettings::getAvailableModes() {
  return {0, 1, 2, 3, 4, 5, 6, 7};
}

void PacmanSettings::setMode(
    game_mode_t m, System& system,
    std::unique_ptr<StellaEnvironmentWrapper> environment) {
  if (m < 8) {
    while (true) {
      // Read the mode we are currently in along with number of players.
      unsigned char mode = readRam(&system, 0xcc) - 1;
      bool one_player = readRam(&system, 0xe6) == 0;

      // Press select until the correct mode is reached for single player only.
      if (mode == m && one_player) {
        break;
      }
      environment->pressSelect(2);
    }

    // Reset the environment to apply changes.
    environment->softReset();
  } else {
    throw std::runtime_error("This game mode is not supported.");
  }
}

}  // namespace ale
