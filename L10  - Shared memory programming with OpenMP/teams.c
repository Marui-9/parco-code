#include <stdio.h>
#include <omp.h>

int main() {
  #pragma omp teams num_teams(2)
  {
    // each team can spawn its own threads
    #pragma omp parallel
    {
      printf("team %d, thread %d\n", omp_get_team_num(), omp_get_thread_num());
    }
  }
  return 0;
}
