#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
// TODO replace this include with your implemenation
#include "dummy_met.h"

// Function to split a comma-separated string into float vector
std::vector<float> parse_csv_line(const std::string& line) {
    std::vector<float> values;
    std::stringstream ss(line);
    std::string item;
    while (std::getline(ss, item, ',')) {
        if (!item.empty()) {
            values.push_back(std::stof(item));
        }
    }
    return values;
}

std::vector<std::vector<float>> read_file(const std::string& filename){
    std::ifstream infile(filename);
    std::string line;
    std::vector<std::vector<float>> all_events;

    while (std::getline(infile, line)) {
        all_events.push_back(parse_csv_line(line));
    }
    return all_events;
}

int main() {

    std::cout << "Reading pt.csv and phi.csv" << std::endl;
    // read the pt and phi csv files
    std::vector<std::vector<float>> pt = read_file("pt.csv");
    std::vector<std::vector<float>> phi = read_file("phi.csv");
    unsigned short n_events = pt.size();

    // create a container for the result
    std::vector<float> met(n_events, 0.);

    std::cout << "Computing MET for " << n_events << " events" << std::endl;

    // run compute_met function for every event
    for(unsigned short n = 0; n < n_events; n++){
        // copy the values from one event to an array for HLS
        unsigned short n_particles = pt.at(n).size();

        // create arrays for pt and eta
        // the types T_pt and T_phi are defined in the MET header for convenience
        T_pt* pt_event_arr = new T_pt[n_particles];
        T_phi* phi_event_arr = new T_phi[n_particles];
        std::copy(pt.at(n).begin(), pt.at(n).end(), pt_event_arr);
        std::copy(phi.at(n).begin(), phi.at(n).end(), phi_event_arr);

        // compute the MET (HLS simulation)
        met.at(n) = compute_met(n_particles, pt_event_arr, phi_event_arr);

        // delete the event data arrays
        delete[] pt_event_arr;
        delete[] phi_event_arr;
    }

    // Open file for writing
    std::ofstream outfile("met_hls.csv");

    if (!outfile.is_open()) {
        std::cerr << "Error opening file for writing\n";
        return 1;
    }

    for (float val : met) {
        outfile << val << '\n';
    }

    outfile.close();
    std::cout << "Data written to met_hls.csv\n";
    return 0;

}
