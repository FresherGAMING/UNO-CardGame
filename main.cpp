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

void init(){
	srand(time(NULL));
}

void displayCards(){
	cout << "Kartu yang anda miliki : " << endl;
	int i;
	for(i = 0; i < player_cards.size(); i++){
		cout << i + 1 << ". " << player_cards[i] << endl;
	}
}

string getCardColor(string card_id){
	for(int i = 0; i <= 4; i++){
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
		card_id.replace(0, color.size() + 1, "");
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
	if(card_id == "+4" || card_id == "color_change")return true;
	cout << current_card << endl;
	if(getCardColor(card_id) != "" && getCardColor(current_card) != ""){
		if(getCardColor(card_id) == getCardColor(current_card))return true;
		if(getCardNumber(card_id) == getCardNumber(current_card) && getCardNumber(card_id) != "999")return true;
		if(getCardAction(card_id) != "" && getCardAction(card_id) == getCardAction(current_card))return true;
	}
	return false;
}

void sendError(string type){
	if(type == "invalid-cards"){
		cout << endl << "Kartu/aksi yang anda masukkan salah!" << endl;
		displayCards();
	} else if(type == "unable"){
		cout << endl << "Kartu tidak bisa digunakan!" << endl;
	}
}

string lower(string ori_str){
	string str = "";
	for(size_t i = 0; i < ori_str.size(); i++){
		str += tolower(ori_str[i]);
	}
	return str;
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
	for(i = 0; i <= 8; i++){
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

void processOpponent(){
	for(int i = 0; i <= bot_cards.size(); i++){
		if(isAllowedCard(bot_cards[i])){
			current_card = bot_cards[i];
			vector<string>::iterator ite = find(bot_cards.begin(), bot_cards.end(), bot_cards[i]);
			if(ite != bot_cards.end()){
				bot_cards.erase(ite);
			}
			return;
		}
	}
	takeCard("bot", getRandomizeCard());
	
}

removeCard(string card_id){
	vector<string>::iterator i = find(player_cards.begin(), player_cards.end(), card_id);
	if(i != player_cards.end()){
		player_cards.erase(i);
	}
}

void sendActionInput(){
	string action;
	cin >> action;
	action = lower(action);
	if(find(player_cards.begin(), player_cards.end(), action) != player_cards.end()){
		if(!isAllowedCard(action)){
			sendError("unable");
			cout << "Kartu saat ini : " << current_card << endl;
			cout << "Masukkan id kartu atau ambil: ";
			sendActionInput();
			return;
		}
		current_card = action;
		removeCard(action);
		processOpponent();
		cout << "Kartu saat ini : " << current_card << endl;
		cout << "Masukkan id kartu atau ambil: ";
		sendActionInput();
		
	} else if(lower(action.substr(0, 5)) == "ambil") {
		string card = getRandomizeCard();
		takeCard("player", card);
		cout << "Anda mengambil kartu dan mendapatkan " << card << endl;
		processOpponent();
		cout << "Kartu saat ini : " << current_card << endl;
		cout << "Masukkan id kartu atau ambil: ";
		sendActionInput();
	} else {
		sendError("invalid-cards");
		cout << "Kartu saat ini : " << current_card << endl;
		cout << "Masukkan id kartu atau ambil: ";
		sendActionInput();
	}
}

void startGame(){
	current_card = "";
	decideCards();
	current_card = getRandomizeCard();
	
}

void input(){
	string uno;
	cin >> uno;
	if(lower(uno) != "uno"){
		input();
		return;
	}
	startGame();
	string input_card;
	cout << "Anda jalan pertama!" << endl;
	cout << "Anda bisa menggunakan kata 'ambil' untuk mengambil kartu" << endl;
	cout << "Kartu saat ini : " << current_card << endl;
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
	
}
