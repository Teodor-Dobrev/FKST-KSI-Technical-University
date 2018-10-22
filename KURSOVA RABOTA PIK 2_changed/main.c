#include "mail.h"

#define number_of_menu_options 4 // from 1 to n

void function_caller ();
void menu();
void arrow_or_not (int current_position);
void arrow_mover ();
void function_caller ();

int arrow_position = 1;
char input = '0';

int main() {
    menu();
    return 0;
}

// menu() - the menu visualization; where the user chooses what functionality to be run
void menu() {
    while (input != 27) { // esc key = 27

        // If you add more options make sure that you change number_of_menu_options at the top
        arrow_or_not(1);
        printf ("1: Add a new mail to the list\n");
        arrow_or_not(2);
        printf ("2: Show the mails in a list (briefly)\n");
        arrow_or_not(3);
        printf ("3: Find a mail with it\'s UID number \n");
        arrow_or_not(4);
        printf ("4: Delete all spam emails\n");
        printf ("\nPress ESC to exit!\n");
        input = getch();
        system("cls");

        //when enter is pressed the arrowed option runs its function
        if (input == 13) {input = '1'; function_caller();} // enter key = 13;

        arrow_mover();
    }
}

// arrow_or_not() - decides if an arrow should be drawn at a certain menu option
void arrow_or_not (int current_position) {
    char arrow[] = "---> "; //5 places
    char blank[] = "     "; //also 5 places

    HANDLE  hConsole = GetStdHandle(STD_OUTPUT_HANDLE); //something about windows colors
    if (arrow_position == current_position) {
            SetConsoleTextAttribute(hConsole, 4); // 4 = red
            printf("%s", arrow);
            SetConsoleTextAttribute(hConsole, 2); // 2 = green
    } else {printf("%s", blank);}
}

// arrow_mover() - changes the arrow movement in the menu function
void arrow_mover() {
    if ((input >= '1') && (input <= (char)(number_of_menu_options+48))) {
        arrow_position = (int)(input - 48); // ascii numbers to int numbers conversion
    } else if (input == 0x48) { // up arrow key = 0x48
        if (arrow_position != 1) {
            arrow_position--;
        } else {
            arrow_position = number_of_menu_options;
        }
    } else if (input == 0x50) { //down arrow key = 0x50
        if (arrow_position != number_of_menu_options) {
            arrow_position++;
        } else {
            arrow_position = 1;
        }
    }
}

// function_caller () - Calls the chosen function by the user from the menu
void function_caller () {
    HANDLE  hConsole = GetStdHandle(STD_OUTPUT_HANDLE); //something about windows colors
    SetConsoleTextAttribute(hConsole, 6); //yellow
    int id;
    switch (arrow_position) {
        case 1:
            puts("You have chosen to add a new mail!\n");
            SetConsoleTextAttribute(hConsole, 2); //green
            puts("____________________________________________________\n");
            add_mail();
            break;
        case 2:
            puts("You have chosen to show all the mails briefly!\n");
            SetConsoleTextAttribute(hConsole, 2); //green
            puts("_________________________________________________________________________________\n");
            reveal_mail();
            break;
        case 3:
            puts("You have chosen to search for a certain mail by its UID number!\n");
            SetConsoleTextAttribute(hConsole, 2); //green
            puts("_________________________________________________________________________________\n");
            printf("\nPlease enter an ID: ");
            fflush(stdin);
            scanf("%d", &id);
            puts("_________________________________________________________________________________\n");
            find_by_UID(id);
            break;
        case 4:
            puts("You have chosen to delete all the spam mails! GOOD CHOICE!\nIf you have many mails the removal is going to take a while!!!\n");
            SetConsoleTextAttribute(hConsole, 2); //green
            puts("_________________________________________________________________________________");
            int res = delete_spam();
            if (res == 1) {puts("Cannot find the file with the bad words...");}
            break;
        default: puts("\n\n\tHow did you even get here, there is no function with that name\n\tPlease check your #define number_of_menu_options at the top of main.c");
    }

    SetConsoleTextAttribute(hConsole, 4); //red
    puts("\n\nPress any button to go back to the menu...");
    SetConsoleTextAttribute(hConsole, 2); //green
    getch();
    system("cls");
}
