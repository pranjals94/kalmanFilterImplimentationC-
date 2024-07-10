#include <iostream>
#include <fstream>

// Define the Kalman Filter structure
typedef struct {
    double q; // process noise covariance
    double r; // measurement noise covariance
    double x; // estimated value
    double p; // estimation error covariance
    double k; // kalman gain
} KalmanFilter;

// Initialize the Kalman Filter
void kalman_init(KalmanFilter *kf, double q, double r, double p, double initial_value) {
    kf->q = q;
    kf->r = r;
    kf->p = p;
    kf->x = initial_value;
}

// Perform a Kalman Filter update
double kalman_update(KalmanFilter *kf, double measurement) {
    // Prediction update
    kf->p = kf->p + kf->q;

    // Measurement update
    kf->k = kf->p / (kf->p + kf->r);
    kf->x = kf->x + kf->k * (measurement - kf->x);
    kf->p = (1 - kf->k) * kf->p;

    return kf->x;
}

int main() {
 
    // Example usage
    KalmanFilter kf;
    kalman_init(&kf, 1, 2, 0.1, 0);//q process noise covariance
                                    //r measurement noise covariance

    double measurements[]={-200,200,255,-255};
    int num_measurements = sizeof(measurements) / sizeof(measurements[0]);

    // Open a CSV file
    std::ofstream file("DataOutput.csv");

    // Check if the file is open
    if (file.is_open()) {
        for (int i = 0; i < num_measurements; i++) {
            double estimate = kalman_update(&kf, measurements[i]);

            file << measurements[i];
            file << ",";
            file << estimate;
            file << "\n";

            printf("Measurement: %f, Estimate: %f\n", measurements[i], estimate);
        }
        file.close();
        printf("DataOutput.csv file created Successfully."); //excel supported
    } else {
        std::cerr << "Failed to create the CSV file." << std::endl;
    }

    // int data[] = {34,45,90,78};
    // // Check if the file is open
    // if (file.is_open()) {
    //     file << data[0];
    //     file << ",";
    //     file << data[1];
    //     file << "\n";
    //     file << data[2];
    //     file << "\n";
    //     file << data[3];
    //     file.close();
    //     std::cout << "CSV file 'example.csv' created successfully." << std::endl;
    // } else {
    //     std::cerr << "Failed to create the CSV file." << std::endl;
    // }

    return 0;
}
