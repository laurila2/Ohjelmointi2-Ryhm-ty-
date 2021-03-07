/* RASSE
 *
 * Kuvaus:
 *  Ohjelma toteuttaa muistipelin. Pelissä on vaihteleva määrä kortteja ja
 * pelaajia. Pelin alussa käyttäjältä kysytään myös siemenluku, koska kortit
 * arvotaan satunnaisesti pelilaudalle.
 *  Joka kierroksella vuorossa oleva pelaaja antaa kahden kortin
 * koordinaatit (yhteensä neljä lukua), minkä jälkeen kyseiset kortit
 * käännetään näkyviin ja kerrotaan, ovatko ne parit vai ei.
 * Jos pelaaja sai parit, kortit poistetaan pelilaudalta, pelaajan
 * pistesaldoa kasvatetaan, ja hän saa uuden vuoron. Jos pelaaja ei saanut
 * pareja, kortit käännetään takaisin piiloon, ja vuoro siirtyy seuraavalle
 * pelaajalle.
 *  Ohjelma tarkistaa pelaajan antamat koordinaatit. Koordinaattien pitää
 * olla sellaiset, että niiden määrä kortti löytyy pelilaudalta.
 *  Muutosten jälkeen pelilauta tulostetaan aina uudelleen. Kortit kuvataan
 * kirjaimina alkaen A:sta niin pitkälle, kuin kortteja on. Kun pelilauta
 * tulostetaan, näkyvissä oleva kortti kuvataan kyseisellä kirjaimella.
 * Piiloon käännettyä korttia kuvaa risuaita (#), ja laudalta poistetun
 * kortin kohdalle tulostetaan piste.
 *  Peli päättyy, kun kaikki parit on löydetty, ja pelilauta on tyhjä.
 * Tällöin kerrotaan, kuka tai ketkä voittivat eli saivat eniten pareja.
 *
 * Ohjelman kirjoittaja
 * Nimet: Santeri Laurila (274301)
 * Käyttäjätunnus: laurila2
 * E-Mail: santeri.laurila@tuni.fi
 *
 * Huomioita ohjelmasta ja sen toteutuksesta:
 *
 * */

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> split(const std::string &s,
                               const char delimiter,
                               bool ignore_empty = false)
{
    std::vector<std::string> result;
    std::string tmp = s;

    while (tmp.find(delimiter) != std::string::npos)
    {
        std::string new_part = tmp.substr(0, tmp.find(delimiter));
        tmp = tmp.substr(tmp.find(delimiter) + 1, tmp.size());
        if (not(ignore_empty and new_part.empty()))
        {
            result.push_back(new_part);
        }
    }
    if (not(ignore_empty and tmp.empty()))
    {
        result.push_back(tmp);
    }
    return result;
}

// Prints a RASSE
void print_rasse()
{
    std::cout <<
                 "=====//==================//===\n"
                 "  __<<__________________<<__   \n"
                 " | ____ ____ ____ ____ ____ |  \n"
                 " | |  | |  | |  | |  | |  | |  \n"
                 " |_|__|_|__|_|__|_|__|_|__|_|  \n"
                 ".|                  RASSE   |. \n"
                 ":|__________________________|: \n"
                 "___(o)(o)___(o)(o)___(o)(o)____\n"
                 "-------------------------------" << std::endl;
}

// Asks input file
bool ask_file()
{
    std::string filename;
    std::cout << "Give a name for input file: ";
    std::cin >> filename;

    std::string line;

    // Read input file
    std::ifstream file_in;
    file_in.open(filename);
    if (!file_in.is_open())
    {
        std::cout << "Error: File could not be read." << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

// Short and sweet main.
int main()
{
    print_rasse();
    ask_file();

    return EXIT_SUCCESS;
}
