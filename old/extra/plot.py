from turtle import color
import matplotlib.pyplot as plt
import pandas as pd

resultsDf = pd.read_csv("strchr_new_new_resultfile")
resultsDf.sort_values(by='slen',inplace=True)
x = resultsDf.iloc[:,0].to_numpy()
yGlibc = resultsDf.iloc[:,1].to_numpy()
yCustom = resultsDf.iloc[:,2].to_numpy()

plt.loglog(x,yGlibc,'bo-', label='glibc implementation', linewidth=2)
plt.loglog(x,yCustom,'o-', color='orange', label='custom implementation', linewidth=2)
plt.xlabel("Position of character in string (length = 5 Mil) (log scale)")
plt.ylabel("Time Taken (ms) (log scale)")
plt.title("strchr benchmark")
plt.legend()
plt.tight_layout()
plt.savefig("strchr_new_loglog_plot.png", dpi=300)
