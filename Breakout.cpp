#include <iostream>
#include <windows.h>
#include <conio.h>
#include <fstream>
#include <string>
#include <ctime>
#include <cstdio>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")



using namespace std;

struct paddleInfo {
    int x, y;
    int oldx;
    int length;
};

struct ballInfo {
    int x, y;
    int oldx, oldy;
    int vx , vy;
    int speedx, speedy;
};

struct userInfo {
    string userName;
    string password;
    int highScore;

};

struct gameInfo {
    int score;
    string userName;
    string date = "";
    int totalTime;
    int remainedHearts;
    bool result;
};



paddleInfo paddle;
ballInfo ball;
gameInfo game[1000];
userInfo users[1000];




string mainMenuOption[6] = {"New Game", "Load Game", "Help", "Game History", "Setting", "Exit"};
string pauseMenuOption[4] = {"Continue", "Restart", "Save the game", "Quit"};
string registerMenuOption[2] = {"Login to your account", "Creat account"};
string endGameMenuOptions[2] = {"Play again", "Quit"};
string settingMenuOptions[6] = {"Ball shape", "Clear game history", "Active sound", "Log out", "Delete aacount", "Quit"};
string ballShapeOptions[8] = {"●","🟣","🔵","🟠","🌏","🥎","🏀","Quit"};

int currentUser;
int currentBall = 0;
int countGame=0;
int countUsers=0;
bool activeGame = false;
int boardWidth, boardLength;
int terLength, terWidth;
int startX, startY;
int remainedHearts = 3;
bool blockAlive[6][10];

int ballWidth = (ballShapeOptions[currentBall]=="●") ? 1 : 2; 

bool gamePaused = false;
bool endGame = false;
bool quitGame = false;
bool loadedGame=false;
bool loggedIn = false;
bool soundActive = true;

time_t gameStartTime;  //زمان شروع بازی     
time_t pauseStartTime; //  pause زمان شزوع  
time_t gameEndTime;   
int totalPauseSecond=0; 
bool timerStarted=false;


string blocks[6][10] = {
    {"\033[95m████","\033[35m████","\033[94m████","\033[96m████","\033[95m████","\033[35m████","\033[94m████","\033[95m████","\033[35m████","\033[94m████\033[0m"},
    {"\033[96m████","\033[95m████","\033[35m████","\033[94m████","\033[96m████","\033[95m████","\033[96m████","\033[35m████","\033[94m████","\033[35m████\033[0m"},
    {"\033[35m████","\033[94m████","\033[96m████","\033[95m████","\033[35m████","\033[94m████","\033[35m████","\033[95m████","\033[35m████","\033[94m████\033[0m"},
    {"\033[94m████","\033[96m████","\033[95m████","\033[35m████","\033[94m████","\033[96m████","\033[95m████","\033[94m████","\033[95m████","\033[96m████\033[0m"},
    {"\033[95m████","\033[35m████","\033[94m████","\033[96m████","\033[95m████","\033[35m████","\033[94m████","\033[95m████","\033[35m████","\033[94m████\033[0m"},
    {"\033[96m████","\033[95m████","\033[35m████","\033[94m████","\033[96m████","\033[95m████","\033[96m████","\033[35m████","\033[94m████","\033[35m████\033[0m"}};



void setCursorPosition(int x, int y);
void hideCursor();

void loadGameHistory();
void sortAndSaveGames();
void gameHistory();
void clearGameHistory();

void loadUsers();
void updateUsersInfo();
void login();
void creatAccount();
void deleteAccount();

void mainMenu(); 
void drawMainMenu(int currentOption); 
void updateMainMenu(int previousOption, int currentOption); 

void pauseMenu();
void drawPauseMenu(int currentOption);
void updatePauseMenu(int previousOption, int currentOption);

void registerMenu();
void drawRegisterMenu(int currentOption); 
void updateRegisterMenu(int previousOption, int currentOption);

void settingMenu();
void drawSettingMenu(int currentOption);
void updateSettingMenu(int previousOption, int currentOption);

void ballShapeMenu();
void drawBallShapeMenu(int currentOption);
void updateBallShapeMenu(int previousOption, int currentOption);

void endGameMenu();
void drawEndGameMenu (int currentOption);
void updateEndGameMenu(int previousOption, int currentOption);

void setupGame();
void handleInput();
void gameLoop();
void drawGameScreen();
void updateGameScreen();
void updateBallLocation();
void blockDistruction(int &nextX,int &nextY);
int remainedBlocks();
void showGameResult(bool result);

string getSaveFileName(const string& username);

bool saveGame();
bool loadGame();
void drawHelp();

void drawBoard(int length, int width);

void getTerminalSize();

void showGameTime(); 








int main() {

    SetConsoleOutputCP(CP_UTF8);
    loadGameHistory();
    loadUsers();
    mainMenu();
    
}

void clearGameHistory() {

    countGame=0;

    //پاک کردن تاریخچه بازی
    ofstream file("gameHistory.txt");
    file.close();
}

void deleteAccount() {

    drawBoard(58, 10);
    setCursorPosition(startX + 2 , startY + 4);
    cout<<"Are you sure you want to delete your account? (y/n) ";

    char input;
    while(true) {
        input = getch();
        if(input=='y' || input=='Y') break;
        else if(input=='n' || input=='N') return;
    }

    for(int i=currentUser ; i<countUsers-1 ; i++) {
        users[i] = users[i+1];
    }

    countUsers--;
    updateUsersInfo();

    string fileName = getSaveFileName(users[currentUser].userName);
    remove(fileName.c_str());

    drawBoard(42, 10);
    setCursorPosition(startX + boardLength/2 - 9 , startY + 4);
    cout<<"Account deleted!✅";
    Sleep(1000);
}


void settingMenu() {

    int currentOption = 0;
    int previousOption;
    drawSettingMenu(currentOption);
    while(true) {
        char input = getch();

        switch (input) {
        case 'w': case 'W' :
            previousOption = currentOption;
            if(currentOption>0) currentOption--;
            updateSettingMenu(previousOption, currentOption);
            break;
        
        case 's' : case 'S' :
            previousOption = currentOption;
            if(currentOption<5) currentOption++;
            updateSettingMenu(previousOption, currentOption);
            break;
        
        case 13 :
            switch (currentOption) {
            case 0:   //shop
                ballShapeMenu();
                drawSettingMenu(currentOption);
                break;
            case 1:  //clear game history
                clearGameHistory();
                drawBoard(42, 10);
                setCursorPosition(startX + boardLength/2 - 11 , startY + 4);
                cout<<"Game history cleared!✅";
                Sleep(1000);
                drawSettingMenu(currentOption);
                break;
            case 2:   //sound
                soundActive = !soundActive;
                drawSettingMenu(currentOption);
                break;
            case 3:   //log out
                loggedIn=false;
                drawBoard(42, 10);
                setCursorPosition(startX + boardLength/2 - 11 , startY + 4);
                cout<<"You're logged out!✅";
                Sleep(1000);
                drawSettingMenu(currentOption);
                break;
            case 4:  //delete account
                if(!loggedIn) login();
                if(loggedIn) deleteAccount();
                drawSettingMenu(currentOption);
                break;
            case 5:  //quit
                return;  
            }
        }
    }
}


void drawSettingMenu(int currentOption) {

    drawBoard(42, 20);
    for(int i=0 ; i<6 ; i++) {
        setCursorPosition(startX + boardLength/2 - settingMenuOptions[i].size()/2, startY + 2 + (i*3));
        if(i==currentOption) {
            if(i==2 && soundActive) cout<<"\033[38;5;198m"<<settingMenuOptions[i]<<"\033[0m"<<"✅";
            else if(i==2 && !soundActive) cout<<"\033[38;5;198m"<<settingMenuOptions[i]<<"\033[0m"<<"❌";
            else if(i!=2)cout<<"\033[38;5;198m"<<settingMenuOptions[i]<<"\033[0m";
        }
        else {
            if(i==2 && soundActive) cout<<settingMenuOptions[i]<<"✅";
            else if(i==2 && !soundActive) cout<<settingMenuOptions[i]<<"❌";
            else if(i!=2) cout<<settingMenuOptions[i];
        }
    }
}


void updateSettingMenu(int previousOption, int currentOption) {

    setCursorPosition(startX + boardLength/2 - settingMenuOptions[previousOption].size()/2, startY + 2 + (previousOption*3));
    if(previousOption==2 && soundActive) cout<<settingMenuOptions[previousOption]<<"✅";
    else if(previousOption==2 && !soundActive) cout<<settingMenuOptions[previousOption]<<"❌";
    else if(previousOption != 2)cout<<settingMenuOptions[previousOption];

    setCursorPosition(startX + boardLength/2 - settingMenuOptions[currentOption].size()/2, startY + 2 + (currentOption*3));
    if(currentOption==2 && soundActive) cout<<"\033[38;5;198m"<<settingMenuOptions[currentOption]<<"\033[0m"<<"✅";
    else if(currentOption==2 && !soundActive) cout<<"\033[38;5;198m"<<settingMenuOptions[currentOption]<<"\033[0m"<<"❌";
    else if(currentOption!=2) cout<<"\033[38;5;198m"<<settingMenuOptions[currentOption]<<"\033[0m";
}


void ballShapeMenu() {
    int currentOption = 0;
    int previousOption;
    drawBallShapeMenu(currentOption);
    while(true) {
        char input = getch();

        switch (input) {
        case 'w': case 'W' :
            previousOption = currentOption;
            if(currentOption>0) currentOption--;
            updateBallShapeMenu(previousOption, currentOption);
            break;
        
        case 's' : case 'S' :
            previousOption = currentOption;
            if(currentOption<7) currentOption++;
            updateBallShapeMenu(previousOption, currentOption);
            break;
        
        case 13 :

            switch (currentOption) {
            case 0:
                currentBall=0;
                drawBallShapeMenu(currentOption);
                break;
            case 1:  
                currentBall=1;
                ballWidth = 2;
                drawBallShapeMenu(currentOption);
                break;
            case 2:
                currentBall=2;
                ballWidth = 2;
                drawBallShapeMenu(currentOption);
                break;

            case 3:
                currentBall=3;
                ballWidth = 2;
                drawBallShapeMenu(currentOption);
                break;
            case 4:
                currentBall=4;
                ballWidth = 2;
                drawBallShapeMenu(currentOption);
                break;
            case 5:
                currentBall=5;
                ballWidth = 2;
                drawBallShapeMenu(currentOption);
                break;
            case 6:
                currentBall=6;
                ballWidth = 2;
                drawBallShapeMenu(currentOption);
                break;
            case 7:  //quit
                return;
            }
        }
    }
}


void drawBallShapeMenu(int currentOption) {

    drawBoard(42, 20);
    
    for(int i=0 ; i<8 ; i++) {
        setCursorPosition(startX + boardLength/2 - ballShapeOptions[i].size()/2, startY+3+(i*2));

        if(i==currentOption && i==currentBall) {  //توپ انتخاب شده و توپی که کرسر روی  ان قرار دارد
            setCursorPosition(startX + boardLength/2 - ballShapeOptions[i].size()/2 - 1, startY+3+(i*2));
            cout<<"\033[38;5;198m<\033[0m"<<ballShapeOptions[i]<<"\033[38;5;198m>\033[0m"<<"✅";
        }
        else if(i==currentBall) {
            cout<<"\033[38;5;198m<\033[0m"<<ballShapeOptions[i]<<"\033[38;5;198m>\033[0m";
        }
        else {
            cout<<ballShapeOptions[i];
        }
    } 
    
}
void updateBallShapeMenu(int previousOption, int currentOption) {
    //   پاک کردن <>از کزینه قبلی منو  
    setCursorPosition(startX + boardLength/2 - ballShapeOptions[previousOption].size()/2 - 1, startY + 3 + (previousOption*2));
    if(previousOption==currentBall) cout<<" "<<ballShapeOptions[previousOption]<<"✅";
    else cout<<" "<<ballShapeOptions[previousOption]<<"     ";

    setCursorPosition(startX + boardLength/2 - ballShapeOptions[currentOption].size()/2 - 1, startY + 3 + (currentOption*2));
    if(currentOption==currentBall) cout<<"\033[38;5;198m<\033[0m"<<ballShapeOptions[currentOption]<<"\033[38;5;198m>\033[0m" <<"✅";
    else cout<<"\033[38;5;198m<\033[0m"<<ballShapeOptions[currentOption]<<"\033[38;5;198m>\033[0m";
}


void loadGameHistory() {

    ifstream file("gameHistory.txt");

    if(!file.is_open()) {
        return;
    }

    string line;
    gameInfo temp;
    while(getline(file, line) ) {
        temp.userName = line;

        getline(file, line);
        temp.date = line;

        getline(file, line);
        temp.remainedHearts = stoi(line);

        getline(file, line);
        temp.score = stoi(line);

        getline(file, line);
        temp.result = stoi(line);

        getline(file, line);
        temp.totalTime = stoi(line);

        game[countGame] = temp;
        countGame++;
    }
    file.close();
}


void sortAndSaveGames() {

    for(int i=0 ; i<countGame-1 ; i++) {
        for(int j=0 ; j<countGame-1-i ; j++) {

            if(game[j].score < game[j+1].score) {
                swap(game[j], game[j+1]);
            }
        }
    }

    ofstream file("gameHistory.txt");

    if(!file.is_open()) {
        return;
    }

    for(int i=0 ; i<countGame ; i++) {

        file<<game[i].userName<<endl;
        file<<game[i].date<<endl;
        file<<game[i].remainedHearts<<endl;
        file<<game[i].score<<endl;
        file<<game[i].result<<endl;
        file<<game[i].totalTime<<endl;
    }

    file.close();
}

void updateUsersInfo() {

    ofstream file("users.txt");

    if(file.is_open()) {

        for(int i=0 ; i<countUsers ; i++) {

            file<<users[i].userName<<endl;
            file<<users[i].password<<endl;
            file<<users[i].highScore<<endl;
           
        }
        file.close();
    }
}


void gameHistory() { 

    if(countGame==0) {
        drawBoard(42, 10);
        setCursorPosition(startX + boardLength/2 - 14, startY + 4);
        cout<<"No game has been finished!❌";
        Sleep(2000);
        return;
    }

    drawBoard(70, 26);
    for(int i=0 ; i<countGame ; i++) {

        if(game[i].date=="") continue;  //یعنی بازی تمام نشده است
        setCursorPosition(startX+2, startY+1+((i%4)*6));
        cout<<"Game "<<i+1<<":";

        setCursorPosition(startX+2, startY+2+((i%4)*6));
        if(game[i].result) {
            cout<<game[i].userName<<" won the game!🎉";
        }
        else {
            cout<<game[i].userName<<" lost the game!💔";
        }

        setCursorPosition(startX+2, startY+3+((i%4)*6));
        cout<<"Score : "<<game[i].score;

        setCursorPosition(startX+2, startY+5+((i%4)*6));
        cout<<"End date and time : "<<game[i].date;

        setCursorPosition(startX+2, startY+4+((i%4)*6));
        cout<<"Remained hearts : "<<game[i].remainedHearts;

        setCursorPosition(startX+1, startY+6+((i%4)*6));
        for(int i=0 ; i<boardLength-2 ; i++) {
            cout<<"\033[35m═\033[m";
        }

        if((i%4==3 ) || (i==countGame-1)) {  //اگر اخر صفحه یا در حال نمایش اخرین بازی باشیم
            setCursorPosition(startX+2, startY+25);
            if(i<countGame-1 && i<=3) { //  اگر صفحه ی بعد وجود داشت و صفحه قبل وجود نداشت
                cout<<"Press \u2192(next page) or Q(menu)";
            }
            else if(i<countGame-1 && i>3) {  //اگر صفحه قبل و بعد وجود داشت
                cout<<"Press \u2192(next page) or \u2190(last page) or Q(menu)";
            }
            else if(i==countGame-1 && i>3) {  //صفحه قبل وجود داشته باشد اما بعد وجود نداشته باشد
                cout<<"Press \u2190(last page) or Q(menu)";
            }
            else cout<<"Press Q to go to menu"; //فقط یک صفحه داریم


            while(true) {

                char input = getch();

                if(input=='Q' || input=='q') return;
                else if (input==77 && i<countGame-1) {  //برای رفتن به صفحه بعد
                    drawBoard(70, 26);
                    break;
                }
                else if(input==75 && i>3) {   //برای رفتن به صفحه قبل
                    i=i-5-(i%4);
                    drawBoard(70, 26);
                    break;
                }
            }
        }
    }
}

void loadUsers() {

    ifstream file("users.txt");
    
    if(!file.is_open()) {
        return;
    }

    string line;
    userInfo temp;
    while(getline(file, line) ) {

        temp.userName = line;

        getline(file, line);
        temp.password = line;

        getline(file, line);
        temp.highScore = stoi(line);

        users[countUsers] = temp;
        countUsers++;
    }
    file.close();
}


void registerMenu() {

    int currentOption = 0;
    int previousOption;
    drawRegisterMenu(currentOption);
    while(true) {
        char input = getch();

        switch (input) {
        case 'w': case 'W' :
            previousOption = currentOption;
            if(currentOption>0) currentOption--;
            updateRegisterMenu(previousOption, currentOption);
            break;
        
        case 's' : case 'S' :
            previousOption = currentOption;
            if(currentOption<1) currentOption++;
            updateRegisterMenu(previousOption, currentOption);
            break;
        
        case 13 :
            switch (currentOption) {
            case 0:  //login
                login();
                return;
            case 1: //create acoount
                creatAccount();
                return;
            }
        }
    }
}


void drawRegisterMenu(int currentOption) {

    drawBoard(42, 10);
    for(int i=0 ; i<2 ; i++) {
        setCursorPosition(startX + boardLength/2 - registerMenuOption[i].size()/2, startY + 3 + (i*3));
        if(i==currentOption) {
            cout<<"\033[38;5;198m"<<registerMenuOption[i]<<"\033[0m";
        }
        else {
            cout<<registerMenuOption[i];
        }
    }
}

void updateRegisterMenu(int previousOption, int currentOption) {

    setCursorPosition(startX + boardLength/2 - registerMenuOption[previousOption].size()/2, startY + 3 + (previousOption*3));
    cout<<registerMenuOption[previousOption];

    setCursorPosition(startX + boardLength/2 - registerMenuOption[currentOption].size()/2, startY + 3 + (currentOption*3));
    cout<<"\033[38;5;198m"<<registerMenuOption[currentOption]<<"\033[0m";

}


void drawEndGameMenu(int currentOption) {

    drawBoard(42, 7);
    for(int i=0 ; i<2 ; i++) {
        setCursorPosition(startX + boardLength/2 - endGameMenuOptions[i].size()/2, startY + 2 + (i*2));
        if(i==currentOption) {
            cout<<"\033[38;5;198m"<<endGameMenuOptions[i]<<"\033[0m";
        }
        else {
            cout<<endGameMenuOptions[i];
        }
    }
}

void endGameMenu() {

    int currentOption = 0;
    int previousOption;
    while(true) {
        char input = getch();

        switch (input) {
        case 'w': case 'W' :
            previousOption = currentOption;
            if(currentOption>0) currentOption--;
            updateEndGameMenu(previousOption, currentOption);
            break;
        
        case 's' : case 'S' :
            previousOption = currentOption;
            if(currentOption<1) currentOption++;
            updateEndGameMenu(previousOption, currentOption);
            break;
        
        case 13 :
            switch (currentOption) {
            case 0:   //play again
                setupGame();
                gameLoop();
                return;
            case 1: //quit
                return;
            }
        }
    }
}


void updateEndGameMenu(int previousOption, int currentOption) {

    setCursorPosition(startX + boardLength/2 - endGameMenuOptions[previousOption].size()/2, startY + 2 + (previousOption*2));
    cout<<endGameMenuOptions[previousOption];

    setCursorPosition(startX + boardLength/2 - endGameMenuOptions[currentOption].size()/2, startY + 2 + (currentOption*2));
    cout<<"\033[38;5;198m"<<endGameMenuOptions[currentOption]<<"\033[0m";
}

void creatAccount() {

    string tempName;
    drawBoard(42, 10);
    setCursorPosition(startX + 3, startY + 2);
    cout<<"Enter your username : ";
    cin>>tempName;

    for(int i=0 ; i<countUsers ; i++) {
        if(tempName==users[i].userName) {
            drawBoard(42, 10);
            setCursorPosition(startX + boardLength/2 - 13, startY + 5);
            loggedIn = false;
            cout<<"Username is alredy taken❌";
            Sleep(1500);
            return;
        }
    }

    users[countUsers].userName = tempName;
    setCursorPosition(startX + 3, startY + 4);
    cout<<"Enter your password : ";
    cin>>users[countUsers].password;
    users[countUsers].highScore = 0; //رکورد اولیه

    setCursorPosition(startX + boardLength/2 - 10, startY + 5);
    cout<<"Account created!✅";
    Sleep(1500);
    currentUser = countUsers;
    loggedIn = true;

    countUsers++;

    updateUsersInfo();
}


void login() {

    string username, password;
    drawBoard(42, 10);
    setCursorPosition(startX + 3, startY + 3);
    cout<<"Enter your username : ";
    cin>>username;
    setCursorPosition(startX + 3, startY + 5);
    cout<<"Enter your password : ";
    cin>>password;

    loggedIn = false;
    for(int i=0 ; i<countUsers ; i++) {
        if(users[i].userName==username && users[i].password==password) {
            currentUser=i;
            loggedIn = true;
            drawBoard(42, 10);
            setCursorPosition(startX + boardLength/2 - 10, startY + boardWidth/2 - 1);
            cout<<"Login successful!✅";
            setCursorPosition(startX + boardLength/2 - 11, startY + boardWidth/2);
            cout<<"Welcome back, "<<username<<"!";
            Sleep(2000);
            return;
        }
    }

    if(!loggedIn) {
        drawBoard(42, 10);
        setCursorPosition(startX + boardLength/2 - 10, startY + boardWidth/2 - 1);
        cout<<"Account not found!❌";
        Sleep(2000);
    }

}

void mainMenu() {

    int currentOption = 0;
    int previousOption;
    drawMainMenu(currentOption);
    while(true) {
        char input = getch();

        switch (input) {
        case 'w': case 'W' :
            previousOption = currentOption;
            if(currentOption>0) currentOption--;
            updateMainMenu(previousOption, currentOption);
            break;
        
        case 's' : case 'S' :
            previousOption = currentOption;
            if(currentOption<5) currentOption++;
            updateMainMenu(previousOption, currentOption);
            break;
        
        case 13 :

            switch (currentOption) {
            case 0:   //new game
                quitGame=false;
                if(!loggedIn) registerMenu();
                if(loggedIn) {
                    gameLoop();
                }
                drawMainMenu(currentOption);
                break;
            case 1:  //load game
                quitGame=false;
                if(!loggedIn) login();
                if(loggedIn && loadGame()) {
                    gameLoop();
                }
                else if (loggedIn && !loadGame()){
                    drawBoard(42, 10);
                    setCursorPosition(startX + boardLength/2 - 8 , startY + 4);
                    cout<<"NO SAVED GAME!❌"<<endl;
                    Sleep(1000);
                
                }
                drawMainMenu(currentOption);
                break;
            case 2:  //help
                drawHelp();
                drawMainMenu(currentOption);
                break;

            case 3: //game history
                gameHistory();
                drawMainMenu(currentOption);
                break;
            case 4: //setting
                settingMenu();
                drawMainMenu(currentOption);
                break;
            case 5: //exit
                exit(0);
            }
        }
    }
}


void updateMainMenu(int previousOption, int currentOption) {

    setCursorPosition(startX + boardLength/2 - mainMenuOption[previousOption].size()/2, startY + 2 + (previousOption*3));
    cout<<mainMenuOption[previousOption];

    setCursorPosition(startX + boardLength/2 - mainMenuOption[currentOption].size()/2, startY + 2 + (currentOption*3));
    cout<<"\033[38;5;198m"<<mainMenuOption[currentOption]<<"\033[0m";

}


void drawBoard(int length, int width) {

    boardLength = length;
    boardWidth = width;
    getTerminalSize();
    // محاسبه موقعیت شروع صفحه
    startX = terLength/2 - boardLength/2;
    startY = terWidth/2 - boardWidth/2;

    system("cls");
    // رسم گوشه ها
    setCursorPosition(startX, startY);
    cout<<"\033[35m╔";
    setCursorPosition(startX+boardLength-1, startY);
    cout<<"╗";
    setCursorPosition(startX, startY+boardWidth-1);
    cout<<"╚";
    setCursorPosition(startX+boardLength-1, startY+boardWidth-1);
    cout<<"╝";
    // رسم دیوار های بالا و پایین
    for(int i=startX+1 ; i<startX+boardLength-1 ; i++) {
        setCursorPosition(i, startY);
        cout<<"═";
        setCursorPosition(i, startY+boardWidth-1);
        cout<<"═";
    }
    // رسم دیوار های چپ و راست
    for(int j=startY+1 ; j<startY+boardWidth-1 ; j++) {
        setCursorPosition(startX, j);
        cout<<"\033[35m║\033[0m";
        setCursorPosition(startX+boardLength-1, j);
        cout<<"\033[35m║\033[0m";
    }

    hideCursor();
}


void drawMainMenu(int currentOption) {

    drawBoard(42, 20);
    
    for(int i=0 ; i<6 ; i++) {
        setCursorPosition(startX + boardLength/2 - mainMenuOption[i].size()/2, startY+2+(i*3));
        if(i==currentOption) {
            cout<<"\033[38;5;198m"<<mainMenuOption[i]<<"\033[0m";
        }
        else {
            cout<<mainMenuOption[i];
        }
    } 
}



void getTerminalSize(){

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hOut, &csbi);
    terLength = csbi.srWindow.Right - csbi.srWindow.Left + 1; // عرض پنجره
    terWidth  = csbi.srWindow.Bottom - csbi.srWindow.Top + 1; // ارتفاع پنجره 
}

void setCursorPosition(int x, int y) {
    static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    cout.flush();
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(hOut, coord);
}

void hideCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}


void setupGame() {

    paddle.length = (ballWidth==2) ? 10 : 9;
    drawBoard(42, 25);
    game[countGame].score = 0;
    ball.vy = -1;
    ball.speedx = 1;
    ball.speedy = 1;

    activeGame = false;
    remainedHearts = 3;
    //موقعیت اولیه راکت
    paddle.x = startX + boardLength/2 - paddle.length/2;
    paddle.y = startY + boardWidth - 3;

    //موقعیت اولیه توپ
    ball.x = paddle.x + paddle.length / 2 - (ballWidth - 1);
    ball.y = paddle.y - 1;
    //مکان قبلی توپ و راکت در ابتدا مکان فعلی در نظر گرفته میشود
    paddle.oldx=paddle.x;
    ball.oldx=ball.x;
    ball.oldy=ball.y;

    // زنده بودن اجر ها در ابتدای بازی
    for(int i=0 ; i<6 ; i++){
        for(int j=0 ; j<10 ; j++){
            blockAlive[i][j]=true;
        }
    }

    gameStartTime=time(0);
    totalPauseSecond=0;
    timerStarted=true;
    gamePaused=false;
}

void drawGameScreen() {

    drawBoard(42, 25);
    //رسم موقعیت راکت
    setCursorPosition(paddle.x, paddle.y);
    for(int i=0 ; i<paddle.length ; i++) {
        cout<<"\033[0m▀";
    }

    //رسم موقعیت توپ
    setCursorPosition(ball.x, ball.y);
    cout<<"\033[0m"<<ballShapeOptions[currentBall];
    
    // // رسم اجر ها
    for(int i=0 ; i<6 ; i++){
        setCursorPosition(startX+1, startY+1+i);
        for(int j=0 ; j<10 ; j++) {
            if(blockAlive[i][j]) cout<<blocks[i][j];
            else cout<<"    ";
        }
    }



    setCursorPosition(startX + boardLength/2 - 3, startY - 2);
    for(int i=0 ; i<remainedHearts ; i++) {
        cout<<"❤  ";
    }
    for(int i=0 ; i<3-remainedHearts ; i++) {
        cout<<"💔 ";
    }


    int length = 25;
    int width = 10;

    int x = startX + boardLength + 5;
    int y = startY + boardWidth/2 - width/2;
    // رسم جدول اطلاعات در کنار جدول بازی
    // رسم گوشه ها
    setCursorPosition(x, y);
    cout<<"\033[35m╔";
    setCursorPosition(x + length -1, y);
    cout<<"╗";
    setCursorPosition(x, y + width - 1);
    cout<<"╚";
    setCursorPosition(x + length - 1, y + width-1);
    cout<<"╝";
    // رسم دیوار های بالا و پایین
    for(int i = x + 1 ; i < x + length - 1 ; i++) {
        setCursorPosition(i, y);
        cout<<"═";
        setCursorPosition(i, y + width - 1);
        cout<<"═";
    }
    // رسم دیوار های چپ و راست
    for(int j = y + 1 ; j < y + width - 1 ; j++) {
        setCursorPosition(x, j);
        cout<<"\033[35m║\033[0m";
        setCursorPosition(x + length - 1, j);
        cout<<"\033[35m║\033[0m";
    }

    setCursorPosition(x + 2, y + 1);
    cout<<"High score : "<<users[currentUser].highScore;
    setCursorPosition(x + 2, y + 3);
    cout<<"Score : "<<game[countGame].score;
    setCursorPosition(x + 2, y + 5);
    cout<<"remained blocks : "<<remainedBlocks();
    hideCursor();



}

void handleInput() {

    if(kbhit()) {

        char move = getch();
        switch (move) {

        case 'a': case 'A' :
            paddle.oldx = paddle.x;
            if(paddle.x - 3 >= startX + 1) {
                paddle.x -= 3;
            }
            else paddle.x = startX + 1;
            if(!activeGame) {
                    ball.oldx = ball.x;
                    ball.oldy = ball.y;
                    ball.x = paddle.x + paddle.length/2 - (ballWidth - 1);
                }
            break;
        case 'd' : case 'D' :
            paddle.oldx = paddle.x;
            if(paddle.x + paddle.length + 3 <= startX + boardLength - 1) {
                paddle.x += 3;
            }
            else paddle.x = (startX + boardLength - 1) - paddle.length;
            if(!activeGame) {
                    ball.oldx = ball.x;
                    ball.oldy = ball.y;
                    ball.x = paddle.x + paddle.length/2 - (ballWidth - 1);
            }
            break;
        case ' ' : // برای شروع بازی space
            if(!activeGame) {
                activeGame = true;
                ball.vy = -1;
                // تعیین جهت اولیه توپ با توجه به موقعیت راکت در شروع بازی
                if(ball.x >= startX + boardLength/2) ball.vx = 1;
                else if(ball.x < startX + boardLength/2) ball.vx = -1;
            }
            break;
        case 'p' : case 'P' : // برنامه pause برای
            pauseMenu();
            return;
        }
    }
}

void blockDistruction(int &nextX , int &nextY) {

    
    int ballLeft  = nextX;
    int ballRight = nextX + ballWidth - 1; 

    for(int i=0 ; i<6 ; i++) {
        for(int j=0 ; j<10 ; j++) {

            if(!blockAlive[i][j]) continue;
            // مختصات شروع اجرها
            int blockY = startY + 1 + (i);
            int blockX = startX + 1 + (j*4);


            int left  = blockX;  // سمت چپ بلوک
            int right = blockX + 3 - (ballWidth - 1); //سمت راست بلوک

            bool hitInside = (ballRight >= left && ballLeft<= right && nextY == blockY);
            if (!hitInside) continue;

            bool fromTop = ball.vy > 0;  //حرکت از بالا به پایین
            bool fromBottom = ball.vy < 0; // حرکت از پایین به بالا
            bool fromLeft = ball.vx > 0; // حرکت از چپ به راست
            bool fromRight = ball.vx < 0; //حرکت از راست به چپ

            bool topBlock = (i>0 && blockAlive[i-1][j]);  //سالم بودن اجر بالا
            bool bottomBlock = (i<5 && blockAlive[i+1][j]); //سالم بودن اجر پایین
            bool leftBlock = (j>0 && blockAlive[i][j-1]); //سالم بودن اجر چپ
            bool rightBlock = (j<9 && blockAlive[i][j+1]); // سالم بودن اجر راست

            // حرکت های مورب و برخورد با گوشه
            if (fromTop && fromRight && topBlock && rightBlock) {
                ball.vx *= -1;
                ball.vy *= -1;
            }
            else if (fromTop && fromLeft && topBlock && leftBlock) {
                ball.vx *= -1;
                ball.vy *= -1;
            }
            else if (fromBottom && fromRight && bottomBlock && rightBlock) {
                ball.vx *= -1;
                ball.vy *= -1;
            }
            else if (fromBottom && fromLeft && bottomBlock && leftBlock) {
                ball.vx *= -1;
                ball.vy *= -1;
            }
            else {
                bool cameFromSide = (ball.x + 1 < left  && ballRight >= left) || (ball.x > right && ballLeft <= right);

                if(cameFromSide) ball.vx *= -1; //برخورد از چپ یا راست
                else ball.vy *= -1;  //برخورد از بالا یا پایین
            }

            setCursorPosition(blockX,blockY);
            cout<<"    ";
            blockAlive[i][j] = false;
            if(soundActive) PlaySound(TEXT("sounds\\breakingBlock.wav"), NULL, SND_FILENAME | SND_ASYNC);

            game[countGame].score += 10;
            if(game[countGame].score > users[currentUser].highScore) {
                users[currentUser].highScore = game[countGame].score;
                updateUsersInfo();
            }

            return;
        }
    }
}


void updateBallLocation() {

    int ballRight = ball.x;
    int ballLeft = ball.x + ballWidth - 1;
    //موقعیت بعدی توپ
    int nextX = ball.x + (ball.vx * ball.speedx);
    int nextY = ball.y + (ball.vy * ball.speedy);

    //برخورد توپ با دیوار سمت چپ
    if (nextX <= startX + 1) {
        ball.vx *= -1;
        nextX = startX + 1;
    }
    //برخورد توپ با دیوار سمت راست
    else if (nextX>= startX + boardLength - 2 - (ballWidth - 1)) {
        ball.vx *= -1;
        nextX = startX + boardLength - 2 - (ballWidth-1);
    }
    //برخورد توپ با سقف
    if (nextY <= startY + 1) {
        ball.vy *= -1;
        nextY = startY + 1;
    }
    blockDistruction(nextX,nextY);
    //افتادن توپ
    if (nextY == paddle.y + 2) {
        if(soundActive) PlaySound(TEXT("sounds\\Fall.wav"), NULL, SND_FILENAME | SND_ASYNC);
        activeGame = false;
        remainedHearts--;
        nextX = paddle.x + paddle.length/2;
        nextY = paddle.y - 1;
        ball.speedx = 1;
    }

    ball.oldx = ball.x;
    ball.oldy = ball.y;

    ball.x = nextX;
    ball.y = nextY;

    //برخورد توپ با راکت
    if(ball.y==paddle.y-1 && ballLeft >= paddle.x && ballRight <= paddle.x + paddle.length && activeGame) {

        ball.vy = -1;

        int hit = (ball.x + ballWidth - 1) - paddle.x;

        if (hit <= 2) ball.vx = -2;
        else if (hit <= 3) ball.vx = -1;
        else if (hit >= 6) ball.vx = 2;
        else if (hit >= 5) ball.vx = 1;
        else ball.vx = 0;    //hit=4
    }
}



int remainedBlocks() {
    int count = 0;
    for(int i=0 ; i<6 ; i++){
        for(int j=0 ; j<10 ; j++){
            if(blockAlive[i][j]) count++;
        }
    }
    return count;
}



void gameLoop() {

    drawBoard(42, 25);
    if(!loadedGame) setupGame(); 
    loadedGame=false;   
    drawGameScreen();
    
    int frameCounter=0;

    while(true) {

        handleInput();

        if(gamePaused) {
            drawBoard(42, 25);
            drawGameScreen();
            gamePaused=false;
        }

        if(quitGame) return;

        if(activeGame && frameCounter%6==0) {
            updateBallLocation();
        }
        if(remainedBlocks()==0) {
            showGameResult(true);
            return;
        }
        if(remainedHearts==0) {
            showGameResult(false);
            return;
        }

        frameCounter++;
        updateGameScreen();
        Sleep(10);
    }
}

void updateGameScreen() {

    
    if(ball.x != ball.oldx || activeGame) {  

        //پاک کردن جای قبلی توپ
        setCursorPosition(ball.oldx, ball.oldy);
        cout<<" ";
        //رسم دوباره توپ
        setCursorPosition(ball.x, ball.y);
        cout<<"\033[0m"<<ballShapeOptions[currentBall];

    }
    

    if(paddle.x != paddle.oldx) {

        if(paddle.x>paddle.oldx) {//راکت یه سمت راست حرکت کرده است

            for(int i = paddle.oldx ; i < paddle.x ; i++) {
                setCursorPosition(i, paddle.y);
                cout<<" ";
            }
        }

        else {//راکت یه سمت چپ حرکت کرده است

            for(int i = paddle.x + paddle.length; i < paddle.oldx + paddle.length; i++) {
                setCursorPosition(i, paddle.y);
                cout<<" ";
            }
        }

        //رسم دوباره راکت
        setCursorPosition(paddle.x, paddle.y);
        for(int i=0 ; i<paddle.length ; i++) {
            cout<<"\033[0m▀";
        }
    }

    
    //پاک کردن جان های  قبلی
    setCursorPosition(startX + boardLength/2 -2, startY - 2);
    cout<<"         ";

    //رسم دوباره جان ها
    setCursorPosition(startX + boardLength/2 - 3, startY - 2);
    for(int i=0 ; i<remainedHearts ; i++) {
        cout<<"❤  ";
    }
    for(int i=0 ; i<3-remainedHearts ; i++) {
        cout<<"💔 ";
    }

    int length = 25;
    int width = 10;

    int x = startX + boardLength + 5;
    int y = startY + boardWidth/2 - width/2;

    setCursorPosition(x + 2, y + 1);
    cout<<"High score : "<<users[currentUser].highScore;
    setCursorPosition(x + 2, y + 3);
    cout<<"Score : "<<game[countGame].score;
    setCursorPosition(x + 2, y + 5);
    cout<<"remained blocks : "<<remainedBlocks();
    

    setCursorPosition(startX + 6, startY + boardWidth);
    cout<<users[currentUser].userName;

    setCursorPosition(startX + 25, startY + boardWidth);
    showGameTime();

    hideCursor();
}

void showGameResult(bool result) {
    
    time_t endtime=time(0);
    int totalTime= difftime(endtime,gameStartTime) - totalPauseSecond;
    string endTime = ctime(&endtime);
    endTime.erase(endTime.size()-1); // پاک کردن \n اخر تابع
    


    drawEndGameMenu(0);
    
    setCursorPosition(startX + boardLength/2 - 5, startY - 4);
    if(result) {
        if(soundActive) PlaySound(TEXT("sounds\\victory.wav"), NULL, SND_FILENAME | SND_ASYNC);
        cout<<" Victory🎉";
    }
    else {
        if(soundActive) PlaySound(TEXT("sounds\\gameOver.wav"), NULL, SND_FILENAME | SND_ASYNC);
        cout<<"Game over!";
    }
    
    setCursorPosition(startX + boardLength/2 - 4, startY - 6);
    for(int i=0 ; i<remainedHearts ; i++) {
        cout<<"❤  ";
    }
    for(int i=0 ; i<3 - remainedHearts ; i++) {
        cout<<"💔 ";
    }

    setCursorPosition(startX + 2, startY - 1);
    cout<<"Score : "<<game[countGame].score;

    setCursorPosition(startX + 29,startY-1);
    showGameTime();

    endGameMenu();

    game[countGame].userName = users[currentUser].userName;
    game[countGame].date = endTime;
    game[countGame].remainedHearts = remainedHearts;
    game[countGame].result = result;
    game[countGame].totalTime = totalTime;
    countGame++;
    sortAndSaveGames();
    loadedGame = false;
    //پاک کردن بازی سیو شده
    ofstream file(getSaveFileName(users[currentUser].userName));
    file.close();
}


void pauseMenu() {

    if(!gamePaused && timerStarted){
        pauseStartTime=time(0);
        gamePaused=true;
    }

    int currentOption = 0;
    int previousOption;
    drawPauseMenu(currentOption);
    while(true) {
        char input = getch();

        switch (input) {
        case 'w': case 'W' :
            previousOption = currentOption;
            if(currentOption>0) currentOption--;
            updatePauseMenu(previousOption, currentOption);
            break;
        
        case 's' : case 'S' :
            previousOption = currentOption;
            if(currentOption<3) currentOption++;
            updatePauseMenu(previousOption, currentOption);
            break;
        
        case 13 :
            switch (currentOption) {
            case 0: //continue
                totalPauseSecond+=difftime(time(0),pauseStartTime);
                return;
            case 1:  //restart
                setupGame();
                drawGameScreen();
                return;
            case 2:  //save
                drawBoard(42, 10);
                if(saveGame()){
                    setCursorPosition(startX + boardLength/2 -4 , startY + 4);
                    cout<<"SAVED✅";
                    Sleep(1000);
                }
                else{
                    setCursorPosition(startX + boardLength/2 - 9, startY + 5);
                    cout<<"SAVE FAILED❌";
                    Sleep(1000);
                }
                drawPauseMenu(currentOption);
                break;
            case 3:  //quit
                quitGame = true;
                return;
            }
        }
    }
}


void drawPauseMenu(int currentOption) {

    drawBoard(42, 20);
    
    for(int i=0 ; i<4 ; i++) {
        setCursorPosition(startX + boardLength/2 - pauseMenuOption[i].size()/2, startY + 3 + (i*4));
        if(i==currentOption) {
            cout<<"\033[38;5;198m"<<pauseMenuOption[i]<<"\033[0m";
        }
        else {
            cout<<pauseMenuOption[i];
        }
    } 
}

void updatePauseMenu(int previousOption, int currentOption) {

    setCursorPosition(startX + boardLength/2 - pauseMenuOption[previousOption].size()/2, startY + 3 + (previousOption*4));
    cout<<pauseMenuOption[previousOption];

    setCursorPosition(startX + boardLength/2 - pauseMenuOption[currentOption].size()/2, startY + 3 + (currentOption*4));
    cout<<"\033[38;5;198m"<<pauseMenuOption[currentOption]<<"\033[0m";

}

string getSaveFileName(const string& username){
    return "save_" + username + ".txt";
}


bool saveGame(){ 
    ofstream file(getSaveFileName(users[currentUser].userName));
    if(! file.is_open()){
        return false;
    }
    file<<users[currentUser].userName<<endl;
    int total=0;
    if(timerStarted){
        total= int(difftime(time(0),gameStartTime)-totalPauseSecond);
        if(total<0) total=0;
    }
    file<<total<<endl;
    file<<game[countGame].score<<endl;//امتیاز
    file<<remainedHearts<<endl;  //جان های باقیمانده
    file<<activeGame<<endl;      // ایا توپ در حال حرکته
    file<<paddle.x<<" "<<paddle.y<<" "<<paddle.oldx<<" "<<paddle.length<<endl;  // موقعیت راکت
    file<<ball.x<<" "<<ball.y<<" "<<ball.oldx<<" "<<ball.oldy<<" "<<ball.vx<<" "<<ball.vy<<" "<<ball.speedx<<" "<<ball.speedy<<endl;        //موقعیت توپ
    // وضعبت اجر ها
    for(int i=0 ; i<6 ; i++){
        for(int j=0 ; j<10 ; j++){
            file<<blockAlive[i][j]<<" ";
        }
    }
    file.close();
    return true;
}

bool loadGame(){


    string savePath = getSaveFileName(users[currentUser].userName);
    ifstream file(savePath);

    if(!file.is_open()){
        return false;
    }

    string saveUser;
    file>> saveUser;
    if(saveUser != users[currentUser].userName){
        file.close();
        return false;
    }
    int total;
    file>>total;
    file>>game[countGame].score;
    file>>remainedHearts;
    file>>activeGame;
    file>>paddle.x>>paddle.y>>paddle.oldx>>paddle.length;
    file>>ball.x>>ball.y>>ball.oldx>>ball.oldy>>ball.vx>>ball.vy>>ball.speedx>>ball.speedy;
    for(int i=0 ; i<6 ; i++){
        for(int j=0;j<10;j++){
            file>>blockAlive[i][j];
        }
    }
    file.close();
    gameStartTime=time(0)-total;
    totalPauseSecond=0;
    timerStarted=true;
    loadedGame=true;
    return true;
}


void drawHelp(){ 

    drawBoard(70, 26);
    

    int x = startX + 2;   
    int y = startY + 1;   

    setCursorPosition(x, y);
    cout << "HOW TO PLAY BREAKOUT";

    y++;  

    setCursorPosition(x, y++);
    cout << "You control a paddle at the bottom of the screen.";

    setCursorPosition(x, y++);
    cout << "A ball bounces around and breaks the blocks above.";

    y++;

    setCursorPosition(x, y++);
    cout << "GOAL:";
    setCursorPosition(x, y++);
    cout << "- Destroy all blocks without losing the ball.";

    y++;

    setCursorPosition(x, y++);
    cout << "CONTROLS:";
    setCursorPosition(x, y++);
    cout << "A     : Move paddle left";
    setCursorPosition(x, y++);
    cout << "D     : Move paddle right";
    setCursorPosition(x, y++);
    cout << "SPACE : Start the ball";
    setCursorPosition(x, y++);
    cout << "P     : Pause the game";

    y++;

    setCursorPosition(x, y++);
    cout << "RULES:";
    setCursorPosition(x, y++);
    cout << "- If the ball falls, you lose one heart.";
    setCursorPosition(x, y++);
    cout << "- You start with 3 hearts.";

    y++;

    setCursorPosition(x, y++);
    cout << "WIN / LOSE:";
    setCursorPosition(x, y++);
    cout << "- Break all blocks to win.";
    setCursorPosition(x, y++);
    cout << "- Lose all hearts to lose.";

    y += 2;
    setCursorPosition(x, y++);
    cout << "Press any key to return to menu...";

    getch();
}


void showGameTime(){ 

   if(!timerStarted) return;
    time_t now=time(0);
    int playedSeconds;
    if(gamePaused)  playedSeconds=difftime(pauseStartTime,gameStartTime)-totalPauseSecond;
    else playedSeconds=difftime(now,gameStartTime)-totalPauseSecond;
    if(playedSeconds<0) playedSeconds=0;
    int min= playedSeconds / 60;
    int sec= playedSeconds % 60;
    
    cout<<"Time: ";
    if(min<10) cout<<"0";
    cout<<min<<":";
    if(sec<10) cout<<"0";
    cout<<sec; 
}


