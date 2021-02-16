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

#ifndef PLAYER_HH
#define PLAYER_HH

#include "card.hh"
#include <string>

class Player
{
public:
    // Rakentaja: luo annetun nimisen pelaajan.
    Player(const std::string& name);

    // Palauttaa pelaajan nimen.
    std::string get_name() const;

    // Palauttaa pelaajan tähän asti keräämien parien määrän.
    unsigned int number_of_pairs();

    // Siirtää annetun kortin pelilaudalta pelaajalle,
    // eli lisää kortin pelaajan keräämiin kortteihin
    // ja poistaa sen pelilaudalta.
    void add_card(Card& card);

    // Tulostaa pelaajan tilanteen: nimen ja tähän asti kerättyjen parien määrän.
    void print();

private:
    // Lisää tarvittavat attribuutit tähän
    const std::string NAME_;
    int pairs_;
};

#endif // PLAYER_HH
