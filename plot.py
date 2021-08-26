import pandas as pd
from matplotlib import pyplot as plt
import numpy as np

data = pd.read_csv('output.csv')
data.plot()
plt.show()