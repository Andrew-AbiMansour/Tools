from matplotlib.pyplot import figure, show
import numpy as np
from matplotlib.image import NonUniformImage

def pcolor(z,y=np.arange(10),x=np.arange(10),interp='bilinear')
  '''
  z = np.random.rand(10,10)
  '''

  fig = figure()
  ax = fig.add_subplot(111)
  im = NonUniformImage(ax, interpolation=interp, extent=(-1,1,-1,1),
                       cmap=cm.Purples)
  im.set_data(x, y, z)
  ax.images.append(im)
  ax.set_xlim(-1,1)
  ax.set_ylim(-1,1)
  ax.set_title(interp)
  
  show()
