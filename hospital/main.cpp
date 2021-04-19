/* ~~~ SAIRAALA ~~~
 *
 * Kuvaus:
 *  Ohjelma toteuttaa Sairaalan. Sairaalalla on potilaita, hoitajia ja hoitojaksoja.
 *  Potilaille voidaan määrätä lääkkeitä. Syötteet voidaan lukea myös syötetiedostosta.
 *
 * Ohjelman kirjoittaja:
 * Nimi: Santeri Laurila (274301)
 * Käyttäjätunnus: laurila2
 * E-Mail: santeri.laurila@tuni.fi
 *
 * */

#include "cli.hh"
#include "hospital.hh"
#include <string>

const std::string PROMPT = "Hosp> ";


int main()
{
    Hospital* hospital = new Hospital();
    Cli cli(hospital, PROMPT);
    while ( cli.exec() ){}

    delete hospital;
    return EXIT_SUCCESS;
}
