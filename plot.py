from matplotlib import pyplot as plt
import pandas as pd

data = pd.read_csv("./out.csv")
# print(data)
data.plot()
plt.show()