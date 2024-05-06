/**********************************************************************************************************************
 *  Author:       Hosam Mohamed
 *  Date:         -
 *  Version:      1.0
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         FILE:  GPIO_ButtonLED.cpp
 *
 *  Description: This file is responsible for controlling LEDs using button by (Read/Write) from GPIO files.
 *
 *********************************************************************************************************************/

/************************************* GLOBAL VARIABLES *************************************/
#include <iostream>
#include <fstream>

class ReadWrite
{
private:
    const std::string path;
    std::fstream file;

public:
    ReadWrite() = delete;
    ReadWrite(const char *path) : path(path)
    {
    }
    void WriteToFile(std::string data)
    {
        file.open(path, std::ios::out);
        file.write(data.c_str(), data.size());
        file.close();
    }
    std::string ReadFromFile()
    {
        file.open(path, std::ios::in);
        std::string line;
        std::getline(file, line);
        file.close();
        return line;
    }
};

class Display
{
private:
    static bool buttonState;

public:
    Display()
    {
    }
    static void DisplayOnChange(std::string val)
    {
        if (val.find("1") != std::string::npos && buttonState == false)
        {
            std::cout << "Button Pressed and LED is ON \n";
            buttonState = true;
        }
        else if (val.find("0") != std::string::npos && buttonState == true)
        {
            std::cout << "Button Released and LED is OFF \n";
            buttonState = false;
        }
    }
};

bool Display::buttonState = false;

int main()
{
    /* First File: button */
    ReadWrite button("/dev/enigma_char3");
    /* Second File: led */
    ReadWrite led("/dev/enigma_char4");

    while (1)
    {
        std::string input = button.ReadFromFile();
        led.WriteToFile(input);

        Display::DisplayOnChange(input);
    }
}