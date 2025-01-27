import numpy as np
import matplotlib.pyplot as plt

# 1. Load the data
#    Each line in "datafile.txt" contains a single float, e.g.:
#       1.23
#       3.45
#       0.99
#    ...
data = np.loadtxt("photon_energies.csv", dtype=float)

xmin = 0.0
xmax = 10.0

# 2. Create the histogram
#    'bins=50' is an example choice; you can adjust it.
#    'edgecolor' gives a thin line around each bar for clarity.
plt.hist(data, bins=100, range=(xmin, xmax), alpha=0.5, label="Rest Frame", color='red', edgecolor='red')

# 3. Label and display
plt.yscale("log")
plt.xlabel("Energy (GeV)")
plt.ylabel("Yields")
plt.title("Histogram of Data")
plt.show()
