from random import random as r

numlin=50

matt = []
for j in range(numlin):
    matt.append([r()*20, r()*10,r()*20,r()*10])

from matplotlib.pyplot import plot, show, subplots
import matplotlib.animation as animation

fig,ax=subplots(figsize=(8,5))
ax.set_xlim([0,20])
ax.set_ylim([0,10])


def animate(i):
    animlist = plot(matt[i][0],matt[i][1],'r',matt[i][2],matt[i][3],'r',marker='o',markersize=8)
    return animlist

def init():
    return []

anim=animation.FuncAnimation(fig,animate,frames=numlin,interval=100,init_func=init,blit=True,repeat=0)
show()