
import numpy as np
import matplotlib.pyplot as plt

# 1. Load the two datasets
#    Each file has one float per line.
data1 = np.loadtxt("photon_energies_lab.csv", dtype=float)
data2 = np.loadtxt("photon_energies.csv", dtype=float)

xmin = 0.0
xmax = 10.0

# 2. Create overlaid histograms on the same figure
#    'alpha' adjusts transparency so both can be seen.
#    'label' sets legend labels; 'edgecolor' helps distinguish bin edges.
plt.hist(data2, bins=100, range=(xmin, xmax), alpha=0.5, label="Rest Frame", color='red', edgecolor='red')
plt.hist(data1, bins=100, range=(xmin, xmax), alpha=0.5, label="Lab Frame", color='blue', edgecolor='blue')


# 3. Labeling and legend
plt.yscale("log")
plt.xlabel("Energy (GeV)")
plt.ylabel("Yields")
plt.title("Comparison of Two Datasets")
plt.legend()  # shows "File 1" and "File 2" labels

# 4. Show the figure
plt.show()
