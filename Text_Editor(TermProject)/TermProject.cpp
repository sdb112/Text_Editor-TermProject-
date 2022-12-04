#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
#include <sstream>
#include <cctype>
#include <regex>

using namespace std;

#define MAINMENU "n: 다음페이지, p:이전페이지, i:삽입, d:삭제, c:변경, s:찾기, t:저장후종료"
#define MAINLINE "-------------------------------------------------------------------------------------"

class TermProject
{
private:

	vector<string> Letters;
	static TermProject* INSTANCE;

	int MAX_LINE_RANGE = 20;
	int CurrentPage = 1;
	int LastPage;
	int FirstPage;
	int Count_Line;
	string ConsoleMsg = "";
	string Input;

	TermProject()
	{
		Letters = {};
	}
public:
	int MIN_LINE;
	int MAX_LINE;
	string Input_Console() { //입력 getter
		return this->Input;
	}
	string consoleMsg() {
		return this->ConsoleMsg; //상황별 getter
	}
	void setConsoleMsg(string newConsoleMsg) {
		this->ConsoleMsg = newConsoleMsg;
	}

	vector<string> myLetters() //getter
	{
		return Letters;
	}
	void setMyLetters(vector<string>& newMyLetters) //setter
	{
		Letters = newMyLetters;
	}

	static TermProject* getInstance() //singleTon의 Termproject 객체를 반환하는 instance()함수 
	{
		if (!INSTANCE)
			INSTANCE = new TermProject();
		return INSTANCE;
	}


	void viewByPage(string consoleMsg); //페이지 별로 section을 나눠주는 함수
	void viewByLine(); //20줄로 나누어주는 함수
	void viewByLine(int line);
	int letter_In_String(string x, string y); //명령에서 ',' 를 확인하여 숫자를 반환하는데 사용

	void CommandCenter(string cmd); //입력키에 따라 명령 배분 및 비정상입력 대응.
	void N();
	void P();
	void I(int LineNumber, int Number, string letter);
	void C(string targetLetter, string newLetter);
	void S(string targetLetter);
	void T();
	//각 알파벳 입력값에 따른 함수

	void reset(string consoleMsg);
	//입력에 따른 행동을 한뒤 페이지를 리셋하는 함수
	void reset_For_S(string consoleMsg, int min_line);
	//입력에 따른 행동을 한뒤 페이지를 리셋하는 함수(S 입력을 받기 위해 사용)
	vector<string> split(string input, char delimiter);
	//명령에서 필요한 부분만 뽑아 사용하기 위한 함수
	void inputCommand();
	//입력을 위한 함수
	vector<string> divide_Line(vector<string> base);
	//75바이트씩 분할하여 주는 함수
};

TermProject* TermProject::INSTANCE = 0;

int TermProject::letter_In_String(string J, string S) { //i,d,c 와 같이 ,가 들어가는 명령 함수에서 두번째 인자의 개수를 알려줌.
	int count = 0;
	for (unsigned int n = 0; n < J.size(); n++) {
		char temp = J[n];
		for (unsigned int i = 0; i < S.size(); i++) {
			if (temp == S[i]) {
				count++;
			}
		}
	}
	return count;
}



void TermProject::CommandCenter(string cmd) { //입력값 처리 및 잘못된 명령 처리
	int count = letter_In_String(cmd, ","); //,를 통해 파악
	int numOfexept1 = letter_In_String(cmd, ")");
	int numOfexept2 = letter_In_String(cmd, "(");

	vector<string> splitedCmd = split(cmd, '(');
	setConsoleMsg("");
	regex r("^[a-zA-Z0-9]*$"); //레겍스 사용

	//각 명령어에 맞게 if문 처리 및 비정상 입력 처리.
	if (cmd == "N" || cmd == "n") {
		N();
	}
	else if (cmd == "P" || cmd == "p") {
		P();
	}
	else if (cmd == "T" || cmd == "t") {
		T();
	}
	else if (numOfexept1 == 0 || numOfexept2 == 0) {
		setConsoleMsg("Spaces can not be entered. Please try again");
		reset(ConsoleMsg);
	}
	else if (splitedCmd[0] == "I" || splitedCmd[0] == "i") {
		if (count == 2) {
			vector<string> splitedArgNum = split(splitedCmd[1], ',');
			vector<string> splitedArgStr = split(splitedArgNum[2], ')');

			bool check1 = regex_match(splitedArgNum[0], r);
			bool check2 = regex_match(splitedArgNum[1], r);
			if (!check1 || !check2) {
				setConsoleMsg("It is the wrong approach. Special Characters can not be entered. Please try again.");
				reset(ConsoleMsg);
			}
			else if (MIN_LINE > stoi(splitedArgNum[0]) || stoi(splitedArgNum[0]) > MAX_LINE) {
				setConsoleMsg("It is the wrong approach. There are no line on this page. Please try again.");
				reset(ConsoleMsg);
			}
			else {
				I(stoi(splitedArgNum[0]), stoi(splitedArgNum[1]), splitedArgStr[0]);
			}
		}
		else {
			setConsoleMsg("It is the wrong approach. (Please check the format again) Please try again.");
			reset(ConsoleMsg);
		}
	}

	else if (splitedCmd[0] == "S" || splitedCmd[0] == "s") {
		if (count < 2) {
			vector<string> splitedArgStr = split(splitedCmd[1], ')');
			S(splitedArgStr[0]);
		}
		else {
			setConsoleMsg("It is the wrong approach. (Please check the format again) Please try again.");
			reset(ConsoleMsg);
		}
	}
	else if (splitedCmd[0] == "C" || splitedCmd[0] == "c") {
		if (count == 1) {
			vector<string> splitedArgStr1 = split(splitedCmd[1], ',');
			vector<string> splitedArgStr2 = split(splitedArgStr1[1], ')');

			bool check1 = regex_match(splitedArgStr1[0], r);
			bool check2 = regex_match(splitedArgStr2[0], r);
			if (!check1 || !check2) {
				setConsoleMsg("It is the wrong approach. Special Characters can not be entered. Please try again.");
				reset(ConsoleMsg);
			}
			else {
				C(splitedArgStr1[0], splitedArgStr2[0]);
			}
		}
		else {
			setConsoleMsg("It is the wrong approach. (Please check the format again) Please try again.");
			reset(ConsoleMsg);
		}
	}
	else {
		setConsoleMsg("It is the wrong approach. (Please check the format again) Please try again.");
		reset(ConsoleMsg);
	}
}

void TermProject::inputCommand() {
	string command;
	cin >> command;
	this->Input = command;
	cout << MAINLINE << endl;

}

void TermProject::viewByPage(string consoleMsg) {
	cout << MAINLINE << endl;
	cout << MAINMENU << endl;
	cout << MAINLINE << endl;
	cout << "(콘솔메시지)" << consoleMsg << endl;
	cout << MAINLINE << endl;
	TermProject::inputCommand();
};

void TermProject::viewByLine() {

	Count_Line = myLetters().size();

	MIN_LINE = MAX_LINE_RANGE * (CurrentPage - 1) + 1;
	MAX_LINE = (MAX_LINE_RANGE * CurrentPage);


	if (myLetters().size() % 20 == 0) {
		LastPage = myLetters().size() / 20;
	}
	else {
		LastPage = myLetters().size() / 20 + 1;
	}
	//출력할 라인이 20개 이상일떄
	if (CurrentPage != LastPage) {
		for (int i = MAX_LINE_RANGE * (CurrentPage - 1); i < (MAX_LINE_RANGE * CurrentPage); i++) {
			if (i / 9 == 0) {
				cout << " " << i + 1 << "|" << " " << myLetters()[i] << endl;
			}
			else {
				cout << i + 1 << "|" << " " << myLetters()[i] << endl;
			}
		}
	}
	//그게 아닌 경우 마지막라인 + 새로운 라인
	else {
		int indexFromMid = myLetters().size() - 20;
		for (int i = indexFromMid; i < myLetters().size(); i++) {
			if (i / 9 == 0) {
				cout << " " << i + 1 << "|" << " " << myLetters()[i] << endl;
			}
			else {
				cout << i + 1 << "|" << " " << myLetters()[i] << endl;
			}
		}
	}
};

void TermProject::viewByLine(int line) {
	for (int i = line; i < line + 20; i++) {
		if (i / 9 == 0) {
			cout << " " << i + 1 << "|" << " " << myLetters()[i] << endl;
		}
		else {
			cout << i + 1 << "|" << " " << myLetters()[i] << endl;
		}
	}

}

void TermProject::reset(string consoleMsg) {
	consoleMsg = TermProject::consoleMsg();
	TermProject::getInstance()->viewByLine();
	TermProject::getInstance()->viewByPage(consoleMsg);
}

vector<string> TermProject::split(string input, char delimiter) {
	vector<string> result;
	stringstream stringstream(input);
	string temp;

	while (getline(stringstream, temp, delimiter)) {
		result.push_back(temp);
	}

	return result;
}

vector<string> TermProject::divide_Line(vector<string> base)
{
	string temp;
	vector<string> text;

	for (int i = 0; i < (int)base.size(); i++) {
		temp = temp + base[i] + " ";
		//하나의 스트링을 75바이트씩 분할된 라인으로 저장한다.
		if (temp.length() > 75) {
			if (temp.length() == 76 && temp[temp.length() - 1] == ' ') { //길이가 딱 맞을때 
				temp.erase(temp.length() - 1, 1);
				text.push_back(temp);
				temp = "";

				continue;
			}

			int size = base[i].length() + 1;
			temp.erase(temp.length() - size, temp.length());
			text.push_back(temp);

			temp = base[i] + " ";

		}
		if (i == base.size() - 1 && temp.length() <= 75) {
			text.push_back(temp);
			temp = "";
		}
	}
	//저장된 벡터를 반환
	return text;
}


//==============================명령 함수===============================
void TermProject::N() {
	//페이지를 넘기는 함수
	if (CurrentPage == (LastPage - 1)) {
		CurrentPage++;
		setConsoleMsg("This is the last page!");
		reset(consoleMsg());
	}
	else if (CurrentPage != LastPage) {
		CurrentPage++;
		reset(consoleMsg());
	}
	else {
		setConsoleMsg("This is the last page. Can't turn the page anymore");
		reset(consoleMsg());
	}

};
void TermProject::P() {
	//이전 페이지로 돌아가는 함수
	if (CurrentPage == 1) {
		setConsoleMsg("This is the first page!");
		reset(consoleMsg());
	}
	else {
		CurrentPage--;
		reset(consoleMsg());
	}
};

void TermProject::I(int LineNumber, int LetterNumber, string letter) {
	//letter을 삽입하는 함수
	if (LineNumber > (MAX_LINE_RANGE * CurrentPage) || LineNumber <= (MAX_LINE_RANGE * (CurrentPage - 1)) || letter.size() > 75) {
		setConsoleMsg("wrong Input. Please try again.");
		reset(ConsoleMsg);
	}
	int lineIndex = LineNumber - 1;
	int wordIndex = LetterNumber - 1;
	letter = ' ' + letter + ' ';
	int count = 0;
	string buffer = "";
	string newString = "";
	string temp2;
	vector<string> Inserted;
	for (vector<string>::iterator i = Letters.begin(); i != Letters.end(); i++) {
		if (count == lineIndex) {
			vector<string> temp = split(Letters[count], ' ');
			int numOfInserted = temp.size();
			if (numOfInserted < LetterNumber) {
				setConsoleMsg("Second input is too big. Please try again.");
				reset(ConsoleMsg);
			}
			temp[wordIndex].insert(0, letter);
			for (int i = 0; i < temp.size(); i++) {
				buffer = buffer + ' ' + temp[i];
			}


			stringstream s(newString);
			Letters[count] = buffer;
			for (int i = 0; i < Letters.size(); i++) {
				newString = newString + ' ' + Letters[i];
			}
			stringstream s2(newString);
			while (s2 >> temp2) {
				Inserted.push_back(temp2);
			}
			vector<string> divided = TermProject::getInstance()->divide_Line(Inserted);
			Letters = divided;
			break;
		}
		else {
			count++;
		}
	}
	setConsoleMsg("Selected Letter : " + letter + " is Inserted! ");
	reset(ConsoleMsg);
};

void TermProject::C(string targetLetter, string newLetter) {
	//선택된 letter변경하는 함수
	if (targetLetter.size() > 75 || newLetter.size() > 75) {
		setConsoleMsg("Wrong input. Please try again.");
		reset(ConsoleMsg);
	}

	int count = 0;
	int countTarget = 0;

	string buffer = "";
	string newString = "";
	string temp2;
	vector<string> Inserted;
	for (vector<string>::iterator i = Letters.begin(); i != Letters.end(); i++) {
		vector<string> temp = split(Letters[count], ' ');
		for (int i = 0; i < temp.size(); i++) {
			if (temp[i] == targetLetter) {
				temp[i] = newLetter;
				countTarget++;
			}
			buffer = buffer + ' ' + temp[i];
		}
		count++;
	}

	if (countTarget == 0) {
		setConsoleMsg("Selected Letter is not exist. Please try again.");
		reset(ConsoleMsg);
	}
	else {
		stringstream s2(buffer);
		while (s2 >> temp2) {
			Inserted.push_back(temp2);
		}
		vector<string> divided = TermProject::getInstance()->divide_Line(Inserted);
		Letters = divided;

		setConsoleMsg("Selected Letter is Changed.");
		reset(ConsoleMsg);
	}

};



void TermProject::S(string targetWord) {
	//선택된 letter찾는 함수
	if (targetWord.size() > 75) {
		setConsoleMsg("Wrong input. Please try again.");
		reset(ConsoleMsg);
	}

	int count = 0;
	int countForLine = 0;
	string buffer = "";
	string newString = "";
	string temp2;

	vector<string> Inserted;
	for (vector<string>::iterator i = Letters.begin(); i != Letters.end(); i++) {
		vector<string> temp = split(Letters[count], ' ');
		for (int j = 0; j < temp.size(); j++) {
			if (temp[j] == targetWord) {
				this->MIN_LINE = count;
				countForLine = count;
				break;
			}
		}
		count++;
	}
	if (countForLine == 0) {
		setConsoleMsg("Selected Letter is not exist. Please try again.");
		reset(ConsoleMsg);
	}
	else {
		setConsoleMsg("Found the selected Letter!");
		reset_For_S(ConsoleMsg, (this->MIN_LINE - 1));
	}

};

void TermProject::reset_For_S(string consoleMsg, int minline) {
	consoleMsg = TermProject::consoleMsg();
	TermProject::getInstance()->viewByLine(MIN_LINE);
	TermProject::getInstance()->viewByPage(consoleMsg);
}


void TermProject::T() {
	//저장한 후 종료하는 함수
	ofstream out("./test.txt");
	if (out.is_open()) {
		for (auto i : Letters) {
			out << i;
			out << " ";
		}
	}
	setConsoleMsg("Saved.");
	TermProject::getInstance()->viewByLine();
	cout << MAINLINE << endl;
	cout << MAINMENU << endl;
	cout << MAINLINE << endl;
	cout << "(콘솔메시지)" << consoleMsg() << endl;
	cout << MAINLINE << endl;
	exit(0);
};
//==========================================================================


int main() {
	vector <string> FullMsg;
	string temp;
	ifstream OpenFile;
	OpenFile.open("./test.txt", ios_base::in); //파일을 열고 txt를 받아온다.
	OpenFile.seekg(0, ios::end); //파일끝까지 탐색

	int end = OpenFile.tellg(); //전체 글자수 리턴
	OpenFile.clear();
	temp.resize(end);
	OpenFile.seekg(0, ios::beg);

	OpenFile.read(&temp[0], end);

	string buffer;
	stringstream stringstream(temp);

	//버퍼와 sstream을 이용하여 가져온 텍스트를 뿌려줌.
	while (stringstream >> buffer) {
		FullMsg.push_back(buffer);
	}

	OpenFile.close();
	//텀프로젝트 객체를 하나 생성
	vector<string> divided = TermProject::getInstance()->divide_Line(FullMsg);
	TermProject::getInstance()->setMyLetters(divided);
	//생성한 객체를 각각 함수에 넣어 화면에 뿌려줌.
	TermProject::getInstance()->viewByLine();
	TermProject::getInstance()->viewByPage(TermProject::getInstance()->consoleMsg());

	while (1) {
		//생성된 인스턴스를 커맨드 센터를 통해 명령을 제어함.
		TermProject::getInstance()->CommandCenter(TermProject::getInstance()->Input_Console());
	}
}