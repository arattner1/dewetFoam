//Calculate ThermoCapillary force
const volVectorField gradSigma = fvc::grad( sigmaPtr->sigma() );
const volVectorField n_int = fvc::reconstruct( mixture.nHatf() );
const volVectorField tau_TC = gradSigma - n_int*(gradSigma & n_int);
ThermoCapillaryForce = mag( fvc::grad(alpha1) )*tau_TC*alpha1;
