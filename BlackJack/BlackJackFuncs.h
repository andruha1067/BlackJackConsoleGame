// Black Jack Game Function Declarations
#include <vector>
#include <string>

// Check if string entered into menu is a number
bool IsNumeric(const std::string aPlayerInput);

// Displays main menu
void PrintMainMenu();

// Handle main menu input
bool HandleMainMenu(char aPlayerInput);

// Displays 2nd level menu
void PrintMenu();

// Handle 2nd level menu input
bool HandleMenu(std::string aPlayerInput, double &aPlayerTot);

// Displays game menu
void PrintGameMenu();

// Handle game menu input
char HandleGameMenu(char &aPlayerInput);

// Displays game over menu
void PrintGameOverMenu();

// Handle game over menu input
char HandleGameOverMenu(char &aPlayerInput);

void PrintBetMenu();

double HandleGameBet(const double &aPlayerTot);

// Determine score from drawn card
const int MapCardToScore(const std::string & aCard);

// Display dealt cards
void DisplayCards(const std::vector<std::string> &aPlayerCards, const std::vector<std::string> &aDealerCards, const char &aPlayerAction);

// Printing updated game screen with scores
void PrintScore(const int &aPlayerScore, const int &aDealerScore, const char &aPlayerAction);

// Computes scores of both user and dealer based on current hand, and calls PrintScore
const std::vector<int> GetScore(const std::vector<std::string> &aPlayerCards, const std::vector<std::string> &aDealerCards, const char &aPlayerAction);

// Determine is player has black jack
bool IsBlackJack(const int &aPlayerScore);

// Print total amount
void PrintTotAmount(const double &aPlayerTot);

// Calculate and track total amount
bool CalcTotAmount(double &aPlayerTot, const double &aPlayerReturns);

void PrintGameReturns(const double &aWinAmount, const int &aPlayerWinLoseDraw);

// Calculate current bet returns
const double CalcBetReturns(const double &aPlayerBetAmount, const int &aPlayerScore, const int &aPlayerWinLoseDraw, const bool &aBlackJack);

// Determine game result (win, lose, draw)
bool GameOver(const std::vector<int> &aScores, const char &aPlayerHitOrStayAction, int &aPlayerWinLoseDraw, const bool aBlackJack);

// reshuffles card deck after amount of cards left in deck has fallen below a threshold value
void ReshuffleDeck(std::vector<std::string> &aCardDeck, const std::vector<std::string> &aFullDeck);

// Update card deck
void UpdateCardDeck(std::vector<std::string> &aCardDeck, const size_t &aCardIdx);

// Deal cards - Dispenses cards for first deal and every draw after that
void Deal(std::vector<std::string> &aCardDeck, std::vector<std::string> &aPlayerCards);

// Clear player hand vector when new game is begun
void ClearPlayerHand(std::vector<std::string> &aPlayerHand);

void ResetFlagsToNewGame(bool &aFirstDeal, bool &aGameOver, char &aPlayerAction, std::vector<std::string> &aPlayerCards, std::vector<std::string> &aDealerCards);

// Quit function
void PrintQuitMessage(const double &aPlayerTot);

void PrintCashoutMessage(const double &aPlayerTot);