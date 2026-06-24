// TODO include your own implementation here
#include "dummy_met.h"

// this defines the size of internal buffer that's used
static constexpr int MAX_N_PARTICLES = 256;

/* Function to copy the particles of one event into internal buffer,
*  and cast them to internal data type used in MET calculation
*/
void copy_inputs(int particles_start,                  // start index of event in external memory
                 int n_particles,                      // number of particles in the event
                 float* pt_in, float* phi_in,          // float (pT, phi) read from external memory
                 T_pt* pt_int, T_phi* phi_int){        // locally buffered (pT, phi) in different types
  for(unsigned short n = 0; n < n_particles; n++){
    pt_int[n] = pt_in[particles_start + n];   // there is an implicit cast from float to T_pt here
    phi_int[n] = phi_in[particles_start + n]; // there is an implicit cast from float to T_phi here
  }
}

/* Function to copy the MET of one event into the external memory,
*  and cast it from the internal MET type to float
*/
void copy_outputs(int n_event, T_met met_int, float* met_out){
  met_out[n_event] = met_int; // there is an implicit cast from T_met to float here
}

void met_accelerator(int n_events, unsigned short* n_particles, float* pt, float* phi, float* met){
  // data interfaces
  #pragma HLS interface mode=m_axi port=n_particles offset=slave bundle=gmem0
  #pragma HLS interface mode=m_axi port=pt offset=slave bundle=gmem0
  #pragma HLS interface mode=m_axi port=phi offset=slave bundle=gmem0
  #pragma HLS interface mode=m_axi port=met offset=slave bundle=gmem0
  // control interfaces
  #pragma HLS interface mode=s_axilite port=n_events bundle=control
  #pragma HLS interface mode=s_axilite port=n_particles bundle=control
  #pragma HLS interface mode=s_axilite port=pt bundle=control
  #pragma HLS interface mode=s_axilite port=phi bundle=control
  #pragma HLS interface mode=s_axilite port=met bundle=control
  #pragma HLS interface mode=s_axilite port=return bundle=control

  #pragma HLS dataflow

  // internal buffers for particles in one event
  T_pt pt_int[MAX_N_PARTICLES];
  T_phi phi_int[MAX_N_PARTICLES];

  // events have different numbers of particles so we keep track of the index
  int n_particle = 0;
  // loop over events with 
  for(int n_event = 0; n_event < n_events; n_event++){
    #pragma HLS pipeline
    unsigned short n_particles_event = n_particles[n_event];
    copy_inputs(n_particle, n_particles_event, pt, phi, pt_int, phi_int);
    T_met met_int = compute_met(n_particles_event, pt_int, phi_int);
    copy_outputs(n_event, met_int, met);
    n_particle += n_particles_event;
  }
}