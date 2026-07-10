// TODO include your own implementation here
#include "example_met.h"
#include "hls_stream.h"

// this defines the size of internal buffer that's used
static constexpr int MAX_N_PARTICLES = 256;

/* Function to copy the particles of one event from the external memory into internal streams,
*  and cast them to internal data type used in MET calculation
*/
void load(// these are the input arrays from external memory
          int n_events,
          unsigned short* n_particles,
          float* pt,
          float* phi, 
          // the following are streams that will be passed to the next function internally
          hls::stream<unsigned short>& n_particles_stream,
          hls::stream<T_pt>& pt_stream,
          hls::stream<T_phi>& phi_stream){
  unsigned short particle_idx = 0; // events can have different numbers of particles, so keep track of the index here
  for(int n_event = 0; n_event < n_events; n_event++){
    unsigned short n_particles_event = n_particles[n_event];
    n_particles_stream.write(n_particles_event);
    for(unsigned short n = 0; n < n_particles_event; n++, particle_idx++){
      T_pt the_pt = (T_pt) pt[particle_idx]; // read from the external memory and cast from float to T_pt
      T_phi the_phi = (T_phi) phi[particle_idx]; // read from the external memory and cast from float to T_phi
      pt_stream.write(the_pt);
      phi_stream.write(the_phi);
    }
  }
}

/* Function to compute the MET over all events
*  It reads from the particle stream to an internal buffer, and calls your compute_MET function
*/
void compute(int n_events,
             hls::stream<unsigned short>& n_particles_stream, 
             hls::stream<T_pt>& pt_stream,
             hls::stream<T_phi>& phi_stream,
             hls::stream<T_met>& met_stream){
  // internal buffers for particles in one event
  T_pt pt_int[MAX_N_PARTICLES];
  T_phi phi_int[MAX_N_PARTICLES];

  for(int n_event=0; n_event < n_events; n_event++){
    unsigned short n_particles_event = n_particles_stream.read();
    for(unsigned short n=0; n < n_particles_event; n++){
      pt_int[n] = pt_stream.read();
      phi_int[n] = phi_stream.read();
    }
    T_met met_int = compute_met(n_particles_event, pt_int, phi_int);
    met_stream.write(met_int);
  }
}

/* Function to copy the MET of one event into the external memory,
*  and cast it from the internal MET type to float
*/
void store(int n_events,
           hls::stream<T_met>& met_stream,
           float* met){
  for(int n = 0; n < n_events; n++){
    T_met the_met = met_stream.read(); // read from the internal stream
    met[n] = (float) the_met;          // cast to float and write to the external memory
  }
}

void met_accelerator(int n_events, unsigned short* n_particles, float* pt, float* phi, float* met){
  // data interfaces
  #pragma HLS interface mode=m_axi port=n_particles offset=slave bundle=gmem0 depth=2532
  #pragma HLS interface mode=m_axi port=pt offset=slave bundle=gmem0 depth=2532
  #pragma HLS interface mode=m_axi port=phi offset=slave bundle=gmem0 depth=2532
  #pragma HLS interface mode=m_axi port=met offset=slave bundle=gmem0 depth=2532
  // control interfaces
  #pragma HLS interface mode=s_axilite port=n_events bundle=control
  #pragma HLS interface mode=s_axilite port=n_particles bundle=control
  #pragma HLS interface mode=s_axilite port=pt bundle=control
  #pragma HLS interface mode=s_axilite port=phi bundle=control
  #pragma HLS interface mode=s_axilite port=met bundle=control
  #pragma HLS interface mode=s_axilite port=return bundle=control

  #pragma HLS dataflow

  hls::stream<unsigned short> n_particles_stream;
  hls::stream<T_pt> pt_stream;
  hls::stream<T_phi> phi_stream;
  hls::stream<T_met> met_stream;

  load(n_events, n_particles, pt, phi, n_particles_stream, pt_stream, phi_stream);
  compute(n_events, n_particles_stream, pt_stream, phi_stream, met_stream);
  store(n_events, met_stream, met);
}