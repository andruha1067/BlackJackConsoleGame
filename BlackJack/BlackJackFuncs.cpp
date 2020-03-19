// Black Jack Game Function Declarations
#include <iostream>
#include <cctype>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cmath>

using namespace std;

const int RESHUFFLE_AMNT {10};
const int MIN_BUY_IN {50};
const int MIN_BET_DOLLARS {10};
const int DEALER_HIT_SCORE_LIMIT {17};
const int MAX_GAME_SCORE {21};
const double BLACK_JACK_SCORE_FACTOR {1.5};
const vector<int> CARD_TO_SCORE {11, 10, 10, 10, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};

enum GAME_RESULTS
{
    WIN             =  1,
    LOSE            = -1,
    DRAW          =   0,
};

enum GAME_STATES
{
    HIT   = 'H',
    STAY = 'S',
    BET   = 'B',
    CASHOUT = 'C',
};

enum CARD_KEY
{
    ACE = 'A',
    KING = 'K',
    QUEEN = 'Q',
    JACK = 'J',
    TEN = '1',
    NINE = '9',
    EIGHT = '8',
    SEVEN = '7',
    SIX = '6',
    FIVE = '5',
    FOUR = '4',
    THREE = '3',
    TWO = '2',
    ACE_ALT = 'L'
};

enum CARD_SCORE
{
    ACE_SCORE = 11,
    KING_SCORE = 10,
    QUEEN_SCORE = 10,
    JACK_SCORE = 10,
    TEN_SCORE = 10,
    NINE_SCORE = 9,
    EIGHT_SCORE = 8,
    SEVEN_SCORE = 7,
    SIX_SCORE = 6,
    FIVE_SCORE = 5,
    FOUR_SCORE = 4,
    THREE_SCORE = 3,
    TWO_SCORE = 2,
    ACE_ALT_SCORE = 1
};

bool IsNumeric(const string aPlayerInput)
{
    bool isChar {false};
    for (char c : aPlayerInput)
    {
        isChar = isalpha(c);
        if (isChar)
        {
            return false;
        }
    }
    
    // if entire string is numeric (may need to be an int)
    return true;
}

// Displays main menu
void PrintMainMenu()
{
    cout << "\n==================================================" << endl;
    cout << "\n\tWELCOME TO SINGLE DECK BLACK JACK!" << endl;
    cout << "\n==================================================" << endl;
    cout << "\n-------------------- MAIN MENU -------------------" << endl;
    cout << "   (P) Play Game" << endl;
    cout << "   (Q) Quit Game" << endl;
    cout << "\nEnter Selection: ";
}

// Handle main menu input
bool HandleMainMenu(char aPlayerInput)
{
    if (isalpha(aPlayerInput))
    {
        aPlayerInput = toupper(aPlayerInput);
        
        if (aPlayerInput == 'P')
        {
             return true;
        }
    }
    
    return false;
}

// Displays 2nd level of main menu
void PrintMenu()
{
    cout << "\n----------------------- MENU ---------------------" << endl;
    cout << "   (-) Enter Buy-In Amount ($" << MIN_BUY_IN << " min)" << endl;
    cout << "   (Q) Quit Game" << endl;
    cout << "\nEnter Selection: ";
}

// Handle game menu input
bool HandleMenu(string aPlayerInput, double &aPlayerTot)
{
    cout << "\n--------------------------------------------------" << endl;
    // check if string is a number
    if (IsNumeric(aPlayerInput))
    {
        string::size_type sz;
        aPlayerTot = stod(aPlayerInput, &sz);
        
        if (aPlayerTot >= MIN_BUY_IN)
        {
            cout << "Your starting balance: $" << aPlayerTot << endl;
            
            return true;
        }
        else
        {
            aPlayerTot = static_cast<double>(MIN_BUY_IN);
            
            cout << "\n   -> Insufficient amount!" << endl;
            cout << "\nYour starting balance will be $" << aPlayerTot << endl;
            
            return true;
        }
    }
    
    return false;
}

// Displays game menu
void PrintGameMenu()
{
    cout << "\n   (H) Hit" << endl;
    cout << "   (S) Stay" << endl;
    cout << "\nEnter Selection: ";
}

// Handle game menu input
char HandleGameMenu(char &aPlayerInput)
{
    cout << "\n--------------------------------------------------" << endl;
    aPlayerInput = toupper(aPlayerInput);
    
    if (aPlayerInput == HIT)
    {
        cout << "You chose to HIT" << endl;
    }
    else if (aPlayerInput == STAY)
    {
        cout << "You chose to STAY" << endl;
    }
    else // anything other than H, S, B is assumed Quit (Q)
    {
        cout << "   -> Invalid! Defaulting to STAY" << endl;
        aPlayerInput = STAY;
    }
    
    return aPlayerInput;
}

// Displays game over menu
void PrintGameOverMenu()
{
    cout << "\n--------------------------------------------------" << endl;
    cout << "   (B) Place Bet" << endl;
    cout << "   (C) Cash Out" << endl;
    cout << "\nEnter Selection: ";
}

// Handle game over menu input
char HandleGameOverMenu(char &aPlayerInput)
{
    aPlayerInput = toupper(aPlayerInput);
    
    cout << "\n--------------------------------------------------" << endl;
    
    if (aPlayerInput == BET)
    {
        cout << "You chose to BET" << endl;
    }
    else if (aPlayerInput == CASHOUT)
    {
        cout << "You chose to CASHOUT" << endl;
    }
    else // anything other than H, S, B is assumed Quit (Q)
    {
        cout << "   -> Invalid! Defaulting to BET" << endl;
        aPlayerInput = BET;
    }
    
    return aPlayerInput;
}

void PrintBetMenu()
{
     cout << "\nEnter bet amount ($" << MIN_BET_DOLLARS << " min): ";
}

double HandleGameBet(const double &aPlayerTot)
{
    double playerBet {0};
    
    do
    {
        PrintBetMenu();
        
        cin >> playerBet;
        
        playerBet = fabs(playerBet);
        
        if (playerBet < MIN_BET_DOLLARS)
        {
            cout << "\n   -> Insufficient amount!" << endl;
        }
        else if (playerBet > aPlayerTot)
        {
            cout << "\n   -> Bet cannot exceed remaining balance!" << endl;
        }
        
    } while (playerBet < MIN_BET_DOLLARS || playerBet > aPlayerTot);
    
    cout << "\n--------------------------------------------------" << endl;
    cout << "Game bet: $" << playerBet << endl;
    
    return playerBet;
}

// Determine score from drawn card
const int MapCardToScore(const string &aCard)
{
    int score {};
    
    char mapKey = aCard.at(0);
    
    if (mapKey == ACE)
    {
        score = ACE_SCORE;
    }
    else if (mapKey == KING)
    {
        score = KING_SCORE;
    }
    else if (mapKey == QUEEN)
    {
        score = QUEEN_SCORE;
    }
    else if (mapKey == JACK)
    {
        score = JACK_SCORE;
    }
    else if (mapKey == TEN)
    {
        score = TEN_SCORE;
    }
    else if (mapKey == NINE)
    {
        score = NINE_SCORE;
    }
    else if (mapKey == EIGHT)
    {
        score = EIGHT_SCORE;
    }
    else if (mapKey == SEVEN)
    {
         score = SEVEN_SCORE;
    }
    else if (mapKey == SIX)
    {
         score = SIX_SCORE;
    }
    else if (mapKey == FIVE)
    {
         score = FIVE_SCORE;
    }
    else if (mapKey == FOUR)
    {
         score = FOUR_SCORE;
    }
    else if (mapKey == THREE)
    {
         score = THREE_SCORE;
    }
    else if (mapKey == TWO)
    {
         score = TWO_SCORE;
    }
    else
    {
         score = static_cast<int>(ACE_ALT_SCORE);
    }
    
    return score;
}

// Display dealt cards
void DisplayCards(const vector<string> &aPlayerCards, const vector<string> &aDealerCards, const char &aPlayerAction)
{
    cout << "\n   - Your Cards: ";
    
    for (string playerCard : aPlayerCards)
    {
        cout << playerCard << " ";
    }
    
    cout << "\n   - Dealer Cards: ";
    
    size_t lastCardIdx = aDealerCards.size() - 1;
    
    for (size_t i {0}; i < aDealerCards.size(); ++i)
    {
        if (aPlayerAction == HIT && i == lastCardIdx)
        {
            cout << "_X_" << " ";
            
        }
        else
        {
            cout << aDealerCards.at(i) << " ";
        }
    }
    
    cout << endl;
}

// Printing updated game screen with scores
void PrintScore(const int &aPlayerScore, const int &aDealerScore, const char &aPlayerAction)
{
    if (aPlayerAction == STAY)
    {
        cout << "\n   - YOUR SCORE: " << aPlayerScore << ",  DEALER SCORE: " << aDealerScore << endl;
    }
    else
    {
        cout << "\n   - YOUR SCORE: " << aPlayerScore << endl;
    }
}

// Computes scores of both user and dealer based on current hand, and calls PrintScore
const vector<int> GetScore(const vector<string> &aPlayerCards, const vector<string> &aDealerCards, const char &aPlayerAction)
{
    int playerScore {};
    for (string playerCard : aPlayerCards)
    {
        playerScore += MapCardToScore(playerCard);
    }
    
    int dealerScore {};
    for (string dealerCard : aDealerCards)
    {
        dealerScore += MapCardToScore(dealerCard);
    }
    
//    PrintScore(playerScore, dealerScore, aPlayerAction);
    
    const vector<int> scores {playerScore, dealerScore};
    
    return scores;
}

// Determine is player has black jack
bool IsBlackJack(const int &aPlayerScore)
{
    if (aPlayerScore == MAX_GAME_SCORE)
    {
        return true;
    }
    
    return false;
}

// Print total amount
void PrintTotAmount(const double &aPlayerTot)
{
    cout << "\nYour Total: $" << aPlayerTot << endl;
}

// Calculate and track total amount
bool CalcTotAmount(double &aPlayerTot, const double &aPlayerReturns)
{
    aPlayerTot += aPlayerReturns;
    PrintTotAmount(aPlayerTot);
    
    if (aPlayerTot >= MIN_BET_DOLLARS)
    {
        return true;
    }
    
    return  false;
}

void PrintGameReturns(const double &aWinAmount, const int &aPlayerWinLoseDraw)
{
    if (aPlayerWinLoseDraw == WIN)
    {
        cout << "\nYou Won $" << fabs(aWinAmount) << endl;
    }
    else if (aPlayerWinLoseDraw == LOSE)
    {
        cout << "\nYou Lost $" << fabs(aWinAmount);
    }
    else if (aPlayerWinLoseDraw == DRAW)
    {
        cout << "\nYou Won/Lost $" << fabs(aWinAmount);
    }
}

// Calculate current bet returns
const double CalcBetReturns(const double &aPlayerBetAmount, 
                                         const int &aPlayerScore, 
                                         const int &aPlayerWinLoseDraw,
                                         const bool &aBlackJack)
{
    double winAmount {};
    
    if (aPlayerWinLoseDraw == WIN)
    {
        // user got black jack - automatic win
        if (aPlayerScore == MAX_GAME_SCORE  && aBlackJack)
        {
            winAmount = BLACK_JACK_SCORE_FACTOR * aPlayerBetAmount;
        }
        else
        {
            winAmount = aPlayerBetAmount;
        }
    }
    else if (aPlayerWinLoseDraw == LOSE)
    {
        winAmount = LOSE * aPlayerBetAmount; // amount bet is lost
    }
    else if (aPlayerWinLoseDraw == DRAW)
    {
        winAmount = DRAW; // amount bet is lost
    }
    else
    {
        cout << "   - WARNING: THIS IS A BUG" << endl;
    }

    PrintGameReturns(winAmount, aPlayerWinLoseDraw);
    
    return winAmount;
}

// Determine game result (win, lose, draw)
bool GameOver(const vector<int> &aScores, const char &aPlayerHitOrStayAction, int &aPlayerWinLoseDraw, const bool aBlackJack)
{
    //cout << "Determine game result..." << endl;
    int playerScore {aScores.at(0)};
    int dealerScore {aScores.at(1)};
    bool gameOver {false};
    
    if (aBlackJack)
    {
        cout << "\n\tYOU WIN, YOU GOT BLACK JACK!" << endl;
        aPlayerWinLoseDraw = WIN;
        return gameOver = true;
    }
    
    if (playerScore > MAX_GAME_SCORE)
    {
        //cout << playerScore << " " << dealerScore << endl;
        cout << "\n\tYOU LOSE, YOU BUSTED!" << endl;
        aPlayerWinLoseDraw = LOSE;
        return gameOver = true;
    }
    
    if (dealerScore > MAX_GAME_SCORE)
    {
        //cout << playerScore << " " << dealerScore << endl;
        cout << "\n\tYOU WIN, DEALER BUSTED!" << endl;
        aPlayerWinLoseDraw = WIN;
        return gameOver = true;
    }
    
    bool playerStays = (aPlayerHitOrStayAction == STAY);
    
    if (dealerScore >= DEALER_HIT_SCORE_LIMIT 
        && playerStays)
    {        
        if (playerScore > dealerScore)
        {
            cout << "\n\t\tYOU WIN!" << endl;
            aPlayerWinLoseDraw = WIN;
        }
        else if (playerScore < dealerScore)
        {
            cout << "\n\t\tYOU LOSE!" << endl;
            aPlayerWinLoseDraw = LOSE;
        }
        else if (playerScore == dealerScore)
        {
            cout << "\n\t\tIT'S A DRAW!" << endl;
            aPlayerWinLoseDraw = DRAW;
        }
        else
        {
            cout << "\n\tTHIS IS A CATCH-ALL AND SHOULDN'T EXECUTE!!!" << endl;
        }
        
        return gameOver = true;
    }
    
    return gameOver = false;
}

void ReshuffleDeck(vector<string> &aCardDeck, const vector<string> &aFullDeck)
{   
    //cout << "\n\tThere are " << aCardDeck.size() << " cards left in the deck" << endl;
    if (aCardDeck.size() < RESHUFFLE_AMNT)
    {
        cout << "\n\tThere are " << aCardDeck.size() << " cards left in the deck" << endl;
        cout << "\t     Reshuffling the deck..." << endl;
        
        aCardDeck = aFullDeck;
        
        cout << "\n\t       READY TO PLAY!" << endl;
    }
}

// Update card deck
void UpdateCardDeck(vector<string> &aCardDeck, const size_t &aCardIdx)
{
    aCardDeck.erase(aCardDeck.begin() + aCardIdx);
}

// Deal cards - Runs once at the beginning of game (2 and 2)
void Deal(vector<string> &aCardDeck, vector<string> &aPlayerCards)
{
    size_t noCardsInDeck {aCardDeck.size()};
    //cout << "Dealing Cards..." << endl;
    srand(time(nullptr));
    unsigned int randCardIdx = rand() % noCardsInDeck;
    //cout << "Random card number " << randCardIdx << " out of " << noCardsInDeck << endl;
    aPlayerCards.push_back(aCardDeck.at(randCardIdx));
    
    UpdateCardDeck(aCardDeck, randCardIdx); // Update card deck
}

// Clear player hand vector when new game is begun
void ClearPlayerHand(vector<string> &aPlayerHand)
{
    aPlayerHand.clear();
}

void ResetFlagsToNewGame(bool &aFirstDeal, bool &aGameOver, char &aPlayerAction, vector<string> &aPlayerCards, vector<string> &aDealerCards)
{
    aFirstDeal = true;
    aGameOver = false;
    aPlayerAction = HIT;
    ClearPlayerHand(aPlayerCards);
    ClearPlayerHand(aDealerCards);
}

// Quit function
void PrintQuitMessage(const double &aPlayerTot)
{
    cout << "\n--------------------------------------------------" << endl;
    cout << "\n\t    See You Next Time" << endl;
    cout << "\n\t\tGOODBYE!\n" << endl;
}

void PrintCashoutMessage(const double &aPlayerTot)
{
    cout << "\n--------------------------------------------------" << endl;
    //cout << "\n         Welcome to Single Deck Black Jack!" << endl;
    cout << "\n\tYou Have Cashed Out With $" << aPlayerTot << endl;
}