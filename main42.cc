#include "Pythia8/Pythia.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace Pythia8;

int main() {
    // Initialize Pythia
    Pythia pythia;

    // Set up Pythia to decay all unstable particles
    pythia.readString("HadronLevel:Decay = on"); // Decay unstable hadrons
     // Turn off all processes (no hard scattering, just tau decay)
    pythia.readString("ProcessLevel:all = off");
    //pythia.readString("ParticleDecays:limitTau0 = on"); // Decay particles with finite lifetime
    //pythia.readString("ParticleDecays:tau0Max = 1000.0"); // Set maximum lifetime for decays (in mm/c)

    // Ensure specific particles (mesons and muons) are allowed to decay
    pythia.readString("15:mayDecay = on");  // Allow taus to decay
    pythia.readString("13:mayDecay = off");  // Allow muons to decay
    pythia.readString("23:mayDecay = on");  // Allow Z boson to decay
    pythia.readString("23:onMode = off"); // turn off all Z decay
    pythia.readString("23:onIfAny = 15"); // only allow Z decay to tau
    pythia.readString("15:onMode = off"); // turn off all tau decay
    pythia.readString("15:onIfAny = 11"); // allow only tau decay to electrons
    pythia.readString("211:mayDecay = off"); // Allow pi+/- to decay
    pythia.readString("111:mayDecay = off"); // Allow pi0 to decay
    pythia.readString("321:mayDecay = off"); // Allow K+/- to decay
    pythia.readString("311:mayDecay = off"); // Allow K0 to decay
    pythia.readString("221:mayDecay = off"); // Allow eta to decay
    pythia.readString("331:mayDecay = off"); // Allow eta' to decay
    pythia.readString("223:mayDecay = off"); // Allow omega to decay
    pythia.readString("333:mayDecay = off"); // Allow phi to decay

    // Enable QED radiation in the parton shower (for FSR)
    // Enable QED radiation in decays (for radiative decays)
    pythia.readString("PartonLevel:FSR = on");
    pythia.readString("TimeShower:QEDshowerByL = on");
    pythia.readString("TimeShower:QEDshowerByQ = on");
 
    //Show the details of the first five events
    pythia.readString("Next:numberShowEvent = 10");


    // Initialize Pythia
    pythia.init();

    // Vectors to store electron/positron and photon energies
    std::vector<double> electronEnergies;
    std::vector<double> photonEnergies;
    std::vector<double> photonEnergies_Lab;
    std::vector<double> tauEnergies;
    std::vector<double> tauEnergies_Lab;

    // Number of events to generate
    int nEvents = 1000000;

    // particle gun masss Px, Py, Pz, E
    const int pdgID = 23;
    const double Mass = pythia.particleData.m0(pdgID); 
    const double Px = 0.0;
    const double Py = 0.0;
    const double Pz = 0.0;
    const double E = sqrt(Px*Px+Py*Py+Pz*Pz+Mass*Mass);

    // Event loop
    for (int iEvent = 0; iEvent < nEvents; ++iEvent) {
        // Create a new event
        pythia.event.reset();

        // Add a tau+ and tau- at rest to the event
        //double tauMass = 1.776; // Tau mass in GeV
        pythia.event.append(pdgID, 1, 0, 0, 0, 0, 0, 0, Px, Py, Pz, E, Mass); // tau+
        //pythia.event.append(13, 1, 0, 0, 0, 0, 0, 0, 100.0, 0.0, 0.0, tauMass, tauMass); // tau+
     //   pythia.event.append(-15, 1, 0, 0, 0, 0, 0, 0, 0.0, 0.0, 0.0, tauMass, tauMass); // tau-

        // Generate decays of the taus and other unstable particles
        pythia.next();


        // flag found FSR photons
        bool foundFSR = false;
        // Loop over all particles in the event
        for (int i = 0; i < pythia.event.size(); ++i) {
          // Check if the particle is an electron (ID = 11) or positron (ID = -11)
          if (abs(pythia.event[i].id()) == 11) {
            // Store the electron/positron energy in the vector
            // std::cout << pythia.event[i].e() << ", " <<pythia.event[i].status() << "\n";
            electronEnergies.push_back(pythia.event[i].e());
          }
          // Check if the particle is a photon (ID = 22)
          if (pythia.event[i].id() == 22) {
            // Store the photon energy in the vector
            //    std::cout << pythia.event[i].e() << ", " <<pythia.event[i].status() << "\n";
            int iMother1 = pythia.event[i].mother1();
            int iMother2 = pythia.event[i].mother2();

            // check if FSR photon is from an electron
            if (iMother1 >= 0 && std::abs(pythia.event[iMother1].id()) == 11) {
              foundFSR = true;
            }
            if (iMother2 >= 0 && std::abs(pythia.event[iMother2].id()) == 11) {
              foundFSR = true;
            }
            //  Store the photon energy in the vector only if it is found as a FSR
            if (foundFSR) {
              // 2) Suppose we already have an event in pythia.event,
              // and we found a photon (FSR) at some index iPhoton.
              int iPhoton = i; // This is just an example index.

              // 3) Trace back the mother chain of this photon until we find a tau (PDG ID = ±15)
              int iCheck = iPhoton;
              int iTau   = -1;  // will store the tau index if found
              while (true) {
                int iMom1 = pythia.event[iCheck].mother1();
                int iMom2 = pythia.event[iCheck].mother2();

                // If no valid mother index (i.e. < 0), break out of the loop
                if (iMom1 < 0 && iMom2 < 0) break;

                // For simplicity, pick mother1 if it exists, else mother2
                int iMom = (iMom1 >= 0) ? iMom1 : iMom2;
                int idMom = pythia.event[iMom].id();

                // Check if this mother is a tau
                if (std::abs(idMom) == 15) {
                  iTau = iMom;
                  break;
                }

                // Otherwise, keep going up
                iCheck = iMom;
              }

              // 4) If we found a tau, retrieve its 4-momentum and the photon's 4-momentum
              if (iTau >= 0) {
                Vec4 pTauLab   = pythia.event[iTau].p();     // tau in lab frame
                Vec4 pGammaLab = pythia.event[iPhoton].p();  // photon in lab frame

                // Compute the (negative) velocity of the tau
                double bx = -pTauLab.px()/pTauLab.e();
                double by = -pTauLab.py()/pTauLab.e();
                double bz = -pTauLab.pz()/pTauLab.e();

                // Construct a "beta" four‐vector for the boost
                Vec4 betaIn(bx, by, bz, 1.0);
                // 5) Boost the photon momentum from the lab frame into tau's rest frame
                // Make a copy of the photon momentum so we can modify it
                Vec4 pGammaInTauFrame = pGammaLab;

                // Now perform the boost in place
                pGammaInTauFrame.bst(betaIn);

                // also test Tau itself
                Vec4 pTauInTauFrame = pTauLab;
                // Now perform the boost in place
                pTauInTauFrame.bst(betaIn);
                
                // 6) Now you can analyze the photon in the tau rest frame,
                // e.g. check its energy, angles, etc.
                std::cout << "Photon energy in tau rest frame: " 
                          << pGammaInTauFrame.e() << " GeV\n";
                // store it
                photonEnergies.push_back(pGammaInTauFrame.e());
                photonEnergies_Lab.push_back(pGammaLab.e());
                tauEnergies.push_back(pTauInTauFrame.e());
                tauEnergies_Lab.push_back(pTauLab.e());

              } else {
                std::cout << "No tau found in the mother chain of photon i=" << iPhoton << "\n";
              }
                
            }
          }
            
        }
        if (foundFSR) {
          std::cout << "\n\n-----------------------  Found FSR  ----------------------\n";
          pythia.event.list();
          std::cout << "\n\n---------------------------------------------------------- " << std::endl;
        }
    }

    // Save the electron/positron energies to a file
    std::ofstream electronFile("electron_energies.csv");
    for (double energy : electronEnergies) {
        electronFile << energy << "\n";
    }
    electronFile.close();

    // Save the photon energies to a file
    std::ofstream photonFile("photon_energies.csv");
    for (double energy : photonEnergies) {
        photonFile << energy << "\n";
    }
    photonFile.close();


    // Save the photon energies to a file
    std::ofstream photonFileLab("photon_energies_lab.csv");
    for (double energy : photonEnergies_Lab) {
        photonFileLab << energy << "\n";
    }         
    photonFileLab.close();


    // Save the tau energies to a file
    std::ofstream tauFile("tau_energies.csv");
    for (double energy : tauEnergies) {
      tauFile << energy << "\n";
    }
    tauFile.close();


    // Save the tau energies to a file
    std::ofstream tauFileLab("tau_energies_lab.csv");
    for (double energy : tauEnergies_Lab) {
      tauFileLab << energy << "\n";
    }
    tauFileLab.close();

    // Print statistics
    pythia.stat();

    return 0;
}
