#include <iostream>
#include <cctype>
#include <vector>
#include <string>
#include "BlackJackFuncs.h"

using namespace std;

const int DEALER_HIT_SCORE_LIMIT {17};

enum GAME_STATES
{
    HIT   = 'H',
    STAY = 'S',
    BET   = 'B',
    CASHOUT = 'C',
};


const vector<string> FULL_CARD_DECK { "A_h", "A_d", "A_c", "A_s", "K_h", "K_d", "K_c", "K_s", "Q_h", "Q_d", "Q_c", "Q_s",
                                                            "J_h", "J_d", "J_c", "J_s", "10_h", "10_d", "10_c", "10_s", "9_h", "9_d", "9_c", "9_s",
                                                            "8_h", "8_d", "8_c", "8_s", "7_h", "7_d", "7_c", "7_s", "6_h", "6_d", "6_c", "6_s",
                                                            "5_h", "5_d", "5_c", "5_s", "4_h", "4_d", "4_c", "4_s", "3_h", "3_d", "3_c", "3_s",
                                                            "2_h", "2_d", "2_c", "2_s"};

int main()
{
    const int numCardsEach {2};
    bool playGame {false};
    bool keepPlaying {false};

    const vector<string> cFullDeck {FULL_CARD_DECK};
    
    do // MAIN MENU - while playGame
    {        
        PrintMainMenu();
        
        char mainMenuChoice {};
        cin >> mainMenuChoice;
        
        playGame = HandleMainMenu(mainMenuChoice);
        
        if (!playGame)
        {
            PrintQuitMessage(0);
            break;
        }
        
        do // 2nd MENU - while playGame
        {
            vector<string> cardDeck {FULL_CARD_DECK};
            double totalDollarAmount {};
            
            PrintMenu();
            
            string menuChoice {};
            cin >> menuChoice;

            keepPlaying = HandleMenu(menuChoice, totalDollarAmount);
            
            // if keepPlaying is false, user wants to quit so print quit message and exit do...while loop (program)
            if (!keepPlaying)
            {
                playGame = keepPlaying;
                PrintQuitMessage(totalDollarAmount);
                break;
            }

            // GAME LOOP
            vector<string> playerCards {}; // {cardDeck.at(0), cardDeck.at(6)};
            vector<string> dealerCards {}; // {cardDeck.at(3 + 4*6), cardDeck.at(40)};
            char playerAction {HIT};
            bool gameOver {false};
            bool firstDeal {true};
            bool blackJack {false};
            double betAmount {};
            int playerWinLoseDraw {0};
            
            do
            {              
                vector<int> scores {};
                int playerScore {};
                int dealerScore {};
                
                if (firstDeal)
                {
                    betAmount = HandleGameBet(totalDollarAmount);
                    
                    // First Deal to Player and Dealer
                    for (int dealCount {0}; dealCount < numCardsEach; ++dealCount)
                    {
                        Deal(cardDeck, playerCards); // Deal 2 cards to user
                        Deal(cardDeck, dealerCards); // Deal 2 cards to user
                    }
                    
                    //FirstDeal(cardDeck, playerCards, dealerCards, numCardsEach);
                    scores = GetScore(playerCards, dealerCards, playerAction);
                    playerScore = scores.at(0);
                    blackJack = IsBlackJack(playerScore);
                    firstDeal = false;
                }
                
                DisplayCards(playerCards, dealerCards, playerAction);
                
                scores = GetScore(playerCards, dealerCards, playerAction);
                
                playerScore = scores.at(0);
                dealerScore = scores.at(1);
                
                PrintScore(playerScore, dealerScore, playerAction);
                
                gameOver = GameOver(scores, playerAction, playerWinLoseDraw, blackJack);
                
                if (!gameOver)
                {
                    if (playerAction == HIT)
                    {
                        PrintGameMenu(); // Print game menu options for user to pick, e.g. hit, stay, quit, etc
                        
                        char gameMenuChoice {};
                        cin >> gameMenuChoice;
                        
                        playerAction = HandleGameMenu(gameMenuChoice);
                    }
                    
                    switch (playerAction)
                    {
                        case HIT:
                        {
                            Deal(cardDeck, playerCards);
                            break;
                        }
                        case STAY:
                        {                      
                            if (dealerScore < DEALER_HIT_SCORE_LIMIT)
                            {
                                Deal(cardDeck, dealerCards);
                            }
                            break;
                        }
                    }
                }
                else // if game is over
                {
                    playerScore = scores.at(0);
                    
                    const double returnAmount = CalcBetReturns(betAmount, playerScore, playerWinLoseDraw, blackJack);
                    
                    bool totMoreThanMinBet = CalcTotAmount(totalDollarAmount, returnAmount);
                    
                    char playerGameOverAction {};
                    // if total amount is greater than minimum bet, allow user to choose to play or cashout
                    // otherwise, automatic cashout
                    if (totMoreThanMinBet)
                    {
                        PrintGameOverMenu(); // Print game menu options for user to pick, e.g. hit, stay, quit, etc
                    
                        char cashoutOrBet {};
                        cin >> cashoutOrBet;
                        
                        playerGameOverAction = HandleGameOverMenu(cashoutOrBet);
                    }
                    else
                    {
                        cout << "\n\tYou have less than minimum bet amount" << endl;
                        playerGameOverAction = CASHOUT;
                    }

                    switch (playerGameOverAction)
                    {
                        case BET:
                        {
                            ReshuffleDeck(cardDeck, cFullDeck);
                            ResetFlagsToNewGame(firstDeal, gameOver, playerAction, playerCards, dealerCards);
                            break;
                        }
                        case CASHOUT:
                        {
                            gameOver = true;
                            keepPlaying = false;
                            PrintCashoutMessage(totalDollarAmount);
                            break;
                        }
                        default:
                        {
                            cout << "Do else \"default\" STUFF" << endl;
                            break;
                        }
                    }
                } // end of else gameOver
            } while (!gameOver);
        } while (keepPlaying); // end of 2nd MENU do...while (playGame )
    } while (playGame); // end of MAIN MENU do...while (playGame)

    system("pause");
    
    return 0;
}


//                if (HIT)
//                    calc user score
//                    if (not bust)
//                        update deck/print and handle menu
//                    if (bust)
//                        print bust
//                        start over
//                if (STAY)
//                    update deck
//                    while (dealer score < 17) 
//                        dealer hits
//                        calc dealer score
//                        if (dealer score >= 17 && dealer score <= 21)
//                            dealer stays so break loop
//                        else
//                            dealer busts so break loop
//                
//                - determine winner
//                - start over