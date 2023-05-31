#include "SortHandler.h"

int main()
{
    // Create the SFML window with fixed size
    sf::RenderWindow window(sf::VideoMode(1000, 500),
                            "Sorting settings", sf::Style::Titlebar | sf::Style::Close);

    sf::Font font;
    if (!font.loadFromFile("C:/Users/milek/OneDrive/Pulpit/frontfun/c_cpp/cpp_4_sem/SortVisualizer/arial.ttf"))
    {
        std::cout << "Error loading font!" << std::endl;
        return 1;
    }

    // Create buttons for each sorting algorithm
    std::vector<sf::RectangleShape> buttons(10);
    std::vector<sf::Text> buttonTexts(10);

    int buttonWidth = 150;
    int buttonHeight = 50;
    int buttonSpacing = 25;
    int windowWidth = static_cast<int>(window.getSize().x);
    int windowHeight = static_cast<int>(window.getSize().y);
    int buttonX = (windowWidth - buttonWidth * 5 - buttonSpacing * 4) / 2;
    int buttonY = (windowHeight - buttonHeight * 2 - buttonSpacing) / 2;
    int buttonIndex = 0;

    // Fast look up tables
    sf::Color colors[10] = {
            sf::Color::Red,
            sf::Color::Green,
            sf::Color::Blue,
            sf::Color::Yellow,
            sf::Color::Magenta,
            sf::Color::Cyan,
            sf::Color(255, 127, 0),
            sf::Color(255, 0, 127),
            sf::Color(127, 255, 0),
            sf::Color(0, 127, 255)
    };

    std::string algorithms[10] = {
            "BUBBLESORT",
            "SELECTIONSORT",
            "INSERTIONSORT",
            "QUICKSORT",
            "MERGESORT",
            "SHELLSORT",
            "COMBSORT",
            "HEAPSORT",
            "RADIXSORT",
            "COUNTINGSORT"
    };

    int presets[10][2] = {
            {15000, 100}, // Bubble Sort
            {1000, 1000}, // Selection Sort
            {25000, 100}, // Insertion Sort
            {10000, 100}, // Quick Sort
            {25000, 100}, // Merge Sort
            {3000, 100},  // Shell Sort
            {10000, 100}, // Comb Sort
            {5000, 100},  // Heap Sort
            {10000, 100}, // Radix Sort
            {500, 1000}   // Counting Sort
    };

    for (int row = 0; row < 2; row++)
    {
        for (int col = 0; col < 5; col++)
        {
            buttons[buttonIndex].setSize(sf::Vector2f(static_cast<float>(buttonWidth),
                                                      static_cast<float>(buttonHeight)));
            buttons[buttonIndex].setPosition(static_cast<float>(buttonX + (buttonWidth + buttonSpacing) * col),
                                             static_cast<float>(buttonY + (buttonHeight + buttonSpacing) * row));
            buttons[buttonIndex].setFillColor(colors[buttonIndex]);

            buttonTexts[buttonIndex].setFont(font);
            buttonTexts[buttonIndex].setString(algorithms[buttonIndex]);
            buttonTexts[buttonIndex].setCharacterSize(16);
            buttonTexts[buttonIndex].setFillColor(sf::Color::Black);
            buttonTexts[buttonIndex].setOrigin(buttonTexts[buttonIndex].getLocalBounds().left + buttonTexts[buttonIndex].getLocalBounds().width / 2.0f,
                                               buttonTexts[buttonIndex].getLocalBounds().top + buttonTexts[buttonIndex].getLocalBounds().height / 2.0f);
            buttonTexts[buttonIndex].setPosition(buttons[buttonIndex].getPosition().x + static_cast<float>(buttonWidth) / 2,
                                                 buttons[buttonIndex].getPosition().y + static_cast<float>(buttonHeight) / 2);
            buttonIndex++;
        }
    }

    // Generate text to show in input Boxes
    sf::Text textHigher, textSleep;
    textHigher.setPosition(300, 100);
    textHigher.setFont(font);
    textHigher.setCharacterSize(24);
    textHigher.setFillColor(sf::Color::Black);

    sf::Text textHigherInfo("Size n                                                                           "
                            "Sleep interval [us]", font, 24);
    textHigherInfo.setFillColor(sf::Color::Black);
    textHigherInfo.setPosition(175, 100);

    textSleep.setPosition(550, 100);
    textSleep.setFont(font);
    textSleep.setCharacterSize(24);
    textSleep.setFillColor(sf::Color::Black);

    // Generate input Boxes
    sf::RectangleShape inputBoxHigher(sf::Vector2f(200, 30));
    inputBoxHigher.setPosition(300, 100);
    inputBoxHigher.setFillColor(sf::Color::White);
    inputBoxHigher.setOutlineThickness(2);
    inputBoxHigher.setOutlineColor(sf::Color::Black);
    sf::RectangleShape inputBoxSleep(sf::Vector2f(200, 30));
    inputBoxSleep.setPosition(550, 100);
    inputBoxSleep.setFillColor(sf::Color::White);
    inputBoxSleep.setOutlineThickness(2);
    inputBoxSleep.setOutlineColor(sf::Color::Black);

    bool activeHigher = false;
    bool activeSleep = false;

    while (window.isOpen())
    {
        sf::Event event{};
        while (window.pollEvent(event))
        {
            switch(event.type)
            {
                case sf::Event::TextEntered:
                {
                    if (activeHigher)
                    {
                        if (event.text.unicode >= 48 && event.text.unicode <= 57) // check if the entered character is a number
                        {
                            textHigher.setString(textHigher.getString() + static_cast<char>(event.text.unicode));
                            // limit the amount of text to 6 numbers
                            textHigher.setString(textHigher.getString().substring(0, 6));
                        }
                    }
                    else if (activeSleep)
                    {
                        if (event.text.unicode >= 48 && event.text.unicode <= 57) // check if the entered character is a number
                        {
                            textSleep.setString(textSleep.getString() + static_cast<char>(event.text.unicode));
                            // limit the amount of text to 6 numbers
                            textSleep.setString(textSleep.getString().substring(0, 6));
                        }
                    }
                    break;
                }
                case sf::Event::MouseButtonPressed:
                {
                    for (int i = 0; i < buttons.size(); i++)
                    {
                        if (buttons[i].getGlobalBounds().contains(sf::Vector2f(
                                static_cast<float>(event.mouseButton.x),
                                static_cast<float>(event.mouseButton.y)
                                )))
                        {
                            const sf::String& higher = textHigher.getString();
                            const sf::String& sleep = textSleep.getString();
                            int valueHigher = higher.isEmpty() ? presets[i][0] : std::stoi(static_cast<std::string>(higher));
                            int valueSleep = sleep.isEmpty() ? presets[i][1] : std::stoi(static_cast<std::string>(sleep));

                            std::cout << algorithms[i] << std::endl;
                            SortHandler newSortHandler(algorithms[i], valueHigher, valueSleep);
                            newSortHandler.sorting();
                        }
                    }
                    activeHigher = (inputBoxHigher.getGlobalBounds().contains(sf::Vector2f(
                            static_cast<float>(event.mouseButton.x),
                            static_cast<float>(event.mouseButton.y)
                            )));
                    activeSleep = (inputBoxSleep.getGlobalBounds().contains(sf::Vector2f(
                            static_cast<float>(event.mouseButton.x),
                            static_cast<float>(event.mouseButton.y)
                            )));
                    break;
                }
                case sf::Event::Closed:
                {
                    window.close();
                    break;
                }
                case sf::Event::KeyPressed:
                {
                    if (event.key.code == sf::Keyboard::Escape)
                        window.close();
                    if (event.key.code == sf::Keyboard::BackSpace)
                    {
                        // If an input box is active, remove the last character from the input box
                        if (activeHigher && !textHigher.getString().isEmpty())
                            textHigher.setString(textHigher.getString().substring(0, textHigher.getString().getSize() - 1));
                        else if (activeSleep && !textSleep.getString().isEmpty())
                            textSleep.setString(textSleep.getString().substring(0, textSleep.getString().getSize() - 1));
                    }
                    break;
                }
            }
        }
        inputBoxHigher.setOutlineColor(activeHigher ? sf::Color::Red : sf:: Color::Black);
        inputBoxSleep.setOutlineColor(activeSleep ? sf::Color::Red : sf::Color::Black);

        window.clear(sf::Color::White);
        // Draw buttons and button texts
        for (int i = 0; i < buttons.size(); i++) {
            window.draw(buttons[i]);
            window.draw(buttonTexts[i]);
        }
        window.draw(inputBoxHigher);
        window.draw(textHigherInfo);
        window.draw(textHigher);
        window.draw(inputBoxSleep);
        window.draw(textSleep);
        window.display();
    }

    return 0;
}