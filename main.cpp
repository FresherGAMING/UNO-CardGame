// UNO CARD GAME PROJECT BASED IN C++
// Reyhan Janstephen - 535250034
#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <map>
#include <vector>
#include <algorithm>
using namespace std;
const char* uno_colors[] = {"red", "blue", "green", "yellow"};
const char* uno_cards[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "0"};
const char* action_cards[] = {"skip", "reverse", "+2"};
const char* uno_exclusive[] = {"+4", "color_change"};

string current_card = "";

vector<string> player_cards;
vector<string> bot_cards;

int stacks = 0;

bool stacks_active;

void displayCards();

string getCardColor(string card_id);

string getCardAction(string card_id);

string getCardNumber(string card_id);

bool isAllowedCard(string card_id);

void sendError(string type);

string lower(string ori_str);

string ucfirst(const string& ori_str);

string getRandomizeCard();

void decideCards();

void takeCard(string p, string card);

void win(string p);

void processOpponent();

void removeCard(string card_id);

void cardUse(string p, string card_id);

void sendActionInput();

void startGame();

void input();

string getDisplayName(string card_string);

void init(){
	srand(time(NULL));
}

void displayCards(){
	cout << "Kartu yang anda miliki : " << endl;
	int i;
	for(i = 0; i < player_cards.size(); i++){
		cout << i + 1 << ". " << getDisplayName(player_cards[i]) << " [" << player_cards[i] << "]" << endl;
	}
}

string getCardColor(string card_id){
	for(int i = 0; i < 4; i++){
		if(card_id.length() < strlen(uno_colors[i]))break;
		if(card_id.substr(0, strlen(uno_colors[i])) == uno_colors[i]){
			return uno_colors[i];
		}
	}
	return "";
}

string getCardAction(string card_id){
	string color = getCardColor(card_id);
	if(color != ""){
		card_id.replace(0, color.size() + 1, "");
		for(int i = 0; i < 3; i++){
			if(action_cards[i] == card_id){
				return action_cards[i];
			}
		}
	}
	return "";
}

string getCardNumber(string card_id){
	string color = getCardColor(card_id);
	if(color != ""){
		card_id.replace(0, color.length() + 1, "");
		if(!card_id.empty()){
			for(int i = 0; i <= 10; i++){
				if(uno_cards[i] == card_id){
					return uno_cards[i];
				}
			}
		}
	}
	return "999";
}

bool isAllowedCard(string card_id){
	if(stacks > 0 && getCardAction(card_id) != "+2" && card_id != "+4")return false;
	if(card_id == "+4")return true;
	if(stacks > 0 && current_card == "+4" && getCardAction(card_id) == "+2")return true;
	if(current_card == "+4" && stacks <= 0)return true;
	if(stacks <= 0 && card_id == "color_change")return true;
	if(current_card.substr(0, 12) == "color_change"){
		string c = current_card;
		c.replace(0, 13, "");
		if(c == getCardColor(card_id)){
			return true;
		}
	}
	if(getCardColor(card_id) != "" && getCardColor(current_card) != ""){
		if(getCardColor(card_id) == getCardColor(current_card))return true;
		if(getCardNumber(card_id) == getCardNumber(current_card) && getCardNumber(card_id) != "999")return true;
		if(getCardAction(card_id) != "" && getCardAction(card_id) == getCardAction(current_card))return true;
	}
	return false;
}

void sendError(string type){
	if(type == "invalid-cards"){
		cout << "Kartu/aksi yang anda masukkan salah!" << endl << endl;
	} else if(type == "unable"){
		cout << "Kartu tidak bisa digunakan!" << endl << endl;
	}
}

string lower(string ori_str){
	string str = "";
	for(size_t i = 0; i < ori_str.size(); i++){
		str += tolower(ori_str[i]);
	}
	return str;
}

string ucfirst(const string& ori_str){
	string modif_str = ori_str;
	if(modif_str.length() > 0){
		modif_str[0] = toupper(modif_str[0]);
	}
	return modif_str;
}

string getRandomizeCard(){
	int result = rand() % 108 + 1;
	if(result <= 76){
		string color = uno_colors[rand() % (sizeof(uno_colors) / sizeof(uno_colors[0]))];
		string card_name = uno_cards[rand() % (sizeof(uno_cards) / sizeof(uno_cards[0]))];
		return color + "_" + card_name;
	} else if(result <= 100){
		string color = uno_colors[rand() % (sizeof(uno_colors) / sizeof(uno_colors[0]))];
		string card_name = action_cards[rand() % (sizeof(action_cards) / sizeof(action_cards[0]))];
		return color + "_" + card_name;
	} else if(result <= 108){
		return uno_exclusive[rand() % sizeof(uno_exclusive) / sizeof(uno_exclusive)[0]];
	}
}

void decideCards(){
	int i;
	player_cards.clear();
	bot_cards.clear();
	for(i = 1; i <= 7; i++){
		player_cards.push_back(getRandomizeCard());
		bot_cards.push_back(getRandomizeCard());
	}
	displayCards();
}

void takeCard(string p, string card){
	if(p == "player"){
		player_cards.push_back(card);
	} else {
		bot_cards.push_back(card);
	}
}

void win(string p){
	if(p == "bot"){
		cout << "Bot Menang!" << endl;
	} else {
		cout << "Anda Menang!" << endl;
	}
	cout << endl;
	cout << "Apakah Anda ingin bermain lagi ?" << endl;
	cout << "Ketik UNO untuk bermain lagi" << endl;
	input();
}

void processOpponent(){
	for(int i = 0; i < bot_cards.size(); i++){
		if(isAllowedCard(bot_cards[i])){
			current_card = bot_cards[i];
			vector<string>::iterator ite = find(bot_cards.begin(), bot_cards.end(), bot_cards[i]);
			if(ite != bot_cards.end()){
				bot_cards.erase(ite);
			}
			cardUse("bot", current_card);
			return;
		}
	}
	if(stacks > 0){
		cout << "Bot mengambil kartu (x" << stacks << ")" << endl << endl;
		for(int i = 1; i <= stacks; i++){
			takeCard("bot", getRandomizeCard());
		}
		stacks = 0;
		return;
	}
	takeCard("bot", getRandomizeCard());
	cout << "Bot mengambil kartu (x1)" << endl << endl;
	
}

void removeCard(string card_id){
	vector<string>::iterator i = find(player_cards.begin(), player_cards.end(), card_id);
	if(i != player_cards.end()){
		player_cards.erase(i);
	}
}

string chooseColor(){
	cout << "Silahkan pilih warna [green | blue | red | yellow]: ";
	string color;
	cin >> color;
	color = lower(color);
	for(int i = 0; i < 4; i++){
		if(uno_colors[i] == color){
			return color;
		}
	}
	cout << endl;
	return chooseColor();
}

void cardUse(string p, string card_id){
	string action = getCardAction(card_id);
	if(action == "+2")stacks += 2;
	if(card_id == "+4")stacks += 4;
	if(p == "player"){
		if(player_cards.size() == 1)cout << "Anda: UNO!" << endl;
		cout << "Anda menggunakan kartu " << getDisplayName(card_id) << endl;
		if(player_cards.size() == 0){
			cout << "Anda: UNO Game!" << endl;
			win(p);
			return;
		}
		if(action == "skip" || action == "reverse"){
			cout << endl;
			displayCards();
			cout << "Kartu saat ini : " << getDisplayName(current_card) << endl;
			cout << "Masukkan id kartu atau ambil: ";
			sendActionInput();
			return;
		}
		if(card_id == "color_change"){
			string color = chooseColor();
			current_card = card_id + "_" + color;
		}
		cout << endl;
		processOpponent();
		displayCards();
		cout << "Kartu saat ini : " << getDisplayName(current_card) << endl;
		cout << "Masukkan id kartu atau ambil: ";
		sendActionInput();
	} else {
		if(bot_cards.size() == 1)cout <<  "Bot: UNO!" << endl;
		cout << "Bot menggunakan kartu " << getDisplayName(card_id) << endl << endl;
		if(bot_cards.size() == 0){
			cout << "Bot: UNO Game!" << endl;
			win(p);
			return;
		}
		if(action == "skip" || action == "reverse"){
			processOpponent();
			return;
		}
		if(card_id == "color_change"){
			current_card = card_id + "_" + uno_colors[rand() % 3];
		}
	}
}

void sendActionInput(){
	string action;
	cin >> action;
	action = lower(action);
	if(find(player_cards.begin(), player_cards.end(), action) != player_cards.end()){
		if(!isAllowedCard(action)){
			sendError("unable");
			displayCards();
			cout << "Kartu saat ini : " << getDisplayName(current_card) << endl;
			cout << "Masukkan id kartu atau ambil: ";
			sendActionInput();
			return;
		}
		current_card = action;
		removeCard(action);
		cardUse("player", action);		
	} else if(lower(action.substr(0, 5)) == "ambil") {
		string card;
		if(stacks > 0){
			for(int i = 1; i <= stacks; i++){
				card = getRandomizeCard();
				takeCard("player", card);
				cout << "Anda mengambil kartu dan mendapatkan " << getDisplayName(card) << endl;
			}
			stacks = 0;
			cout << endl;
			processOpponent();
			displayCards();
			cout << "Kartu saat ini : " << getDisplayName(current_card) << endl;
			cout << "Masukkan id kartu atau ambil: ";
			sendActionInput();
			return;
		}
		card = getRandomizeCard();
		takeCard("player", card);
		cout << "Anda mengambil kartu dan mendapatkan " << getDisplayName(card) << endl << endl;
		processOpponent();
		displayCards();
		cout << "Kartu saat ini : " << getDisplayName(current_card) << endl;
		cout << "Masukkan id kartu atau ambil: ";
		sendActionInput();
	} else {
		sendError("invalid-cards");
		displayCards();
		cout << "Kartu saat ini : " << getDisplayName(current_card) << endl;
		cout << "Masukkan id kartu atau ambil: ";
		sendActionInput();
	}
}

void startGame(){
	current_card = "";
	stacks = 0;
	player_cards.clear();
	bot_cards.clear();
	decideCards();
	string color = uno_colors[rand() % 4];
	string num = uno_cards[rand() % 10];
	current_card = color + "_" + num;
	
}

void input(){
	string uno;
	cout << ">> ";
	cin >> uno;
	if(lower(uno) != "uno"){
		input();
		return;
	}
	cout << endl;
	startGame();
	string input_card;
	cout << "Anda jalan pertama!" << endl;
	cout << "Anda bisa menggunakan kata 'ambil' untuk mengambil kartu" << endl;
	cout << "Atau memasukkan id kartu untuk menggunakan kartu yang anda miliki" << endl;
	cout << "Kartu saat ini : " << getDisplayName(current_card) << endl;
	cout << "Masukkan id kartu atau ambil: ";
	sendActionInput();
}


int main(){
	init();
	cout << "Selamat datang di permainan UNO berbasis C++" << endl;
	cout << "Di permainan ini, kalian akan melawan bot yang sudah saya siapkan" << endl;
	cout << "Author : Reyhan Janstephen (535250034)" << endl;
	cout << "Silahkan ketik UNO untuk memulai permainan!" << endl;
	input();
	return 0;
}

string getDisplayName(string card_string){
	for(int i = 0; i < 4; i++){
		if(card_string.length() < strlen(uno_colors[i]))continue;
		if(card_string.substr(0, strlen(uno_colors[i])) == uno_colors[i]){
			return ucfirst(uno_colors[i]) + " " + ucfirst(card_string.substr(strlen(uno_colors[i]) + 1, card_string.length()));
		}
	}
	if(card_string.substr(0, 12) == "color_change"){
		if(card_string.length() >= 13){
			return "Color Change - " + ucfirst(card_string.substr(13));
		}
		return "Color Change";
	}
	return card_string;
}
