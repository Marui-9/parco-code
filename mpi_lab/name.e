pong.c: In function ‘main’:
pong.c:40:19: error: ‘number’ undeclared (first use in this function)
   40 |         MPI_Send(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
      |                   ^~~~~~
pong.c:40:19: note: each undeclared identifier is reported only once for each function it appears in
[proxy:0:0@hpc-c11-node25.maas.icts.unitn.it] [proxy:0:0@hpc-c11-node25.maas.icts.unitn.it] HYDU_create_process (/apps/devel/mpich-3.2.1/src/pm/hydra/utils/launch/launch.c:74): HYDU_create_process (/apps/devel/mpich-3.2.1/src/pm/hydra/utils/launch/launch.c:74): execvp error on file ./ping_pong (No such file or directory)
execvp error on file ./ping_pong (No such file or directory)
