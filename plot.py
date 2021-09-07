from matplotlib import pyplot as plt
import pandas as pd

data = pd.read_csv("./out.csv")
# print(data)
# data.plot(x = 'dist_x',  figsize=(10, 20))
data.plot()
plt.show()