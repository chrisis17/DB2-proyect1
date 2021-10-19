#include <iostream>
#include <fstream>
using namespace std;

std::string getFile()
{
  std::ifstream file("imdb.csv");
  std::string line;
  std::string result = "";
  std::string cut;
  while (std::getline(file, line))
  {
    cout << line << endl;
    cut = line.substr(0, line.size() - 10);
    cut += '|';
    cout << cut << endl;
  }
  file.close();
  return result;
}

int main()
{

  string a = getFile();
  return 0;
}