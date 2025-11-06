#include <stdio.h>

#define ROWS 10         // Number of rows in the grid
#define COLS 10         // Number of columns in the grid
#define TIME_STEPS 100  // Number of time steps for the simulation
#define HOT_SPOT_TEMP 100.0  // Temperature of the initial hot spots

int main() {
    double grid[ROWS][COLS] = {0};       // Initialize the grid to 0
    double new_grid[ROWS][COLS] = {0};   // Temporary grid to store new values

    // Set some initial hot spots
    grid[4][4] = HOT_SPOT_TEMP; // A hot spot in the center
    grid[5][5] = HOT_SPOT_TEMP; // Another hot spot

    // Run the simulation for a specified number of time steps
    for (int t = 0; t < TIME_STEPS; t++) {
        // Apply the heat diffusion model in parallel
        for (int i = 1; i < ROWS - 1; i++) { // Avoid edges
            for (int j = 1; j < COLS - 1; j++) {
                // New temperature is the average of the four neighbors
                new_grid[i][j] = (grid[i - 1][j] + grid[i + 1][j] +
                                  grid[i][j - 1] + grid[i][j + 1]) / 4.0;
            }
        }

        // Copy the new grid back to the original grid for the next time step
        for (int i = 1; i < ROWS - 1; i++) {
            for (int j = 1; j < COLS - 1; j++) {
                grid[i][j] = new_grid[i][j];
            }
        }
    }

    // Print the final grid
    printf("Final grid after %d time steps:\n", TIME_STEPS);
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            printf("%6.2f ", grid[i][j]);
        }
        printf("\n");
    }

    return 0;
}
