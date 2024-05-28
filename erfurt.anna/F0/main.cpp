#include <limits>
#include <string>
#include <functional>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include "Commands.hpp"
#include "Utilites.hpp"

int main(int argc, char* argv[])
{
  if (argc < 2)
  {
     std::cout << "Required filename argument\n";
     return 1;
  }
  std::ifstream fin(argv[1]);
  if (!fin.is_open())
  {
    std::cout << "File cannot be opened\n";
    return 2;
  }

  std::vector< erfurt::Dictionary > dictionaries = erfurt::createDictionaryFromFile(fin);

  using namespace std::placeholders;
  std::map<std::string, std::function<void(std::istream & in, std::ostream & out)>> commands;
  commands["INSERT"] = std::bind(erfurt::makeInsert, _1, std::cref(dictionaries), _2);
  commands["FINDTRANSLATE"] = std::bind(erfurt::makeFindTranslate, _1, std::cref(dictionaries), _2);
  commands["DELETE"] = std::bind(erfurt::makeDelete, _1, std::cref(dictionaries), _2);
  commands["PRINT"] = std::bind(erfurt::makePrint, _1, std::cref(dictionaries), _2);
  commands["ISTRANSLATE"] = std::bind(erfurt::makeIsTranslate, _1, std::cref(dictionaries), _2);
  commands["MERGE"] = std::bind(erfurt::makeMerge, _1, std::cref(dictionaries), _2);
  commands["ADDCOMMON"] = std::bind(erfurt::makeCommon, _1, std::ref(dictionaries), _2);
  commands["ADDUNIQUE"] = std::bind(erfurt::makeUnique, _1, std::ref(dictionaries), _2);
  commands["ADDTRANSLATE"] = std::bind(erfurt::makeAddTranslate, _1, std::cref(dictionaries), _2);
  commands["PRINTDICTIONARIES"] = std::bind(erfurt::makePrintDictionaries, _1, std::cref(dictionaries), _2);
  commands["SAVE"] = std::bind(erfurt::makeSave, _1, std::cref(dictionaries), _2);
  commands["OPEN"] = std::bind(erfurt::makeOpen, _1, std::ref(dictionaries), _2);
  commands["HELP"] = std::bind(erfurt::makeHelp, _1, std::cref(dictionaries), _2);
  commands["CREATEDICTIONARY"] = std::bind(erfurt::makeCreate, _1, std::ref(dictionaries), _2);

  std::cout << "\nDictionaries: ";
  erfurt::makePrintDictionaries(std::cin, dictionaries, std::cout);
  std::cout << '\n';
  std::cout << "Commands: " << '\n';
  for (const auto& cmd : commands)
  {
    std::cout << cmd.first << " ";
  }
  std::cout << '\n' << '\n' << "You can use HELP to get help about the command" << '\n' << '\n';
  while (!std::cin.eof())
  {
    std::string command;
    if (std::cin >> command)
    {
      try
      {
        commands.at(command)(std::cin, std::cout);
        std::cout << '\n';
      }
      catch (const std::out_of_range&)
      {
        std::cout << "<INVALID COMMAND>" << '\n';
      }
      catch (const std::logic_error& e)
      {
        std::cout << e.what() << '\n';
      }
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  return 0;
}
