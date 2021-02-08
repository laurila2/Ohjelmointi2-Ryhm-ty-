#include "player.hh"
#include <card.hh>
#include <iostream>

// Lisää luokan toteutus tähän.
// Kuhunkin julkiseen metodiin pitäisi riittää 1-2 koodiriviä.

// Add class implementation here.
// It should be enough to write only 1-2 code lines in each public method.

Player::Player(const std::string &name):
    _NAME(name),
    _PAIRS(0)
{

}

std::string Player::get_name() const
{
    return _NAME;
}

unsigned int Player::number_of_pairs() const
{
    return _PAIRS;
}

void Player::add_card(Card &card)
{
    ++_PAIRS;
    card.remove_from_game_board();
}

void Player::print() const
{
    std::cout << "*** " << _NAME << " has " << _PAIRS << " pair(s)" << std::endl;
}
