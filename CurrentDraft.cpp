// Name: Scarlett Mitchell-Yang
// Program Name: PLAY: BATTLESHIP
// File Name:   [ Mitchell-Yang_FINAL.cpp ]
// Purpose: Program that allows two players to play Battleship together
// ________ Coding: Ending project for COSC-H212 (Intro to Programming II), with
// ________ ....... the synthesis of learned concepts including:
// ________ ... {pointers}, {structures}, {ASCII conversion}, {dynamic allocation},
// ________ ... {structure pointers}, {passing values by reference}
// Date Created: D/15/04/21
// Last Modified: F/15/05/21

/**************  PROGRAM CONTENTS ***************************************************************************************************************************| 
        -  << File Operations >>

        -  << Structure Definitions >>
                Score       (called once per player)
                Settings    (called 1 time)
                Coordinates (called as a salvo and once per created ship)
                Ship        (called for each type of ship)

        -  << Function Prototypes >>
            ... in order ...  
                Outside of Main
                Called from Within Main
                    - functions used throughout program
                    - functions used in set-up
                    - functions used during the game 

        -  << Running Program >> 
                SECTION M0 - Establish the Settings + Game Mode

                SECTION 00 - Creation of ships and their values
                    Variable Grouping A - Mode initializers: "loopMode"
                    Variable Grouping B - Locators (pointers and integer values)
                    Variable Grouping C - Display Board Creation
                    Variable Grouping D - Creating Ships
                    Variable Grouping E - Pointer Lists
                    Variable Grouping F - Controllers of the Running Program

                SECTION 1 - SETTING UP THE GAME PIECES
                    PART A - Select which ship to set up
                    PART B - Select a coordinate to use
                    PART C - Establishing orientation
                    PART D - Validation
                    PART E -- Calculating player values and creating potential stoppers for future loops

                SECTION 2 - PLAYING THE GAME
                    PART A - Initialize Salvo:
                    PART B - Specifies which player the game is currently running a turn for
                    PART C - RESET / EMPTY SALVO
                    PART D - PROMPT FOR FIRING LOCATIONS
                    PART E - VERIFICATION - Is there a ship in these locations?
                    PART F - DISPLAYING RESULTS
                    PART E -- IS THE GAME WON

                SECTION 3 - VERIFYING WHO WON THE GAME
                    PART A -- WINNER DECLARATION
                    PART B -- RESULTS DISPLAY

        - << Function Definitions >>
            - Display of Messages / Prompts
            - Display of Boards / Representations of the Grid
            - Functions that reset or define the values of certain variables / arrays
            - "getInput_" -- Every type of input that is involved within the program 
            - Functions that validate the existance of a value or situational occurance
            - Search Functions: search or assignment of pre-existing pointers or ship values 
            - SetBoard -- specifically used in Section 1, but are the continued assignment of values after input for placement finishes

|************************************************************************************************************************************************************/

// FILE OPERATIONS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include <iostream>
#include <cctype>
using namespace std;

// STRUCTURES ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// order:   || ...... {Score} >>>>>> {Settings} >>>>>> {Coordinates} >>>>>> {Ship} ...... ||

// Structure that saves the "score" information of each player
struct Score
{   // Essential Values
    string name[2];       // Stores whether the player is: ['USER', 'Player 1', 'COMPUTER', 'Player 2']
    int fleetSize;        // Stores the number of ships in the fleet after one is created by the user
    int numShipsLost = 0; // Stores how many ships are lost -- also ~can~ imply salvo size, depending on the chosen mode
    int maxShots;         // Stores the maximum number of shots that this user can use in the game
    int usedShots = 0;    // Stores the current number of shots this fleet has fired
    // Deciding who is the winner
    int shipsShotDown = 0;       // Saves the number of ships that this player has sunk while firing on the enemy fleet
    int inflictedDamage = 0;     // Saves the amount of damage done by the
    int totalEnemyFleetSunk = 0; // [Ruin-Mode specific] -- Saves the total amount of enemy ships that have sunk
    int sustainedDamage = 0;
    string *fleetDamageReport[3];
};

// Structure that is called only once, and saves the settings of the game
// ... (as selected by the player)
struct Settings
{   // Player information
    Score play1;    // USER / Player 1
    Score play2;    // COMP / Player 2
    // Values that hold the game description
    string gameMode;   // Options: (1) Regular Mode, (2) Salvo Mode, (3) Ruin Mode
    string playerType; // Options: (1) Player 1 vs Player 2, (2) Player vs Computer
    // Loop / Mode initializers: sets up the number of times loops repeat
    int loop_gameSelect;
    int loop_numPlayers;
    int loop_numTurns;
    int *loop_maxshotPtr = NULL;
    int *loop_salvoSizeP = NULL;
    /*   EXPLANATION OF 'loop' VALUES HERE: 
        // --- _gameType: establishes the game type: {'1' = Regular}, {'2' = Salvo}, {'3' = Ruin}
        // --- _numPlayers: "selection" is a double, the ones-place indicating the game mode...
        // ...... and the tenths place representing if its Player vs Player or User vs Computer
        // ...... when _numPlayers = 1, this signifies (User vs Computer), making the next loop occur once
        // ...... when _numPlayers = 2, this signifies (Player vs Player), making the next loop occur twice: once for each player
        // --- _numTurns: defines the number of max-turns of a game, but can be overrided...
        // ....... if  Ruin Mode  is in play and emergency shots are drawn into account.
        // ....... This value is dependent on which mode is being played.
        // --- _maxShots: a pointer to the number of times a player can shoot per turn...
        // ...... minimum is one, but this value is defined later on in the program...
        // ...... The pointer allows for alternation between the two differen 'modes' during...
        // ...... a single turn, signified in the struct 'Score' 
        // ...... --- Address is in between: [settings.play1.maxShots] OR [settings.play2.maxShots]
        // --- _salvoSizeP: a pointer to the size of a ship salvo, strictly dependent on the ship size
        // ...... IF the game mode is NOT on Regular (in that case, the salvo is always 1, and it's ...
        // ...... directed to variable 'regularSalvo'). Otherwise the addresses are between: 
        // ...... --- [settings.play1.fleetSize] OR [settings.play2.fleetSize]
    */
};

// Structure that saves coordinates in character form and integer form ...
// ... (along with special storage values for its "salvo" call)
struct Coordinates
{   // Used for the establishment of each inidividual coordinates
    char orient;            // Either 'V' or 'H'
    int aim[6][3];          // Recording of the element-based equivalents of the (char) coordinates
    char coord[6][7];       /* Description on usage below:  .................................. // 
                                // ___NOTE___ if element '1' is ZERO, that value equals '10'
                                // Type 1: Recorded element values of the ship 
                                // ...... Examples: "C3-22", "A090", "D6-53", "J1-09", "F095"
                                // ... elements 0-2 = (Our denomination of the coordinates), 
                                // ... elements 4-5 = (character values of the array elements)
                                // Type 2: Records the element values of the salvo and the message of whether a shot hit or missed
                                // ...... Examples: "C3MISS", "A0HIT!"
                                // ... elements 0-2 = (Our denomination of the coordinates), 
                                // ... elements 3-5 = (the message of whether or not it hit)
                            /* .............................................................. */
    // Specifically for salvo:
    string hitShipName[6]; // The recorded order of misses / ship hits, including "NULL" or the ship's name
    int hitShip[6];        // The recording of the element-coordinates of the ships
    int recentlySunk[6];   // Specifically for displays: describes how many ships sunk in a single turn
};

// Structure that represents any individual ship
struct Ship
{   // Specifications
    string name;
    char ship[6];
    // Health and damage
    int size;
    int hits = 0;
    bool sunk = false;
    // Position
    Coordinates pos;
    // Additional: [Ruin-Mode specific]
    bool sinking;
    int damage = 0;
    int turnsLeft;
};


// FUNCTION PROTOTYPES ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ... Ordered here in the order in which they are called ...
// ------ (called outside MAIN) ------  // used within the functions //  ------------------------------------------------------------------------------------
/* Rm0 */  void flagReset(char *input, int inputBound, bool flagError1, bool flagError2);
/* Rb0 */  bool flag_confirmBadInput(char *input, int inputBound, char alternateCommand, char boundary1, char boundary2);
/* Ib1 */  int shipCoordError(char *inp, int *coord, int size);
/* Ib0 */  bool coordError(char *inp, int *coord);
/* Sb3 */  int verifyShipPlacement(char *inp, int (*hitboard)[6], int numShips, bool outBounds, bool *orientError);
// ------ (called within MAIN) -------  // used throughout the program //  ----------------------------------------------------------------------------------
/* Pa0 */  void wipeBoard(string type, char boardFill[10][11], char (*boardCopy)[11]);
/* Od0 */  void Pause_Program(int PATHWAY);
/* Pp0 */  Ship *assignPointer(Ship *listOfPointers[5], char inpChar, string shipElem, int loopUpdate);
/* Od1 */  void displayBriefSetUP(int numPlayers, int numTurns, int player);
/* Imy */  char getInput_yesno();
/* Ps0 */  int linearSearch_aim(char *inp, int (*hitboard)[6], int numShips, int aimPtr[3]);
/* Pi0 */  int incrementOne(int value);
// ------ (called within MAIN) -------  // used in the game set-up //  --------------------------------------------------------------------------------------
/* Ia0 */  Settings Launch_Program(int pathway, bool onlyCreatedPvP);
/* Sa1 */  Ship createShip(int type, int numUsers, int (*hitboard)[6]);
/* xxx */  void gameEndValsReset(bool gameWin[2], bool fleetDestroyed[2]);
/* Id1 */  void displayBoard1(char (*board)[11], string heading);
/* Od2 */  void displayShipOptions();
/* Im1 */  void getInput_fetchShip(char *inpFill);
/* Im2 */  void getInput_placeShipCoord(char *inpFill, int *coordFill, int shipSize);
/* Im3 */  void getInput_shipOrient(char *inpFill, int *coordPivot, int shipSize, int (*hitboard)[6], int numShips, string *shipOrder);
/* Ps1 */  int linearSearch_prime(char *inp, int (*hitboard)[6], int check);
/* Sa2 */  void setBoard_charFill(char orient, int index, char *coordFill, char *fPtr, int *coordPivot);
/* Sa3 */  void setBoard_structFill(char orient, Ship *shipPtr, int shipElem, int shipSize, int (*hitboard)[6], char (*boardFill)[11], int *coordPivot);
// ------ (called within MAIN) -------  // used in the gameplay //  -----------------------------------------------------------------------------------------
/* Rl1 */  Coordinates salvoReset();
/* Id5 */  void displayShipSink(string type, Coordinates *salvoPtr);
/* Pi1 */  void fleetDamageIncrement(Coordinates *salvoPtr, Ship *listOfPointers[5], string *fleetOrder, int reviewSize, Score *playerPtr, Score *playerAimPtr);
/* Sb4 */  bool shipSunkFromDamage(Ship *shipPtr, Score *playerPtr, Score *playerAimPtr, Coordinates *salvoPtr, int specificShot);
/* Id2 */  void displayBoard2(char (*boardU)[11], char (*boardC)[11], string headingU, string headingC);
/* Im0 */  void getInput_fillShot(char *inpPt, int *coord, int shotcount, int *usedShots);
/* Ip0 */  void setSalvo_messageFill(string type, Coordinates *salvo, char *salvoCoord, int rowElm);
/* Ra2 */  void shipSunk_changingValues(Ship *shipPtr, Score *playerPtr, Score *playerAimPtr, Coordinates *salvoPtr, int crntShot);
/* Id3 */  void displayEnemyMove(char *salvoCoord, string hitshipName);
/* Id4 */  void displayUserMove(char *salvoCoord, string hitshipName);
/* Pb0 */  bool wasFleetDestroyed(Score *playerPtr, Score *playerAimPtr, int gameMode);
/* Id5 */  void assignGameWinner(Score *player1, Score *player2, Score *playerPtr, Score *playerAimPtr, int gameMode, bool fleetDestroyed[2], bool *stalemate, Score *playerWin, Score *playerLose);
/* xxx */  void assignFleetWin(Score *playerPtr, bool *fleetDestroyed, Score *addressP1, Score *addressP2);
/* xxx */  void assignPlayerWinLose(Score *playerWin, Score *playerLose, Score **addressWin, Score **addressLose);
/* Id6 */  void displayGameResults(Score *playerPtr, Score *playerAimPtr, Ship *ptrListU[5], Ship *ptrListC[5]);




//_________________________________________________________________________________________________________________________________________________________________________________
//_________________________________________________________________________________________________________________________________________________________________________________
//_________________________________________________________________________________________________________________________________________________________________________________
int main()
{
    // SECTION M0 - Establish the Settings + Game Mode ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    bool onlyCreatedPvP = true; // Flag: "only created the player vs player version"
    Settings mode = Launch_Program(0, onlyCreatedPvP);

    
    
    // SECTION 00 - Creation of ships and their values ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
    // Variable Grouping A - Mode initializers: "loopMode" ...............................................................................................................
    if (mode.loop_gameSelect != 1)
        mode.loop_numTurns = (5 + (mode.play1.maxShots / 5)); // ie: 25 turns
    else if (mode.loop_gameSelect == 1)
        mode.loop_numTurns = mode.play1.maxShots; // ie: 50 turns


    // Variable Grouping B - Locators (pointers and integer values) ......................................................................................................
    // --- independent, adaptable uses ----------------------------
    Ship *shipPtr = NULL;                 // Pointer to the current particular ship that will be analyzed / altered
    int *crntCoordSelection = new int[4]; // Pointer that allows for the easy transfer of coordinates (in array-element format) between MAIN and 'getInput_' function
    Score *playerPtr = NULL;              // The pointer to the current player
    Score *playerAimPtr = NULL;           // The pointer to the player that is not currently playing
    int salvoSizeU = -1;                  // Value that holds the calculated size of the player's salvo
    int salvoSizeC = -1;
    int crntShipSize;               // Tracker of the size of the ship currently chosen
    char(*brdPtr)[11] = NULL;       // Pointer to the board display of the player currently playing
    char(*brdRecordPtr)[11] = NULL; // Pointer to the board display of the player NOT currently playing
    char(*brdAimPtr)[11] = NULL;    // Pointer to the board display (without ships) of the player NOT currently playing
    char(*brdTempPtr)[11] = NULL;   // Pointer to the board display (without ships) -- of saved shot locations -- of the player NOT currently playing
    // --- connected, dependent uses -------------------------------
    int regularSalvo = 1; // [Regular Mode specific] - Automatic salvo size (of 1)
    int shipExists = -1;  // Base integer for the below values, which identifies which ship has been located by the 'linearSearch_' functions
    Ship *(*shipPointersList[6]);
    string *fleetOrderPtr = NULL; // Recording of the order at which the user creates and places the ships.
                                  // ... The values within the array are the titles of each ship
    string *fleetOrderAimP = NULL;
    int(*hitboardPtr)[6] = NULL;    /**** Description of importance of variables:   *******************************************************************************| 
                                            // Similar to 'fleetOrderPtr', this is a recording of the order at which the user
                                            // ... creates and places their ships. 
                                            // 'hitboardPtr[#][0]' is always the size of the ship
                                            // 'hitboardPtr[#][1]' + 'hitboardPtr[#][2]' are the beginning coordinates of the ship
                                            // 'hitboardPtr[#][3]' + 'hitboardPtr[#][4]' are the ending coordinates of the ship
                                            // The coordinates are integer values that serve as array elements within the code.
                                            // The exact order of the ships is dictated by 'fleetOrderPtr' and the row-value of 'hitboardPtr',  
                                            // ... which is important for the Cruiser and Submarine, since they have the same size.
                                            // ... Specific ships can be specified thru the 'linearSearch_' functions and variable 'shipExists'
                                        /*****************************************************************************************************************************/
    int(*hitboardAimPtr)[6] = NULL; // ... ^^ This pointer serves the same purpose as the one above, just for the player NOT currently playing
    int hitboardU[6][6] = {{-2, -1, -1, -1, -1}, {-2, -1, -1, -1, -1}, {-2, -1, -1, -1, -1}, {-2, -1, -1, -1, -1}, {-2, -1, -1, -1, -1}};
    int(*hitPtrU)[6] = hitboardU;
    int hitboardC[6][6] = {{-2, -1, -1, -1, -1}, {-2, -1, -1, -1, -1}, {-2, -1, -1, -1, -1}, {-2, -1, -1, -1, -1}, {-2, -1, -1, -1, -1}};
    int(*hitPtrC)[6] = hitboardC;


    // Variable Grouping C - Display Board Creation .....................................................................................................................
    // PERMENANT BOARDS - Shows ships and the enemy's fire
    char boardUSER[10][11]; // for User --------------------------------------------------------
    wipeBoard("NewBoard", boardUSER, NULL);
    char boardCOMP[10][11]; // for Computer ----------------------------------------------------
    wipeBoard("NewBoard", boardCOMP, NULL);
    // TEMPORARY BOARDS
    // Group 1 - Boards that illustrate where shots are fired and if they hit (without ship markers)
    char boardAimUSER[10][11]; // for User: -------------------------------------------
    wipeBoard("NewBoard", boardAimUSER, NULL);
    char boardAimCOMP[10][11]; // for Computer: ---------------------------------------
    wipeBoard("NewBoard", boardAimCOMP, NULL);
    // Group 2 - Duplicate of Group 1 boards, except the firing spots vanish
    char boardTempUSER[10][11]; // for User: ------------------------------------------
    wipeBoard("NewBoard", boardTempUSER, NULL);
    char boardTempCOMP[10][11]; // for Computer: --------------------------------------
    wipeBoard("NewBoard", boardTempCOMP, NULL);


    // Variable Grouping D - Creating Ships ..............................................................................................................................
    // --------- the base creation of ships, for the (USER's / PLAYER1's) fleet
    string fleetOrderU[6]; // Pointer that will hold the order by which these ships are created
    Ship destrU = createShip(1, 0, NULL);
    Ship submU = createShip(2, 0, NULL);
    Ship cruisU = createShip(3, 0, NULL);
    Ship battlU = createShip(4, 0, NULL);
    Ship carrU = createShip(5, 0, NULL);
    // --------- the base creation of ships, for the (COMPUTERS's / PLAYER2's) fleet
    string fleetOrderC[6]; // Pointer that will hold the order by which these ships are created
    Ship destrC = createShip(1, mode.loop_numPlayers, NULL);
    Ship submC = createShip(2, mode.loop_numPlayers, NULL);
    Ship cruisC = createShip(3, mode.loop_numPlayers, NULL);
    Ship battlC = createShip(4, mode.loop_numPlayers, NULL);
    Ship carrC = createShip(5, mode.loop_numPlayers, NULL);


    // Variable Grouping E - Pointer Lists .............................................................................................................................
    Ship *shipPointersU[6];
    shipPointersU[0] = &destrU;
    shipPointersU[1] = &submU;
    shipPointersU[2] = &cruisU;
    shipPointersU[3] = &battlU;
    shipPointersU[4] = &carrU;
    shipPointersU[6] = NULL;
    Ship *shipPointersC[6];
    shipPointersC[0] = &destrC;
    shipPointersC[1] = &submC;
    shipPointersC[2] = &cruisC;
    shipPointersC[3] = &battlC;
    shipPointersC[4] = &carrC;
    shipPointersC[6] = NULL;


    // Variable Grouping F - Controllers of the Running Program ..........................................................................................................
    char *fetchInputPtr = new char[4]; // The secondary character array that is analyzed after user input -
                                       // ... the primary is getInput() dependent and insesitive to errors; valid answers placed in the primary-input...
                                       // ... is afterwards placed into "fetchInputPtr" and moved to the running program for application.
    char response = '-';               // Character value connected to the call of 'menu_yesno()'
    int c_numShips = 0;                // During SECTION 1, is a counter of how many ships have been created so far
    bool returntoCoord = false;        // During SECTION 1, allows for the user to return to their coordinate placement
    bool inpError = false;             // During SECTION 1, an indication that there is some issue with the recent input; flag bool
    bool gameWin[3];              // During SECTION 2, is a flag that indicates whether or not someone has won and the game should stop
    bool fleetDestroyed[3];       // During SECTION 2, is a flag that indicates a player's fleet has been destroyed and the game has been won
    bool retreat = false;              // During SECTION 2, is a flag that indicates the player's choice to stop playing
    bool stalemate = false;
    bool exitNOW = false;    // During SECTION 1, is a flag that is raised if the user wants to stop building their fleet
    bool ENDprogram = false; // Is a flag value that is raised if one of the user wants to stop the game
    gameEndValsReset(gameWin, fleetDestroyed);


    // SECTION 1 - SETTING UP THE GAME PIECES ----------------------------------------------------------------------------------------------------------------------------
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
    // LOOP: Establishes the fleet locations for each player *************************************************************************************************************
    for (int player = 0; (player < mode.loop_numPlayers && !ENDprogram); player++)
    {
        // PREP - Specifies which player the game is currently setting up for ............................................................................................
        if ((player + 1) == 1)
        {   playerPtr = &mode.play1;                     // Pointer for the player
            fleetOrderPtr = &fleetOrderU[0];             // Pointer for the fleet order
            hitboardPtr = hitboardU;                     // Pointer for the list of the vulnerable coordinates
            brdPtr = boardUSER;                          // Pointer for the personal-display board
            mode.loop_maxshotPtr = &mode.play1.maxShots; // Pointer that applies at the end: establishing user's / P1's max shots
            mode.loop_salvoSizeP = &salvoSizeU;
            //*(*shipPointersList[6]);
        }
        else if ((player + 1) == 2)
        {   playerPtr = &mode.play2;                     // Pointer for the player
            fleetOrderPtr = &fleetOrderC[0];             // Pointer for the fleet order
            hitboardPtr = hitboardC;                     // Pointer for the list of the vulnerable coordinates
            brdPtr = boardCOMP;                          // Pointer for the personal-display board
            mode.loop_maxshotPtr = &mode.play2.maxShots; // Pointer that applies at the end: establishing computer's / P2's max shots
            mode.loop_salvoSizeP = &salvoSizeC;
            // *(*shipPointersList[6]);
        }
        // Brief Display 0: (Depends on whether or not there is a computer playing or two players based on the loopMode_numPlayers)
        displayBriefSetUP(mode.loop_numPlayers, 0, (player+1));
        Pause_Program(3);

        // LOOP: Placement of ships **********************************************************************************************************************************
        for (c_numShips = 0; (c_numShips < 5 && !exitNOW && !ENDprogram); c_numShips++)
        {
            // PART 0 - Display the array + available ships ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
            displayBoard1(brdPtr, playerPtr->name[1]);


            // PART A - Select which ship to set up ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
            if (returntoCoord == false)
            { // Call: [Od1]  -- Display of all of the available options of ships
                displayShipOptions();

                do  // LOOP: Obtaining which ship to set up .............................................................................
                {   // PROMPT
                    inpError = false; // Flag Reset
                    cout << endl << "Which ship do you want to place?"
                         << endl << "(If you're finished with setting up your ships, enter 'd')" << endl;
                    // Call: [Im1]  -- Obtains valid input for the above prompt
                    getInput_fetchShip(fetchInputPtr);

                    // PROCESSING - 3 subsections
                    // If a ship option was successfully filled in:
                    if (fetchInputPtr[0] != 'd')
                    { // Call: [Pp0]  -- Assign the correct ship pointer (sensitive to which player is currently setting up their fleet)
                        if ((player + 1) == 1)
                            shipPtr = assignPointer(shipPointersU, fetchInputPtr[0], "NULL", NULL);
                        else if ((player + 1) == 2)
                            shipPtr = assignPointer(shipPointersC, fetchInputPtr[0], "NULL", NULL);
                        // Validate that the ship hasn't already been created
                        if (shipPtr->ship[0] != 'x')
                        {   inpError = true;
                            cout << "This ship has already been created. Please choose another." << endl;
                        }
                    }

                    // If the user indicated that they'd like to stop filling in ships:
                    else
                    { // Error: There must be a minimum of 1 ship within a player's fleet
                        if (c_numShips == 0)
                        {   cout << endl << "You can't play if you haven't placed any ships. "
                                 << endl << "Would you like to exit the game?"
                                 << endl << "Enter 'Y' for Yes and 'N' for No.";
                            response = toupper(getInput_yesno()); // Call [Imy]
                            if (response == 'Y')        ENDprogram = true;
                            else if (response == 'N')   inpError = true;
                            response = '-';
                        }
                        // Accepting that the user wants to stop creating their fleet
                        else
                        {   cout << endl << "Are you sure you want to end the creation of your fleet?"
                                 << endl << "You still have " << (5 - c_numShips) << " ships left to place."
                                 << endl << "Enter 'Y' for Yes and 'N' for No.";
                            response = toupper(getInput_yesno()); // Call [Imy]
                            if (response == 'Y')
                            {   cout << endl << "Ship placement finished. A fleet of " << c_numShips << " ships has been deployed!" << endl;
                                exitNOW = true;
                            }
                            else if (response == 'N')
                                inpError = true;
                            response = '-';
                        }
                    }
                } // LOOP END: Obtaining which ship to set up .........................................................................
                while (inpError == true);
            } // -- END of (PART A)   IF Statement

            // Continuation of PART A: the IF statement here guarantees that a user won't have to continue
            // ... their fleet set-up if they chose to exit.
            if (exitNOW == false)
            { // Finish up PART A
                crntShipSize = shipPtr->size;
                fleetOrderPtr[c_numShips] = shipPtr->name;
                returntoCoord = false; // Flag Reset (specifically if the user chose to reselect their coordinates)


                // PART B - Select a coordinate to use ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                do                   // LOOP: Obtaining the coordinates to place ship .....................................................
                {                    // PROMPT
                    shipExists = -1; // Flag Reset
                    cout << endl << "Where do you want to put the leftmost/topmost part of your ship?" << endl;
                    // Call: [Im2]  -- Obtain valid input for the prompt
                    getInput_placeShipCoord(fetchInputPtr, crntCoordSelection, crntShipSize);

                    // PROCESSING
                    // Call: [Ps0]  -- Determine whether or not a ship exists at that specific coordinate
                    if (c_numShips != 0)
                        shipExists = linearSearch_aim(fetchInputPtr, hitPtrU, c_numShips, NULL);
                    // Error Message, if ship already exists there (-1 indicates that a ship hasn't been created yet)
                    if (shipExists != -1)
                    {
                        cout << endl << "The ship cannot be placed here. "
                             << endl << "Your " << fleetOrderPtr[shipExists] << " is already in that location."
                             << endl << endl;
                        Pause_Program(4);
                    }
                } // LOOP END: Obtaining the coordinates to place ship .................................................
                while (shipExists != -1);

                // Call: [Sa2]  -- Application thru pointers - applying the coordinates through characters representing coordinates (within the ship's struct information)
                setBoard_charFill('0', 0, shipPtr->pos.coord[0], fetchInputPtr, crntCoordSelection);
                // Call: [Sa4]  -- Application thru pointers - applying the coordinates through integer elements representing coordinates (within the ship's struct information)
                setBoard_structFill('0', NULL, c_numShips, crntShipSize, hitboardPtr, NULL, crntCoordSelection); // TYPE 0 applied


                // PART C - Establishing orientation ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                cout << endl << "Place it vertically ('V') or horizontally ('H')?"
                     << endl << "(To return to coordinate placement, enter 'r')" << endl;
                // Call: [Im3]  -- Obtain valid input, and a (potentially) accurate answer
                getInput_shipOrient(fetchInputPtr, crntCoordSelection, crntShipSize, hitboardPtr, c_numShips, fleetOrderPtr);
                // Checks to see if there's a call to return to the coordinate section
                if (tolower(*fetchInputPtr) == 'n' || tolower(*fetchInputPtr) == 'r')
                {
                    returntoCoord = true; // Sets up sentinel
                    c_numShips--;         // Readjusts the number of ships for the extra repeat of loop (as a new ship is NOT being created)
                }


                // PART D - Validation ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                // If no return is initiated, then assign the rest of the ships attributes
                else
                {   int c;
                    // Applying the remaining coordinates through characters representing coordinates (within the ship's struct information)
                    for (c = 1; c < crntShipSize; c++)
                    { // Fill in the ship body
                        shipPtr->ship[c] = '#';
                        // Call: [Sa2]  -- Fill in the basic coordinates within the chars
                        setBoard_charFill(fetchInputPtr[0], c, shipPtr->pos.coord[c], shipPtr->pos.coord[0], crntCoordSelection);
                    }
                    // Placing a stopper value
                    shipPtr->pos.coord[c][0] = '\0';
                    // Call: [Sa3]  -- Applying the information through integer elements representing coordinates (within the ship's struct information)
                    setBoard_structFill(fetchInputPtr[0], shipPtr, c_numShips, crntShipSize, hitboardPtr, brdPtr, crntCoordSelection);
                }

                // Message that confirms ship creation
                if (shipPtr->ship[0] == '<' || shipPtr->ship[0] == 'M')
                    cout << endl << "Your " << shipPtr->name << " was successfully placed!" << endl;
                Pause_Program(3);

            } // -- END of potentially skipped ship-creation thru flag:  "exitNow"

        } // LOOP END: Placement of ships *************************************************************************************************************************


        // PART E -- Calculating player values and creating potential stoppers for future loops ............................................................................................
        // Establishing the correct number of ships after the end of the set up
        playerPtr->fleetSize = c_numShips;
        fleetOrderPtr[c_numShips] = "NULL";
        *mode.loop_salvoSizeP = ((mode.loop_gameSelect == 1) ? 1 : playerPtr->fleetSize);
        if (playerPtr->fleetSize == 5)
            cout << endl << "A full fleet of ships has been deployed!" << endl;
        Pause_Program(1);
        // Creation of stoppers when fleet size isn't maximized by the user
        if (exitNOW == true)
        {   playerPtr->fleetSize--;     // Adjusts the newly incremented value back to the actual number of ships
            playerPtr->maxShots = playerPtr->fleetSize * 20;
            if ((player + 1) == 1)      // Player 1 stoppers
            {   hitboardU[c_numShips][0] = -2;
                fleetOrderU[c_numShips] = "NULL";
            }
            else if ((player + 1) == 2) // Player 2 stoppers
            {   hitboardC[c_numShips][0] = -2;
                fleetOrderC[c_numShips] = "NULL";
            }
        }
        // Establishing extra, mode-dependent values
        if (mode.loop_gameSelect != 1)
            *mode.loop_maxshotPtr = (playerPtr->fleetSize * 20);
        else 
        {   *mode.loop_maxshotPtr = 50; 
            mode.loop_numTurns = 50; 
        }
        // Flag Reset
        exitNOW = false;
    } // LOOP END: Establishes the fleet locations for each player *******************************************************************************************************



    // SECTION 2 - PLAYING THE GAME --------------------------------------------------------------------------------------------------------------------------------------
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
    // PREP --- Set Up +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // Brief Display 1A: the beginning of the game
    displayBriefSetUP(0, mode.loop_numTurns, 0);

    // PART 0 --- Begin Game ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    int crntplayer;
    Coordinates salvo = salvoReset();
    // LOOP: Turn-representation of the game *******************************************************************************************************************************
    for (int c_turn = 0; (!gameWin[0] && !retreat && (c_turn < mode.loop_numTurns)); c_turn++)
    {
        // PART A - Initialize Salvo:
        Coordinates *salvoPtr = &salvo;
        int c_shot;       // "Current" Shot
        char c_aimedChar; // "Current" Location on the board
        int re;           // Row Array-Element
        int ce;           // Column Array-Element
        // int shipEndCoord, rangeShip, coordAim;
        // bool shipSunk;
        // int elemOfShipPartHit;
        shipPtr = NULL;

        // Brief Display 1B:
        cout << endl << endl << ">>>>>>>> TURN " << (c_turn+1) << " <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<";

        // LOOP: Player's Turn Representation *******************************************************************************************************************************
        for (crntplayer = 0; (!gameWin[0] && !retreat && crntplayer < mode.loop_numPlayers); crntplayer++)
        {
            // PART B - Specifies which player the game is currently running a turn for
            if ((crntplayer + 1) == 1)
            {   // --- PLAYER 1 pointer initialization
                mode.loop_maxshotPtr = &mode.play1.maxShots; // Pointer that applies at the end: establishing user's / P1's max shots
                mode.loop_salvoSizeP = &salvoSizeU;          // Pointer that signifies how many times the player can fire
                playerPtr = &mode.play1;                     // Pointer for the player + their score
                fleetOrderPtr = &fleetOrderU[0];             // Pointer for the fleet order
                hitboardPtr = hitboardU;                     // Pointer for the list of the vulnerable coordinates
                brdPtr = boardUSER;                          // Pointer for the personal-display board
                // --- TARGET pointer initialization
                playerAimPtr = &mode.play2;       // Pointer for the player + their score
                fleetOrderAimP = &fleetOrderC[0]; // Pointer for the fleet order
                hitboardAimPtr = hitboardC;       // Pointer for the collection of the target's vulnerable areas of ship
                brdAimPtr = boardAimCOMP;         // Pointer for the personal-display board of the target
                brdTempPtr = boardTempCOMP;       // Pointer for the display board of the player's perspective of the target's board
                brdRecordPtr = boardCOMP;         // Additional pointer for the display board of the player's perspective of the target's board
            }
            else if ((crntplayer + 1) == 2)
            {   // PLAYER 2 pointer initialization
                mode.loop_maxshotPtr = &mode.play2.maxShots; // Pointer that applies at the end: establishing computer's / P2's max shots
                mode.loop_salvoSizeP = &salvoSizeC;          // Pointer that signifies how many times the player can fire
                playerPtr = &mode.play2;                     // Pointer for the player
                fleetOrderPtr = &fleetOrderC[0];             // Pointer for the fleet order
                hitboardPtr = hitboardC;                     // Pointer for the list of the vulnerable coordinates
                brdPtr = boardCOMP;                          // Pointer for the personal-display board
                // --- TARGET pointer initialization
                playerAimPtr = &mode.play1;       // Pointer for the player + their score
                fleetOrderAimP = &fleetOrderU[0]; // Pointer for the fleet order
                hitboardAimPtr = hitboardU;       // Pointer for the collection of the target's vulnerable areas of ship
                brdAimPtr = boardAimUSER;         // Pointer for the personal-display board of the target
                brdTempPtr = boardTempUSER;       // Pointer for the display board of the player's perspective of the target's board
                brdRecordPtr = boardUSER;         // Additional pointer for the display board of the player's perspective of the target's board
            }

            // Brief Display 1C - Notification that one of the player's turns has started
            if (mode.loop_numPlayers == 2)
            {   cout << endl << endl << endl << endl << endl << endl
                     << endl << endl << endl << endl << endl << endl 
                     << endl << "This is " << playerPtr->name[0] << "'s  turn."
                     << endl << "Please turn the moniter out of the view of the other player." << endl
                     << endl;
                Pause_Program(1);
            }
            cout << endl << endl << endl << ">>>>>>>> TURN " << (c_turn + 1) << " <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<";

            // Display the actions of the other player
            cout << endl << endl; 
            if (c_turn != 0 || (crntplayer+1) == 2)
            {   cout << endl << playerAimPtr->name[0] << "'s move: " << endl;
                for (c_shot = 0; (c_shot < *mode.loop_salvoSizeP && salvoPtr->aim[c_shot][0] != -2); c_shot++)
                    displayEnemyMove(salvoPtr->coord[c_shot], salvoPtr->hitShipName[c_shot]);
                cout << endl << endl; 
                Pause_Program(3);
            }
            // Display status 
            cout << endl << endl << endl;
            displayShipSink("playerSink", salvoPtr);

            // [Ruin-Mode specific] - Damage applied to the ships
            if (mode.loop_gameSelect == 3) // ... Taken into account during "Ruin" Mode
            {
                if ((crntplayer+1) == 1)        fleetDamageIncrement(salvoPtr, shipPointersC, fleetOrderU, (playerPtr->fleetSize - playerPtr->numShipsLost), playerPtr, playerAimPtr);
                else if ((crntplayer+1) == 2)   fleetDamageIncrement(salvoPtr, shipPointersU, fleetOrderC, (playerPtr->fleetSize - playerPtr->numShipsLost), playerPtr, playerAimPtr);
            }

            // Board Display << 1 >>
            displayBoard2(brdPtr, brdAimPtr, playerPtr->name[1], playerAimPtr->name[1]);



            // PART C - RESET / EMPTY SALVO ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
            fetchInputPtr[0] = '-';                        // Clears the input-analysis variable
            *salvoPtr = salvoReset();                      // Clears the salvo of its contents from the turn prior
            wipeBoard("CopyBoard", brdTempPtr, brdAimPtr); // Alters the target's display board so that it can be edited further
            if (mode.loop_gameSelect != 1)
                // Makes sure that the salvo doesn't change from 1 in [Regular Mode]
                *mode.loop_salvoSizeP = (playerPtr->fleetSize - playerPtr->numShipsLost);
            else
                *mode.loop_salvoSizeP = 1; 


            // PART D - PROMPT FOR FIRING LOCATIONS ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
            cout << endl << "You have  <" << (playerPtr->maxShots - playerPtr->usedShots) << " shots>  remaining!" << endl;

            // IF-flag that makes it so the player can't fire if they run out of ammo
            if ((playerPtr->maxShots - playerPtr->usedShots) != 0)
            {
                // Prompt Brief Display: Salvo Size
                cout << endl << "     +    " << *mode.loop_salvoSizeP << ((*mode.loop_salvoSizeP == 1) ? " ship is " : " ship are ")
                     << "ready to fire!";
                Pause_Program(5);

                // LOOP: Obtaining locations for the salvo *****************************************************************************************************************
                cout << endl << "Enter Coordinates for salvo..." << endl;
                for (c_shot = 0; (tolower(fetchInputPtr[0]) != 'x' && c_shot < *mode.loop_salvoSizeP); c_shot++)
                {
                    // Obtain the shot coordinates
                    getInput_fillShot(fetchInputPtr, crntCoordSelection, c_shot + 1, &(playerPtr->usedShots));

                    // PROCESSING RESULTS:
                    // Check for implications of 'skipping' a shot or 'retreat'
                    if (tolower(fetchInputPtr[0]) != 'x' && tolower(fetchInputPtr[0]) != 'r')
                    {   // Insert the integer coordinates
                        salvoPtr->aim[c_shot][0] = re = crntCoordSelection[0];
                        salvoPtr->aim[c_shot][1] = ce = crntCoordSelection[1];
                        salvoPtr->coord[c_shot][0] = fetchInputPtr[0]; // Character value
                        // Within the aimed board, certain location of the shot is recorded
                        brdAimPtr[re][ce] = '*';
                        brdTempPtr[re][ce] = '*';
                    }
                    // When the player successfully signifies they want to 'skip' a shot or stop playing
                    else
                    {   // Stoppers and Readjustment
                        salvoPtr->aim[c_shot][0] = -2;
                        c_shot--;
                        // Check-In for the Retreat Option
                        if (tolower(fetchInputPtr[0]) == 'r')
                        {   cout << endl << "Would you like to call your fleet to retreat?"
                                 << endl << "Enter 'Y' for Yes and 'N' for No.";
                            response = toupper(getInput_yesno()); // Call [Imy]
                            if (response == 'Y')
                            {   retreat = true;
                                fetchInputPtr[0] = 'x';
                            }
                            response = '-';
                        }
                    }

                } // LOOP END: Obtaining locations for the salvo ********************************************************************************************************

                // Board Display << 2 >>
                displayBoard2(brdPtr, brdTempPtr, playerPtr->name[1], playerAimPtr->name[1]);
                cout << endl << endl;
                Pause_Program(3);



                // PART E - VERIFICATION - Is there a ship in these locations?  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                // LOOP: Processing the salvo-input ************************************************************************************************************************
                for (c_shot = 0; (c_shot < *mode.loop_salvoSizeP && salvoPtr->aim[c_shot][0] != -2); c_shot++)
                {
                    // Set the current array-coordinate values
                    re = salvoPtr->aim[c_shot][0];
                    ce = salvoPtr->aim[c_shot][1];
                    c_aimedChar = brdRecordPtr[re][ce];

                    // Determine whether or not the shot hit or miss:

                    // >>>>>>>>>>>>>>>>> This particular shot MISSED <<<<<<<<<<<<<<<<<<<<
                    if (c_aimedChar == '~' || c_aimedChar == 'X' || c_aimedChar == 'o' || c_aimedChar == 'O')
                    {   // Recording the shot on the board
                        brdAimPtr[re][ce] = 'O';
                        brdRecordPtr[re][ce] = 'O';
                        // Recording the text-based values of how it works
                        setSalvo_messageFill("missedShot", salvoPtr, &(salvoPtr->coord[c_shot][0]), re);
                        // Recording the hit/miss in the "was there a ship hit?" List
                        salvoPtr->hitShip[c_shot] = -1;
                    }

                    // >>>>>>>>>>>>>>>>> This particular shot HIT something <<<<<<<<<<<<<<<<<<<
                    else
                    {   // Recording the shot on the board
                        brdAimPtr[re][ce] = 'X';
                        brdRecordPtr[re][ce] = 'X';
                        // Recording the text-based values of how it works
                        setSalvo_messageFill("shipHit", salvoPtr, &(salvoPtr->coord[c_shot][0]), re);
                        // Recording the hit/miss in the "was there a ship hit?" List
                        salvoPtr->hitShip[c_shot] = linearSearch_aim(NULL, hitboardAimPtr, playerAimPtr->fleetSize, salvoPtr->aim[c_shot]);
                        salvoPtr->hitShipName[c_shot] = fleetOrderAimP[salvoPtr->hitShip[c_shot]];
                        // Determining the particular ship that was hit
                        if ((crntplayer + 1) == 1)          shipPtr = assignPointer(shipPointersU, NULL, fleetOrderAimP[salvoPtr->hitShip[c_shot]], NULL);
                        else if ((crntplayer + 1) == 2)     shipPtr = assignPointer(shipPointersC, NULL, fleetOrderAimP[salvoPtr->hitShip[c_shot]], NULL);
                        // Change the ship statistics + player's statistics
                        playerPtr->inflictedDamage = incrementOne((*playerPtr).inflictedDamage);
                        shipPtr->hits = incrementOne((*shipPtr).hits);
                        if (shipPtr->hits == shipPtr->size)
                        {   // ^^^ ie: when all of the ship locations have been hit by the player
                            shipSunk_changingValues(shipPtr, playerPtr, playerAimPtr, salvoPtr, c_shot);
                        }
                        else
                        {   // [Ruin-Mode specific]
                            if (mode.loop_gameSelect == 3)
                            {   if (shipPtr->hits > 1)
                                {   shipPtr->sinking = true;
                                    shipPtr->turnsLeft = ((shipPtr->size - shipPtr->hits) * 3) - (shipPtr->hits * 1.25);
                                    shipPtr->sunk = shipSunkFromDamage(shipPtr, playerPtr, playerAimPtr, salvoPtr, c_shot); 
                                }
                            }
                        }
                    }

                } // LOOP END: Processing the salvo-input ****************************************************************************************************************


                // PART F - DISPLAYING RESULTS +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                // Display their results (Hits / Misses)
                cout << endl << endl;
                for (c_shot = 0; (c_shot < *mode.loop_salvoSizeP && salvoPtr->aim[c_shot][0] != -2); c_shot++)
                    displayUserMove(salvoPtr->coord[c_shot], salvoPtr->hitShipName[c_shot]);
                // Board Display << 3 >>
                displayBoard2(brdPtr, brdAimPtr, playerPtr->name[1], playerAimPtr->name[1]);

            } // FLAG END: makes it so the player can't fire if they run out of ammo
            // .... ^^^ Message that indicates munnitions-depletion, from the end of above IF statement
            else
            {   cout << endl << "None of your ships have ammo left. "
                     << endl << "Would you like to retreat?"
                     << endl << "Enter 'Y' for Yes and 'N' for No.";
                response = toupper(getInput_yesno());
                if (response == 'Y')
                    retreat = true;
                response = '-';
            }

            // Fleet Status: Did any ships sink? What damage was sustained?
            cout << endl << endl << endl;
            displayShipSink("enemySink", salvoPtr);

            // [Ruin-Mode specific] Fades the values of Aim for the next turn
            if (mode.loop_gameSelect == 3)
                wipeBoard("FadeAim", brdAimPtr, brdAimPtr);


            // PART E -- IS THE GAME WON
            if (retreat)
            {   cout << endl << playerPtr->name[0] << "'s fleet has retreated!" << endl;
                Pause_Program(4);
            }
            else
            {    fleetDestroyed[0] = fleetDestroyed[crntplayer+1] = wasFleetDestroyed(playerPtr, playerAimPtr, mode.loop_gameSelect);
            }// END OF TURN
            cout << endl << endl << endl << endl << endl << endl << endl << endl;
            Pause_Program(1);

            // Stopping the current loop (if game IS won)
            if (fleetDestroyed[0] == true)
                gameWin[0] = true;

        } // LOOP END: Player's Turn Representation ***********************************************************************************************************************

    } // LOOP END: Turn-representation of the game ***********************************************************************************************************************



    // SECTION 3 - VERIFYING WHO WON THE GAME ----------------------------------------------------------------------------------------------------------------------------
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    // PART 0 -- Creation of WIN and LOSE Variables ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    Score playerWin, playerLose; 


    // PART A -- WINNER DECLARATION ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // Description of what happened:
    assignGameWinner(&mode.play1, &mode.play2, playerPtr, playerAimPtr, mode.loop_gameSelect, fleetDestroyed, &stalemate, &playerWin, &playerLose);
    // Explicit statement of winner:
    if (!stalemate)
    {   cout << endl << playerPtr->name[0] << " won!!!" << endl;
        playerAimPtr->maxShots = playerAimPtr->usedShots;
        Pause_Program(4);
    }


    // PART B -- RESULTS DISPLAY ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // Board Display << 4 >>
    displayBoard2(brdPtr, brdRecordPtr, playerPtr->name[1], playerAimPtr->name[1]);

    // Display results
    displayGameResults(playerPtr, playerAimPtr, shipPointersU, shipPointersC);


    // Cleanup
    delete[] fetchInputPtr;
    delete[] crntCoordSelection;
    Launch_Program(1, false);
    return 0;
}
//_________________________________________________________________________________________________________________________________________________________________________________
//_________________________________________________________________________________________________________________________________________________________________________________
//_________________________________________________________________________________________________________________________________________________________________________________

// FUNCTION DEFINITIONS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/**************  definition order ***************************************************************************************************************************| 

 - Display of Messages / Prompts
 - Display of Boards / Representations of the Grid
 - Functions that reset or define the values of certain variables / arrays
 - "getInput_" -- Every type of input that is involved within the program 
 - Functions that validate the existance of a value or situational occurance
 - Search Functions: search or assignment of pre-existing pointers or ship values 
 - SetBoard -- specifically used in Section 1, but are the continued assignment of values after input for placement finishes

|************************************************************************************************************************************************************/

// ------------------------ Display Message Functions -----------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------------------
// { Od0. }____ Halts the Program in order to divide program sections
void Pause_Program(int PATHWAY)
{ // Pause Menu Introduction
    if (PATHWAY == 1)
    {
        cout << endl << "Press [ENTER] to continue." << endl << "-----------------------------------------------------------------------------";
        cin.get(); // Pause
    }
    // Pause Menu Extended
    else if (PATHWAY == 2)
    {
        cout << endl << ">> >> >>   " << "Press [ENTER] to continue." << endl << "-----------------------------------------------------------------------------";
        cin.get(); // Pause
    }
    // Pause Menu Shortened
    else if (PATHWAY == 3)
    {
        cout << endl << ">> >> >>   " << "Press [ENTER] to continue.";
        cin.get(); // Pause
    }
    // Pause Menu Brief
    else if (PATHWAY == 4)
    {
        cout << endl << ">> >> >>   ";
        cin.get(); // Pause
    }
    // Pause Message Display
    else if (PATHWAY == 5)
    {
        cout << "    +   ";
        cin.get(); // Pause
    }
}

// { Od1. }____
void displayBriefSetUP(int numPlayers, int numTurns, int player)
{   // SECTION 1 - Header statement in section 1
    if (player != 0)
    {
        cout << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl;
        if (numPlayers == 2)
            cout << endl << "GAME SET UP:  Player " << player
                 << endl << endl  << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl
                 << "Set up your ships here, outside of the other player's view." << endl;
        else
            cout << endl << "GAME SET UP: " << endl << "Set up your ships below." << endl;
    }
    // SECTION 2 - Header statement in section 2
    else if (numTurns != 0)
    {
        cout << endl << endl << "You will have a maximum of  -" << numTurns << " turns-  to destroy your opponent's fleet.";
        Pause_Program(4);
        cout << endl << "Entering 'x' instead of standard coordinates into your salvo will "
             << endl << "end your shot selection for that turn." << endl
             << endl  << "Entering 'retreat' will cause your fleet to disengage and return to port." << endl;
        cout << endl << endl << "SET UP of the game has ended."
             << endl << "Continue to START the game." << endl;
        Pause_Program(2);
    }
}

// { Od2. }____
void displayShipOptions()
{
    cout << endl << "Options: "
         << endl << "   1 - Destroyer" << " -  < >"
         << endl << "   2 - Submarine" << " -  < # >"
         << endl << "   3 - Cruiser" << "   -  < # >"
         << endl << "   4 - Battleship" << "-  < # # >"
         << endl << "   5 - Carrier" << "   -  < # # # >"
         << endl << endl;
}

// { Id3. }____
void displayEnemyMove(char *salvoCoord, string hitShipName)
{   // Message
    cout << salvoCoord[2] << salvoCoord[3]
         << salvoCoord[4] << salvoCoord[5] << " ";
    // Coordinates
    cout << "[" << salvoCoord[0]
         << ((salvoCoord[1] != '0') ? salvoCoord[1] : 10)
         << "]  ";
    // Description
    if (salvoCoord[2] == 'M')       cout << "- The enemy hit water!";
    else if (salvoCoord[2] == 'H')  cout << "- Your " << hitShipName << " was damaged!";
    Pause_Program(5);
}

// { Id4. }____
void displayUserMove(char *salvoCoord, string hitshipName)
{ // Coordinates
    cout << "[" << salvoCoord[0]
         << ((salvoCoord[1] != '0') ? salvoCoord[1] : 10)
         << "]  ";
    // Message
    cout << salvoCoord[2] << salvoCoord[3]
         << salvoCoord[4] << salvoCoord[5] << " ";
    // Description
    if (salvoCoord[2] == 'M')
        cout << "- This shot hit water!";
    else if (salvoCoord[2] == 'H')
        cout << "- You damaged the enemy ship!";
    Pause_Program(5);
}

// { Id5. }____
void displayShipSink(string type, Coordinates *salvoPtr)
{
    bool shipSunk = false;
    // Display for the end of a player's turn: indicates if a target's ship has been sunk
    if (type == "enemySink")
    {   for (int i = 0; i < 5; i++)
            if (salvoPtr->recentlySunk[i] != -2)
            {   
                if (shipSunk == false)
                {   shipSunk = true;
                    cout << "  +    An enemy ship has been destroyed!";
                }
                else
                    cout << "  +    Another enemy ship has been destroyed!";
                Pause_Program(5);
            }
    }
    // Display for the beginning of a player's turn: indicates if the player's ship has been sunk
    else if (type == "playerSink")
    {   for (int i = 0; i < 5; i++)
            if (salvoPtr->recentlySunk[i] != -2)
            {   cout << "  +    Your " << salvoPtr->hitShipName[i] << " has been destroyed!";
                Pause_Program(5);
            }
    }
}

// { Id6. }____
void assignGameWinner(Score *player1, Score *player2, Score *playerPtr, Score *playerAimPtr, int gameMode, bool fleetDestroyed[2], bool *stalemate, Score *playerWin, Score *playerLose)
{   // SELECTION FOR DEFINITE WIN
    if (fleetDestroyed[0] == true)
    {   // Displays for [Regular Mode] or [Salvo Mode]
        // If Player 1 won
        if (fleetDestroyed[1] == true)
        {   assignPlayerWinLose(playerPtr, playerAimPtr, &playerWin, &playerLose);
            cout << endl << playerWin->name[0] << " has destroyed the opposing fleet!" << endl;
        }
        // If Player 2 won
        else if (fleetDestroyed[2] == true)
        {   assignPlayerWinLose(playerAimPtr, playerPtr, &playerWin, &playerLose);
            cout << endl << playerWin->name[0] << " has destroyed the opposing fleet!" << endl;
        }
        // Displays for [Ruin Mode]
        else if (gameMode == 3)
            // Accounts for the player shooting down the last ship
            if (playerPtr->shipsShotDown == playerAimPtr->fleetSize)
            {   assignFleetWin(playerPtr, fleetDestroyed, player1, player2);
                assignPlayerWinLose(playerPtr, playerAimPtr, &playerWin, &playerLose);
                cout << endl << playerWin->name[0] << " has destroyed the opposing fleet!!!" << endl;
            }
            else if (playerAimPtr->shipsShotDown == playerPtr->fleetSize)
            {   assignFleetWin(playerAimPtr, fleetDestroyed, player1, player2);
                assignPlayerWinLose(playerAimPtr, playerPtr, &playerWin, &playerLose);
                cout << endl << playerWin->name[0] << " has destroyed the opposing fleet!!!" << endl;
            }
            // Accounts for the enemy ship sinking on its own
            else if (playerPtr->totalEnemyFleetSunk == playerAimPtr->fleetSize)
            {   assignFleetWin(playerPtr, fleetDestroyed, player1, player2);
                assignPlayerWinLose(playerPtr, playerAimPtr, &playerWin, &playerLose);
                cout << endl << playerWin->name[0] << "'s fleet has been destroyed!!!" << endl;
            }
            else if (playerAimPtr->totalEnemyFleetSunk == playerPtr->fleetSize)
            {   assignFleetWin(playerAimPtr, fleetDestroyed, player1, player2);
                assignPlayerWinLose(playerAimPtr, playerPtr, &playerWin, &playerLose);
                cout << endl << playerWin->name[0] << "'s fleet has been destroyed!!!" << endl;
            }
    }
    // SELECTION FOR DAMAGE / HIT-BASED WIN
    else
    {   // Ship Loss Comparison
        if (playerPtr->shipsShotDown > playerAimPtr->shipsShotDown)
        {   assignPlayerWinLose(playerPtr, playerAimPtr, &playerWin, &playerLose);
            cout << endl << playerWin->name[0] << " destroyed more of " << playerLose->name[0] << "'s fleet!!!" << endl;
        }
        else if (playerPtr->shipsShotDown < playerAimPtr->shipsShotDown)
        {   assignPlayerWinLose(playerAimPtr, playerPtr, &playerWin, &playerLose);
            cout << endl << playerWin->name[0] << " destroyed more of " << playerLose->name[0] << "'s fleet!!!" << endl;
        }
        // Ship Damage Comparison
        else if (playerPtr->inflictedDamage > playerAimPtr->inflictedDamage)
        {   assignPlayerWinLose(playerPtr, playerAimPtr, &playerWin, &playerLose);
            cout << endl << playerWin->name[0] << " damaged more of " << playerLose->name[0] << "'s fleet!!!" << endl;
        }
        else if (playerPtr->inflictedDamage < playerAimPtr->inflictedDamage)
        {   assignPlayerWinLose(playerAimPtr, playerPtr, &playerWin, &playerLose);
            cout << endl << playerWin->name[0] << " damaged more of " << playerLose->name[0] << "'s fleet!!!" << endl;
        }
        // Ship Sustained Damage Comparison
        else if (gameMode == 3)
        {
            if (playerPtr->sustainedDamage > playerAimPtr->sustainedDamage)
            {   assignPlayerWinLose(playerPtr, playerAimPtr, &playerWin, &playerLose);
                cout << endl << playerWin->name[0] << " damaged more of " << playerLose->name[0] << "'s fleet!!!" << endl;
            }
            else if (playerPtr->sustainedDamage < playerAimPtr->sustainedDamage)
            {   assignPlayerWinLose(playerAimPtr, playerPtr, &playerWin, &playerLose);
                cout << endl << playerWin->name[0] << " damaged more of " << playerLose->name[0] << "'s fleet!!!" << endl;
            }
        }
        // Stalemate
        else
        {   *stalemate = true;
            cout << endl << playerPtr->name[0] << "'s and " << playerAimPtr->name[0] << "'s fleets are equally damaged!" << endl;
        }
    }
}

// { Id6. }____
void displayGameResults(Score *playerPtr, Score *playerAimPtr, Ship *ptrListU[5], Ship *ptrListC[5])
{
    //cout << endl << "     " << playerPtr->name[0] << "'s ";
    //cout << endl << "     " << playerAimPtr->name[0] << "'s ";
    Pause_Program(4);
    // Lost Ships / Fleet
    cout << endl << "     " << playerPtr->name[0] << "'s destroyed ship count:   " << playerPtr->numShipsLost << endl;
    cout << endl << "     " << playerAimPtr->name[0] << "'s destroyed ship count:   " << playerAimPtr->numShipsLost << endl;
    Pause_Program(4);
    // Fleet count
    cout << endl << "     " << playerPtr->name[0] << "'s returning fleet number:   " << (playerPtr->fleetSize - playerPtr->numShipsLost) << endl;
    cout << endl << "     " << playerAimPtr->name[0] << "'s returning fleet number:   " << (playerAimPtr->fleetSize - playerAimPtr->numShipsLost) << endl;
    Pause_Program(4);
    // Inflicted Damage
    cout << endl << "     " << playerPtr->name[0] << "'s inflicted damage:   " << playerPtr->inflictedDamage << endl;
    cout << endl << "     " << playerAimPtr->name[0] << "'s inflicted damage:   " << playerAimPtr->inflictedDamage << endl;
    Pause_Program(4);
    // Used ammunition
    cout << endl << "     " << playerPtr->name[0] << "'s used munnitions:   " << playerPtr->usedShots << endl;
    cout << endl << "     " << playerAimPtr->name[0] << "'s used munnitions:   " << playerAimPtr->usedShots << endl;
    Pause_Program(4);
    // Remaining ammunition
    cout << endl << "     " << playerPtr->name[0] << "'s remaining munnitions:   " << (playerPtr->maxShots - playerPtr->usedShots) << endl;
    cout << endl << "     " << playerAimPtr->name[0] << "'s remaining munnitions:   " << (playerAimPtr->maxShots - playerAimPtr->usedShots) << endl;
    Pause_Program(4);
}


// ------------------------- Display Board Functions ------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------------------
// { Id1. }____
void displayBoard1(char (*board)[11], string heading)
{ // Heading
    cout << endl << " " << heading << endl;
    cout << "     "
         << "A B C D E F G H I J" << endl;
    // Advancing thru the rows
    for (int i = 0; i < 10; i++)
    { // Row Numbering
        if (i != 9)
            cout << "  " << (i + 1) << "  ";
        else
            cout << " 10"
                 << "  ";
        // Advancing thru the columns
        for (int d = 0; d < 10; d++)
            cout << board[i][d] << " ";
        // Start the array again
        cout << endl;
    }
}

// { Id2. }____
void displayBoard2(char (*board1)[11], char (*board2)[11], string heading1, string heading2)
{ // Display Headers
    cout << endl << " " << heading1
         << "           "
         << " " << heading2 << endl;
    cout << "     "
         << "A B C D E F G H I J"
         << "           "
         << "     "
         << "A B C D E F G H I J" << endl;
    // Displaying the columns by row
    for (int i = 0; i < 10; i++)
    { // User's Board  -----------------------------
        if (i != 9)
            cout << "  " << (i + 1) << "  ";
        else
            cout << " 10"
                 << "  ";
        // Advancing thru the columns
        for (int d = 0; d < 10; d++)
            cout << board1[i][d] << " ";
        // Computer's Board  -------------------------
        cout << "          ";
        if (i != 9)
            cout << "  " << (i + 1) << "  ";
        else
            cout << " 10"
                 << "  ";
        // Advancing thru the columns
        for (int d = 0; d < 10; d++)
            cout << board2[i][d] << " ";
        cout << endl;
    }
}


// ------------------- Reset / Initialize Value Functions -------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------------------
// { Pi0. }____
int incrementOne(int value)
{
    return (1 + value);
}

//  Calling: gameEndValsReset(gameWin, fleetDestroyed);
void gameEndValsReset(bool gameWin[2], bool fleetDestroyed[2])
{   gameWin[0] = false;
    gameWin[1] = false;
    gameWin[2] = false;
    fleetDestroyed[0] = false; 
    fleetDestroyed[1] = false; 
    fleetDestroyed[2] = false; 
}

// { Rm0. }____
void flagReset(char *input, int inputBound, bool flagError1, bool flagError2)
{ // Flags: bool values (type of input)
    flagError1 = false;
    flagError2 = false;
    // Flags: char values (length of input)
    input[inputBound] = 'x';
    input[inputBound + 1] = 'o';
    input[inputBound + 2] = 'g';
}

// { Pa0. }____
void wipeBoard(string type, char boardFill[10][11], char (*boardCopy)[11])
{ // Row Advancement
    for (int i = 0; i < 10; i++)
    { // Column Advancement
        for (int d = 0; d < 10; d++)
            // Type 1: Creating a new board
            if (type == "NewBoard")
                boardFill[i][d] = '~';
            // Type 2: Copying over a pre-existing board
            else if (type == "CopyBoard")
                boardFill[i][d] = boardCopy[i][d];
            // Type 3: Altering a pre-existing board
            else if (type == "FadeAim")
            { // Fading values
                if (boardCopy[i][d] == 'X')
                    boardFill[i][d] = 'x';
                else if (boardCopy[i][d] == 'O')
                    boardFill[i][d] = 'o';
                // Erasing values
                else
                    boardFill[i][d] = '~';
                // Essentially: if (boardFill[i][d] == '~' && boardFill[i][d] == 'x' && boardFill[i][d] == 'o')
                // ... Or any other value ...
            }
        //else if (boardCopy[i][d] == '*')
        //boardFill[i][d] = 'X';
    }
}

// { Sa1. }____
Ship createShip(int type, int numUsers, int (*hitboard)[6])
{
    if (type == 1) // Ship 1 - "Destroyer"
    {
        Ship destr;
        destr.name = "Destroyer";
        destr.size = 2;
        destr.ship[0] = 'x';
        destr.ship[2] = '\0';
        if (numUsers == 1)
        {   hitboard[0][0] = 2;
            destr.ship[0] = '<';
            destr.ship[1] = '>';
            destr.pos.orient = 'H';
            hitboard[0][1] = 9;
            hitboard[0][2] = 7;
            hitboard[0][3] = 9;
            hitboard[0][4] = 8;
            for (int c = 0; c < 6; c++)
            {
                if (c == 0)
                {   destr.pos.coord[0][c] = 'H';
                    destr.pos.coord[1][c] = 'I';
                }
                else if (c == 1)
                {   destr.pos.coord[0][c] = '1';
                    destr.pos.coord[1][c] = '1';
                }
                else if (c == 2)
                {   destr.pos.coord[0][c] = '0';
                    destr.pos.coord[1][c] = '0';
                }
                else if (c == 3)
                {   destr.pos.coord[0][c] = '9';
                    destr.pos.coord[1][c] = '9';
                }
                else if (c == 4)
                {   destr.pos.coord[0][c] = '7';
                    destr.pos.coord[1][c] = '8';
                }
                else if (c == 5)
                {   destr.pos.coord[0][c] = '\0';
                    destr.pos.coord[1][c] = '\0';
                }
            }
        }
        return destr;
    }
    if (type == 2) // Ship 2 - "Submarine"
    {
        Ship subm;
        subm.name = "Submarine";
        subm.size = 3;
        subm.ship[0] = 'x';
        subm.ship[3] = '\0';
        if (numUsers == 1)
        {   hitboard[1][0] = 3;
            subm.ship[0] = 'M';
            subm.ship[1] = '#';
            subm.ship[2] = 'W';
            subm.pos.orient = 'V';
            hitboard[1][1] = 5;
            hitboard[1][2] = 1;
            hitboard[1][3] = 7;
            hitboard[1][4] = 1;
            for (int c = 0; c < 6; c++)
            {   if (c == 0)
                {   subm.pos.coord[0][c] = 'B';
                    subm.pos.coord[1][c] = 'B';
                    subm.pos.coord[2][c] = 'B';
                }
                else if (c == 1)
                {   subm.pos.coord[0][c] = '6';
                    subm.pos.coord[1][c] = '7';
                    subm.pos.coord[2][c] = '8';
                }
                else if (c == 2)
                {   subm.pos.coord[0][c] = '-';
                    subm.pos.coord[1][c] = '-';
                    subm.pos.coord[2][c] = '-';
                }
                else if (c == 3)
                {   subm.pos.coord[0][c] = '5';
                    subm.pos.coord[1][c] = '6';
                    subm.pos.coord[2][c] = '7';
                }
                else if (c == 4)
                {   subm.pos.coord[0][c] = '1';
                    subm.pos.coord[1][c] = '1';
                    subm.pos.coord[2][c] = '1';
                }
                else if (c == 5)
                {   subm.pos.coord[0][c] = '\0';
                    subm.pos.coord[1][c] = '\0';
                    subm.pos.coord[2][c] = '\0';
                }
            }
        }
        return subm;
    }
    if (type == 3) // Ship 3 - "Cruiser"
    {
        Ship cruis;
        cruis.name = "Cruiser";
        cruis.size = 3;
        cruis.ship[0] = 'x';
        cruis.ship[3] = '\0';
        if (numUsers == 1)
        {   hitboard[2][0] = 3;
            cruis.ship[0] = '<';
            cruis.ship[1] = '#';
            cruis.ship[2] = '>';
            cruis.pos.orient = 'H';
            hitboard[2][1] = 1;
            hitboard[2][2] = 2;
            hitboard[2][3] = 1;
            hitboard[2][4] = 4;
            for (int c = 0; c < 6; c++)
            {   if (c == 0)
                {   cruis.pos.coord[0][c] = 'C';
                    cruis.pos.coord[1][c] = 'D';
                    cruis.pos.coord[2][c] = 'E';
                }
                else if (c == 1)
                {   cruis.pos.coord[0][c] = '2';
                    cruis.pos.coord[1][c] = '2';
                    cruis.pos.coord[2][c] = '2';
                }
                else if (c == 2)
                {   cruis.pos.coord[0][c] = '-';
                    cruis.pos.coord[1][c] = '-';
                    cruis.pos.coord[2][c] = '-';
                }
                else if (c == 3)
                {   cruis.pos.coord[0][c] = '1';
                    cruis.pos.coord[1][c] = '1';
                    cruis.pos.coord[2][c] = '1';
                }
                else if (c == 4)
                {   cruis.pos.coord[0][c] = '2';
                    cruis.pos.coord[1][c] = '3';
                    cruis.pos.coord[2][c] = '4';
                }
                else if (c == 5)
                {   cruis.pos.coord[0][c] = '\0';
                    cruis.pos.coord[1][c] = '\0';
                    cruis.pos.coord[2][c] = '\0';
                }
            }
        }
        return cruis;
    }
    if (type == 4) // Ship 4 - "Battleship"
    {
        Ship battl;
        battl.name = "Battleship";
        battl.size = 4;
        battl.ship[0] = 'x';
        battl.ship[4] = '\0';
        if (numUsers == 1)
        {   hitboard[3][0] = 4;
            battl.ship[0] = '<';
            battl.ship[1] = '#';
            battl.ship[2] = '#';
            battl.ship[3] = '>';
            battl.pos.orient = 'H';
            hitboard[3][1] = 6;
            hitboard[3][2] = 3;
            hitboard[3][3] = 6;
            hitboard[3][4] = 6;
            for (int c = 0; c < 6; c++)
            {   if (c == 0)
                {   battl.pos.coord[0][c] = 'D';
                    battl.pos.coord[1][c] = 'E';
                    battl.pos.coord[2][c] = 'F';
                    battl.pos.coord[3][c] = 'G';
                }
                else if (c == 1)
                {   battl.pos.coord[0][c] = '7';
                    battl.pos.coord[1][c] = '7';
                    battl.pos.coord[2][c] = '7';
                    battl.pos.coord[3][c] = '7';
                }
                else if (c == 2)
                {   battl.pos.coord[0][c] = '-';
                    battl.pos.coord[1][c] = '-';
                    battl.pos.coord[2][c] = '-';
                    battl.pos.coord[3][c] = '-';
                }
                else if (c == 3)
                {   battl.pos.coord[0][c] = '6';
                    battl.pos.coord[1][c] = '6';
                    battl.pos.coord[2][c] = '6';
                    battl.pos.coord[3][c] = '6';
                }
                else if (c == 4)
                {   battl.pos.coord[0][c] = '3';
                    battl.pos.coord[1][c] = '4';
                    battl.pos.coord[2][c] = '5';
                    battl.pos.coord[3][c] = '6';
                }
                else if (c == 5)
                {   battl.pos.coord[0][c] = '\0';
                    battl.pos.coord[1][c] = '\0';
                    battl.pos.coord[2][c] = '\0';
                    battl.pos.coord[3][c] = '\0';
                }
            }
        }
        return battl;
    }
    if (type == 5) // Ship 5 - "Carrier"
    {
        Ship carr;
        carr.name = "Carrier";
        carr.size = 5;
        carr.ship[0] = 'x';
        carr.ship[5] = '\0';
        if (numUsers == 1)
        {   hitboard[4][0] = 5;
            carr.ship[0] = 'M';
            carr.ship[1] = '#';
            carr.ship[2] = '#';
            carr.ship[3] = '#';
            carr.ship[4] = 'W';
            carr.pos.orient = 'V';
            hitboard[4][1] = 2;
            hitboard[4][2] = 9;
            hitboard[4][3] = 6;
            hitboard[4][4] = 9;
            for (int c = 0; c < 6; c++)
            {   if (c == 0)
                {   carr.pos.coord[0][c] = 'J';
                    carr.pos.coord[1][c] = 'J';
                    carr.pos.coord[2][c] = 'J';
                    carr.pos.coord[3][c] = 'J';
                    carr.pos.coord[4][c] = 'J';
                }
                else if (c == 1)
                {   carr.pos.coord[0][c] = '3';
                    carr.pos.coord[1][c] = '4';
                    carr.pos.coord[2][c] = '5';
                    carr.pos.coord[3][c] = '6';
                    carr.pos.coord[4][c] = '7';
                }
                else if (c == 2)
                {   carr.pos.coord[0][c] = '-';
                    carr.pos.coord[1][c] = '-';
                    carr.pos.coord[2][c] = '-';
                    carr.pos.coord[3][c] = '-';
                    carr.pos.coord[4][c] = '-';
                }
                else if (c == 3)
                {   carr.pos.coord[0][c] = '2';
                    carr.pos.coord[1][c] = '3';
                    carr.pos.coord[2][c] = '4';
                    carr.pos.coord[3][c] = '5';
                    carr.pos.coord[4][c] = '6';
                }
                else if (c == 4)
                {   carr.pos.coord[0][c] = '9';
                    carr.pos.coord[1][c] = '9';
                    carr.pos.coord[2][c] = '9';
                    carr.pos.coord[3][c] = '9';
                    carr.pos.coord[4][c] = '9';
                }
                else if (c == 5)
                {   carr.pos.coord[0][c] = '\0';
                    carr.pos.coord[1][c] = '\0';
                    carr.pos.coord[2][c] = '\0';
                    carr.pos.coord[3][c] = '\0';
                    carr.pos.coord[4][c] = '\0';
                }
            }
        }
        return carr;
    }
}

// { Rl1. }____
Coordinates salvoReset()
{
    Coordinates salvo;
    for (int r = 0; r < 6; r++)
    {
        salvo.aim[r][0] = -2;
        salvo.coord[r][0] = '\0';
        salvo.hitShip[r] = -2;
        salvo.hitShipName[r] = "NULL";
        salvo.recentlySunk[r] = -2;
    }
    return salvo;
}

// { Ra2. }____
void shipSunk_changingValues(Ship *shipPtr, Score *playerPtr, Score *playerAimPtr, Coordinates *salvoPtr, int crntShot)
{ // Adjust the player's score
    playerPtr->shipsShotDown = incrementOne((*playerPtr).shipsShotDown);
    salvoPtr->recentlySunk[crntShot] = salvoPtr->hitShip[crntShot];
    // Establish opponent's losses (ship status, fleet size, total ammo)
    shipPtr->sunk = true;
    playerAimPtr->numShipsLost = incrementOne((*playerAimPtr).numShipsLost);
    double tempCalc = ((playerAimPtr->fleetSize - 1) / playerAimPtr->fleetSize);
    playerAimPtr->usedShots += ((playerAimPtr->maxShots * tempCalc) - (playerAimPtr->usedShots * tempCalc));
}

// { Pi1. }____
//   Calling:  fleetDamageIncrement(salvoPtr, shipPointers~, fleetOrderAimP, (playerAimPtr->fleetSize-playerAimPtr->numShipsLost), playerPtr, playerAimPtr);
void fleetDamageIncrement(Coordinates *salvoPtr, Ship *listOfPointers[5], string *fleetOrder, int reviewSize, Score *playerPtr, Score *playerAimPtr)
{
    Ship *shipPtr = NULL;
    // Advances through the list of ships that have been hit in the last turn 
    for (int c = 0; c < reviewSize; c++)
    {   // Specifier if this particular ship HAS been hit
        if (salvoPtr->hitShip[c] != -2 && salvoPtr->hitShip[c] != -1)
        {   // Discovers which ship has been located
            shipPtr = assignPointer(listOfPointers, NULL, fleetOrder[c], NULL);
            // Determines whether or not it is in the process of sinking
            if (shipPtr->sinking == true && shipPtr->sunk == false)
            {   shipPtr->damage = incrementOne((*shipPtr).damage);
                // Determines if the ship has sustained too much damage and is sinking 
                shipPtr->sunk = shipSunkFromDamage(shipPtr, playerAimPtr, playerPtr, salvoPtr, c); 
                if (!shipPtr->sunk) 
                {   if (3 >= (shipPtr->turnsLeft-shipPtr->damage)) 
                    {   cout << "     +  Your " << shipPtr->name << " will sink in " << (shipPtr->turnsLeft-shipPtr->damage) << " turns!"; 
                        Pause_Program(5); 
                    }
                    else
                    {   cout << "     +  Your " << shipPtr->name << " is sinking!" << endl; 
                        Pause_Program(5);
                    }
                }
            }

        }
    }
    // END of LOOP
}


// ------------- "getInput_" Functions - Obtaining User Input ---------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------------------
//  { Imy. }____
char getInput_yesno()
{
    char inp[100];
    do
    {
        cout << endl << "Choice:  ";
        cin.getline(inp, 100);
        if (toupper(inp[0]) != 'Y' && toupper(inp[0]) != 'N')
            cout << "Answer invalid. Please try again." << endl;
    } 
    while (toupper(inp[0]) != 'Y' && toupper(inp[0]) != 'N');
    return toupper(inp[0]);
}

//  { I01. }____ Bookends the running program; sets the game settings at the very beginning of the program
Settings Launch_Program(int pathway, bool onlyCreatedPvP)
{ // Beginning of the program
    if (pathway == 0)
    { // Program Header
        cout << "PLAY: BATTLESHIP" << endl
             << "-----------------------------------------------------------------------------" << endl;
        cout << "This program allows you to play battleship in various different ways."
             << endl << "Please select the way you would like to play: " << endl;
        // Determine Mode
        Settings gameRun;
        char *fetchInput = new char[100];
        bool errorBadInput = false;
        // Prompt 1:
        cout << endl << "GAME TYPE:"
             << endl << "Which mode would you like to play?   " << "(1) - Regular Mode"
             << endl << "                                     " << "(2) - Salvo Mode"
             << endl << "                                     " << "(3) - Ruin Mode"
             << endl << "(Enter 'i' for explainations about each).";
        do
        { // Flag Set-ups
            flagReset(fetchInput, 2, &errorBadInput, NULL);
            // Prompt and Input
            cout << endl << "Choice:  ";
            cin.getline(fetchInput, 100);
            fetchInput[1] = '\0';
            // Check: Input Validation
            errorBadInput = flag_confirmBadInput(fetchInput, 2, 'i', '1', '3');
            // C1 + C2 performed above ^^^ ...  >> Size: (1 char)  >> Answers: [1, 2, 3, OR 'i']
            // C3 ... Was there something wrong with user input, as checked above?
            if (errorBadInput == true) // Error Message
                cout << "Input is invalid. Please try again." << endl;
            else if (tolower(fetchInput[0]) == 'i')
            {
                errorBadInput = true; // Raise flag so that the program can start again.
                cout << endl << "Regular Mode --- Regular rules of battleship apply: each turn, you are allowed"
                     << endl << "                 to fire one shot at your opponent. You have a 50 shot maximum,"
                     << endl << "                 and 5 ships (maximum) per player. Whoever sinks all of their"
                     << endl << "                 opponent's ships first or damages the other's fleet the most"
                     << endl << "                 wins.";
                Pause_Program(3);
                cout << endl << "Salvo Mode --- Regular rules of battleship apply, but now a player can fire"
                     << endl << "               up to five shots per round (with a 100 total shot maximum). This "
                     << endl << "               collection of shots is called a 'salvo', and a player's salvo "
                     << endl << "               size is dependent on the number of opperable ships in their fleet."
                     << endl << "               Sinking a ship means a decrease of the player's salvo size.";
                Pause_Program(4);
                cout << endl << "Ruin Mode --- Regular rules and salvo rules apply. The two differences are that "
                     << endl << "              the locations of shots fired are no longer recorded, and a ship that "
                     << endl << "              sustains a certain amount of damage will start sinking on its own.";
                Pause_Program(4);
            }
            // Answer transfer
            else if (!errorBadInput)
            {   if (fetchInput[0] == '1')
                {   gameRun.loop_gameSelect = 1;
                    gameRun.gameMode = "Regular Mode";
                    gameRun.play1.maxShots = 50;
                    gameRun.play2.maxShots = 50;
                }
                else if (fetchInput[0] == '2')
                {   gameRun.loop_gameSelect = 2;
                    gameRun.gameMode = "Salvo Mode";
                    gameRun.play1.maxShots = 100;
                    gameRun.play2.maxShots = 100;
                }
                else if (fetchInput[0] == '3')
                {   gameRun.loop_gameSelect = 3;
                    gameRun.gameMode = "Ruin Mode";
                    gameRun.play1.maxShots = 100;
                    gameRun.play2.maxShots = 100;
                }
            }
        } while (errorBadInput == true);
        // Prompt 1 - Result Display
        cout << endl << "  <<" << gameRun.gameMode << ">>  has been selected!" << endl;
        // Prompt 2 fill in:
        if (onlyCreatedPvP == true)
        {   gameRun.play1.name[0] = "Player 1";
            gameRun.play1.name[1] = "[PLAYER 1].............";
            gameRun.play2.name[0] = "Player 2";
            gameRun.play2.name[1] = "[PLAYER 2].............";
            gameRun.loop_numPlayers = 2;
        }
        // Pause Program
        Pause_Program(1);

        delete[] fetchInput;
        return gameRun;
    }
    // Ending of the Program
    if (pathway == 1)
    {
        Settings exitProgram;
        // Display
        cout << endl << "-----------------------------------------------------------------------------" << endl
             << "Program has ended. Continue to close this program." << endl
             << ">> >> >>";
        // Pause Program
        cin.get();
        return exitProgram;
    }
}

//  { Im0. }____
//   Calling:     getInput_fillShot(fetchInputPtr, crntCoordSelection, c_salvoShot, &(playerPtr->usedShots))
void getInput_fillShot(char *inpPt, int *coord, int shotcount, int *usedShots)
{   // Set-Up
    bool badInput;
    char *inp = new char[100];
    do
    { // Flag Set up
        flagReset(inp, 4, &badInput, NULL);
        // Prompt and Input
        cout << "  " << shotcount << " >>  ";
        inp[3] = '\0';
        cin.getline(inp, 100); // Only the first 3 characters count
        // Check: Input Validation
        if (!(tolower(inp[0]) == 'r' && tolower(inp[1]) == 'e' && tolower(inp[2]) == 't' && tolower(inp[3]) == 'r' && tolower(inp[4]) == 'e' && tolower(inp[5]) == 'a' && tolower(inp[6]) == 't'))
        {   // C0 ... ^^ Checks that the user does not want to have their fleet retreat
            // C1 ... does the user input exceed the size maximum?
            badInput = flag_confirmBadInput(inp, 4, NULL, NULL, NULL);
            // C2 ... does the user input NOT pass coordinate-validation?
            if (!badInput && tolower(inp[0]) != 'x')
                badInput = coordError(inp, coord);
            // Error Message
            if (badInput == true)
                cout << "  -- Coordinates unrecognized. Please try again." << endl;
        }
        else    badInput = false; // C0 - the confirmation that the player entered a request to retreat
        // Answer transfer
        if (!badInput)
        {   if (tolower(inp[0]) != 'r' && tolower(inp[0]) != 'x')     
                *(usedShots) += 1;
            inpPt[0] = toupper(inp[0]);
            if (inp[1] != 0)    inpPt[1] = inp[1];
            else                inpPt[1] = inp[2];
        }
    } while (badInput == true);
    delete[] inp;
}

//  { Im1. }____
void getInput_fetchShip(char *inpFill)
{ // Set-Up
    bool inpError;
    char *input = new char[100];
    do
    { // Flag Set-ups
        flagReset(input, 2, &inpError, NULL);
        // Prompt and Input
        cout << endl
             << "Choice:  ";
        cin.getline(input, 100);
        input[1] = '\0';
        // Check: Input Validation
        inpError = flag_confirmBadInput(input, 2, 'd', '1', '5');
        // C1 + C2 performed above ^^^ ...  >> Size: (1 char)  >> Answers: [1, 2, 3, 4, 5, OR 'd']
        if (inpError == true) // Error Message
            cout << "Input is invalid. Please try again." << endl;
        // Answer transfer
        else if (!inpError)
            inpFill[0] = input[0];
    } while (inpError == true);
    delete[] input;
}

//  { Im2. }____
void getInput_placeShipCoord(char *inpFill, int *coordFill, int shipSize)
{ // Set-Up
    bool errorBadInput = false;
    bool errorOutofBounds = false;
    int determineError;
    char *input = new char[100];
    do
    { // Flag Set Up
        flagReset(input, 4, &errorBadInput, &errorOutofBounds);
        // Prompt and Input
        cout << endl
             << "Coordinate:  ";
        cin.getline(input, 100); // Only the first 3 characters count
        input[3] = '\0';
        // Check: Input Validation
        // C1 ... Does the input exceed the requested size of the answer?  >> Size: (2-3 char)
        errorBadInput = flag_confirmBadInput(input, 4, NULL, NULL, NULL);
        // C2 ... Do the answers follow the prompt?  >> Answers: ["A1" - "J10"; "a01" - "j10"]
        if (errorBadInput != true && toupper(input[0]) == 'J' && input[1] == '1' && input[2] >= '0')
            errorOutofBounds = true;
        // C3 ... Can the start of a ship coordinate be placed here?
        else
        {   determineError = shipCoordError(input, coordFill, shipSize);
            // C3.a ... Is the input, ulimately, bad?
            if (determineError == 2)
                errorBadInput = true;
            // C3.b ... Is the input located on a pre-existing ship?
            else if (determineError == 1)
                errorOutofBounds = true;
            // C3.c ... The input is NOT bad, so that means that input can be filled into other solutions
            else if (determineError == 0)
            {   inpFill[0] = toupper(input[0]);
                if (input[1] != 0)
                    inpFill[1] = input[1];
                else
                    inpFill[1] = input[2];
            }
        }
        // Error Messages if the input was bad
        if (errorBadInput == true) // Message 1
            cout << "Coordinates unrecognized. Please try again." << endl;
        if (errorOutofBounds == true) // Message 2
            cout << "Ship is unable to be placed in coordinates. Please try again." << endl;
    } while (errorBadInput == true || errorOutofBounds == true);
    delete[] input;
}

//  { Im3. }____
void getInput_shipOrient(char *inpFill, int *coordPivot, int shipSize, int (*hitboard)[6], int numShips, string *shipOrder)
{
    char *input = new char[100];
    bool inpError[3] = {false, false};
    bool errorBadInput = false;
    bool errorOutofBounds = false;
    int existingShip = -2;
    do
    { // Flag Set-ups
        inpError[1] = false;
        flagReset(input, 2, &errorBadInput, &errorOutofBounds);
        // Prompt and Input
        cout << "Choice:  ";
        cin.getline(input, 100);
        input[1] = '\0';
        // Check: Input Validation
        errorBadInput = flag_confirmBadInput(input, 2, 'r', 'H', 'V');
        // C1 + C2 performed above ^^^ ...  >> Size: (1 char)  >> Answers: ['H', 'V', 'h', 'v', OR 'r']
        if (tolower(input[0] != 'r') && errorBadInput != true)
        { // TRIALS: maximum of two repetitions
            for (int t = 0; (t < 2 && !(inpError[0] == true && inpError[1] == true)); t++)
            { // C3 ... Does the orientation make the ship extend past the ends of the board?
                if (!(inpError[0] == true && inpError[1] == true))
                    if (toupper(input[0]) == 'H' && (9 < (coordPivot[1] + shipSize - 1)))
                        errorOutofBounds = true; // ie: is horizontal orientation too long?
                    else if (toupper(input[0]) == 'V' && (9 < (coordPivot[0] + shipSize - 1)))
                        errorOutofBounds = true; // ie: is vertical orientation too long? '
                // C4 ... Does a ship already exist where the new ship would extend? >> if not, "existingShips" would = -1
                existingShip = verifyShipPlacement(input, hitboard, numShips, errorOutofBounds, inpError);
                // Error message for failed C4 (states which ship is already there)
                if (existingShip != -1 && existingShip != -2)
                {
                    cout << endl << "The ship cannot be placed here. "
                         << endl << "Your " << shipOrder[existingShip] << " is already in that location." << endl;
                    if (!(inpError[0] == true && inpError[1] == true))
                        Pause_Program(4);
                }
                // Flag Reset for loop
                errorOutofBounds = false;
            }
        }
        // Error Messages
        if (inpError[0] == true && inpError[1] == true)
        { // TRIAL failure: coordinates placed by user are inadequate for placement
            inpFill[0] = 'N';
            cout << "This ship cannot be placed here. Please enter different coordinates.";
        }
        else if (errorBadInput == true)
            // C1 or C2 failure
            cout << "Input is invalid. Please try again." << endl << endl;
        // Confirmation of Answer below thru assigning the orientation value
        else
            inpFill[0] = toupper(input[0]);
    } while ((errorBadInput == true || (inpFill[0] != 'H' && inpFill[0] != 'V')) && inpFill[0] != 'N' && inpFill[0] != 'R');
    // ^^^ extended version of usual while construction to guarentee that the four options are the only viable inputs
    delete[] input;
}


// ------------------------- Validate Input Functions -----------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------------------
//  { Rb0. }____
bool flag_confirmBadInput(char *input, int inputBound, char alternateCommand, char boundary1, char boundary2)
{   // C1 ... Does the input exceed the requested size of the answer? >> Size: (2-3 char)
    if (!(input[inputBound] == 'x' && input[inputBound + 1] == 'o' && input[inputBound + 2] == 'g'))
        return true; // Word length check
    // C2 ... Do the answers follow the prompt? >> Answers listed in the parent function
    if (boundary1 != NULL && boundary1 != 'H')
        if (tolower(input[0]) != alternateCommand && !(input[0] >= boundary1 && input[0] <= boundary2))
            return true;
        // When the two options, 'H' or 'V' is being validated
        else if (boundary1 == 'H')
            if (tolower(input[0]) != alternateCommand && toupper(input[0]) != 'H' && toupper(input[0]) != 'V')
                return true;
    return false;
}

//  { Ib0. }____
bool coordError(char *inp, int *coord)
{   int re; // the element representing the  ROW / HORIZONTAL / Letter
    int ce; // the element representing the  COLUMN / VERTICAL / Number
    // Validation: the first input is a letter
    if (!(toupper(inp[0]) >= 'A' && toupper(inp[0]) <= 'J'))
        return true;
    // Validation: the second input is equal to '10'
    else if (inp[1] == '1' && inp[2] == '0')
    {   // Confirm Coordinates, TYPE-1
        ce = (toupper(inp[0]) - 65); // Letter
        re = 9;                      // technically, coordinate '10'
    }
    // Validation: the second input is a 2 digit number resembling: "01"
    else if (inp[1] == '0')
        if (!(inp[2] >= '1' && inp[2] <= '9'))
            return true;
        else
        {   // Confirm Coordinates, TYPE-3
            ce = (toupper(inp[0]) - 65); // Letter
            re = (inp[2] - 49);          // Number
        }
    // Validation: the second input is a 1 digit number resembling: "1"
    else if (!(inp[1] >= '1' && inp[1] <= '9'))
        return true;
    // Secure: the coordinate is turned into an element
    else
    {   // Confirm Coordinates, TYPE-2
        ce = (toupper(inp[0]) - 65); // Letter
        re = (inp[1] - 49);          // Number
    }
    // Clean-up: transfer of the answer
    coord[0] = re; // Letter
    coord[1] = ce; // Number
    return false;
}

//  { Ib1. }____
int shipCoordError(char *inp, int *coord, int size)
{   int re, ce;
    if (!(toupper(inp[0]) >= 'A' && toupper(inp[0]) <= 'J'))
        return 2; // Validation: the first input is a letter
    else if (toupper(inp[0]) == 'J' && 10 < (48 - inp[1] + size))
        return 1;                            // Validation-size: the numerical input would extend the ship outside the board
    else if (inp[1] == '1' && inp[2] == '0') // Validation: the second input is equal to '10'
        if ('J' < (toupper(inp[0]) + size))
            return 1; // Validation-size: the alphabetical input would extend the ship outside the board
        else
        {   // Confirm Coordinates, TYPE-1
            ce = (toupper(inp[0]) - 65);
            re = 9; // technically, coordinate '10'
        }
    else if (inp[1] == '0')
        if (!(inp[2] >= '1' && inp[2] <= '9'))
            return 2; // Validation: the second input is a 2 digit number resembling: "01"
        else
        {   // Confirm Coordinates, TYPE-3
            ce = (toupper(inp[0]) - 65);
            re = (inp[2] - 49);
        }
    else if (!(inp[1] >= '1' && inp[1] <= '9'))
        return 2; // Validation: the second input is a 1 digit number resembling: "1"
    else
    {   // Confirm Coordinates, TYPE-2
        ce = (toupper(inp[0]) - 65); // Letters
        re = (inp[1] - 49);          // Number
    }
    // Clean-up: answer transfer
    coord[0] = re;
    coord[1] = ce;
    return 0;
}

//  { Sb3. }____
int verifyShipPlacement(char *inp, int (*hitboard)[6], int numShips, bool outBounds, bool *orientError)
{ // Switches the currently wrong value with the only other option
    if (outBounds == true)
    {
        cout << endl << "Ship cannot be placed ";
        if (toupper(inp[0]) == 'H' && !(orientError[0] == true))
        {   cout << "horizontally";
            inp[0] = 'V';
            orientError[0] = true;
        }
        else if (toupper(inp[0]) == 'V' && !(orientError[1] == true))
        {   cout << "vertically";
            inp[0] = 'H';
            orientError[1] = true;
        }
        cout << " on the board." << endl;
        cout << "Option chosen instead: " << inp[0] << endl << endl;
        // Below: guarentees that the LOOP(for) repeats so that out-of-bounds can be checked
        return -2;
    }
    if (numShips != 1)
        return linearSearch_prime(inp, hitboard, numShips);
}

//  { Sb4. }____
//   Calling:  shipSunkFromDamage(shipPtr, playerPtr, playerAimPtr, salvoPtr, (c)c_shot); 
bool shipSunkFromDamage(Ship *shipPtr, Score *playerPtr, Score *playerAimPtr, Coordinates *salvoPtr, int specificShot)
{
    if (shipPtr->damage >= shipPtr->turnsLeft)
    {   // ^ ie. the ship sinks if the ship has sustained too much damage and has overtaken the turns left
        shipSunk_changingValues(shipPtr, playerPtr, playerAimPtr, salvoPtr, specificShot);
        cout << "     +  Your " << shipPtr->name << " is no longer operational!";
        Pause_Program(5);
        return true;
    }
    return false;
}

//  { Pb0. }____
bool wasFleetDestroyed(Score *playerPtr, Score *playerAimPtr, int gameMode)
{ // Were all the ships shot down?
    if (playerPtr->shipsShotDown == playerAimPtr->fleetSize)
        return true;
    // Are all of the ships destroyed?
    else if (gameMode == 3)
        if (playerPtr->totalEnemyFleetSunk == playerAimPtr->fleetSize)
            return true;
    // Otherwise, the SECTION 2 loop continues
    return false;
}


// ------------------------ Locate Potential Ship Functions ------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------------------
//  { Pp0. }____
Ship *assignPointer(Ship *listOfPointers[5], char inpChar, string shipElem, int loopUpdate)
{ // Return Value
    Ship *returnPtr = nullptr;
    // Testing ---------------------------------------------------------
    if (shipElem != "NULL")
    {   // When the ship has been found thru "Search_aim" and the name can be identified
        if (shipElem == listOfPointers[0]->name)        returnPtr = listOfPointers[0]; // "Destroyer"
        else if (shipElem == listOfPointers[1]->name)   returnPtr = listOfPointers[1]; // "Submarine"
        else if (shipElem == listOfPointers[2]->name)   returnPtr = listOfPointers[2]; // "Cruiser"
        else if (shipElem == listOfPointers[3]->name)   returnPtr = listOfPointers[3]; // "Battleship"
        else if (shipElem == listOfPointers[4]->name)   returnPtr = listOfPointers[4]; // "Carrier"
    }
    else if (inpChar != NULL)
    {   // When a ship is chosen in order to be placed
        if (inpChar == '1')         returnPtr = listOfPointers[0]; // Option 1 was 'Destoyer'
        else if (inpChar == '2')    returnPtr = listOfPointers[1]; // Option 2 was 'Submarine'
        else if (inpChar == '3')    returnPtr = listOfPointers[2]; // Option 3 was 'Cruiser'
        else if (inpChar == '4')    returnPtr = listOfPointers[3]; // Option 4 was 'Battleship'
        else if (inpChar == '5')    returnPtr = listOfPointers[4]; // Option 5 was 'Carrier'
    }
    // Returning the correct pointer -----------------------------------
    return returnPtr;
}

//  { Ps0. }____
int linearSearch_aim(char *inp, int (*hitboard)[6], int numShips, int aimPtr[3])
{                  // Variable Group A - Regular Linear Search Values
    int index = 0; // "Position" - indicator of which pre-existing ship is being reviewed
    int ship = -1; // Return value
    // Variable Group B - Ship Variables
    int r_strt, r_end, c_strt, c_end; // Specifies the coordinates of analyzed ships
    int shipSize;                     // Specifies the length of an analyzed ship
    int r_aim, c_aim;                 // The coordinates of the user input, in element form
    if (aimPtr != NULL)
    { // If the function is called after a salvo firing
        r_aim = aimPtr[0];
        c_aim = aimPtr[1];
    }
    else if (*inp != NULL)
    { // If the function is called to create and place a ship
        r_aim = static_cast<int>(inp[1] - 49);
        c_aim = static_cast<int>(inp[0] - 65);
    }

    // LOOP: analyzing the coordinates of each ship, per ship
    while (index <= numShips && ship == -1)
    { // Extra Validation: Ship must exist
        if (hitboard[index][0] != -2)
        { // Initialize the information about a ship
            shipSize = hitboard[index][0];
            // the current ship coordinates
            r_strt = (hitboard[index][1]);
            c_strt = (hitboard[index][2]);
            r_end = (hitboard[index][3]);
            c_end = (hitboard[index][4]);
            // Check to see if the aimed point is within the coordinates of the ship
            if (r_strt == r_end)                           // If the ship is horizontally oriented
            {
                if (r_aim == r_strt && c_aim == c_strt)    // >> If the chosen coordinate lands at the starting coordinate of the ship
                    ship = index;                          // >>>>>> Then a ship is FOUND
                else if (r_aim == r_strt)                  // >> If the chosen coordinate is in the same row as the selected ship
                    if (c_aim >= c_strt && c_aim <= c_end) // >>>> If the chosen coordinate is between the start and end column of the ship
                        ship = index;                      // >>>>>> Then a ship is FOUND
            }
            if (c_strt == c_end)                           // If the ship is horizontally oriented
            {
                if (c_aim == c_strt && r_aim == r_strt)    // >> If the chosen coordinate lands at the starting coordinate of the ship
                    ship = index;                          // >>>>>> Then a ship is FOUND
                else if (c_aim == c_strt)                  // >> If the chosen coordinate is in the same column as the selected ship
                    if (r_aim >= r_strt && r_aim <= r_end) // >>>> If the chosen coordinate is between the start and end row of the ship
                        ship = index;                      // >>>>>> Then a ship is FOUND
            }
        }
        // Advance the ship that is reviewed
        index++;
    }
    return ship;
}

//  { Ps1. }____
int linearSearch_prime(char *inp, int (*hitboard)[6], int numShips)
{                  // Variable Group A - Regular Linear Search Values
    int index = 0; // "Position" - indicator of which pre-existing ship is being reviewed
    int ship = -1; // Return value
    // Variable Group B - Ship Variables, specifications for the begin and end ship coordinations
    int ro_strt, ro_end, co_strt, co_end; // ... for ships that already exist
    int rn_strt, rn_end, cn_strt, cn_end; // ... for the ship that is being set up
    int shipSize, testSize;               // Lengths of the ships: "testSize" for the new ship being set up
    int r_aim, c_aim;                     // Coordinates (in element form) for the ship to be located
    // Specifications for the new ships to be placed
    testSize = hitboard[numShips][0];
    rn_strt = hitboard[numShips][1];
    cn_strt = hitboard[numShips][2];
    // Specification for ship orientation
    if (toupper(inp[0]) == 'H') // New ship is horizontal
    {   rn_end = rn_strt;
        cn_end = (cn_strt + testSize - 1);
    }
    if (toupper(inp[0]) == 'V') // New ship is vertical
    {   rn_end = (rn_strt + testSize - 1);
        cn_end = cn_strt;
    }

    // if the proto-ship is HORIZONTAL
    if (rn_strt == rn_end)
    {
        r_aim = rn_strt; // The row value to be validated is stablized
        // LOOP: analyzing the coordinates of each ship, per ship
        while (index < numShips && ship == -1)
        { // Initialize the information about a ship
            shipSize = hitboard[index][0];
            ro_strt = hitboard[index][1];
            co_strt = hitboard[index][2];
            ro_end = hitboard[index][3];
            co_end = hitboard[index][4];
            // LOOP: Testing to see if any location of the new ship overlaps with any other part of the current ship
            for (c_aim = cn_strt; (c_aim < (cn_end + 1) && ship == -1); c_aim++)
            { // if the current ship is HORIZONTAL
                if (ro_strt == ro_end)
                { // Check A: is the currently aimed coordinate on the current ship?
                    if (r_aim == ro_strt && c_aim == co_strt)
                        ship = index;
                    // Check B.1: is the row of the aimed-coordinate on a row occupied by the current ship?
                    else if (r_aim == ro_strt)
                        // Check B.2: is the column of the aimed-coordinate within the body of the current ship?
                        if (c_aim >= co_strt && c_aim <= co_end)
                            ship = index;
                }
                // if the current ship is VERTICAL
                else if (co_strt == co_end)
                { // Check A: is the currently aimed coordinate on the current ship?
                    if (c_aim == co_strt && r_aim == ro_strt)
                        ship = index;
                    // Check B.1: is the row of the aimed-coordinate on a row occupied by the current ship?
                    else if (c_aim == co_strt)
                        // Check B.2: is the column of the aimed-coordinate within the body of the current ship?
                        if (r_aim >= ro_strt && r_aim <= ro_end)
                            ship = index;
                }
            }
            // Advance the ship that is reviewed
            index++;
        }
    }
    // if the proto-ship is VERTICAL
    if (cn_strt == cn_end)
    {
        c_aim = cn_strt; // The column value to be validated is stablized
        // LOOP: analyzing the coordinates of each ship
        while (index < numShips && ship == -1)
        { // Initialize the information about a ship
            shipSize = hitboard[index][0];
            ro_strt = hitboard[index][1];
            co_strt = hitboard[index][2];
            ro_end = hitboard[index][3];
            co_end = hitboard[index][4];
            // LOOP: Testing to see if any location of the new ship overlaps with any other part of the current ship
            for (r_aim = rn_strt; (r_aim < (rn_end + 1) && ship == -1); r_aim++)
            { // if the current ship is HORIZONTAL
                if (ro_strt == ro_end)
                { // Check A: is the currently aimed coordinate on the current ship?
                    if (r_aim == ro_strt && c_aim == co_strt)
                        ship = index;
                    // Check B.1: is the row of the aimed-coordinate on a row occupied by the current ship?
                    else if (r_aim == ro_strt)
                        // Check B.2: is the column of the aimed-coordinate within the body of the current ship?
                        if (c_aim >= co_strt && c_aim <= co_end)
                            ship = index;
                }
                // if the current ship is VERTICAL
                else if (co_strt == co_end)
                { // Check A: is the currently aimed coordinate on the current ship?
                    if (c_aim == co_strt && r_aim == ro_strt)
                        ship = index;
                    // Check B.1: is the row of the aimed-coordinate on a row occupied by the current ship?
                    else if (c_aim == co_strt)
                        // Check B.2: is the column of the aimed-coordinate within the body of the current ship?
                        if (r_aim >= ro_strt && r_aim <= ro_end)
                            ship = index;
                }
            }
            // Advance the ship that is reviewed
            index++;
        }
    }
    return ship;
}

//   Calling: assignFleetWin(Score *playerPtr, bool *fleetDestroyed, Score *addressP1, Score *addressP2);
void assignFleetWin(Score *playerPtr, bool *fleetDestroyed, Score *addressP1, Score *addressP2)
{
       if (playerPtr == addressP1)      fleetDestroyed[1] = true; 
       else if (playerPtr == addressP2) fleetDestroyed[2] = true; 
}

//   Calling:  assignPlayerWinLose(player(Aim)Ptr, player(Aim)Ptr, &playerWin, &playerLose)
void assignPlayerWinLose(Score *playerWin, Score *playerLose, Score **addressWin, Score **addressLose)
{
    *addressWin = playerWin;
    *addressLose = playerLose;
}


// ------------------------- Validate Input Functions -----------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------------------
//  { Ip0. }____
void setSalvo_messageFill(string type, Coordinates *salvo, char *salvoCoord, int rowElm)
{ // Recording the coordinates
    if (rowElm == 9)
        salvoCoord[1] = '0';
    else
        salvoCoord[1] = static_cast<char>(rowElm + 1);
    // Recording the message
    if (type == "missedShot")
    {
        salvoCoord[2] = 'M';
        salvoCoord[3] = 'I';
        salvoCoord[4] = 'S';
        salvoCoord[5] = 'S';
        salvoCoord[6] = '\0';
    }
    if (type == "shipHit")
    {
        salvoCoord[2] = 'H';
        salvoCoord[3] = 'I';
        salvoCoord[4] = 'T';
        salvoCoord[5] = '!';
        salvoCoord[6] = '\0';
    }
}

//  { Sa2. }____
//   Calling 1:    setBoard_charFill('0', 0, shipPtr->pos.coord[0], fetchInputPtr, crntCoordSelection);
//   Calling 2:    setBoard_charFill(fetchInputPtr[0], c, shipPtr->pos.coord[c], shipPtr->pos.coord[0], crntCoordSelection);
void setBoard_charFill(char orient, int index, char *coordFill, char *fPtr, int *coordPivot)
{ // TYPE 0: The Creation of the ship
    if (orient == '0')
    {
        coordFill[0] = fPtr[0];
        if (fPtr[1] == '0')
            coordFill[1] = fPtr[2];
        else
            coordFill[1] = fPtr[1];
        coordFill[2] = '-';
        coordFill[3] = static_cast<char>(48 + coordPivot[0]);
        coordFill[4] = static_cast<char>(48 + coordPivot[1]);
        coordFill[5] = '\0';
    }
    // TYPE A: Horizontal Ship
    else if (orient == 'H')
    {
        coordFill[0] = toupper(fPtr[0] + index); // Letter Value
        if ((coordPivot[0]) == 9)                // Numerical Value
            coordFill[1] = '0';
        else if (fPtr[1] == '0')
            coordFill[1] = (fPtr[2]);
        else
            coordFill[1] = (fPtr[1]);
        coordFill[2] = '-';
        // Establishes the element value - row
        coordFill[3] = static_cast<char>(48 + coordPivot[0]);
        coordFill[5] = '\0';
        // Establishes the element value - column
        if (coordPivot[1] + index != 10)
            coordFill[4] = static_cast<char>(48 + index + coordPivot[1]);
        else
        {
            coordFill[4] = '1';
            coordFill[5] = '0';
            coordFill[6] = '\0';
        }
    }
    // TYPE B: Vertical Ship
    else if (orient == 'V')
    {
        coordFill[0] = toupper(fPtr[0]);  // Letter value
        if ((coordPivot[0] + index) == 9) // Numerical Value
            coordFill[1] = '0';
        else if (fPtr[1] == '0')
            coordFill[1] = (fPtr[2] + index);
        else
            coordFill[1] = (fPtr[1] + index);
        coordFill[2] = '-';
        // Establishes the element value - row
        coordFill[3] = static_cast<char>(48 + index + coordPivot[0]);
        // Establishes the element value - column
        coordFill[4] = static_cast<char>(48 + coordPivot[1]);
        coordFill[5] = '\0';
    }
}

//  { Sa3. }____
//   Calling 1:    setBoard_structFill('0', NULL, c_numShips, crntShipSize, hitboardPtr, NULL, crntCoordSelection);
//   Calling 2:    setBoard_structFill(fetchInputPtr[0], shipPtr, c_numShips, crntShipSize, hitboardPtr, brdPtr, crntCoordSelection);
void setBoard_structFill(char orient, Ship *shipPtr, int shipElem, int shipSize, int (*hitboard)[6], char (*boardFill)[11], int *coordPivot)
{ // TYPE 0: Creation of the ship
    if (orient == '0')
    {
        hitboard[shipElem][0] = shipSize;
        hitboard[shipElem][1] = coordPivot[0];
        hitboard[shipElem][2] = coordPivot[1];
    }
    // TYPE A: Horizontal Ship
    else if (orient == 'H')
    {
        shipPtr->pos.orient = 'H';
        shipPtr->ship[0] = '<';
        shipPtr->ship[shipSize - 1] = '>';
        shipPtr->ship[shipSize] = '\0';
        hitboard[shipElem][3] = (coordPivot[0]);
        hitboard[shipElem][4] = (coordPivot[1] + shipSize - 1);
        for (int c = 0; c < shipSize; c++)
        {
            boardFill[coordPivot[0]][coordPivot[1] + c] = shipPtr->ship[c];
            shipPtr->pos.aim[c][0] = coordPivot[0];
            shipPtr->pos.aim[c][1] = (c + coordPivot[1]);
        }
        shipPtr->pos.aim[shipSize][0] = -1;
    }
    // TYPE B: Vertical Ship
    else if (orient == 'V')
    {
        shipPtr->pos.orient = 'V';
        shipPtr->ship[0] = 'M';
        shipPtr->ship[shipSize - 1] = 'W';
        shipPtr->ship[shipSize] = '\0';
        hitboard[shipElem][3] = (coordPivot[0] + shipSize - 1);
        hitboard[shipElem][4] = (coordPivot[1]);
        for (int c = 0; c < shipSize; c++)
        {
            boardFill[coordPivot[0] + c][coordPivot[1]] = shipPtr->ship[c];
            shipPtr->pos.aim[c][0] = (c + coordPivot[0]);
            shipPtr->pos.aim[c][1] = coordPivot[1];
        }
        shipPtr->pos.aim[shipSize][0] = -1;
    }
}