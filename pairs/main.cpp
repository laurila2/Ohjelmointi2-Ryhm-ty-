/* Muistipeli
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

#include <card.hh>
#include <cstdlib>
#include <iostream>
#include <player.hh>
#include <random>
#include <string>
#include <vector>

using namespace std;

const string INPUT_AMOUNT_OF_CARDS = "Enter the amount of cards (an even number): ";
const string INPUT_SEED = "Enter a seed value: ";
const string INPUT_AMOUNT_OF_PLAYERS = "Enter the amount of players (one or more): ";
const string INPUT_CARDS = "Enter two cards (x1, y1, x2, y2), or q to quit: ";
const string INVALID_CARD = "Invalid card.";
const string FOUND = "Pairs found.";
const string NOT_FOUND = "Pairs not found.";
const string GIVING_UP = "Why on earth you are giving up the game?";
const string GAME_OVER = "Game over!";

using Game_row_type = vector<Card>;
using Game_board_type = vector<vector<Card>>;

// Muuntaa annetun numeerisen merkkijonon vastaavaksi kokonaisluvuksi
// (kutsumalla stoi-funktiota).
// Jos annettu merkkijono ei ole numeerinen, palauttaa nollan
// (mikä johtaa laittomaan korttiin myöhemmin).
unsigned int stoi_with_check(string &str)
{
    bool is_numeric = true;
    for (unsigned int i = 0; i < str.length(); ++i)
    {
        if (not isdigit(str.at(i)))
        {
            is_numeric = false;
            break;
        }
    }
    if (is_numeric)
    {
        return stoi(str);
    }
    else
    {
        return 0;
    }
}

// Täyttää pelilaudan (kooltaan rows * columns) tyhjillä korteilla.
void init_with_empties(Game_board_type& g_board, unsigned int rows, unsigned int columns)
{
    g_board.clear();
    Game_row_type row;
    for(unsigned int i = 0; i < columns; ++i)
    {
        Card card;
        row.push_back(card);
    }
    for(unsigned int i = 0; i < rows; ++i)
    {
        g_board.push_back(row);
    }
}

// Etsii seuraavan tyhjän kohdan pelilaudalta (g_board) aloittamalla
// annetusta kohdasta start ja jatkamalla tarvittaessa alusta.
// (Kutsutaan vain funktiosta init_with_cards.)
unsigned int next_free(Game_board_type& g_board, unsigned int start)
{
    // Selvitetään annetun pelilaudan rivien ja sarakkeiden määrät
    unsigned int rows = g_board.size();
    unsigned int columns = g_board.at(0).size();

    // Aloitetaan annetusta arvosta
    for(unsigned int i = start; i < rows * columns; ++i)
    {
        if(g_board.at(i / columns).at(i % columns).get_visibility() == EMPTY) // vaihdettu
        {
            return i;
        }
    }
    // Jatketaan alusta
    for(unsigned int i = 0; i < start; ++i)
    {
        if(g_board.at(i / columns).at(i % columns).get_visibility() == EMPTY)
        {
            return i;
        }
    }

    // Tänne ei pitäisi koskaan päätyä
    std::cout << "No more empty spaces" << std::endl;
    return rows * columns - 1;
}

// Alustaa annetun pelilaudan (g_board) satunnaisesti arvotuilla korteilla
// annetun siemenarvon (seed) perusteella.
void init_with_cards(Game_board_type& g_board, int seed)
{
    // Selvitetään annetun pelilaudan rivien ja sarakkeiden määrät
    unsigned int rows = g_board.size();
    unsigned int columns = g_board.at(0).size();

    // Arvotaan täytettävä sijainti
    std::default_random_engine randomEng(seed);
    std::uniform_int_distribution<int> distr(0, rows * columns - 1);
    // Hylätään ensimmäinen satunnaisluku (joka on aina jakauman alaraja)
    distr(randomEng);

    // Jos arvotussa sijainnissa on jo kortti, valitaan siitä seuraava tyhjä paikka.
    // (Seuraava tyhjä paikka haetaan kierteisesti funktion next_free avulla.)
    for(unsigned int i = 0, c = 'A'; i < rows * columns - 1; i += 2, ++c)
    {
        // Lisätään kaksi samaa korttia (parit) pelilaudalle
        for(unsigned int j = 0; j < 2; ++j)
        {
            unsigned int cell = distr(randomEng);
            cell = next_free(g_board, cell);
            g_board.at(cell / columns).at(cell % columns).set_letter(c);
            g_board.at(cell / columns).at(cell % columns).set_visibility(HIDDEN);
        }
    }
}

// Tulostaa annetusta merkistä c koostuvan rivin,
// jonka pituus annetaan parametrissa line_length.
// (Kutsutaan vain funktiosta print.)
void print_line_with_char(char c, unsigned int line_length)
{
    for(unsigned int i = 0; i < line_length * 2 + 7; ++i)
    {
        cout << c;
    }
    cout << endl;
}

// Tulostaa vaihtelevankokoisen pelilaudan reunuksineen.
void print(const Game_board_type& g_board)
{
    // Selvitetään annetun pelilaudan rivien ja sarakkeiden määrät
    unsigned int rows = g_board.size();
    unsigned int columns = g_board.at(0).size();

    print_line_with_char('=', columns);
    cout << "|   | ";
    for(unsigned int i = 0; i < columns; ++i)
    {
        cout << i + 1 << " ";
    }
    cout << "|" << endl;

    print_line_with_char('-', columns);
    for (unsigned int i = 0; i < rows; ++i)
    {
        cout << "| " << i + 1 << " | ";
        for (unsigned int j = 0; j < columns; ++j)
        {
            g_board.at(i).at(j).print();
            cout << " ";
        }
        cout << "|" << endl;
    }
    print_line_with_char('=', columns);
}

// Kysyy käyttäjältä tulon ja sellaiset tulon tekijät, jotka ovat
// mahdollisimman lähellä toisiaan.
std::vector<int> ask_product_and_calculate_factors(unsigned int &smaller_factor,
                                                   unsigned int &bigger_factor)
{
    unsigned int product = 0;
    while (not(product > 0 and product % 2 == 0))
    {
        std::cout << INPUT_AMOUNT_OF_CARDS;
        string product_str = "";
        std::getline(std::cin, product_str);

        product = stoi_with_check(product_str);
    }

    std::vector<int> rows_columns;
    for(unsigned int i = 1; i * i <= product; ++i)
    {
        if(product % i == 0)
        {
            smaller_factor = i;
        }
    }
    bigger_factor = product / smaller_factor;
    rows_columns.push_back(smaller_factor);
    rows_columns.push_back(bigger_factor);
    return rows_columns;
}

// Stringien käsittely apufunktio
std::vector<std::string> split(const std::string &text, char separator)
{
    std::vector<std::string> parts;

    std::string::size_type left = 0;
    std::string::size_type right = 0;

    while (right != std::string::npos)
    {
        right = text.find(separator, left);
        std::string part = text.substr(left, right - left);

        if (part != "")
        {
            parts.push_back(part);
        }

        left = right + 1;
    }
    return parts;
}

// Luo pelaaja-oliot
std::vector<Player> create_players(const std::string &text)
{
    std::vector<Player> players;

    std::string::size_type left = 0;
    std::string::size_type right = 0;

    while (right != std::string::npos)
    {
        right = text.find(" ", left);
        std::string player = text.substr(left, right - left);

        if (player != "")
        {
            Player pelaaja = Player(player);
            players.push_back(player);
        }

        left = right + 1;
    }
    return players;
}

// Kysyy pelaajien lukumäärän
std::vector<Player> ask_players()
{
    int players_amount = 0;

    while (!(players_amount > 0))
    {
        std::string player_amount_str = "";
        std::cout << INPUT_AMOUNT_OF_PLAYERS;
        std::cin >> player_amount_str;
        players_amount = stoi_with_check(player_amount_str);
    }

    std::string nimet = "";
    std::cout << "List " << players_amount << " players: ";
    std::getline(std::cin >> std::ws, nimet);

    std::vector<Player> players = create_players(nimet);
    return players;
}

void print_scores(std::vector<Player> &players)
{
    for (auto &player : players)
    {
        player.print();
    }
}

// Selvittää voittajan ja tekee tulostuksen
void calculate_scores(std::vector<Player> &players)
{
    unsigned int korkein_maara_pareja = 0;
    int counter = 0;

    for (auto &player : players)
    {
        if (player.number_of_pairs() > korkein_maara_pareja)
        {
            korkein_maara_pareja = player.number_of_pairs();
        }
    }

    for (auto &player : players)
    {
        if (player.number_of_pairs() == korkein_maara_pareja)
            ++counter;
    }

    if (counter == 1)
    {
        for (auto &player : players)
        {
            if (player.number_of_pairs() == korkein_maara_pareja)
            {
                std::cout << player.get_name() << " has won with "
                          << player.number_of_pairs() / 2 << " pairs." << endl;
            }
        }
    }
    else
    {
        std::cout << "Tie of " << counter << " players with "
                  << korkein_maara_pareja / 2 << " pairs." << std::endl;
    }
}

// Kysyy korttien koordinaatit ja tekee virheentarkistuksen, ym. :-)
bool ask_cards(Game_board_type &g_board,
               Player &player,
               std::vector<Player> &players,
               int &empty_cards_counter)
{
    // Selvitetään annetun pelilaudan rivien ja sarakkeiden määrät
    int rows = g_board.size();
    int columns = g_board.at(0).size();
    int cards_overall = rows * columns;

    while (true)
    {
        std::cout << player.get_name() << ": " << INPUT_CARDS;

        std::string coords = "";
        char separator = ' ';

        std::getline(std::cin >> std::ws, coords);
        std::vector<std::string> parts = split(coords, separator);
        std::vector<int> coords_int;

        if (parts.at(0) == "q")
        {
            std::cout << GIVING_UP << endl;
            return false;
            break;
        }
        else if (parts.size() == 4)
        {
            int osa = 0;
            for (auto &part : parts)
            {
                if (stoi_with_check(part) != 0)
                {
                    osa = stoi_with_check(part);
                    coords_int.push_back(osa);
                }
            }

            if (coords_int.size() == 4)
            {
                int x1 = coords_int[0]
                         - 1; // koordinaattien uudelleenkalibrointia
                int y1 = coords_int[1] - 1;
                int x2 = coords_int[2] - 1;
                int y2 = coords_int[3] - 1;

                // Jos koordinaatit ovat pienempiä tai yhtäsuuria kuin pelilaudan koko..
                if (0 <= x1 and x2 <= rows and 0 <= y1 and y2 <= columns)
                {
                    if (x1 == x2 and y1 == y2)
                    {
                        std::cout << INVALID_CARD << std::endl;
                        continue;
                    }

                    // Ei sallita jo käännettyjen korttien avaamista
                    if (g_board.at(y1).at(x1).get_visibility() == EMPTY)
                    {
                        std::cout << INVALID_CARD << std::endl;
                        continue;
                    }
                    else if (g_board.at(y2).at(x2).get_visibility() == EMPTY)
                    {
                        std::cout << INVALID_CARD << std::endl;
                        continue;
                    }

                    g_board.at(y1).at(x1).turn();
                    g_board.at(y2).at(x2).turn();
                    print(g_board);

                    if (g_board.at(y1).at(x1).get_letter()
                        == g_board.at(y2).at(x2).get_letter())
                    {
                        player.add_card(g_board.at(y1).at(x1));
                        g_board.at(y2).at(x2);
                        empty_cards_counter += 2;

                        std::cout << FOUND << std::endl;
                        print_scores(players);
                        print(g_board);

                        // Jos pelilauta on tyhjänä, peli loppuu
                        if (empty_cards_counter == cards_overall)
                        {
                            std::cout << GAME_OVER << std::endl;
                            calculate_scores(players);
                            return false;
                            break;
                        }
                        continue;
                    }
                    else if (g_board.at(y1).at(x1).get_letter()
                             != g_board.at(y2).at(x2).get_letter())
                    {
                        std::cout << NOT_FOUND << std::endl;
                        print_scores(players);
                        g_board.at(y1).at(x1).turn();
                        g_board.at(y2).at(x2).turn();
                    }

                    break;
                }
            }
        }
        std::cout << INVALID_CARD << std::endl;
    }
    return true;
}

// Jos palauttaa falsen, peli päättyy, true jatkaa peliä.
bool play_game(Player &player,
               Game_board_type &g_board,
               std::vector<Player> &players,
               int &empty_cards_counter)
{
    if (ask_cards(g_board, player, players, empty_cards_counter) == false)
    {
        return false;
    }
    return true;
}

int main()
{
    Game_board_type game_board;

    unsigned int factor1 = 1;
    unsigned int factor2 = 1;

    ask_product_and_calculate_factors(factor1, factor2);
    init_with_empties(game_board, factor1, factor2);

    string seed_str = "";
    std::cout << INPUT_SEED;
    std::getline(std::cin, seed_str);

    int seed = stoi_with_check(seed_str);
    init_with_cards(game_board, seed);

    std::vector<Player> players = ask_players();

    int vuoro = 0;
    int empty_cards_counter = 0;

    while (true)
    {
        //Pelaaja vuorosssa:
        print(game_board);
        Player &player_in_turn = players[vuoro++ % players.size()];
        if (play_game(player_in_turn, game_board, players, empty_cards_counter)
            == false)
        {
            return EXIT_SUCCESS;
        };
    }

    return EXIT_SUCCESS;
}
