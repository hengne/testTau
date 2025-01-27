# testTau

This repository provides a simple example of generating **Z -> tau tau** events in [Pythia8](https://pythia.org/) where each tau decays exclusively to **e + 2nu**, with **FSR (Final-State Radiation)** photons allowed from the electron. It saves various energies to CSV files and provides Python scripts to plot them.

---

## Contents

- **`env.sh`**  
  Environment setup script, where you specify your Pythia8 installation path.

- **`compile.sh`**  
  Shell script for compiling the C++ code against Pythia8.

- **`main42.cc`**  
  The main C++ source, uses Pythia8 to generate events and record photon/tau/electron energies in CSV files.

- **CSV files**  
  - `electron_energies.csv`
  - `photon_energies.csv`
  - `photon_energies_lab.csv`
  - `tau_energies.csv`
  - `tau_energies_lab.csv`
  
  Each contains one energy value per line. (They will be overwritten or created anew each run.)

- **`plot.py`**  
  Python script to plot the **photon_energies.csv** distribution in log scale.

- **`plot2.py`**  
  Python script to compare **lab-frame** vs. **rest-frame** photon energies in a single overlaid plot.

- **`README.md`**  
  This file, explaining how to set up and run everything.

---

## 1. Prerequisites

- **Pythia8** installed on your system.  
- A **C++ compiler** (e.g. `g++`).  
- **Python 3** with `numpy` and `matplotlib` for plotting.

---

## 2. Environment Setup

1. **Edit `env.sh`:**  
   Inside `env.sh`, replace:
   ```bash
   export PYTHIA8=/Users/lihengne/local/llp/pythia8313
   ```
   with the actual path to your Pythia8 installation.

2. **Source** the script:
   ```bash
   source env.sh
   ```
   This ensures your environment variables (`PATH`, `DYLD_LIBRARY_PATH` or `LD_LIBRARY_PATH`) are set so that `pythia8-config` is recognized and the Pythia8 library can be found at runtime.

---

## 3. Compilation

Run:
```bash
source compile.sh
```
This calls:
```bash
g++ main42.cc -o main42 $(pythia8-config --cxxflags --ldflags)
```
and produces the executable **`main42`**.

---

## 4. Running the Program

Execute:
```bash
./main42
```
### What it does:

1. **Generates** 1,000,000 events where:
   - A single **Z** boson (PDG=23) is placed at rest.
   - The **Z** decays only into **tau tau** (forced).
   - Each **tau** decays only into **e nu nu** (forced).
   - **FSR** is enabled, so electrons can emit photons.

2. **Loops** over the resulting event record:
   - Finds photons that came from electron FSR.
   - Computes a Lorentz boost to put them into the **tau** rest frame.
   - Saves photon energies in both lab and tau rest frames, plus tau energies, to CSV files.
   - Electron energies are also recorded in `electron_energies.csv`.

3. **Prints** debug info, e.g. âFound FSRâ and the event listing for certain events.

After completion, you should see files like:
- `photon_energies.csv` (photon energies in the tau rest frame)
- `photon_energies_lab.csv` (photon energies in the lab frame)
- `tau_energies.csv`
- `tau_energies_lab.csv`
- `electron_energies.csv`

---

## 5. Plotting

### 5.1 Single Dataset

To view the distribution of **photon energies** in the **tau** rest frame:
```bash
python -i plot.py
```
- This script loads `photon_energies.csv` and plots a histogram on a **log scale** for the y-axis.  
- You can adjust the `xmin, xmax, bins=...` inside the script as needed.

### 5.2 Compare Lab vs. Rest Frames

To compare `photon_energies_lab.csv` vs. `photon_energies.csv`:
```bash
python -i plot2.py
```
- This overlays two histograms:
  - **Lab Frame** (blue)  
  - **Rest Frame** (red)  
- Also uses a log scale on the y-axis.

You can further customize the code in `plot2.py` to suit your analysis needs.

---

## 6. Notes and Customization

- If you want to **reduce** the number of events for faster testing, edit:
  ```cpp
  int nEvents = 1000000;
  ```
  in **`main42.cc`** to fewer events.

- The CSV files are regenerated each time you run `./main42`, potentially overwriting existing data. If you want separate outputs per run, rename them or move them in a script.

- You can also tweak other decay or FSR settings by changing the Pythia8 commands in `main42.cc`.

