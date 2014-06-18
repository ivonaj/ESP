#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//clearerr() Ã¼berprÃ¼fen, ob benÃ¶tigt...

typedef struct _person
{
  char name[100];
  struct _personnode *freunde;
} person;

typedef struct _personnode
{
  person *pers;
  struct _personnode *next;
} personnode;

typedef struct llnode
{
  char name[100];
  //personennode *freunde;
  struct llnode *next; 
} llnode;



typedef struct personennode
{
  llnode* Person;
  
  struct personennode *next;
  
}personennode;

  
struct llnode *head = NULL;  //initialiere den head, ist das erste elem.
  
int readCsvFile(char *fileName);
int StandardPrompt();
char* getCommand(char* string, char* command);
int CheckCorruption(FILE *csvfile);
//node *addElement(node *currentElement, char *people);
//void printList(node *liste);
//char* String2(char* csvline);
void freeList();
int anHaengen(char *prasemi, char *postsemi);
int sortList(char *prasemi);
void printList();
int printError(int code);
int deletenode();
//int friendsDup();
int listFriends(char *semicolon);


int main(int argc, char** argv)
{
  int result;
  
  if(argc != 2)
  { 
    printf("usage: %s file.csv\n", argv[0]);
    return -1;
  }

  result = readCsvFile(argv[1]);
  if(result!=0)
  {
    return printError(result);
  }

  //was passiert sonst?

  
  /* einlesen der gesamten datei in den speicher
  char * buffer;
  fseek(csvfile, 0, SEEK_END);
  long int lSize = ftell(csvfile);
  rewind(csvfile);
  buffer = (char*) malloc (sizeof(char)*lSize);
  if (buffer == NULL) 
  {
    fputs ("error: out of memory",stderr); 
    fclose(csvfile);
    return -2;
  }
  if(fread (buffer,sizeof(char),lSize,csvfile) != lSize) 
  {
    fputs ("error: could not read file",stderr); 
    fclose(csvfile);
    free(buffer);
    return -4;
  }
  */
  
  //CheckCorruption(csvfile);  
  
/*  
  printf("success\n");
  return 0;
  
  
  node *liste, *currentElement;
  liste = (node*) malloc(sizeof(node));
  
  liste->next=NULL;
  //liste->right=NULL;
  currentElement = liste;
  
  

  char csvline[400];
  while((fgets(csvline, 400, csvfile)) != NULL );
  {
   

    
    char semi[] = ";";
    char* prasemic;
    prasemic = strstr(csvline, semi); //liefert einen pointer zurÃ¼ck, wo er es findet
    *prasemic = 0; __attribute__((unused))//Ã¼berschreiben ; mit NULL
    char* string1 = csvline;
    
     
  currentElement = addElement(currentElement,string1);
  
  char* string3= String2(csvline);
  currentElement = addElement(currentElement,string3);
 
  }
  
  printList(liste);

  StandardPrompt();
  */
 return EXIT_SUCCESS;
}

//------------------------------------------------------------------------------
int readCsvFile(char *fileName)
{
  FILE *csvFile = NULL;
  char csvline[400];
  char* semicolon;
  char* newline;
  int result;

  csvFile = fopen(fileName, "r");
  if(csvFile == NULL)                  //Datei existiert nicht fopen
  {
    return -3;
  }
  
  do
  {
    if(fgets(csvline, 400, csvFile) == NULL)
    {
      //variante 1
      if(feof(csvFile) != 0)
      {
        fclose(csvFile);
        return -5; 
      }
      fclose(csvFile);
      return -4; 
      /* variante 2
      if(ferror(csvFile)!=0)
      {
        printf("error: could not read file\n");
        return -4; 
      }
      printf("error: file corrupt1\n");
      return -5;
      */
    }
    //zeile gelesen, daten stehen in csvline
    semicolon=strchr(csvline, ';');
    if(semicolon == NULL) //kein semikolon in der zeile
    {
      fclose(csvFile);
      return -5; 
    }
    if(semicolon == csvline) // strichpunkt am anfang der zeile
    {
      fclose(csvFile);
      return -5; 
    }
    *semicolon='\0';
    semicolon++;
    if(*semicolon == '\n' || *semicolon == '\0') //sp am ende der zeile
    {
      fclose(csvFile);
      return -5; 
    }
    if(strchr(semicolon, ';') != NULL) //noch ein strichpunkt
    {
      fclose(csvFile);
      return -5; 
    }
    newline=strchr(semicolon, '\n');
    if(newline != NULL) //newline character am ende wegschneiden
    {
      *newline='\0';
    }
    if(strcmp(csvline, semicolon)==0) //beide namen gleich
    {
      fclose(csvFile);
      return -5; 
    }
    //TODO freundschaft in der jeweiligen freundesliste eintragen
    //TODO freundschaftsduplicat checken: csv datei duplizieren und vergleichen
    //      oder string1 und string zwei vergleichen
    
    
    if((result=anHaengen(csvline,semicolon))!=0)
    {
      fclose(csvFile);
      return result;
    }
    if((result=anHaengen(semicolon,csvline))!=0)
    {
      fclose(csvFile);
      return result;
    }
      
    //int friends(csvline, semicolon);
    
  } while(feof(csvFile) == 0);
  
/*
  if((result=friendsDup())!= 0)
  {
    //fclose(csvFile);
    return result;
  }
*/
  
    StandardPrompt();
  
  fclose(csvFile);
  return 0;
}
//------------------------------------------------------------------------------
int listFriends(char *semicolon)
{
/*
  personennode *new_elem2 = NULL;
  memcpy(new_elem2->Person, semicolon, 99);
  new_elem2->Person[99] = '\0';
  
  
  
  
  return Person;
*/
  return 0;
}

//------------------------------------------------------------------------------
int anHaengen(char *prasemi, char* postsemi)
{
  int result;
  llnode *new_elem = NULL;
  llnode *prev_elem = NULL;
  llnode *cur_elem = NULL; //Zeiger für den Zugriff auf einzelne Elmente

  if((new_elem = malloc(sizeof(llnode))) == NULL) 
  {
    return -2;
  }
  //speicher vom buffer in das listenlement kopieren
  memcpy(new_elem->name, prasemi, 99);
  //sollte kein /0 im kopierten string sein, wird er abgeschnitten
  new_elem->name[99]='\0';
  
  cur_elem = head;
  while(cur_elem!=NULL && (result=strcmp(cur_elem->name, new_elem->name))<0)
  {
    prev_elem=cur_elem;
    cur_elem=cur_elem->next;
  }
  
  //listFriends(postsemi);
  if(result==0)
  {
    free(new_elem);
  }
  else
  {
    new_elem->next = cur_elem;
    if(prev_elem!=NULL)
    {
      prev_elem->next = new_elem;
    }
    else
    {
      head=new_elem;
    }
  }
  return 0;  
}

//------------------------------------------------------------------------------
 
int sortList(char *prasemi)
{
  return 0;
/*
    llnode *zeiger;
    llnode *zeiger1;
    
    if(head == NULL) //ist es das erste Element in der Liste?
    {
      anHaengen(prasemi);
    }
    else // Suche  bis es gefunden wird
    {
      zeiger = head;
      while(zeiger != NULL && (strcmp(zeiger->name, copy)<0)) //abdef
      {
        zeiger = zeiger->next; //zeiger ist geht bis  vor d
      }
      if(zeiger == NULL) // wenn null ist dann hinten anhÃ¤ngen
      {
        anHaengen(prasemi);
      }
      else if(zeiger == head) //unser Elment das kleinste ist - vor head einfÃ¼gen
      {
        zeiger = (struct llnode*) malloc(sizeof(struct llnode));
        if(NULL == head)
        {
          printf("Kein Speicher\n");
          return;
        }
        strcpy(head->name,copy);
        head->next = zeiger;
        //free(copy);  
        }
        else //Element ist irgendwo in der Mitte
        {
          zeiger1 = head;
          while(zeiger1->next != zeiger)//suchen das Element das nicht vor Zeiger steht
          {
            zeiger1=zeiger1->next;
          }
          zeiger = (struct llnode*) malloc(sizeof(struct llnode));
          if(NULL==zeiger)
          {
            printf("Kein Speicher\n");
            return;
          }
        strcpy(zeiger->name,copy);
        zeiger->next = zeiger1->next;
        zeiger1->next = zeiger;
        //free(copy);  
        }
      }
*/
}

//------------------------------------------------------------------------------ 
void printList()
{
  llnode *cur_elem;
  cur_elem = head;

  while(cur_elem != NULL)
  {
    printf("%s\n", cur_elem->name);
    cur_elem = cur_elem->next;
  }
}

//------------------------------------------------------------------------------ 
void freeList()
{
  llnode *zeiger;
  while(head!=NULL)
  {
    zeiger = head;
    head = head -> next;
    free(zeiger);
  }
}

//------------------------------------------------------------------------------ 
int printError(int code)
{
  freeList();
  switch(code)
  {
  case -1: printf("usage: ./assa file.csv\n"); break;
  case -2: printf("error: out of memory\n"); break;
  case -3: printf("error: file not found\n"); break;
  case -4: printf("error: could not read file\n"); break;
  case -5: printf("error: file corrupt\n"); break;
  default: printf("undefined error\n"); break;
  }
  return code;
}

//------------------------------------------------------------------------------
int deletenode()
{
  llnode *cur_elem = NULL;
  llnode *next_elem = NULL;
  llnode *help_elem = NULL;
  char input[100];
  int result;

  printf("name? ");
  fgets(input,100,stdin); //scanf würde nur bis leerzeichen lesen
                          //gets ist verboten, weil BufferOverflow
  
  cur_elem = head;
  if((result=strcmp(cur_elem->name, input))==0)
  {
    next_elem=cur_elem->next;
    free(cur_elem);
    head=next_elem; //TODO Frage: Hab da ewig herumtun müssen, warum head?
  }
  else 
  {
    while(cur_elem->next!=NULL)
    {
      help_elem = cur_elem->next;
      if((strcmp(help_elem->name, input) == 0))
      { 
        printf("%s deleted!\n", input);
        cur_elem->next=help_elem->next;
        free(help_elem);
        return 0;
      }
      else
      {
        printf("Person not found!\n");
        return 30;
      }
    }
  } 
return 0;
}
//------------------------------------------------------------------------------

//TODO Freundschaftsduplikate auch, wenn Namen vertauscht sind, wie z.B.: PersonA;PersonB und umgekehrt
//TODO Programm stürzt ab

/*
int friendsDup()
{
  llnode *cur_elem = NULL;
  llnode *prev_elem  = NULL;
  
  
  cur_elem=head;
  prev_elem=cur_elem->next->next;
  
  while((prev_elem->next->next)!=NULL)
  {
    if(memcmp(cur_elem, prev_elem, 100)==0)
    {
      return -5;
    }
    else if(memcmp(cur_elem, prev_elem->next, 100)==0)
    {
      return -5;
    } 
    else if(memcmp(cur_elem->next, prev_elem, 100)==0)
    {
      return -5;
    }      
    else if(memcmp(cur_elem->next, prev_elem->next, 100)==0)      
    {
      return -5;
    }                

                || strcmp (cur_elem, prev_elem->next)
               || strcmp(cur_elem->next, prev_elem)|| 
               strcmp(cur_elem->next,prev_elem->next)))==0)

    //prev_elem->next->next=prev_elem->next;
   }  
return 0;
  
}
*/


//------------------------------------------------------------------------------
/* erster Versuch linked list
node *addElement(node *currentElement, char *people) 
{
  node *nextElement;
  if((currentElement == NULL) || (currentElement -> next != NULL)) 
  {
    return NULL;
  }
  
  
  
  currentElement -> next = (struct node*)malloc(sizeof(node));
  nextElement = (node*) currentElement->next;
  nextElement -> people = people;
  nextElement -> next = NULL;
  
  return nextElement;
}
//------------------------------------------------------------------------------
void printList(node *liste)
{
  node *actElement = liste;
  int i = 1; 
  while(actElement != NULL)
  {
    printf("%d. Person: %s \n)", i, actElement -> people);
    actElement = (node*) actElement -> next;
    i++;
  
  }
}
*/
//------------------------------------------------------------------------------
// StandardPrompt

int StandardPrompt()
  {
    char command[20] = " ";
    char string[400];
    char argument[400];
  
    while (strcmp(command,"quit\n") != 0)
    {
      printf("\n");
      printf("cmd: info, list-nodes, delete-node, quit\n");
      printf("esp> ");
      fgets (string,400,stdin);
      strcpy(argument,string);
      strcpy(command,getCommand(string, command));
      
      
      if(strcmp(command,"info\n")==0)
      {
        printf("-----------------------\n"
                "Version 0.1\n0664jester\n"
                "-----------------------\n");
      }

      else if(strcmp(command,"quit\n")==0)
      {

      }
      else if(strcmp(command, "list-nodes\n")==0)
      {
        printList();
      }
      else if (strcmp(command,"delete-node\n")==0)
      {
        deletenode();
      }
      else
      {
        printf("command unknown.\n");
      }
    }
  return 1;
  }
  
//------------------------------------------------------------------------------

char* getCommand(char* string, char* command)
{
   char *ptr;
   
   ptr = strtok(string, " ");
   if (strlen(ptr) < 21)
   {
    strcpy(command, ptr);
   }
   else
   {
    strcpy(command,"Unknown");
   }
   
  return command;
}

//------------------------------------------------------------------------------

/* Eigener Versuch CsvRead
int CheckCorruption(FILE *csvfile)
{
  char csvline[400];
  while((fgets(csvline, 400, csvfile)) != NULL );
  {
    if((strlen(csvline)) == 0) //datei ist leer
    {
      printf("false");
      printf("error: file corrupt1\n");
      return -5; 
    }

    if((strchr(csvline, ';')) == NULL) //wirft NULLPointer zurÃ¼ck, wenn kein ;
    {
      puts(csvline);
      printf("error: file corrupt2\n");
      return -5;
    }
    
    char semi[] = ";";
    char *postsemic;
    postsemic = (strrchr(csvline, ';'));
    char* string2 = postsemic+1;

    char *prasemic;
    prasemic = strstr(csvline, semi); //liefert einen pointer zurÃ¼ck, wo er es findet
    *prasemic = 0; __attribute__((unused)) //Ã¼berschreibe ; mit NULL
    char* string1 = csvline;

    if(strncmp(string1, string2, sizeof(string1)) == 0)
    {
      printf("error: file corrupt3\n");
      return -5; 
    }
    
    if((strlen(string1) || strlen(string2)) == 0) //ein wort ist nicht vorhanden
    {
      printf("error: file corrupt4\n");
      return -5; 
    }
  }
  return 1;
}
*/
//------------------------------------------------------------------------------
/* Vllt brauchbaur string splitten
char* String2(char* csvline)
{
  //char semi[] = ";";
    char *postsemic;
    postsemic = (strrchr(csvline, ';'));
    char* string2 = postsemic+1;
    return string2;
}
*/
