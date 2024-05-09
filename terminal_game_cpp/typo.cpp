#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#include <conio.h>
#include <windows.h>
#include <chrono>

using namespace std::chrono;


std::vector<int> random_number_generator();
std::vector<std::string> reading_file(const std::vector<int>& numbers);
std::string concatenate_strings(const std::vector<std::string>& words);
void output(std::string& full_text);
void input(std::string& full_text, int& duration, int& miscount);
void wpm_calculation(int duration, int miscount, std::string& full_text);


int main() {

  std::vector<int> numbers = random_number_generator();
  std::vector<std::string> words = reading_file(numbers);
  std::string str = concatenate_strings(words);
  output(str);

  int duration, miscount;
  input(str, duration, miscount);

  wpm_calculation(duration, miscount, str);

  return 0;
}

std::vector<int> random_number_generator() {

  std::srand((unsigned) time(NULL));
  std::vector<int> numbers;

  for (int i = 0; i <= 15; i++) {

    int random = 1 + (rand() % 2940);
    numbers.push_back(random);
  }

  return numbers;
}

std::vector<std::string> reading_file(const std::vector<int>& numbers) {

  std::ifstream inputFile("dale-chall-word-list.txt");
  std::vector<std::string> words;

  //sorting vector to avoid unnecessary rechecking.
  std::vector<int> sorted_numbers = numbers;
  std::sort(sorted_numbers.begin(), sorted_numbers.end());

  std::string line;
  int current_line = 1;
  int index = 0;

  // reading file, matching number of line and randomly
  // generated number and storaging words into a vector;
  while (std::getline(inputFile, line) && index < sorted_numbers.size()) {

    if (!line.empty() && current_line == sorted_numbers[index]) {

      words.push_back(line);
      index++;
    }
    current_line++;
  }

  inputFile.close();

  return words;
}

// vector to string;
std::string concatenate_strings(const std::vector<std::string>& words) {
  
  std::string full_text;

  for(const std::string& s: words) {
    full_text += s + " ";
  }

  if (!full_text.empty()) {

    full_text.pop_back();
  }

  return full_text;
}

void output(std::string& full_text) {

  // just for beautiful output 
  // clear the terminal 
  std::system("cls");

  std::string result(120, '=');
  std::cout << result << std::endl;
  
  std::cout << full_text;

  std::cout << "|" << result << "|" << std::endl;
}

void input(std::string& full_text, int& duration, int& miscount) {

  std::string input_text;
  char input_char;
  int position = 0;
  miscount = 0;

  // get the console handle
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

  // time measurement
  auto start = high_resolution_clock::now();

  while (input_text.size() < full_text.size()) {
    if (_kbhit()) {
      input_char = _getch();
      std::cout << input_char;
      
      // stop if '+' is pressed
      if (input_char == '+') {
        break;
      }
      // additional position argument, because there's no point in changing position 
      // when backspace is pressed if position is 0.
      else if (input_char == '\b' && position != 0) {
        input_text.pop_back();
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        --position;
      }
      else {
        input_text += input_char;
        // checking whether both strings match. if no add one to misscount
        if (input_text[position] != full_text[position]) {
          SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
          miscount += 1;
        }
        else {
          SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
          ++position;
        }
      }
    }
  }
  auto stop = high_resolution_clock::now();

  duration = duration_cast<milliseconds>(stop - start).count();

  SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

  std::cout << '\n';
}

void wpm_calculation(int duration, int miscount, std::string& full_text) {

  double minutes = duration / 60000.0;
  double effective_chars = full_text.size() - miscount;
  double words_typed = effective_chars / 5.0;

  int final_wpm = words_typed / minutes;

  std::cout << "your wpm is " << final_wpm;
}
