#include "parser.h"
#include "interpreter.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

int main_inner(const std::string &source, bool printPretty, const std::string &outputFile = "")
{
    try
    {
        Parser parser(source);
        auto program = parser.parseProgram();

        Interpreter interpreter;
        interpreter.execute(program.get());

        // Generate output string
        std::string jsonOutput = interpreter.getOutput(printPretty);

        // Output to file or console
        if (!outputFile.empty())
        {
            std::ofstream ofs(outputFile);
            if (!ofs)
            {
                std::cerr << "Cannot write to " << outputFile << std::endl;
                return 3;
            }
            ofs << jsonOutput << std::endl;
            std::cout << "Output saved to " << outputFile << std::endl;
        }
        else
        {
            std::cout << jsonOutput << std::endl;
        }
        return 0;
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <script.file> [--pretty] [--output <file.json>]\n";
        return 1;
    }

    bool pretty = false;
    std::string outputFile = "";
    std::string path = argv[1];

    // Parse command line arguments
    for (int i = 2; i < argc; i++)
    {
        std::string arg = argv[i];
        if (arg == "--pretty" || arg == "-p")
        {
            pretty = true;
        }
        else if ((arg == "--output" || arg == "-o") && i + 1 < argc)
        {
            outputFile = argv[i + 1];
            i++;
        }
        else if (arg.substr(0, 9) == "--output=")
        {
            outputFile = arg.substr(9);
        }
    }

    std::ifstream ifs(path);
    if (!ifs)
    {
        std::cerr << "Cannot open " << path << std::endl;
        return 2;
    }
    std::stringstream ss;
    ss << ifs.rdbuf();
    std::string src = ss.str();
    return main_inner(src, pretty, outputFile);
}