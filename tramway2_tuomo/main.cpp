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
#include <map>


// Alustetaan vakiomuuttujat
const char INPUT_FILE_FIELD_DELIMETER = ';';
const size_t LINE_FIELD_INDEX = 0;
const size_t STOP_FIELD_INDEX = 1;
const size_t DIST_FIELD_INDEX = 2;
const size_t VALID_FIELD_COUNT_LOW = 2;
const size_t VALID_FIELD_COUNT_HIGH = 3;

// Tietorakenne pysäkille
struct Stop{
    std::string name;
};
// Tietorakenne pysäkin etäisyydelle
using dist_type = double;
// Tietorakenne linjoille, jotka sisältävät pysäkkejä
using Tramway = std::map<std::string, std::map<dist_type, Stop>>;


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

// Lukee syötetiedoston rivi kerrallaan, ja lisää rivit vektoriin
// return:   true: tiedoston lukeminen onnistui
//           false: tiedoston lukeminen epäonnistui
bool read_input_file(const std::string filepath, std::vector<std::string>& lines)
{
    // Avataan tiedosto
    std::ifstream input_file_stream;
    input_file_stream.open(filepath);

    // Tulostetaan virheilmoitus ja palataan funktiosta, mikäli tiedosto ei aukea
    if(!input_file_stream.is_open()){
        std::cout << "Error: File cannot be opened." << std::endl;
        return false;
    }

    // Lisätään tiedoston rivit vektoriin
    std::string line;
    while(std::getline(input_file_stream, line)){
        lines.push_back(line);
    }
    input_file_stream.close();
    return true;
}

// Lisää pysäkin Tramway-tietorakenteeseen, halutulle linjalle
// return:   true: lisäys onnistui
//           false: lisäys epäonnistui
bool add_stop(Tramway& rasse_data, const std::string& line_name,
              dist_type distance, const std::string& stop_name){

    // Muuttuja, joka kertoo onko lisättävä pysäkki ainut linjallaan
    bool is_unique  = true;

    // Tarkistetaan, onko linjaa olemassa
    if(rasse_data[line_name].count(distance) != 0){
        is_unique = false;
    }

    // Iteroidaan linja
    for(auto& dist_stop_pair : rasse_data[line_name]){

        // Pysäkkiä ei lisätä, mikåli linjalta löytyy jo saman niminen pysäkki
        if(dist_stop_pair.second.name == stop_name){
            is_unique = false;
            break;
        }
    }

    // Lisätään pysäkki linjalle, tai tulostetaan virheilmoitus
    if(is_unique){
        rasse_data[line_name][distance] = Stop{stop_name};
    }
    else{
        std::cout << "Error: Stop/line already exists." << std::endl;
    }

    return is_unique;
}


// Lisää linjan Tramway-tietorakenteeseen
// return:   true: lisäys onnistui
//           false: lisäys epäonnistui
bool add_line(Tramway& rasse_data, const std::string& line_name){

    // Tarkistetaan, onko lisättävä linja jo olemassa
    if(rasse_data.count(line_name) != 0){
        std::cout << "Error: Stop/line already exists." << std::endl;
        return false;
    }

    // Lisätään tietorakenteeseen haluttu linja
    rasse_data[line_name] = {};
    return true;
}


// Tarkistaa, onko syötetiedoston rivi oikeellisessa muodossa
// return:   true: on
//           false: ei
bool validate_input_fields(Tramway& rasse_data, const std::vector<std::string>& fields){

    // Tarkistetaan onko rivillä hyväksytty määrä erottimella pilkottuja osia
    if(fields.size() < VALID_FIELD_COUNT_LOW && fields.size() > VALID_FIELD_COUNT_HIGH){
        return false;
    }

    // Iteroidaan rivin osat
    for(size_t i = 0; i < fields.size(); ++i){
        std::string field = fields.at(i);

        // Tarkistetaan rivin osat yksi kerrallaan
        if(field == ""
                && i != DIST_FIELD_INDEX
                && rasse_data.count(fields.at(LINE_FIELD_INDEX))){
            return false;
        }
    }
    return true;
}


// Käy syötetiedoston sisällön läpi, ja lisää sen tiedot Tramway-tietorakenteeseen mikäli nämä ovat oikeelliset
// return:   true: läpikäynti onnistui
//           false: läpikäynti epäonnistui
bool parse_input_file(const std::vector<std::string>& rows, Tramway& rasse_data){

    // Iteroidaan rivit sisältävä vektori
    for(const auto& row : rows){
        // Jaetaan rivi osiin erotinmerkin perusteella
        std::vector<std::string> fields = split(row, INPUT_FILE_FIELD_DELIMETER);
        // Tarkistetaan, onko rivin muoto oikeellinen
        if(!validate_input_fields(rasse_data, fields)){
            std::cout << "Error: Invalid format in file." << std::endl;
            return false;
        }

        // Lisätään rivin data muuttujiin
        std::string line_name = fields.at(LINE_FIELD_INDEX);
        std::string stop_name = fields.at(STOP_FIELD_INDEX);
        dist_type distance = 0.0;

        // Jos rivillä on kolme osaa joista etäisyys ei ole tyhjä, otetaan etäisyys talteen
        if(fields.size() == VALID_FIELD_COUNT_HIGH
                && fields.at(DIST_FIELD_INDEX) != ""){

            distance = static_cast<dist_type>(std::stod(fields.at(DIST_FIELD_INDEX)));
        }
         // Jos lisättävää linjaa ei löydy tietorakenteesta, lisätään tämä
        if(rasse_data.count(line_name) == 0){
            add_line(rasse_data, line_name);
        }

        // Lisätään pysäkki
        if(!add_stop(rasse_data, line_name, distance, stop_name)){
            return false;
        }
    }

    return true;
}

void print_lines(const Tramway& rasse_data){

    std::cout << "All tramlines in alphabetical order:" << std::endl;

    for(const auto& line : rasse_data){
        std::cout << line.first << std::endl;
    }
}

bool interface(){

    // Luodaan tietorakenne linjoja/pysäkkejä varten
    Tramway rasse_data;

    // Kysytään käyttäjältä syötetiedosto
    std::cout << "Give a name for input file: " << std::endl;
    std::string filepath;
    std::getline(std::cin, filepath);
    std::vector<std::string> lines;

    // Luetaan tiedosto
    if(!read_input_file(filepath, lines)){
        return false;
    }

    // Käydään tiedosto läpi
    if(!parse_input_file(lines, rasse_data)){
        return false;
    }

    while(true){

        // Kysytään syöte
        std::cout << "tramway> ";
        std::string user_input = "";
        std::getline(std::cin, user_input);
        std::vector<std::string> input_fields = split(user_input, ' ');

        // "QUIT"
        if(input_fields.at(0) == "QUIT"){
            return false;
        }

        // "LINES"
        if(input_fields.at(0) == "LINES"){
            print_lines(rasse_data);
        }
    }
    return true;
}



// Short and sweet main.
int main(){
    print_rasse();

    if (interface()){
        return EXIT_SUCCESS;
    }
    else{
        return EXIT_FAILURE;
    }
}
