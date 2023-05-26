// Copyright 2023 Randy Aguero Bermudez
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define MAX_SHOT_DISTANCE 25.0
// Max shot put distance is 25 meters

typedef struct {
  int athlete_number;
  double *best_shot;
} athlete;

typedef struct {
  int score;
  int team_number;
  athlete *athletes;
  int team_size;
} team;

void *shot_put();

void initialized_team(team *team, int team_numb, int team_size);

void analize_match(team *team_1, team *team_2);

void destroy_teams(team *team_1, team *team_2);

// procedure main:
int main(int argc, char *argv[]) {
  // argc anumber of arguments
  // argv array with arguments
  if (argc != 2) {
    fprintf(stderr,
            "Error, you must enter an positive odd number as argument\n");
    return 1;
  }

  const size_t TEAM_SIZE = atoi(argv[1]);
  if (TEAM_SIZE % 2 == 0) {
    fprintf(stderr, "Error, A odd number of athletes must be entered\n");
    return 1;
  }

  team team_1;
  team team_2;
  initialized_team(&team_1, 1, TEAM_SIZE);
  initialized_team(&team_2, 2, TEAM_SIZE);
  // Teams generated

  int error_T1 = 0;
  int error_T2 = 0;

  pthread_t shot_put_threads_T1[TEAM_SIZE];
  pthread_t shot_put_threads_T2[TEAM_SIZE];
  // Each athlete will have an thread;ma

  unsigned int seed = time(NULL);
  // Random Number seed, used to generate distance

  for (size_t j = 0; j < TEAM_SIZE; j++) {
    error_T1 =
        pthread_create(&shot_put_threads_T1[j], /*thread atributes*/ NULL,
                       shot_put, /*function arguments*/ (void **)&seed);

    error_T2 =
        pthread_create(&shot_put_threads_T2[j], /*thread atributes*/ NULL,
                       shot_put, /*function arguments*/ (void **)&seed);
    if ((error_T1 || error_T2) != EXIT_SUCCESS) {
      fprintf(stderr,
              "Error: An athlete from a team failed to throw the shot\n");
      break;
    }
  }

  for (size_t j = 0; j < (size_t)TEAM_SIZE; j++) {
    pthread_join(shot_put_threads_T1[j],
                 (void **)(&team_1.athletes[j].best_shot));
    pthread_join(shot_put_threads_T2[j],
                 (void **)(&team_2.athletes[j].best_shot));
  }

  if ((error_T1 || error_T2) == EXIT_SUCCESS) {
    analize_match(&team_1, &team_2);
  }

  if ((error_T1 || error_T2) != EXIT_SUCCESS) {
    return 1;
  }

  destroy_teams(&team_1, &team_2);
  return 0;
}

void *shot_put(void *seed) {
  double *best_shot = (double *)malloc(sizeof(double));
  *best_shot = -1;

  for (int i = 0; i < 2; i++) {
    double distance_temp = (rand_r(seed) % (int)MAX_SHOT_DISTANCE);
    if (distance_temp > *best_shot) {
      *best_shot = distance_temp;
    }
  }
  return best_shot;
}

void initialized_team(team *team, int team_numb, int team_size) {
  team->athletes = (athlete *)malloc(sizeof(athlete) * team_size);
  team->team_number = team_numb;
  team->team_size = team_size;
  // Dinamic Array of athletes.

  for (size_t i = 0; i < (size_t)team_size; i++) {
    team->athletes[i].athlete_number = i + 1;
    team->athletes[i].best_shot = NULL;
  }
}

void analize_match(team *team_1, team *team_2) {
  size_t points_T1 = 0;
  size_t points_T2 = 0;
  for (size_t i = 0; i < (size_t)team_1->team_size; i++) {
    printf("T1 A%zu: %lf m vs T2 A%zu : %lf m\n", i + 1,
           *team_1->athletes[i].best_shot, i + 1,
           *team_2->athletes[i].best_shot);

    if (*team_1->athletes[i].best_shot > *team_2->athletes[i].best_shot) {
      printf("Best Shot: %lf m : Athlete %zu from team 1\n\n",
             *team_1->athletes[i].best_shot, i + 1);
      points_T1++;
    } else if (*team_1->athletes[i].best_shot <
               *team_2->athletes[i].best_shot) {
      printf("Best Shot: %lf m  Athlete %zu from team 2\n\n",
             *team_2->athletes[i].best_shot, i + 1);
      points_T2++;
    }
  }

  if (points_T1 > points_T2) {
    printf("Team 1 Wins: %zu points - Team 2 Loses: %zu points\n", points_T1,
           points_T2);
  } else if (points_T1 < points_T2) {
    printf("Team 2 Wins: %zu points - Team 1 Loses: %zu points\n", points_T2,
           points_T1);
  } else {
    printf("There is a draw. Points for each team:\n %zu", points_T1);
  }
}

void destroy_teams(team *team_1, team *team_2) {
  for (size_t i = 0; i < (size_t)team_1->team_size; i++) {
    free(team_1->athletes[i].best_shot);
    free(team_2->athletes[i].best_shot);
    // Best_shot destroy
  }
  free(team_1->athletes);
  free(team_2->athletes);

  // Athletes destroy
}
