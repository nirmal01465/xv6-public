// Prevent the system <string.h> from being included.
#define _STRING_H 1
#define __STRING_H 1

#include "./string.h"  // Use the local xv6 string header.
#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

// If not defined in Makefile, provide default credentials
#ifndef USERNAME
#define USERNAME "xv6user"
#endif

#ifndef PASSWORD
#define PASSWORD "xv6pass"
#endif

// Helper function to remove newline characters from input.
void remove_newline(char *s) {
  int i = 0;
  while(s[i] != '\0'){
    if(s[i] == '\n'){
      s[i] = '\0';
      break;
    }
    i++;
  }
}

// Login routine: allows a maximum of three attempts.
void login() {
  char uname[32], pword[32];
  int attempts = 0;
  while(attempts < 3) {
    printf(1, "Enter Username: ");
    gets(uname, sizeof(uname));
    remove_newline(uname);
    if(strcmp(uname, USERNAME) == 0) {
      printf(1, "Enter Password: ");
      gets(pword, sizeof(pword));
      remove_newline(pword);
      if(strcmp(pword, PASSWORD) == 0){
         printf(1, "Login successful\n");
         return;
      } else {
         printf(1, "Incorrect Password\n");
      }
    } else {
      printf(1, "Incorrect Username\n");
    }
    attempts++;
  }
  printf(1, "Maximum login attempts reached. Exiting.\n");
  exit();
}

char *argv[] = { "sh", 0 };

int main(void)
{
  int pid, wpid;

  if(open("console", O_RDWR) < 0){
    mknod("console", 1, 1);
    open("console", O_RDWR);
  }
  dup(0);  // stdout
  dup(0);  // stderr

  // Run the login process before starting the shell.
  login();

  for(;;){
    printf(1, "init: starting sh\n");
    pid = fork();
    if(pid < 0){
      printf(1, "init: fork failed\n");
      exit();
    }
    if(pid == 0){
      exec("sh", argv);
      printf(1, "init: exec sh failed\n");
      exit();
    }
    while((wpid = wait()) >= 0 && wpid != pid)
      printf(1, "zombie!\n");
  }
}
