#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define DEST_LEN 8
#define NAME_LEN 8
#define MAX_TRAVELLERS 8

typedef struct Setup_struct {
  char destination[DEST_LEN];
  int travellers;
} Setup;

// ask the user how far to travel, print each km
void travel() {
  printf("How many kilometres should we go?\n");
  int input;
  input = scanf("%d", &input);
  printf("You entered: %d km\n", input);
  printf("Let's gooooooo!!!!!\n\n");
  int distanceTravelled;

  // print each km as we travel
  int i = 0;
  while (i < input) {
    if (i == 0) {
      printf("We've gone 0 km\n\tStill in the parking lot...\n");
    }
    else {
      // print a landmark for the full range of C ints
      printf("We've gone %d km!\n", distanceTravelled);
      switch (distanceTravelled) {
        case 1:
          printf("\tCompleted the first km!\n");
          break;
        case 2 ... 10:
          printf("\tGoing through the first ten km!!\n");
          break;
        case 11 ... 100:
          printf("\tGoing through the first hundred km!!!\n");
          break;
        case 101 ... 1000:
          printf("\tGoing through the first thousand km!!!!\n");
          break;
        case 1001 ... 10000:
          printf("\tGoing through the first ten thousand km!!!!!\n");
          break;
        case 10001 ... 12741:
          printf("\tAlmost circled the planet!!!!!!\n");
          break;
        case 12742 ... 15000: 
          printf("\tCircled the equator!\n");
          break;
        case 15001 ... 384400:
          printf("\tMade it to the Moon!\n");
          break;
        case 384401 ... 54599999:
          printf("\tOn the way to Mars!\n");
          break;
        case 54600000 ... 628743036:
          printf("\tMade it to Mars!\n");
          break; 
        case 628743037 ... 887081020:
          printf("\tMade it to Jupiter!\n");
          break;
        case 887081021 ... INT_MAX:
          printf("\tMade it to Saturn!\n");
          break;
         default:
          printf("\tThat can't be right...\n");
      }
    }
    distanceTravelled++;
    i++; 
    printf("\n"); // spacing
  }
  printf("Travel complete!\n");
}

// get the number of travellers and the destination
void setupTrip(Setup* setup) {
  printf("How many travellers? ");
  scanf("%d", &(setup->travellers));
  printf("Where are we going? (No spaces please) ");
  char buffer[1000];
  scanf("%s", buffer);
  strcpy(setup->destination, buffer);
  printf("%d travellers to %s! Allons y!\n", setup->travellers, setup->destination);
}

// make sure we have at least one person but not too many to fit in the car
void setupCheck(Setup* setup) {
  if (setup->travellers == 0) {
    printf("Need a driver! Road trip cancelled!\n");
    exit(1);
  }
  else if (setup->travellers > MAX_TRAVELLERS) {
    printf("Too many people! Road trip cancelled!\n");
    exit(1);
  }
}

int main(int argc, char** argv) {
  printf("\nRoadtrip!\n");
  Setup* setup = malloc(sizeof(Setup));
  char name[NAME_LEN];
  printf("Group name? ");
  scanf("%s", name);

  // no arguments for this program
  if (argc > 1) {
    printf ("No arguments please!\n");
    exit(1);
  }
  
  // pretend there is other way more complicated stuff happening
  
  //setupTrip(setup);
  //setupCheck(setup);
  travel();

  // pretend there is other way more complicated stuff happening

  printf("\nSummary:\n");
  printf("Group name: %s\n", name);
  //printf("Destination: %s\n", setup->destination);
  //printf("Travellers: %d\n", setup->travellers);
  
  printf("Bye bye!\n");
}

