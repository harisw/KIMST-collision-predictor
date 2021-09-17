import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import numpy as np

#setup plotting area
fig, ax = plt.subplots(figsize=(5, 3))
ax.set(xlim=(-3, 3), ylim=(-1, 1))

x = np.linspace(-3, 3, 91)
t = np.linspace(1, 25, 30)
#t = np.linspace(1, 10, 10)
X2, T2 = np.meshgrid(x, t)
 
sinT2 = np.sin(2*np.pi*T2/T2.max())
F = 0.9*sinT2*np.sinc(X2*(1 + sinT2))

# line = ax.plot(x, F[0, :], color='k', lw=2)[0]

# def animate(i):
#     line.set_ydata(F[i, :])
scat = ax.scatter(x[::3], F[0, ::3])

def animate(i):
    y_i = F[i, ::3]
    scat.set_offsets(np.c_[x[::3], y_i])

anim = FuncAnimation(fig, animate, interval=30, frames=len(t)-1)
plt.draw()
plt.show()
#anim.save('filename.mp4', writer='imagick')