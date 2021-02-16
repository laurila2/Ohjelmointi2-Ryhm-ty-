/* Luokka: Player
 * --------------
 * Kuvaa yhtä pelaajaa muistipelissä.
 *
 * COMP.CS.110 K2021
 * 
 * Ohjelman kirjoittaja
 * Nimet: Santeri Laurila (274301)
 * Käyttäjätunnus: laurila2
 * E-Mail: santeri.laurila@tuni.fi
 *
 */

#include "player.hh"
#include "card.hh"

#include <iostream>

Player::Player(const std::string &name) : NAME_(name), pairs_(0) {}

std::string Player::get_name() const
{
    return NAME_;
}

unsigned int Player::number_of_pairs()
{
    return pairs_;
}

void Player::add_card(Card &card)
{
    ++pairs_;
    card.remove_from_game_board();
}

void Player::print()
{
    std::cout << "*** " << NAME_ << " has " << pairs_ << " pair(s)."
              << std::endl;
}
