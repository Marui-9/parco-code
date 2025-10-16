#include <iostream>
using namespace std;


int input[10] = {22, 33, 53, 75, 29, 94, 99, 22, 11, 10};

int find_max(int (*input)[10], int max=0) {
    for (int i = 0; i < 10; i++){
        if ((*input)[i] > max){
            max = (*input)[i];
        }
    }
    return max;
}
int reverse(int* input) {
    int* start = &input[0];
    int* end = &input[9]; // since input has 10 elements, last index is 9
    while (start < end) {
        int temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }
    return *input;
}
int main() {
    int max_value = find_max(&input);
    std::cout << "Max: " << max_value << std::endl;
    int reversed = reverse(input);
    std::cout << "Reversed: " 
                << input[0] << " " << input[1] << " " << input[2] << " "
                << input[3] << " " << input[4] << " " << input[5] << " "
                << input[6] << " " << input[7] << " " << input[8] << " "
                << input[9] << std::endl;       
    return 0;
}