#include <iostream>
#include <string>
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

void init(){
//	uno_colors = ;
//	uno_cards = ;
//	action_cards = ;
//	uno_exclusive = ;
}

void displayCards(){
	cout << "Kartu yang anda miliki : " << endl;
	int i;
	for(i = 0; i < player_cards.size(); i++){
		cout << i + 1 << ". " << player_cards[i] << endl;
	}
}

string getCardPrefix(string card_id){
	for(int i = 0; i <= 4; i++){
		if(card_id.substr(0, uno_colors[i].size()) == uno_colors[i]){
			return uno_colors[i];
		}
	}
	return "";
	
}

bool isAllowedCard(string card_id){
	if(getCardPrefix(card_id) != ""){
		current_card = card_id;
	}
}

void sendError(string type){
	switch(type){
		case "invalid-cards":
			cout << endl << "Kartu/aksi yang anda masukkan salah!" << endl;
			displayCards();
			return;
		case "unable":
			cout << endl << "Kartu tidak bisa digunakan!" << endl;
			return;
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
	srand(time(NULL));
	player_cards.clear();
	bot_cards.clear();
	for(i = 0; i <= 8; i++){
		player_cards.push_back(getRandomizeCard());
		bot_cards.push_back(getRandomizeCard());
	}
	displayCards();
}

void takeCard(string p, string card){
	
}

bool sendActionInput(){
	string action;
	cin >> action;
	if(find(player_cards.begin(), player_cards.end(), action) != player_cards.end()){
		if(get)
	} else if(lower(action.substr(0, 5)) == "ambil") {
		
	} else {
		sendError("invalid-cards");
		cout << "Masukkan kartu atau ambil: ";
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
	cout << "Masukkan Kartu atau ambil: ";
	sendActionInput();
}


int main(){
	cout << "Selamat datang di permainan UNO berbasis C++" << endl;
	cout << "Di permainan ini, kalian akan melawan bot yang sudah saya siapkan" << endl;
	cout << "Author : Reyhan Janstephen (535250034)" << endl;
	cout << "Silahkan ketik UNO untuk memulai permainan!" << endl;
	input();
	return 0;
}

string getDisplayName(string card_string){
	
}
