#include "mail.h"
#include <time.h>
#define FILE_NAME "bad_words.txt"
#define BINARY_FILE_NAME "binary_file_mail_list.bin"

struct Mail {
        int UID;
        char receiver[128];
        char copy_to[128];
        char sender[128];
        char title[128];
        char content[1024];
        char date[20];
};

struct Node {
    struct Mail mail;
    struct Node *next;
};

typedef struct Node Node;
typedef struct Mail Mail;

int is_empty(FILE *fb, int boolean);
FILE *open_file(char* mode);
void  add_element (Node **add_mail_object, int i);
void  free_linked_list (Node *head);
char * small_letters(char str[264]);
void deleteNode(struct Node **head, struct Node *n);

char * small_letters(char str[264]) {
    int iterator;
    for(iterator= 0; str[iterator]; iterator++){
            if (str[iterator] >= 'A' && str[iterator] <= 'Z') {
                str[iterator] = str[iterator] - ('A'-'a');
            }
    }
    return str;
}

void  free_linked_list (Node *head) {
    Node *temp = head;

    while(head != NULL) {
        head = head->next;
        free(temp);
        temp = head;
    }
}

void deleteNode(struct Node **head, struct Node *current) {

    if (current == NULL) {
        return;
    }

    if(*head == current) {
        if((*head)->next == NULL) {
            printf("Only 1 node. Cannot delete all nodes !!!");
        } else {
            *head = (*head)->next;
            //head->mail = head->next->mail;
            //current = head->next;
            //head->next = head->next->next;
        }
    } else {
        struct Node *prev = *head;
        while(prev->next != NULL && prev->next != current) {
            printf("prev->mail.UID: %d\n",prev->mail.UID);
            prev = prev->next;
        }
        printf("Is error here?");
        if (prev->next != NULL) {
            prev->next = prev->next->next;
        }
    }
}

FILE *open_file(char* mode) {
    FILE *fb;
    fb = fopen(BINARY_FILE_NAME, mode);
    if (fb == NULL) {
            printf("Error opening the file: %s in mode: %s\nTrying to create a new one with mode: wb !\n", BINARY_FILE_NAME, mode);
            fclose(fb);

            fb = fopen(BINARY_FILE_NAME, "wb");

            if (fb ==NULL) {printf("Error opening the file: %s in mode: wb\n", BINARY_FILE_NAME);}

            fclose(fb);
            fb = fopen(BINARY_FILE_NAME, mode);
            if (fb==NULL) {printf("Error opening the file: %s in mode: %s AGIAIN !!!\n", BINARY_FILE_NAME, mode);}
    }
    return fb;
}

void  add_element (Node **add_mail_object, int i) {
    (*add_mail_object)->mail.UID = i;

    printf("UID: %d\n",(*add_mail_object)->mail.UID);
    fflush(stdin);
    printf("Receiver: ");
    gets((*add_mail_object)->mail.receiver);
    printf("Send a copy to:");
    gets((*add_mail_object)->mail.copy_to);
    printf("Sender:");
    gets((*add_mail_object)->mail.sender);
    printf("Title:");
    gets((*add_mail_object)->mail.title);
    printf("Content:");
    gets((*add_mail_object)->mail.content);

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char date[20];
    snprintf(date, sizeof(date), "%02d.%02d.%04d %02d:%02d:%02d\n", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
    printf("Date: %s\n\n", date); // Holds the current time for adding a mail to the list
    strcpy((*add_mail_object)->mail.date, date);

    (*add_mail_object)->next=NULL;
}

// is_empty(FILE *fb) - Gets the FILE pointer and returns 1 if it is empty and 0 if it is NOT empty
int is_empty(FILE *fb, int boolean) {
    fseek(fb, 0L, SEEK_END); //0L - const long literal 0
    if (ftell(fb) == 0L) { // ftell - get current position of stream
        if (boolean) {puts("EMPTY BINARY FILE!!!");} //PUTS only if told so
        return 1; //Empty
    }
    rewind(fb);
    return 0; //NOT empty
}

// add_mail()  - adds a new mail (Node) object to the binary file mail.bin
void add_mail() {
    struct Node* head = malloc(sizeof(Node));
    //struct Node* add_mail_object = malloc(sizeof(Node));
    struct Node* current = malloc(sizeof(Node));

    if (head == NULL) {exit(2);}
    //if (add_mail_object == NULL) {exit(3);}
    if (current == NULL) {exit(4);}

    FILE *fb = open_file("rb");

    if (is_empty(fb, 1)) {
        fclose(fb);
        fflush(fb);

        fb = open_file("wb");
        add_element (&head, 1);

        head->next = NULL;

        if (!fwrite(&head->mail, sizeof(Mail), 1, fb)) {
            puts("Error writing whole struct to file");
        }

        fflush(fb);

    } else {

        int UID_number = 0;
        int i = 0;
        Node *prev;

        while (fread(&current->mail, sizeof(Mail), 1, fb)) {
            if(!i) {head = current; i = 1;}

            UID_number = current->mail.UID;

            prev = current;

            current = current->next;
            current = malloc(sizeof(Node));
            prev->next = current;
        }

        //ok now we add the add_mail_object here and write the list again
        add_element (&current, UID_number + 1);

        fclose(fb);
        fflush(fb);

        fb = fopen(BINARY_FILE_NAME, "wb");

        if (fb == NULL) {printf("Error opening the file: %s in mode: wb !\n", BINARY_FILE_NAME);}

        current = head;

        while (fwrite(&current->mail, sizeof(Mail), 1, fb)) {
            current = current->next;
        }
    }

    free_linked_list(head);

    //free(add_mail_object);
    //free(current);
    fflush(fb);
    fclose(fb);
}

void reveal_mail() {
    struct Node* head = malloc(sizeof(Node));
    struct Node* current = malloc(sizeof(Node));

    if (head == NULL) {exit(5);}
    if (current == NULL) {exit(6);}

    FILE *fb = open_file("rb");
    int i = 0;
    Node *prev;


    while (fread(&current->mail, sizeof(Mail), 1, fb)) {
        if(!i) {head = current; i = 1;}

        char heading[32];
        snprintf(heading, sizeof(heading), "Email with UID | %d | ", current->mail.UID);

        puts("");
        printf(heading);
        printf("Sender: ");
        puts(current->mail.sender);
        printf(heading);
        printf("Title:  ");
        puts(current->mail.title);
        printf(heading);
        printf("Date:   ");
        puts(current->mail.date);

        printf("_______________________________________________\n");

        prev = current;
        current = current->next;
        current = malloc(sizeof(Node));
        prev->next = current;
    }

    free_linked_list(head);
    //free(current);
    fclose(fb);
}

void find_by_UID(int id) {
    struct Node* head = malloc(sizeof(Node));
    struct Node* current = malloc(sizeof(Node));
    if (head == NULL) {exit(7);}
    if (current == NULL) {exit(8);}

    FILE *fb = open_file("rb");
    int i = 0;
    Node *prev;

    int last_UID = 0;
    while (fread(&current->mail, sizeof(Mail), 1, fb)) {
        if(!i) {head = current; i = 1;} else {i = 0;}
        last_UID = current->mail.UID;
        if (current->mail.UID == id) {

            char heading[32];
            snprintf(heading, sizeof(heading), "Email with UID | %d |", id);

            printf(heading);
            printf(" Receiver: ");
            puts(current->mail.receiver);
            printf(heading);
            printf(" Copy to:  ");
            puts(current->mail.copy_to);
            printf(heading);
            printf(" Sender:   ");
            puts(current->mail.sender);
            printf(heading);
            printf(" Title:    ");
            puts(current->mail.title);
            printf(heading);
            printf(" Content:  ");
            puts(current->mail.content);
            printf(heading);
            printf(" Date:     ");
            puts(current->mail.date);

            puts("_____________________________________________________");

            //free(head);
            //free(current);
            break;
        }

        prev = current;
        current = current->next;
        current = malloc(sizeof(Node));
        prev->next = current;
    }

     if (current->mail.UID != id) {
        printf("Email with UID %d not found\n", id);
        printf("Maximum UID: %d", last_UID);
    }

    free_linked_list(head);
    //free(current);
    close(fb);
}

int delete_spam() {
    struct Node* head = malloc(sizeof(Node));
    struct Node* current = malloc(sizeof(Node));
    if (head == NULL) {exit(9);}
    if (current == NULL) {exit(10);}

    int i = 0;
    Node *prev = head;
    int  bad_words_count = 0;
    int  words_count = 0;
    char lines[1000][264];
    char line[1000];

    FILE *fb = open_file("rb");
    FILE *f = fopen(FILE_NAME, "r");

    if (f == NULL) {
        printf("\nError reading file: %s\n ", FILE_NAME);
        return 1;
    } else {printf("\nFile %s successfully read!\n\n", FILE_NAME);}

    // Reads the text file line by line
    while (fgets(line, sizeof(line), f)) {
        line[strlen(line) - 1] = '\0';
        strcpy(lines[bad_words_count], line);
        //lines[bad_words_count] = line;
        bad_words_count++;
        //printf("Word = %s\n", line);
    }

    printf("Spam words in file %s: %d\n\n", FILE_NAME,  bad_words_count);
    puts("_____________________________________________________");

    fclose(f);

    char ch = ' '; // For deletion process control

    while (fread(&current->mail, sizeof(Mail), 1, fb)) {
        if(!i) {head = current; i = 1;}

        char *words[1000];
        char * word;
        char *cpy_content = malloc(sizeof(current->mail.content));
        strcpy(cpy_content, current->mail.content);

        word = strtok (cpy_content, "  ");
        words[words_count] = word;

        while (word != NULL) {
            word = strtok (NULL, "  ");
            if (word == NULL) {break;}
            words_count++;
            //printf ("Word = %s\n", word);
            words[words_count ] = word;
        }

        //printf("Words count = %d\n", words_count);

        int k;
        int occurences = 0;
        for (k = 0; k < bad_words_count; k++) {
            char *tmp = current->mail.content;

            while((tmp = strstr(small_letters(tmp), small_letters(lines[k])))) {
                occurences++;
                tmp++;
            }
        }

        if ((float)occurences > (float)words_count/5) {
            printf("\nEmail with UID = %d is SPAM \nSPAM: %d out of %d words\n", current->mail.UID, occurences, words_count+1);

            //The deletion of spam emails goes here:

            puts("Do you want to delete the mail? (Y/N)");
            puts("Delete all (A) ");
            while(ch != 'Y' && ch != 'y' && ch != 'N' && ch != 'n' && ch != 'A' && ch != 'a') {
                if(ch != 'a' && ch != 'A') {ch = getch();}
                if (ch == 'Y' || ch == 'y' || ch == 'A' || ch == 'a') {

                }
            }

            if (ch == 'n' || ch == 'N') {
                prev = current;
                current = malloc(sizeof(Node));
                prev->next = current;
            }

            ch = ' ';
            puts("_____________________________________________________");

        } else {
            prev = current;
            current = malloc(sizeof(Node));
            prev->next = current;
        }
        words_count = 0;
    }

    if (prev->next == NULL) {
        free(head);
        free(current);
        fclose(fb);
        return;
    }

    prev->next = NULL;

    fclose(fb);
    fflush(fb);
    fb = open_file("wb");

    if (fb == NULL) {printf("Error opening the file: %s in mode: wb !\n", BINARY_FILE_NAME);}

    current = head;

    while (fwrite(&current->mail, sizeof(Mail), 1, fb)) {
        printf("UID = %d\n", current->mail.UID);
        current = current->next;
    }

    free_linked_list(head);
    //free(current);
    fclose(fb);
    return 0;
}
