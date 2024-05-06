/**********************************************************************************************************************
 *  Author:       Hosam Mohamed
 *  Date:         -
 *  Version:      1.0
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         FILE:  main.cpp
 *
 *  Description: This file is responsible for controlling (Read/Write) from a character device driver.
 *                  Which is under /dev/
 *
 *********************************************************************************************************************/

/************************************* GLOBAL VARIABLES *************************************/
#include <iostream>
#include <fstream>

class ReadWrite
{
private:
    const std::string path;

        
public:
    ReadWrite() = delete;
    ReadWrite(const char *path) : path(path)
    {
    }
    void WriteToDriver(const char *data)
    {
        std::ofstream file;
        file.open(path);
        file << data;
        file.close();
    }
    void ReadFromDriver()
    {
        std::ifstream file;
        file.open(path);
        std::string line;
        while (std::getline(file, line))
        {
            std::cout << line << std::endl;
        }
        file.close();
    }
};

int main()
{
    // First File: enigma_dio1_dir
    ReadWrite enigma_dio1_dir("/dev/enigma_dio1_dir");
    // Second File: enigma_char4
    ReadWrite enigma_char4("/dev/enigma_char4");

    while(1)
    {
        char input[2];
        std::cout << "Enter 1 to write to enigma_dio1_dir or 2 to read from enigma_char4: \n 3 to exit\n";

        std::cin >> input;

        if (input[0] == '1')
        {
            std::cout << "Enter the data to write to enigma_dio1_dir: ";
            std::cin >> input;
            input[1] = '\n';
            enigma_dio1_dir.WriteToDriver(input);
        }
        else if (input[0] == '2')
        {
            enigma_char4.ReadFromDriver();
        }
        else if (input[0] == '3')
        {
            break;
        }
        else
        {
            std::cout << "Invalid input" << std::endl;
        }
    }
}