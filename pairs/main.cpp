#include <player.hh>
#include <card.hh>
#include <iostream>
#include <vector>
#include <random>

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

// Pelaajat sisältävä vektori
using Players_type = vector<Player>;

// Muuntaa annetun numeerisen merkkijonon vastaavaksi kokonaisluvuksi
// (kutsumalla stoi-funktiota).
// Jos annettu merkkijono ei ole numeerinen, palauttaa nollan
// (mikä johtaa laittomaan korttiin myöhemmin).
//
// Converts the given numeric string to the corresponding integer
// (by calling stoi).
// If the given string is not numeric, returns 0
// (which leads to an invalid card later).
unsigned int stoi_with_check(const string& str)
{
    bool is_numeric = true;
    for(unsigned int i = 0; i < str.length(); ++i)
    {
        if(not isdigit(str.at(i)))
        {
            is_numeric = false;
            break;
        }
    }
    if(is_numeric)
    {
        return stoi(str);
    }
    else
    {
        return 0;
    }
}

// Täyttää pelilaudan (kooltaan rows * columns) tyhjillä korteilla.
//
// Fills the game board, the size of which is rows * columns, with empty cards.
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
//
// Finds the next free position in the game board (g_board), starting from the
// given position start and continuing from the beginning if needed.
// (Called only by the function init_with_cards.)
unsigned int next_free(Game_board_type& g_board, unsigned int start)
{
    // Selvitetään annetun pelilaudan rivien ja sarakkeiden määrät
    //
    // Finding out the number of rows and columns of the game board
    unsigned int rows = g_board.size();
    unsigned int columns = g_board.at(0).size();

    // Aloitetaan annetusta arvosta
    //
    // Starting from the given value
    for(unsigned int i = start; i < rows * columns; ++i)
    {
        if(g_board.at(i / columns).at(i % columns).get_visibility() == EMPTY) // vaihdettu
        {
            return i;
        }
    }
    // Jatketaan alusta
    //
    // Continuing from the beginning
    for(unsigned int i = 0; i < start; ++i)
    {
        if(g_board.at(i / columns).at(i % columns).get_visibility() == EMPTY)
        {
            return i;
        }
    }
    // Tänne ei pitäisi koskaan päätyä
    //
    // You should never reach this
    std::cout << "No more empty spaces" << std::endl;
    return rows * columns - 1;
}

// Alustaa annetun pelilaudan (g_board) satunnaisesti arvotuilla korteilla
// annetun siemenarvon (seed) perusteella.
//
// Initializes the given game board (g_board) with randomly generated cards,
// based on the given seed value.
void init_with_cards(Game_board_type& g_board, int seed)
{
    // Selvitetään annetun pelilaudan rivien ja sarakkeiden määrät
    //
    // Finding out the number of rows and columns of the game board
    unsigned int rows = g_board.size();
    unsigned int columns = g_board.at(0).size();

    // Arvotaan täytettävä sijainti
    //
    // Drawing a cell to be filled
    std::default_random_engine randomEng(seed);
    std::uniform_int_distribution<int> distr(0, rows * columns - 1);
    // Hylätään ensimmäinen satunnaisluku (joka on aina jakauman alaraja)
    //
    // Wiping out the first random number (that is always the lower bound of the distribution)
    distr(randomEng);

    // Jos arvotussa sijainnissa on jo kortti, valitaan siitä seuraava tyhjä paikka.
    // (Seuraava tyhjä paikka haetaan kierteisesti funktion next_free avulla.)
    //
    // If the drawn cell is already filled with a card, next empty cell will be used.
    // (The next empty cell is searched for circularly, see function next_free.)
    for(unsigned int i = 0, c = 'A'; i < rows * columns - 1; i += 2, ++c)
    {
        // Lisätään kaksi samaa korttia (parit) pelilaudalle
        //
        // Adding two identical cards (pairs) in the game board
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
//
// Prints a line consisting of the given character c.
// The length of the line is given in the parameter line_length.
// (Called only by the function print.)
void print_line_with_char(char c, unsigned int line_length)
{
    for(unsigned int i = 0; i < line_length * 2 + 7; ++i)
    {
        cout << c;
    }
    cout << endl;
}

// Tulostaa vaihtelevankokoisen pelilaudan reunuksineen.
//
// Prints a variable-length game board with borders.
void print(const Game_board_type& g_board)
{
    // Selvitetään annetun pelilaudan rivien ja sarakkeiden määrät
    //
    // Finding out the number of rows and columns of the game board
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
    for(unsigned int i = 0; i < rows; ++i)
    {
        cout << "| " << i + 1 << " | ";
        for(unsigned int j = 0; j < columns; ++j)
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
//
// Asks the desired product from the user, and calculates the factors of
// the product such that the factor as near to each other as possible.
void ask_product_and_calculate_factors(unsigned int& smaller_factor, unsigned int& bigger_factor)
{
    unsigned int product = 0;
    while(not (product > 0 and product % 2 == 0))
    {
        std::cout << INPUT_AMOUNT_OF_CARDS;
        string product_str = "";
        std::getline(std::cin, product_str);
        product = stoi_with_check(product_str);
    }

    for(unsigned int i = 1; i * i <= product; ++i)
    {
        if(product % i == 0)
        {
            smaller_factor = i;
        }
    }
    bigger_factor = product / smaller_factor;
}

// Luo parametrina annetun määrän Player-olioita, joiden nimet luetaan yhdeltä,
// käyttäjän syöttämältä riviltä.
Players_type enter_players()
{
    Players_type players;
    unsigned int player_amount = 0;

    // Kysytään pelaajien määrää kunnes käyttäjä syöttää määräksi yksi tai useampi
    while(not (player_amount > 0))
    {
        // Tarkistetaan syöte
        std::cout << INPUT_AMOUNT_OF_PLAYERS;
        std::string player_amount_str = "";
        std::getline(std::cin, player_amount_str);
        player_amount = stoi_with_check(player_amount_str);
    }

    std::cout << "List" << player_amount << " players: ";

    // Luetaan pelaajien nimet yhdeltä riviltä, ja luodaan Player-oliot
    for(unsigned int i = 0; i < player_amount; ++i)
    {
        std::string player_name = "";
        std::cin >> player_name;
        Player player = Player(player_name);

        // Lisätään uusi pelaaja tietorakenteeseen
        players.push_back(player);
    }
    return players;
}

bool check_coordinates_validity(const Game_board_type game_board, std::vector<unsigned int>& coordinates, unsigned int factor1, unsigned int factor2)
{

    unsigned int x1 = coordinates.at(0);
    unsigned int y1 = coordinates.at(1);
    unsigned int x2 = coordinates.at(2);
    unsigned int y2 = coordinates.at(3);

    // Tarkistetaan ovatko kaikki koordinaatit lukuja, ja suurempia kuin 0
    if(y1 != 0 && y2 != 0 && x1 != 0 && x2 != 0){

        // Tarkistetaan sijaitsevatko kaikki koordinaatit pelilaudalla
        if(y1 <= factor1 && y2 <= factor1 && x1 <= factor2 && x2 <= factor2){

            // Tarkistetaan sijaitsevatko käännettävät kortit samoissa koordinaateissa
            if (!(y1 == y2 && x1 == x2)){

                // Tarkistetaan onko kortit poistettu laudalta
                if(game_board[y1-1][x1-1].get_visibility() != EMPTY && game_board[y2-1][x2-1].get_visibility() != EMPTY){
                    return true;
                }
            }
        }
    }
    std::cout << INVALID_CARD << std::endl;
    coordinates.clear();
    return false;
}

bool enter_cards(const Players_type::value_type& player, std::vector<unsigned int>& coordinates, const int factor1, const int factor2)
{
    while(true) {

        // Sivuutetaan kaikki "jonossa" olevat merkit
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // Kysytään syöte
        std::cout << player.get_name() << ": " << INPUT_CARDS << std::flush;

        // Luetaan syöte merkki kerrallaan
        for(int i = 0; i < 4; ++i){

            std::string value;
            std::cin >> value;

            // Lopetetaan ohjelma, jos syöte on "q"
            if(value == "q"){
                cout << GIVING_UP;
                return false;
            }

            // Muutetaan merkki kokonaisluvuksi
            int int_value = stoi_with_check(value);

            // Aloitetaan silmukka alusta, jos luettu merkki on 0 tai ei-kokonaisluku
            if(int_value != 0){

                // Selvitetään jakojäännöksen avulla, onko kyseessä x- vai y-koordinaatti,
                // Ja tarkistetaan löytyykö se pelilaudalta
                // Lisätään vektoriin, mikäli näin on

                // X-koordinaatit
                if(i%2 == 0){
                    if(int_value <= factor2){
                        coordinates.push_back(int_value);
                    }
                    else{
                        std::cout << INVALID_CARD <<std::endl;
                        coordinates.clear();
                        break;
                    }
                }
                // Y-koordinaatit
                if(i%2 == 1){
                    if(int_value <= factor1){
                        coordinates.push_back(int_value);
                    }
                    else{
                        std::cout << INVALID_CARD << std::endl;
                        coordinates.clear();
                        break;
                    }
                }
            }
            else{
                std::cout << INVALID_CARD << std::endl;
                coordinates.clear();
                break;
            }
        }
        if(coordinates.size() == 4){
            return true;
        }
        else{
            continue;
        }
    }
}


void play(Game_board_type game_board, std::vector<unsigned int>& coordinates){
    unsigned int x1 = coordinates.at(0);
    unsigned int y1 = coordinates.at(1);
    unsigned int x2 = coordinates.at(2);
    unsigned int y2 = coordinates.at(3);

    game_board[y1-1][x1-1].turn();
    game_board[y2-1][x2-1].turn();
    print(game_board);
    coordinates.clear();
    return;
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

    // Alustetaan koordinaatit sisältävä vektori
    std::vector<unsigned int> coordinates;

    Players_type players = enter_players();
    Players_type::size_type player_amount = players.size();

    Players_type::size_type player_index = 0;

    while (true) {

        Players_type::value_type in_turn = players.at(player_index);

        if(!enter_cards(in_turn, coordinates, factor1, factor2)){
            break;
        }

        else{

            if(!check_coordinates_validity(game_board, coordinates, factor1, factor2)){
                continue;
            }
            else{
                play(game_board, coordinates);

                if(player_index == player_amount - 1){
                    player_index = 0;
                }
                else{
                    ++player_index;
                }
            }
        }
    }
    return EXIT_SUCCESS;
}



