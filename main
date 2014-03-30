#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node
{
  char *name;
  struct node *next;
 // struct node *prev;  
} node;
  
  
int readCsvFile(char *fileName);
int StandardPrompt();
char* getCommand(char* string, char* command);
int CheckCorruption(FILE *csvfile);
node *addElement(node *currentElement, char *people);
void printList(node *liste);
char* String2(char* csvline);


  node *personenListe = NULL;


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
    return result;
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

  csvFile = fopen(fileName, "r");
  if(csvFile == NULL)                  //Datei existiert nich fopen
  {
    printf("error: file not found!\n");
    //exit(-3); ... fraglich ob erlaubt
    return -3;
  }
  
  do
  {
    if(fgets(csvline, 400, csvFile) == NULL)
    {
      //variante 1
      if(feof(csvFile)!=0)
      {
        printf("error: file corrupt1\n");
        fclose(csvFile);
        return -5; 
      }
      printf("error: could not read file\n");
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
      printf("error: file corrupt1\n");
      fclose(csvFile);
      return -5; 
    }
    if(semicolon == csvline) // strichpunkt am anfang der zeile
    {
      printf("error: file corrupt2\n");
      fclose(csvFile);
      return -5; 
    }
    *semicolon='\0';
    semicolon++;
    if(*semicolon == '\n' || *semicolon == '\0') //sp am ende der zeile
    {
      printf("error: file corrupt\n");
      fclose(csvFile);
      return -5; 
    }
    if(strchr(semicolon, ';')!=NULL) //noch ein strichpunkt
    {
      printf("error: file corrupt\n");
      fclose(csvFile);
      return -5; 
    }
    newline=strchr(semicolon, '\n');
    if(newline!=NULL) //newline character am ende wegschneiden
    {
      *newline='\0';
    }
    if(strcmp(csvline, semicolon)==0) //beide namen gleich
    {
      printf("error: file corrupt\n");
      fclose(csvFile);
      return -5; 
    }
    //TODO beide namen in der personen liste speichern, falls nicht vorhanden
    //TODO freundschaft in der jeweiligen freundesliste eintragen
    //TODO freundschaftsduplicat checken
  } while(feof(csvFile)==0);

  fclose(csvFile);
  return 0;
}

//------------------------------------------------------------------------------




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
//------------------------------------------------------------------------------
// StandardPrompt

int StandardPrompt()
  {
    char command[20] = " ";
    char string[400];
    char argument[400];
  
    while (strcmp(command,"quit\n") != 0)
    {
      printf("cmd: info, quit\n");
      printf("esp> ");
      fgets (string,400,stdin);
      strcpy(argument,string);
      strcpy(command,getCommand(string, command));
      
      
      if (strcmp (command,"info\n") == 0)
      {
        printf("-----------------------\n"
                "Version 0.1\n0664jester\n"
                "-----------------------\n");
      }

      else if (strcmp (command,"quit\n") == 0)
      {

      }
      else if (strcmp(command, "list-nodes\n") == 0)
      {
        
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
//------------------------------------------------------------------------------

char* String2(char* csvline)
{
  //char semi[] = ";";
    char *postsemic;
    postsemic = (strrchr(csvline, ';'));
    char* string2 = postsemic+1;
    return string2;
}
