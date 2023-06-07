
/* 
 * File:   main.cpp
 * Author: Ngoc Nguyen
 * Student ID: 2921768
 * Created on April 23, 2023, 2:10 PM
 * Purpose: A simple blackjack program with 52 cards deck
 */

#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <ctime>
#include <list> // using list to store player hand
#include <vector> // using vector to store card deck
#include <iomanip>

using namespace std;

struct Card
{
    int value;
    string cardRank;
    string cardSuit;
};

//a simple struct for player, can also use this for dealer
//

struct Player
{
    int score = 0;
    list<int> cardValue;
    vector<Card> cardOnHand;
};

void initializeCardDecks(vector<Card>& cardDecks);
void printCard(Card card);
void printDeck(vector<Card> cardDecks);
void playerPrint(Player player);

void aceFucntion(Card& ace);
void valueFix(Player& player, int& countAce);
int betCheck(int bet, int chips);

int main(int argc, char** argv)
{
    srand(time(NULL));

    vector<Card> cardDeck;
    string doWhileGameInput;
    string userInput;
    string userHit;

    int chips = 100;
    int bet;

    cout << "WELCOME TO THE CASINO!!!" << endl;
    cout << "I WILL GIVE YOU SOME CHIPS FOR A FRESH START." << endl;

    do
    {
        initializeCardDecks(cardDeck);
        Player dealer;
        Player user;
        int aceCountUser;
        int aceCountDealer;

        cout << " You have: " << chips << " chips" << endl;
        if (chips == 0)
        {
            cout << "You have no Chips left, maybe next time?";
        }
        else
        {
            cout << "It's time to play some BlackJack." << endl;

            cout << "How much would you like to bet?" << endl;
            cin >> bet;

            bet = betCheck(bet, chips);

            chips = chips - bet;

            // In a real game your bet is removed from your total chips and put on the table
            cout << "You bet: " << bet << " chips." << endl;
            cout << "Let's start your adventure." << endl;
            cout << "Shuffling ..." << endl;

            user.cardOnHand.push_back(cardDeck.back());
            user.cardValue.push_back(cardDeck.back().value);
            user.score = user.score + user.cardOnHand.back().value;
            cardDeck.pop_back();

            dealer.cardOnHand.push_back(cardDeck.back());
            dealer.cardValue.push_back(cardDeck.back().value);
            dealer.score = dealer.score + dealer.cardOnHand.back().value;
            cardDeck.pop_back();

            user.cardOnHand.push_back(cardDeck.back());
            user.score = user.score + user.cardOnHand.back().value;
            user.cardValue.push_back(cardDeck.back().value);
            cardDeck.pop_back();

            dealer.cardOnHand.push_back(cardDeck.back());
            dealer.cardValue.push_back(cardDeck.back().value);
            dealer.score = dealer.score + dealer.cardOnHand.back().value;
            cardDeck.pop_back();

            aceCountUser = count(user.cardValue.begin(), user.cardValue.end(), 11);
            valueFix(user, aceCountUser);

            playerPrint(user);
            //case when user have blackjack and dealer doesn't
            if (user.score == 21 && dealer.score != 21)
            {
                cout << "The dealer now showing their card: " << endl;
                printDeck(dealer.cardOnHand);
                cout << "With the score of: " << dealer.score << endl;

                cout << "You have blackjack!!! You won !!!";
                chips = chips + bet * 2.5;
            }
                //both have blackjack
            else if (user.score == 21 && dealer.score == 21)
            {

                cout << "The dealer now showing their card: " << endl;
                printDeck(dealer.cardOnHand);
                cout << "With the score of: " << dealer.score << endl;

                cout << "You and dealer both have blackjack!!" << endl;
                cout << "It's a PUSH" << endl;
                cout << "The bet will now return to you!" << endl;

                chips = chips + bet;
            }
                //the other case
            else
            {
                double probability;

                float prob[] = {9.8, 13.4, 18.0, 23.2, 23.9, 14.3, 5.4, -4.3, -16.9, -16.0};
                cout << "The dealer show you: " << endl;
                printCard(dealer.cardOnHand.front());
                cout << "With the score of: " << dealer.cardValue.front() << endl;
                cout << "\t\tYour advantage with basic strategy is " << prob[dealer.cardOnHand[0].value - 2] << "%";

                int showing = 3;
                int lucky = 21 - user.score;
                if (lucky > 10 && lucky != 11)
                {
                    lucky = 10;
                }

                int numOfLucky = 0;

                if (lucky != 1)
                {
                    if (dealer.cardOnHand[0].value == lucky)
                    {
                        numOfLucky++;
                    }


                    for (int i = 0; i < 2; ++i)
                    {
                        if (user.cardOnHand[i].value == lucky)
                        {
                            numOfLucky++;
                        }
                    }
                }
                    //the case where lucky equal to 1, which means that it's an Ace, but ace has an initial value of 11 then
                    //we compare it with 11 instead
                else
                {
                    if (dealer.cardOnHand[0].value == 11)
                    {
                        numOfLucky++;
                    }


                    for (int i = 0; i < 2; ++i)
                    {
                        if (user.cardOnHand[i].value == 11)
                        {
                            numOfLucky++;
                        }
                    }
                }
                
                if (lucky == 10)
                {
                    probability = (16 - numOfLucky) / static_cast<double> (52 - showing);
                }
                else
                {
                    probability = (4 - numOfLucky) / static_cast<double> (52 - showing);
                }

                cout << "\n\t\tThe probability of you getting " << lucky << " as your next card is " << fixed << setprecision(3) << (probability * 100) << "%." << endl;
                cout << "Would you like another card?(Y/N)";
                cin >> userHit;

                while (tolower(userHit[0]) == 'y' && user.score <= 21 && tolower(userInput[0]) != 'y')
                {
                    cout << "Drawing a card...";
                    user.cardOnHand.push_back(cardDeck.back());
                    user.cardValue.push_back(cardDeck.back().value);
                    user.score = user.score + user.cardOnHand.back().value;
                    cardDeck.pop_back();
                    //if that is an Ace, ace count + 1;
                    if (user.cardValue.back() == 11)
                    {
                        aceCountUser++;
                    }
                    valueFix(user, aceCountUser);
                    playerPrint(user);

                    if (user.score < 21)
                    {
                        showing++;
                        lucky = 21 - user.score;
                        numOfLucky = 0;

                        if (lucky != 1)
                        {
                            if (dealer.cardOnHand[0].value == lucky)
                            {
                                numOfLucky++;
                            }


                            for (int i = 0; i < 2; ++i)
                            {
                                if (user.cardOnHand[i].value == lucky)
                                {
                                    numOfLucky++;
                                }
                            }
                        }
                        //the case where lucky equal to 1, which means that it's an Ace, but ace has an initial value of 11 then
                        //we compare it with 11 instead
                        else
                        {
                            if (dealer.cardOnHand[0].value == 11)
                            {
                                numOfLucky++;
                            }


                            for (int i = 0; i < 2; ++i)
                            {
                                if (user.cardOnHand[i].value == 11)
                                {
                                    numOfLucky++;
                                }
                            }
                        }

                        if(lucky == 10)
                        {
                            probability = (16 - numOfLucky) / static_cast<double> (52 - showing);
                        }
                        else
                        {
                            probability = (4 - numOfLucky) / static_cast<double> (52 - showing);
                        }

                        cout << "\n\t\tThe probability of you getting " << lucky
                                << " as your next card is " << fixed << setprecision(3) << (probability * 100) << "%." << endl;

                        cout << "Would you like another card?";
                        cin >> userHit;
                    }
                    else
                    {
                        userHit = 'n';
                    }

                }
                //user done with drawing
                //count the Ace dealer have
                aceCountDealer = count(dealer.cardValue.begin(), dealer.cardValue.end(), 11);
                //fix value
                valueFix(dealer, aceCountDealer);

                cout << "The dealer now showing their card: " << endl;
                printDeck(dealer.cardOnHand);
                cout << "With the score of: " << dealer.score << endl;

                if (user.score > 21 && dealer.score > 17)
                {
                    cout << "You're bust! You lost your bet!";
                }
                else
                {

                    while (dealer.score < 17)
                    {
                        dealer.cardOnHand.push_back(cardDeck.back());
                        dealer.cardValue.push_back(cardDeck.back().value);
                        dealer.score = dealer.score + dealer.cardOnHand.back().value;
                        cardDeck.pop_back();

                        if (dealer.cardValue.back() == 11)
                        {
                            aceCountDealer++;
                        }

                        cout << "Dealer just got: ";
                        printCard(dealer.cardOnHand.back());
                        cout << "Now dealer's score is: " << dealer.score << endl;
                    }

                    if (user.score > 21 && dealer.score =< 21)
                    {
                        cout << "You're bust! You lost your bet!";
                    }
                    else if ((user.score > 21 && dealer.score > 21) || user.score == dealer.score)
                    {
                        cout << "Push!" << "Your bet will now return" << endl;
                        chips = chips + bet;
                    }
                    else if (dealer.score > 21)
                    {
                        cout << "Dealer bust! You won!" << endl;
                        chips = chips + bet * 2;
                    }
                    else if (dealer.score > user.score)
                    {
                        cout << "Dealer won! You lost your bet";
                    }
                    else
                    {
                        cout << "You won!" << endl;
                        chips = chips + bet * 2;
                    }
                }
            }
        }
        cout << "Would you like to play again? (Y/N)" << endl;
        cin >> doWhileGameInput;
    }

    while (tolower(doWhileGameInput[0]) == 'y' && chips != 0);

    return 0;
}

/**
 * A function to create card deck and shuffle
 * 
 * @param cardDecks
 */
void initializeCardDecks(vector<Card>& cardDecks)
{
    for (int i = 1; i <= 52; i++)
    {
        //create a temporary card
        Card cardTemp;
        //Assign cardRank and Value
        //Ace
        if (i % 13 == 1)
        {
            cardTemp.value = 11;
            cardTemp.cardRank = "Ace";
        }
            //Two
        else if (i % 13 == 2)
        {
            cardTemp.value = 2;
            cardTemp.cardRank = "Two";
        }
            //Three
        else if (i % 13 == 3)
        {
            cardTemp.value = 3;
            cardTemp.cardRank = "Three";
        }
            //Four
        else if (i % 13 == 4)
        {
            cardTemp.value = 4;
            cardTemp.cardRank = "Four";
        }
            //Five
        else if (i % 13 == 5)
        {
            cardTemp.value = 5;
            cardTemp.cardRank = "Five";
        }
            //Six
        else if (i % 13 == 6)
        {
            cardTemp.value = 6;
            cardTemp.cardRank = "Six";
        }
            //Seven
        else if (i % 13 == 7)
        {
            cardTemp.value = 7;
            cardTemp.cardRank = "Seven";
        }
            //Eight
        else if (i % 13 == 8)
        {
            cardTemp.value = 8;
            cardTemp.cardRank = "Eight";
        }
            //Nine
        else if (i % 13 == 9)
        {
            cardTemp.value = 9;
            cardTemp.cardRank = "Nine";
        }
            //Ten
        else if (i % 13 == 10)
        {
            cardTemp.value = 10;
            cardTemp.cardRank = "Ten";
        }
            //Jack
        else if (i % 13 == 11)
        {
            cardTemp.value = 10;
            cardTemp.cardRank = "Jack";
        }
            //Queen
        else if (i % 13 == 12)
        {
            cardTemp.value = 10;
            cardTemp.cardRank = "Queen";
        }
            //King
        else if (i % 13 == 0)
        {
            cardTemp.value = 10;
            cardTemp.cardRank = "King";
        }
        //now done with card value and rank, need to do the suit
        if (i >= 1 && i <= 13)
        {
            cardTemp.cardSuit = "Spades";
        }
        else if (i >= 14 && i <= 26)
        {
            cardTemp.cardSuit = "Clubs";
        }
        else if (i >= 27 && i <= 39)
        {
            cardTemp.cardSuit = "Diamonds";
        }
        else
        {
            cardTemp.cardSuit = "Heart";
        }
        //push the the deck
        cardDecks.push_back(cardTemp);
    }

    random_shuffle(cardDecks.begin(), cardDecks.end());

}

/**
 * Just a function to print out a card
 * @param card
 * print out in this format: "cardRank of cardSuits. Value: value
 */
void printCard(Card card)
{
    cout << card.cardRank << " of " << card.cardSuit;
    cout << endl;
}

/**
 * a function to print out cardDecks this is just for checking not actually use in game
 * @param cardDecks
 */
void printDeck(vector<Card> cardDecks)
{
    for (int i = 0; i < cardDecks.size(); i++)
    {
        printCard(cardDecks[i]);
    }
}

/**
 * Function That checks the bet the player made to make sure the player cannot bet
 * more than they have or negative number
 * @param bet the amount of chips the player will bet
 * @param chips that the player currently has
 * @return the correct bet amount.
 */
int betCheck(int bet, int chips)
{
    while (cin.fail())
    {
        cin.clear();
        cin.ignore(256, '\n');
        cout << "Please enter a valid input";
        cin >> bet;
    }

    if (bet <= 0 || bet > chips)
    {
        do
        {
            cout << "OOPS, I BELIEVE THAT WAS A MISTAKE.";
            cout << "YOU CAN'T BET NEGATIVE OR MORE CHIPS THAN YOU OWN!!" << endl;
            cout << " " << endl << "How many chips do you want to bet?" << endl;
            cin >> bet;
        }
        while (bet <= 0 || chips < bet);
    }

    return bet;
}

/**
 * a function to print out the value and the card on the player hand
 * @param player
 */
void playerPrint(Player player)
{
    cout << endl;
    cout << "You have: " << endl;
    printDeck(player.cardOnHand);
    cout << "And your score is: " << player.score;
    cout << endl;
    cout << endl;
}

/**
 * a function to fix the score of player if they have ace 
 * An Ace will have a value of 11 
 * unless that would give a player or the dealer a score in excess of 21;
 * in which case, it has a value of 1.
 * @param player
 */
void valueFix(Player& player, int& countAce)
{
    while (player.score > 21 && countAce != 0)
    {
        player.score = player.score - 10;
        countAce--;
    }
}
